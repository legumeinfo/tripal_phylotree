<?php

/**
 * @file
 * Implements the phylotree node content type
 */

/**
 * Implements hook_node_info().
 *
 * Provide information to drupal about the node types that we're creating
 * in this module. 
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_info() {
  $nodes = array();
  $nodes['chado_phylotree'] = array(
    'name'        => t('Phylotree'),
    'base'        => 'chado_phylotree',
    'description' => t('A phylotree from the chado database'),
    'has_title'   => TRUE,
    'locked'      => TRUE,
    'chado_node_api' => array(
      'base_table' => 'phylotree',
      'hook_prefix' => 'chado_phylotree',
      'record_type_title' => array(
        'singular' => t('Phylotree'),
        'plural' => t('Phylotrees')
      ),
      
      /* sync_filters: tripal is hardcoded to look for this
       sync_filter settings: type_id and organism_id. (phylotree does
       not have organism_id but need to set it false anyways. */
      'sync_filters' => array(
        'type_id' => FALSE,
        'organism_id' => FALSE
      ),
    )
  );
  return $nodes;
}


/**
 * Implements hook_node_view(). Acts on all content types
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_view($node, $view_mode, $langcode) {
  
  if($node->type != 'chado_phylotree') { return; }
  
  switch($view_mode) {
  case 'full':
    $node->content['tripal_phylotree_base'] = array(
      '#theme' => 'tripal_phylotree_base',
      '#node' => $node,
      '#tripal_toc_id'    => 'base',
      '#tripal_toc_title' => 'Overview',
      '#weight' => -100,
    );
    $node->content['tripal_phylotree_phylogram'] = array(
      '#theme' => 'tripal_phylotree_phylogram',
      '#node' => $node,
      '#tripal_toc_id'    => 'phylotree_phylogram',
      '#tripal_toc_title' => 'Phylogram',
      '#weight' => -90,
    );
    $node->content['tripal_phylotree_radial'] = array(
      '#theme' => 'tripal_phylotree_radial',
      '#node' => $node,
      '#tripal_toc_id'    => 'phylotree_circ_dendrogram',
      '#tripal_toc_title' => 'Circular Dendrogram',
      '#weight' => -80,
    ); 
    $node->content['tripal_phylotree_organisms'] = array(
      '#theme' => 'tripal_phylotree_organisms',
      '#node' => $node,
      '#tripal_toc_id'    => 'phylotree_organisms',
      '#tripal_toc_title' => 'Organisms',
      '#weight' => -70,
    );
    $node->content['tripal_phylotree_references'] = array(
      '#theme' => 'tripal_phylotree_references',
      '#node' => $node,
      '#tripal_toc_id'    => 'phylotree_references',
      '#tripal_toc_title' => 'Cross References',
    );
     $node->content['tripal_phylotree_analysis'] = array(
      '#theme' => 'tripal_phylotree_analysis',
      '#node' => $node,
      '#tripal_toc_id'    => 'phylotree_analysis',
      '#tripal_toc_title' => 'Analysis',
    );  
    break;
    
  case 'teaser':
    $node->content['tripal_phylotree_teaser'] = array(
      '#theme' => 'tripal_phylotree_teaser',
      '#node' => $node,
    );
    break;
    }
}

/**
 * Implementation of hook_form().
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_form($node, &$form_state) {

  $form = array();

  // Default values can come in the following ways:
  //
  // 1) as elements of the $node object.  This occurs when editing an existing phylotree
  // 2) in the $form_state['values'] array which occurs on a failed validation or
  //    ajax callbacks from non submit form elements
  // 3) in the $form_state['input'[ array which occurs on ajax callbacks from submit
  //    form elements and the form is being rebuilt
  //
  // set form field defaults
  $phylotree    = null;
  $phylotree_id = null;
  $pname        = '';
  $leaf_type    = '';
  $analysis_id  = '';
  $dbxref        = '';
  $comment      = '';

  // if we are editing an existing node then the phylotree is already part of the node
  if (property_exists($node, 'phylotree')) {
    $phylotree      = $node->phylotree;
    $phylotree      = chado_expand_var($phylotree, 'field', 'phylotree.comment');
    $phylotree_id   = $phylotree->phylotree_id;
    $pname          = $phylotree->name;
    $leaf_type      = $phylotree->type_id ? $phylotree->type_id->name : '';
    $comment        = $phylotree->comment;
    $analysis_id    = $phylotree->analysis_id->analysis_id;
    $dbxref         = $phylotree->dbxref_id->db_id->name . ":" . $phylotree->dbxref_id->accession;

    // keep track of the phylotree id
    $form['phylotree_id'] = array(
      '#type' => 'value',
      '#value' => $phylotree_id,
    );
  }
  // if we are re constructing the form from a failed validation or ajax callback
  // then use the $form_state['values'] values
  if (array_key_exists('values', $form_state) and isset($form_state['values']['pname'])) {
    $pname        = $form_state['values']['pname'];
    $leaf_type    = $form_state['values']['leaf_type'];
    $analysis_id  = $form_state['values']['analysis_id'];
    $dbxref       = $form_state['values']['dbxref'];
    $comment      = $form_state['values']['desc'];
  }
  // if we are re building the form from after submission (from ajax call) then
  // the values are in the $form_state['input'] array
  if (array_key_exists('input', $form_state) and !empty($form_state['input'])) {
    $pname        = $form_state['input']['pname'];
    $leaf_type    = $form_state['input']['leaf_type'];
    $analysis_id  = $form_state['input']['analysis_id'];
    $comment      = $form_state['input']['desc'];
    $dbxref       = $form_state['input']['dbxref'];
  }

  $form['pname']= array(
    '#type' => 'textfield',
    '#title' => t('Tree Name'),
    '#required' => TRUE,
    '#default_value' => $pname,
    '#description' => t('Enter the name used to refer to this phylogenetic tree.'),
    '#maxlength' => 255
  );

  //$type_options = tripal_get_cvterm_default_select_options('phylotree', 'type_id', 'phylotree types');
  //$type_options[0] = 'Select a Type';
  $type_cv = tripal_get_default_cv('phylotree', 'type_id');
  $cv_id = $type_cv->cv_id;

  $form['leaf_type'] = array(
    '#title'       => t('Tree Leaf Type'),
    '#type'        => 'textfield',
    '#description' => t("Choose the type of node that is used for the tree. The term must be a valid sequence ontology (SO) term, or the word 'taxonomy' if a taxonomic tree."),
    '#required'    => FALSE,
    '#default_value' => $leaf_type,
    '#autocomplete_path' => "admin/tripal/chado/tripal_cv/cvterm/auto_name/$cv_id",
  );

  // get the list of organisms
  $sql = "SELECT * FROM {analysis} ORDER BY name";
  $arset = chado_query($sql);
  $analyses = array();
  $analyses[''] = '';
  while ($analysis = $arset->fetchObject()) {
    $analyses[$analysis->analysis_id] = $analysis->name;
  }
  $form['analysis_id'] = array(
    '#title'         => t('Analysis'),
    '#type'          => 'select',
    '#description'   => t("Choose the analysis from which this phylogenetic tree was derived"),
    '#required'      => TRUE,
    '#default_value' => $analysis_id,
    '#options'       => $analyses,
  );

  $form['dbxref'] = array(
    '#title'       => t('Database Cross-Reference'),
    '#type'        => 'textfield',
    '#description' => t("Enter a database cross-reference of the form 
        [DB name]:[accession]. The database name must already exist in the 
        database. If the accession does not exist it is automatically added."),
    '#required'    => TRUE,
    '#default_value' => $dbxref,
  );
  
  $form['desc']= array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#required' => TRUE,
    '#default_value' => $comment,
    '#description' => t('Enter a description for this tree.'),
  );

  return $form;
}

/**
 * Implementation of hook_validate().
 *
 * This validation is being used for three activities:
 *   CASE A: Update a node that exists in both drupal and chado
 *   CASE B: Synchronizing a node from chado to drupal
 *   CASE C: Inserting a new node that exists in niether drupal nor chado
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_validate($node, $form, &$form_state) {

  // if this is a delete then don't validate
  if($node->op == 'Delete') {
    return;
  }

  // we are syncing if we do not have a node ID but we do have a phylotree_id. We don't
  // need to validate during syncing so just skip it.
  if (is_null($node->nid) and property_exists($node, 'phylotree_id') and $node->phylotree_id != 0) {
    return;
  }

  // remove surrounding white-space on submitted values
  $node->pname   = trim($node->pname);
  $node->desc     = trim($node->desc);
  
  // validate the dbxref for this tree
  $matches = array();
  if (preg_match('/^(.*?):(.*)$/', $node->dbxref, $matches)) {
    $db_name = $matches[1];
    $accession = $matches[2];
    $values = array(
      'name' => $db_name
    );
    $dbxref = chado_generate_var('db', $values);
    if (!$dbxref) {
      form_set_error('dbxref', t("The database provided in the database cross reference does not exist. Please add the database."));
    }
  }
  else {
    form_set_error('dbxref', t("The database cross reference is not in the correct format."));
  }

  // Validating for an update
  if (property_exists($node, 'nid')) {

    // make sure the phylotree type is a real sequence ontology term or the word
    // 'taxonomy'.
    if ($node->leaf_type and $node->leaf_type != 'taxonomy') {
      $type = tripal_get_cvterm(array(
          'name' => $node->leaf_type,
          'cv_id' => array('name' =>'sequence')
      ));
      if (!$type) {
        form_set_error('leaf_type', t("The leaf type is not a valid name from the Sequence Ontology."));
      }
    }

    // if this is an update, we want to make sure that a different phylotree for
    // doesn't already have this name. We don't want to give
    // two phylotrees the same uniquename
    if (property_exists($node, 'phylotree_id') and $node->phylotree_id != 0) {
      $sql = "
        SELECT *
        FROM {phylotree} P
        WHERE
          P.name = :name AND NOT P.phylotree_id = :phylotree_id
      ";
      $args = array(':name' => $node->pname, ':phylotree_id' => $node->phylotree_id);
      $result = chado_query($sql, $args)->fetchObject();
      if ($result) {
        form_set_error('pname', t("Phylotree update cannot proceed. The tree name '$node->pname' is in use by another tree. Please provide a unique name for this tree."));
      }
    }
    
    // TODO: validate the dbxref
  }
  // Validating for an insert
  else {

    // make sure the feature type is a real sequence ontology term
    if ($node->leaf_type and $node->leaf_type != 'taxonomy') {
      $type = tripal_get_cvterm(array(
          'name' => $node->leaf_type,
          'cv_id' => array('name' => 'sequence')
      ));
      if (!$type) {
        form_set_error('leaf_type', t("The leaf type is not a valid name from the Sequence Ontology."));
      }
    }

    // if this is an insert then we just need to make sure this name doesn't
    // already exist for this organism if it does then we need to throw an error
    $sql = "
      SELECT *
      FROM {phylotree} F
      WHERE P.name  = :name 
    ";
    $args = array(':name' => $node->pname);
    $result = chado_query($sql, $args)->fetchObject();
    if ($result) {
      form_set_error('pname', t("Phylotree insert cannot proceed. The tree name '$node->pname' already exists for this tree. Please provide a unique name for this tree."));
    }
    
    // TODO: validate the dbxref
  }
}
/**
 * Implements hook_node_presave(). Acts on all node content types.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_presave($node) {

  switch ($node->type) {
    // This step is for setting the title for the Drupal node.  This title
    // is permanent and thus is created to be unique.  Title changes provided
    // by tokens are generated on the fly dynamically, but the node title
    // seen in the content listing needs to be set here. Do not call
    // the chado_get_node_title() function here to set the title as the node
    // object isn't properly filled out and the function will fail.
    case 'chado_phylotree':
      // for a form submission the 'phylotreename' field will be set,
      // for a sync, we must pull from the phylotree object
      if (property_exists($node, 'phylotreename')) {
        // set the title
        $node->title = $node->pname;
      }
      else if (property_exists($node, 'phylotree')) {
        $node->title = $node->phylotree->name;
      }
      break;
  }
}

/**
 * Implements hook_node_insert().
 * Acts on all content types.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_insert($node) {

  switch ($node->type) {
    case 'chado_phylotree':
      
      $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid);
      $values = array('phylotree_id' => $phylotree_id);
      $phylotree = chado_generate_var('phylotree', $values);
      $phylotree = chado_expand_var($phylotree, 'field', 'phylotree.comment');
      $node->phylotree = $phylotree;
      
      // Now use the API to set the path.
      chado_set_node_url($node);

      // Now get the title
      $node->title = chado_get_node_title($node);

      break;
  }
}

/**
 * Implements hook_node_update().
 * Acts on all content types.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_update($node) {

  switch ($node->type) {
    case 'chado_phylotree':
      
      $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid);
      $values = array('phylotree_id' => $phylotree_id);
      $phylotree = chado_generate_var('phylotree', $values);
      $phylotree = chado_expand_var($phylotree, 'field', 'phylotree.comment');
      $node->phylotree = $phylotree;

      // Now get the title
      $node->title = chado_get_node_title($node);

      break;
  }
}

/**
 * Implements [content_type]_chado_node_default_title_format().
 *
 * Defines a default title format for the Chado Node API to set the titles on
 * Chado phylotree nodes based on chado fields.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_chado_node_default_title_format() {
  return '[phylotree.name]';
}

/**
 * Implements hook_chado_node_default_url_format().
 *
 * Designates a default URL format for phylotree nodes.
 */
