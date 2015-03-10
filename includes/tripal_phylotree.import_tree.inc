<?php


/**
 * 
 * @param $file_name
 *   The name of the file containing the phylogenetic tree to import.
 * @param $format
 *   The format of the file. Currently only the 'newick' file format is supported.
 * @param $options
 *   A set of options for importing the file.  The options are required
 *     'name':       The name of the tree. This will be displayed to users.
 *     'description: A description about the tree
 *     'anlaysis:    The name of the analysis describing how this tree was created.
 *     'cvterm':     Any SO term or the word 'taxonomy'.
 *     'dbxref':     [DB name]:[accesion]
 *   These are optional:
 *     'leaf_type':  A sequence ontology term or the word 'organism'. If the
 *                   type is 'organism' then this tree represents a 
 *                   taxonomic tree.  The default, if not specified, is the
 *                   term 'polypeptide'. 
 *     'taxonomy':   Set to TRUE if the input file represents taxonomy. 
 *                   Otherwise, the tree is expected to be sequence-based.
 *     'name_re':    If the leaf type is NOT 'organism', then the value of
 *                   this field can be a regular expression to pull out
 *                   the name of the feature from the node label in the 
 *                   intput tree. If no value is provided the entire label is used.
 *     'uname_re':   If the leaf type is NOT 'organism', then the value of
 *                   this field can be a regular expression to pull out the
 *                   uniquene name of the feature from the node label in the
 *                   input tree. If no value is provided the entire label is used.
 *
 */
function tripal_phylotree_import_tree_file($file_name, $format, $options = array()) {

  // Set some option details
  if (!array_key_exists('leaf_type', $options)) {
    $options['leaf_type'] = 'polypeptide';
  }
  if (!array_key_exists('match', $options)) {
    $options['match'] = 'uniquename';
  }
  if (!array_key_exists('name_re', $options)) {
    $options['name_re'] = '^(.*)$';
  }
  if (!array_key_exists('uname_re', $options)) {
    $options['uname_re'] = '^(.*)$';
  }
  if (!array_key_exists('taxonomy', $options)) {
    $options['taxonomy'] = FALSE;
  }
  $options['name_re'] = trim($options['name_re']);
  $options['uname_re'] = trim($options['uname_re']);
  
  
  // TODO: make sure either name_re or uname_re are valid REs.
  
  $transaction = db_transaction();
  print "\nNOTE: Loading of this tree file is performed using a database transaction. \n" .
      "If the load fails or is terminated prematurely then the entire set of \n" .
      "insertions/updates is rolled back and will not be found in the database\n\n";
  try {
    // Add or find the dbxref for this tree
    $matches = array();
    if (!preg_match('/^(.*?):(.*)$/', $options['dbxref'], $matches)){
      // ERROR
    }
    $db_name = $matches[1];
    $accession = $matches[2];
    $values = array(
      'accession' => $accession,
      'db_id' => array(
        'name' => $db_name
      ),
    );
    $dbxref = chado_generate_var('dbxref', $values);
    if (!$dbxref) {
      $db = chado_generate_var('db', array('name' => $db_name));
      $dbxref = tripal_insert_dbxref(array(
        'db_id' => $db->db_id,
        'accession' => $accession,
      ));
    }
  
    // Find the analysis that matches the given name
    $values = array('name' => $options['analysis']);
    $analysis = chado_generate_var('analysis', $values);
    if (!$analyis) {
      // ERROR
    }
  
    // Make the the CVterm is either 'taxonomy' or a SO term and get the
    // cvterm_id
    $matches = array();
    if ($options['cvterm'] == 'taxonomy') {
      
    }
    else {
      $values = array(
        'name' => $options['cvterm'],
        'cv_id' => array(
          'name' => 'sequence'
        ),
      );
      $cvterm = chado_generate_var('cvterm', $values);
      if (!$cvterm) {
        // ERROR
      }
    }
  
    $values = array(
      'name' => $options['cvterm']
    );
    
    // check to see if a phylotree with this name already exists
    $values = array(
      'name' => $options['name']
    );
    $phylotree = chado_generate_var('phylotree', $values);
    
    if (!$phylotree) {
      // Insert the new tree node
      $values = array(
        'name' => $options['name'],
        'analysis_id' => $analysis->analysis_id,
        'type_id' => $cvterm->cvterm_id,
        'dbxref_id' => $dbxref->dbxref_id,
        'comment' => $options['description'],
      );
      $phylotree = chado_insert_record('phylotree', $values);
      if (!$phylotree) {
        // ERROR
      }
    }
    else {
      // If the tree already exists, then update it and remove all the nodes
      // so that we can re-add them
      if ($phylotree) {
        $match = array('name' => $options['name']);
        chado_update_record('phylotree', $match, $values);
        $match = array('phylotree_id' => $phylotree->phylotree_id);
        chado_delete_record('phylonode', $match);
      }
    }
  
    // Get the vocabulary terms used to describe nodes in the tree
    $values = array(
      'name' => 'phylo_leaf',
      'cv_id' => array(
         'name' => 'tripal_phylogeny',
      ),
    );
    $leaf = chado_generate_var('cvterm', $values);
    if (!$leaf) {
      // ERROR
    }
    $values['name'] = 'phylo_interior';
    $internal = chado_generate_var('cvterm', $values);
    if (!$internal) {
      // ERROR
    }
    $values['name'] = 'phylo_root';
    $root = chado_generate_var('cvterm', $values);
    if (!$root) {
      // ERROR
    }
    $vocab = array(
      'leaf' => $leaf,
      'internal' => $internal,
      'root' => $root,
    );

    // Parse the file according to the format indicated.
    if ($format == 'newick') {
      // Parse the tree into the expected nested node format.
      module_load_include('inc', 'tripal_phylotree', 'includes/parsers/tripal_phylotree.newick_parser');
      $tree = tripal_phylotree_parse_newick_file($file_name);
      
      // Assign the right and left indecies to the tree ndoes
      tripal_phylotree_assign_tree_indices($tree);
    }
    // Iterate through the tree nodes and add them to Chado in accordance
    // with the details in the $options array.
    tripal_phylotree_import_tree($tree, $phylotree, $options, $vocab);

  }
  catch (Exception $e) {
    print "\n"; // make sure we start errors on new line
    watchdog_exception('T_blastXML_load', $e);
    $transaction->rollback();
    print "FAILED: Rolling back database changes...\n";
  }
  print "\nDone.\n";
}

