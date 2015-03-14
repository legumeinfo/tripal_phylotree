<?php
/**
 * @file
 * Contains function relating to drush-integration of this module.
 */

/**
 * Describes each drush command implemented by the module
 *
 * @return
 *   The first line of description when executing the help for a given command
 *
 * @ingroup tripal_drush
 */
function tripal_phylotree_drush_help($command) {
  switch ($command) {
    case 'trp-insert-phylotree':
      return dt('Adds a new phylotree record.');
      break;
    case 'trp-update-phylotree':
      return dt('Updates an existing phylotree record.');
      break;
    case 'trp-delete-phylotree':
      return dt('Deletes an existing phylotree record.');
      break;
  }
}

/**
 * Registers a drush command and constructs the full help for that command
 *
 * @return
 *   And array of command descriptions
 *
 * @ingroup tripal_drush
 */
function tripal_phylotree_drush_command() {
  $items = array();
  $items['trp-insert-phylotree'] = array(
    'description' => dt('Adds a new phylotree record.'),
    'arguments' => array(),
    'examples' => array(),
    'options' => array(
      'name'        => dt('The name of the tree. The name of the tree must be unique.'),
      'description' => dt('A description for the tree. Use quotes.'),
      'analysis'    => dt('The name of the analysis used to generate this tree. This should be the name of an analysis record already present in Chado. Use quotes.'),
      'leaf_type'   => dt('The Sequence Ontology term for the leaf node type of the tree (e.g. polypeptide). If this is a taxonomic the use the word "taxonomy".'),
      'file'        => dt('The full path to the file containing the tree.'),
      'format'      => dt('The format of the input file. Currently, only the "newick" file format is supported.'),
      'dbxref'      => dt('Optional: A database cross-reference of the form DB:ACCESSION. Where DB is the database name, which is already present in Chado, and ACCESSION is the unique identifier for this tree in the remote database.'),
      'sync'        => dt('Optional: Set to 1 if this tree should be synced with Drupal.'),
      'match'       => dt('Optional: Set to "uniquename" if the leaf nodes should be matched with the feature uniquename'),
      'name_re'     => dt('Optional: If the leaf type is NOT "taxonomy", then this option can be a regular expression used pull out the name of the feature from the node label in theintput tree.'),
    ),
  );
  
  $items['trp-update-phylotree'] = array(
    'description' => dt('Adds a new phylotree record. If a new file is provided then the entire existing tree will be rebuilt using the file provided.'),
    'arguments' => array(),
    'examples' => array(),
    'options' => array(
      'phylotree_id' => dt('The unique phylotree ID assigned within Chado for the tree that should be upated.'),
      'name'         => dt('Optional: The name of the tree. The name of the tree must be unique.'),
      'description'  => dt('Optional: A description for the tree. Use quotes.'),
      'analysis'     => dt('Optional: The name of the analysis used to generate this tree. This should be the name of an analysis record already present in Chado. Use quotes.'),
      'leaf_type'    => dt('Optional: The Sequence Ontology term for the leaf node type of the tree (e.g. polypeptide). If this is a taxonomic the use the word "taxonomy".'),
      'file'         => dt('Optional: The full path to the file containing the tree.'),
      'format'       => dt('Optional: The format of the input file. Currently, only the "newick" file format is supported.'),
      'dbxref'       => dt('Optional: A database cross-reference of the form DB:ACCESSION. Where DB is the database name, which is already present in Chado, and ACCESSION is the unique identifier for this tree in the remote database.'),
      'sync'         => dt('Optional: Set to 1 if this tree should be synced with Drupal.'),
      'match'        => dt('Optional: Set to "uniquename" if the leaf nodes should be matched with the feature uniquename'),
      'name_re'      => dt('Optional: If the leaf type is NOT "taxonomy", then this option can be a regular expression used pull out the name of the feature from the node label in theintput tree.'),
    ),
  );
  
  $items['trp-delete-phylotree'] = array(
    'description' => dt('Deletes a phylotree record and it\'s corresponding tree nodes.'),
    'arguments' => array(),
    'examples' => array(),
    'options' => array(
      'phylotree_id' => dt('The unique phylotree ID assigned within Chado for the tree that should be upated.'),
    ),
  );
  return $items;
}

/**
 * Deletes a phylotree record.
 *
 * Executed when 'drush trp-delete-phylotree' is called.
 *
 * @ingroup tripal_drush
 */
function drush_tripal_phylotree_trp_insert_phylotree() {
  $options = array(
    'name'         => drush_get_option('name'),
    'description'  => drush_get_option('description'),
    'analysis'     => drush_get_option('analysis'),
    'leaf_type'    => drush_get_option('leaf_type'),
    'tree_file'    => drush_get_option('file'),
    'format'       => drush_get_option('format'),
    'dbxref'       => drush_get_option('dbxref'),
    'sync'         => drush_get_option('sync'),
    'match'        => drush_get_option('match'),
    'name_re'      => drush_get_option('name_re'),
  );
  if (tripal_insert_phylotree($options)) {
    tripal_phylotree_import_tree_file($options['tree_file'], $options['format'], $options); 
    
    if ($options['sync']) {
      chado_node_sync_records('phylotree', FALSE, FALSE,
        array(), $ids = array($options['phylotree_id']));
    }
  }
}
/**
 * 
 */
function drush_tripal_phylotree_trp_update_phylotree() {
  $options = array(
    'phylotree_id' => drush_get_option('phylotree_id'),
    'name'         => drush_get_option('name'),
    'description'  => drush_get_option('description'),
    'analysis'     => drush_get_option('analysis'),
    'leaf_type'    => drush_get_option('leaf_type'),
    'tree_file'    => drush_get_option('file'),
    'format'       => drush_get_option('format'),
    'dbxref'       => drush_get_option('dbxref'),
    'sync'         => drush_get_option('sync'),
    'match'        => drush_get_option('match'),
    'name_re'      => drush_get_option('name_re'),
  );

  if(tripal_update_phylotree($phylotree_id, $options)) { 
    tripal_phylotree_import_tree_file($options['tree_file'], $options['format'], $options);
    
    if ($options['sync']) {
      chado_node_sync_records('phylotree', FALSE, FALSE,
          array(), $ids = array($options['phylotree_id']));
    }
  }
}
/**
 * Deletes a phylotree record.
 *
 * Executed when 'drush trp-delete-phylotree' is called.
 *
 * @ingroup tripal_drush
 */
function drush_tripal_phylotree_trp_delete_phylotree() {
  $phylotree_id = drush_get_option('phylotree_id');
  
  tripal_delete_phylotree($phylotree_id);
}