function chado_phylotree_chado_node_default_url_format() {
  return '/phylotree/[phylotree.name]';
}

/**
 *  Implements hook_insert().
 *
 *  When a new chado_phylotree node is created we also need to add
 *  information to our chado_phylotree table.  This function is called
 *  on insert of a new node of type 'chado_phylotree' and inserts the
 *  necessary information.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_insert($node) {
  $node->pname   = trim($node->pname);
  $node->desc    = trim($node->desc);
  
  // get the leaf type id
  $type = NULL;
  if ($node->leaf_type == 'taxonomy') {
    $values = array(
      'cv_id' => array(
        'name' => 'tripal_phylogeny'
      ),
      'name' => 'taxonomy'
    );
    $type = chado_select_record('cvterm', array('cvterm_id'), $values);
  }
  else {
    $values = array(
      'cv_id' => array(
       'name' => 'sequence'
      ),
      'name' => $node->leaf_type
    );
    $type = chado_select_record('cvterm', array('cvterm_id'), $values);
  }
  
  // get the dbxref
  // Add or find the dbxref for this tree
  $matches = array();
  preg_match('/^(.*?):(.*)$/', $node->dbxref, $matches);
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
    // Add the accession
    $db = chado_generate_var('db', array('name' => $db_name));
    $dbxref = tripal_insert_dbxref(array(
      'db_id' => $db->db_id,
      'accession' => $accession,
    ));
  }
  
  $phylotree_id = '';
  
  // if there is an phylotree_id in the $node object then this must
  // be a sync so we can skip adding the phylotree as it is already
  // there, although we do need to proceed with the rest of the
  // insert
    
  if (!property_exists($node, 'phylotree_id')) {
    $values = array(
      'analysis_id' => $node->analysis_id,
      'name' => $node->pname,
      'dbxref_id' => $dbxref->dbxref_id,
      'comment' => $node->desc,
    );
    if ($type) {
      $values['type_id'] = $type[0]->cvterm_id;
    }
    $phylotree = chado_insert_record('phylotree', $values);
    if (!$phylotree) {
      drupal_set_message(t('Unable to add phylotree.'), 'warning');
      tripal_report_error('tripal_phylotree', TRIPAL_WARNING, 'Insert phylotree: Unable to create phylotree where values: %values',
      array('%values' => print_r($values, TRUE)));
      return;
    }
    $phylotree_id = $phylotree['phylotree_id'];

  }
  else {
    $phylotree_id = $node->phylotree_id;
  }

  // Make sure the entry for this phylotree doesn't already exist in the
  // chado_phylotree table if it doesn't exist then we want to add it.
  $check_org_id = chado_get_id_from_nid('phylotree', $node->nid);
  if (!$check_org_id) {
    $record = new stdClass();
    $record->nid = $node->nid;
    $record->vid = $node->vid;
    $record->phylotree_id = $phylotree_id;
    drupal_write_record('chado_phylotree', $record);
  }
}

/**
 * Implements hook_update().
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_update($node) {

  $node->pname   = trim($node->pname);
  $node->desc = trim($node->desc);

  // get the leaf type id
  $type = NULL;
  if ($node->leaf_type == 'taxonomy') {
    $values = array(
      'cv_id' => array(
        'name' => 'tripal_phylogeny'
      ),
      'name' => 'taxonomy'
    );
    $type = chado_select_record('cvterm', array('cvterm_id'), $values);
  }
  else {
    if ($node->leaf_type) {
      $values = array(
        'cv_id' => array(
          'name' => 'sequence'
        ),
        'name' => $node->leaf_type
      );
      $type = chado_select_record('cvterm', array('cvterm_id'), $values);
    }
  }
  
  // get the dbxref
  // Add or find the dbxref for this tree
  $matches = array();
  preg_match('/^(.*?):(.*)$/', $node->dbxref, $matches);
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
    // Add the accession
    $db = chado_generate_var('db', array('name' => $db_name));
    $dbxref = tripal_insert_dbxref(array(
      'db_id' => $db->db_id,
      'accession' => $accession,
    ));
  }
  
  $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid) ;

  $match = array(
    'phylotree_id' => $phylotree_id,
  );
  $values = array(
    'analysis_id' => $node->analysis_id,
    'name' => $node->pname,
    'dbxref_id' => $dbxref->dbxref_id,
    'comment' => $node->desc,
  );
  if ($type) {
    $values['type_id'] = $type[0]->cvterm_id;
  }

  $options = array('return_record' => TRUE);
  $status = chado_update_record('phylotree', $match, $values, $options);

  if (!$status) {
    drupal_set_message(t('Unable to update phylotree.'), 'warning');
    tripal_report_error('tripal_phylotree', TRIPAL_WARNING,
      'Update phylotree: Unable to update phylotree where values: %values',
      array('%values' => print_r($values, TRUE))
    );
  }
}
/**
 *  Implements hook_load().
 *
 *  When a node is requested by the user this function is called to allow us
 *  to add auxiliary data to the node object.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_load($nodes) {

  foreach ($nodes as $nid => $node) {
    
    $phylotree_id = chado_get_id_from_nid('phylotree', $nid);
    
    // if the nid does not have a matching record then skip this node.
    // this can happen with orphaned nodes.
    if (!$phylotree_id) {
      continue;
    }
    
    $values = array('phylotree_id' => $phylotree_id);
    $phylotree = chado_generate_var('phylotree', $values);
    $nodes[$nid]->phylotree = $phylotree;
    
    // expand the comment field, tripal omits it by default
    $phylotree = chado_expand_var($phylotree, 'field', 'phylotree.comment');
    
    $node->title = chado_get_node_title($node);
  }
}

/**
 * Implements hook_delete().
 *
 * Delete data from drupal and chado databases when a node is deleted
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_delete(&$node) {

  $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid);

  // if we don't have a phylotree id for this node then this isn't a node of
  // type chado_phylotree or the entry in the chado_phylotree table was lost.
  if (!$phylotree_id) {
    return;
  }

  // Remove data from {chado_phylotree}, {node} and {node_revisions} tables of
  // drupal database
  $sql_del = "DELETE FROM {chado_phylotree} WHERE nid = :nid AND vid = :vid";
  db_query($sql_del, array(':nid' => $node->nid, ':vid' => $node->vid));
  $sql_del = "DELETE FROM {node_revision} WHERE nid = :nid AND vid = :vid";
  db_query($sql_del, array(':nid' => $node->nid, ':vid' => $node->vid));
  $sql_del = "DELETE FROM {node} WHERE nid = :nid AND vid = :vid";
  db_query($sql_del, array(':nid' => $node->nid, ':vid' => $node->vid));

  // Remove data from phylotree and phylotreeprop tables of chado
  // database as well
  
  chado_query("DELETE FROM {phylotree} WHERE phylotree_id = :phylotree_id", array(':phylotree_id' => $phylotree_id));
  
}

/**
 * Implement hook_node_access().
 *
 * This hook allows node modules to limit access to the node types they define.
 *
 *  @param $node
 *  The node on which the operation is to be performed, or, if it does not yet exist, the
 *  type of node to be created
 *
 *  @param $op
 *  The operation to be performed
 *
 *  @param $account
 *  A user object representing the user for whom the operation is to be performed
 *
 *  @return
 *  If the permission for the specified operation is not set then return FALSE. If the
 *  permission is set then return NULL as this allows other modules to disable
 *  access.  The only exception is when the $op == 'create'.  We will always
 *  return TRUE if the permission is set.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_node_access($node, $op, $account) {
  
  $node_type = $node;
  if (is_object($node)) {
    $node_type = $node->type;
  }

  if($node_type == 'chado_phylotree') {
    if ($op == 'create') {
      if (!user_access('create chado_phylotree content', $account)) {
        return NODE_ACCESS_DENY;
      }
      return NODE_ACCESS_ALLOW;
    }
    if ($op == 'update') {
      if (!user_access('edit chado_phylotree content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
    if ($op == 'delete') {
      if (!user_access('delete chado_phylotree content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
    if ($op == 'view') {
      if (!user_access('access chado_phylotree content', $account)) {
        return NODE_ACCESS_DENY;
      }
    }
    return NODE_ACCESS_IGNORE;
  }
}

/**
 * phylotree_by_name
 *
 * Lookup tree by name, and redirect to that drupal node. This could
 * also be done for example by URL aliases, for example using pathauto
 * module.
 *
 * @param int $phylotree_id
 * @return string json
 * @ingroup tripal_phylotree
 */
