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
      '#markup' => theme('tripal_phylotree_base', array('node' => $node)),
      '#tripal_toc_id'    => 'base',
      '#tripal_toc_title' => 'Phylogram',
      '#weight' => -100,
    );
    $node->content['tripal_phylotree_radial'] = array(
      '#markup' => theme('tripal_phylotree_radial', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_circ_dendrogram',
      '#tripal_toc_title' => 'Circular Dendrogram',
      '#weight' => -95,
    );
    $node->content['tripal_phylotree_organisms'] = array(
      '#markup' => theme('tripal_phylotree_organisms', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_organisms',
      '#tripal_toc_title' => 'Organisms',
      '#weight' => -90,
    );
    $node->content['tripal_phylotree_go'] = array(
      '#markup' => theme('tripal_phylotree_go', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_go',
      '#tripal_toc_title' => 'Gene Ontology',
      '#weight' => -80,
    );
    $node->content['tripal_phylotree_context'] = array(
      '#markup' => theme('tripal_phylotree_context', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_context',
      '#tripal_toc_title' => 'Context Viewer',
      '#weight' => -70,
    );
    break;
    
  case 'teaser':
    $node->content['tripal_phylotree_teaser'] = array(
      '#markup' => theme('tripal_phylotree_teaser', array('node' => $node)),
    );
    break;
    }
}

/**
 * Implements hook_node_presave(). Acts on all node content types.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_presave($node) {

  switch ($node->type) {
    case 'chado_phylotree':
      // for a form submission the 'phylotreename' field will be set,
      // for a sync, we must pull from the phylotree object
      if (property_exists($node, 'phylotreename')) {
        // set the title
        $node->title = $node->phylotreename;
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
  
  // if there is an phylotree_id in the $node object then this must
  // be a sync so we can skip adding the phylotree as it is already
  // there, although we do need to proceed with the rest of the
  // insert
    
  if (!property_exists($node, 'phylotree_id')) {
    // we aren't supporting editing for phylotrees, yet, so didn't
    // implement inserting of phylotree here!
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
 *  Implements hook_load().
 *
 *  When a node is requested by the user this function is called to allow us
 *  to add auxiliary data to the node object.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_load($nodes) {
  // uid may apparently be a nid, or vid depending how tripal calls
  // this function!  (and how the default view was implemented)
  foreach ($nodes as $nid => $node) {
    
    $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid);
    
    // if the nid does not have a matching record then skip this node.
    // this can happen with orphaned nodes.
    if (!$phylotree_id) {
      continue;
    }
    $phylotree = chado_generate_var('phylotree',
                                     array('phylotree_id' => $phylotree_id));
    $nodes[$nid]->phylotree = $phylotree;
    // Now get the title
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
 * phylotree_json
 *  Get json representation of a phylotree id. See menu item for JSON service 
 *  endpoint.
 *
 * @param int $phylotree_id
 * @return string json
 * @ingroup tripal_phylotree
 */
function phylotree_json($phylotree_id) {
  
  $sql = <<<SQL

   SELECT n.phylonode_id, n.parent_phylonode_id, n.label AS name,
   n.distance AS length, f.feature_id, f.name AS feature_name,
   cvt.name AS cvterm_name, cvt.definition AS cvterm_definition,
   o.organism_id, o.abbreviation, o.common_name, o.genus, o.species,
   cf.nid AS feature_node_id

   FROM chado.phylonode n
   LEFT OUTER JOIN chado.cvterm cvt ON n.type_id = cvt.cvterm_id
   LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id
   LEFT OUTER JOIN chado_feature cf ON cf.feature_id = f.feature_id
   LEFT OUTER JOIN chado.organism o ON f.organism_id = o.organism_id

   WHERE n.phylotree_id = :phylotree_id

SQL;

  
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
    $node = array(
      'phylonode_id' => $phylonode_id,
      'parent_phylonode_id' => (int) $r->parent_phylonode_id,
      'name' => $r->name,
      'length' => (double) $r->length,
      'feature_id' => (int) $r->feature_id,
      'feature_name' => $r->feature_name,
      'cvterm_name' => $r->cvterm_name,
      'cvterm_definition' => $r->cvterm_definition,
      'organism_id' => (int) $r->organism_id,
      'abbreviation' => $r->abbreviation,
      'common_name' => $r->common_name,
      'genus' => $r->genus,
      'species' => $r->species,
      'feature_node_id' => (int) $r->feature_node_id,
    );
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
  
  $sql = 'SELECT o.abbreviation, COUNT(o.abbreviation) AS count '.
    'FROM chado.phylonode n '.
    'LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id '.
    'LEFT OUTER JOIN chado.organism o ON f.organism_id = o.organism_id '.
    'WHERE n.phylotree_id = :phylotree_id '.
    'AND n.feature_id IS NOT NULL '.
    'GROUP BY o.abbreviation';
  $args = array(':phylotree_id' => $phylotree_id);
  $result = chado_query($sql, $args);
  $summary = array();
  foreach($result as $r) {
    $summary[$r->abbreviation] = $r->count;
  }
  return $summary;
}