/**
 * 
 * @param unknown $tree
 */
function tripal_phylotree_assign_tree_indices(&$tree, &$index = 1) {
  // Assign a left and right index to each node.  The child node must 
  // have a right and left index less than that of it's parents.  We
  // increment the index by 100 to give space for new nodes that might
  // be added later.
  if (array_key_exists('name', $tree)) {
    $tree['left_index'] = $index += 100;
    if (array_key_exists('is_leaf', $tree)) {
      $tree['right_index'] = $index += 100;
    }
  }
  if (array_key_exists('branch_set', $tree)) {
    foreach ($tree['branch_set'] as $key => $node) {
      tripal_phylotree_assign_tree_indices($tree['branch_set'][$key], $index);
      $tree['right_index'] = $index += 100;
    }
  }
}
/**
 * Iterates through the tree array and creates phylonodes in Chado.
 * 
 * The fucntion iterates through the tree in a top-down approach adding
 * parent internal nodes prior to leaf nodes.
 * 
 * @param $tree
 * @param $options
 */
function tripal_phylotree_import_tree(&$tree, $phylotree, $options, $vocab, $parent = NULL) {
  if (array_key_exists('name', $tree)) {
    $values = array(
      'phylotree_id' => $phylotree->phylotree_id,
      'left_idx'  => $tree['left_index'],
      'right_idx' => $tree['right_index'],
    );
    // Add in any optional values to the $values array if they are present
    if (!empty($tree['name']) and $tree['name'] != '') {
      $values['label'] = $tree['name'];
    }
    if (!empty($tree['length']) and $tree['length'] != '') {
      $values['distance'] = $tree['length'];
    }
    // Set the type of node
    if ($tree['is_root']) {
      $values['type_id'] = $vocab['root']->cvterm_id;
    }
    else if ($tree['is_internal']) {
      $values['type_id'] = $vocab['internal']->cvterm_id;
      $values['parent_phylonode_id'] = $parent['phylonode_id'];
      // TOOD: a feature may be associated here but it is recommended that it
      // be a feature of type SO:match and should represent the alignment of
      // all features beneath it.
    }
    else if ($tree['is_leaf']) {
      $values['type_id'] = $vocab['leaf']->cvterm_id;
      $values['parent_phylonode_id'] = $parent['phylonode_id'];
      
      // Match this leaf node with an organism or feature depending on the
      // type of tree. But we can't do that if we don't have a name.
      if (!empty($tree['name']) and $tree['name'] != '') {
        if ($options['taxonomy'] != TRUE) {
          
          // This is a sequence-based tree. Try to match leaf nodes with features.
          // First, Get the Name and uniquename for the feature
          $matches = array();
          $sel_values = array();
          if ($options['match'] == "name") {
            $sel_values['name'] = $tree['name'];
            $re = $options['name_re'];
            if (preg_match("/$re/", $tree['name'], $matches)) {
              $sel_values['name'] = $matches[1];
            }
          }
          else {
            $sel_values['uniquename'] = $tree['name'];
            $re = $options['uname_re'];
            if (preg_match("/$re/", $tree['name'], $matches)) {
              $sel_values['uniquename'] = $matches[1];
            }
          }
          $sel_values['type_id'] = array(
            'name' => $options['leaf_type'],
            'cv_id' => array(
              'name' => 'sequence'
            ),
          );
          $sel_columns = array('feature_id');
          $feature = chado_select_record('feature', $sel_columns, $sel_values);
          if (count($feature) > 1) {
            // Found multiple features, cannot make an association.
          } 
          else if (count($feature) == 1) {
            $values['feature_id'] = $feature[0]->feature_id;
          }
          else {
            // Could not find a feature that matches the name or uniquename
          }
        }
        else {
          // This is a taxonomic tree. Try to match leaf nodes with organisms.
          // TODO: support linking of leaf nodes to recrods in the organims table.
        }
      }
    }

    // Insert the new node and then add it's assigned phylonode_id to the node
    $phylonode = chado_insert_record('phylonode', $values);
    $tree['phylonode_id'] = $phylonode['phylonode_id'];
  }
  if (array_key_exists('branch_set', $tree)) {
    foreach ($tree['branch_set'] as $key => $node) {
      tripal_phylotree_import_tree($tree['branch_set'][$key], $phylotree, $options, $vocab, $tree);
    }
  }
}