function phylotree_by_name($name) {
    
    $sql = <<<SQL
 SELECT cp.nid as node_id 
 FROM chado.phylotree t
 LEFT JOIN chado_phylotree cp on cp.phylotree_id = t.phylotree_id
 WHERE t.name = :name
SQL;

    $args = array(':name' => $name);
    $result = chado_query( $sql, $args );
    $nid = $result->fetchField();
    drupal_goto("node/$nid");
}

/**
 * phylotree_json
 *  Get json representation of a phylotree id. See menu item for JSON service 
 *  endpoint.
 *
 * @param int $phylotree_id
 * @return string json
 * @ingroup tripal_phylotree
 */
function phylotree_json($phylotree_id) {
  
  // TODO: consider replacing this SQL with a chado_generate_var() call
  // which would include all of the nids for both features and organisms.
  $sql = "
    SELECT 
      n.phylonode_id, n.parent_phylonode_id, n.label AS name, n.distance AS length, 
      f.feature_id, f.name AS feature_name,
      cvt.name AS cvterm_name,
      o.organism_id, o.common_name, o.abbreviation, o.genus, o.species,
      cf.nid AS feature_node_id, 
      co.nid AS organism_node_id
    FROM {phylonode} n
      LEFT OUTER JOIN {cvterm} cvt             ON n.type_id = cvt.cvterm_id
      LEFT OUTER JOIN {feature} f              ON n.feature_id = f.feature_id
      LEFT OUTER JOIN public.chado_feature cf  ON cf.feature_id = f.feature_id
      LEFT OUTER JOIN {organism} o             ON f.organism_id = o.organism_id
      LEFT OUTER JOIN public.chado_organism co ON co.organism_id = o.organism_id
    WHERE n.phylotree_id = :phylotree_id
  ";

  $args = array(':phylotree_id' => $phylotree_id);
  $result = chado_query( $sql, $args );

  // fetch all the phylonodes into an assoc array indexed by
  // phylonode_id.  convert into from resultset record to array,
  // fixing datatypes. chado_query returns numeric as string and fun
  // stuff like that.
  $phylonodes = array();
  $root_phylonode_ref = null;
  
  foreach ($result as $r) {
    $phylonode_id = (int) $r->phylonode_id;

    // expect all nodes to have these properties
    $node = array(
      'phylonode_id' => $phylonode_id,
      'parent_phylonode_id' => (int) $r->parent_phylonode_id,
      'length' => (double) $r->length,
      'cvterm_name' => $r->cvterm_name
    );
    
    // other props may exist only for leaf nodes
    if($r->name) {
      $node['name'] = $r->name;
    }
    if($r->feature_id) {
      $node['feature_id'] = (int) $r->feature_id;
      $node['feature_name'] = $r->feature_name;
      $node['feature_node_id'] = (int) $r->feature_node_id;
    }
    if($r->organism_id) {
      $node['organism_id'] = (int) $r->organism_id;
      $node['common_name'] = $r->common_name;
      $node['abbreviation'] = $r->abbreviation;
      $node['genus'] = $r->genus;
      $node['species'] = $r->species;
      $node['organism_node_id'] = (int) $r->organism_node_id;
    }
    $phylonodes[$phylonode_id] = $node;
  }
  
  //populate the children[] arrays for each node
  foreach( $phylonodes as $key => &$node ) {
    if($node['parent_phylonode_id'] !== 0) {
      $parent_ref = &$phylonodes[ $node['parent_phylonode_id'] ]; // get
      $parent_ref['children'][] = &$node; // append node refernce to children
    }
    else {
      $root_phylonode_ref = &$node;
    }
  }
  // dump datastructure as json to browser. drupal sets the mime-type correctly.
  drupal_json_output( $root_phylonode_ref );
}

/**
 * phylotree_feature_summary
 *  Get array of feature counts by organism. key = organism
 *  abbreviation. value = number of features for this phylotree having
 *  this organism.
 *
 * @param int phylotree_id
 * @return array
 * @ingroup tripal_phylotree
 */
function phylotree_feature_summary($phylotree_id) {
  
  $sql = "
    SELECT o.abbreviation, COUNT(o.organism_id) AS count 
    FROM {phylonode} n 
      LEFT OUTER JOIN {feature} f  ON n.feature_id = f.feature_id 
      LEFT OUTER JOIN {organism} o ON f.organism_id = o.organism_id 
    WHERE n.phylotree_id = :phylotree_id 
      AND n.feature_id IS NOT NULL 
    GROUP BY o.organism_id
  ";

  $args = array(':phylotree_id' => $phylotree_id);
  $result = chado_query($sql, $args);
  $summary = array();
  foreach($result as $r) {
    $summary[$r->abbreviation] = $r->count;
  }
  return $summary;
}
