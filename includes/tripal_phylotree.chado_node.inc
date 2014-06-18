<?php

/**
 * @file
 * Implements the phylotree node content type
 */

/**
 * Implements hook_node_info().
 *
 * Provide information to drupal about the node types that we're creating
 * in this module
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
 *  Implements hook_form().
 *
 * When editing or creating a new node of type 'chado_phylotree' we need
 *  a form.  This function creates the form that will be used for this.
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_form($node, &$form_state) {
  $form = array();
  // TODO!
  return $form;
}

/**
 * Implements hook_validate().
 *
 * Validates submission of form when adding or updating a phylotree node
 *
 * @ingroup tripal_phylotree
 */
function chado_phylotree_validate($node, $form, &$form_state) {
  // removed, because not supporting editing or inserting from the gui yet.
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
    dd('error: node missing phylotree_id' + $node);
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
    // we aren't supporting editing for phylotrees, yet, so didn't
    // implement updating of phylotree here!
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
    // get the feature details from chado
    $phylotree_id = chado_get_id_from_nid('phylotree', $node->nid);

    // if the nid does not have a matching record then skip this node.
    // this can happen with orphaned nodes.
    if (!$phylotree_id) {
      continue;
    }

    $values = array('phylotree_id' => $phylotree_id);
    $options = array( 'include_fk' =>
       array( 'analysis_id' => 1,  'type_id' => 1, 'dbxref_id' => 1));
    
    $phylotree = chado_generate_var('phylotree', $values, $options);

    // the comment field is a text field so we need to expand it
    $phylotree = chado_expand_var($phylotree, 'field', 'phylotree.comment');

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
  
//  chado_query("DELETE FROM {phylotreeprop} WHERE phylotree_id = :phylotree_id", array(':phylotree_id' => $phylotree_id));
  
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
 * Implements hook_node_view(). Acts on all content types
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_node_view($node, $view_mode, $langcode) {
  switch ($node->type) {
  case 'chado_phylotree':
    if ($view_mode == 'full') {

      $node->content['tripal_phylotree_base'] = array(
        '#markup' => theme('tripal_phylotree_base', array('node' => $node)),
        '#tripal_toc_id'    => 'base',
        '#tripal_toc_title' => 'Overview',
        '#weight' => -100,
      );

      /*
      $node->content['tripal_phylotree_comment'] = array(
        '#markup' => theme('tripal_phylotree_comment', array('node' => $node)),
        '#tripal_toc_id'    => 'comment',
        '#tripal_toc_title' => 'Comment',
        '#weight' => -50,
      );
      $node->content['tripal_phylotree_dbxref'] = array(
        '#markup' => theme('tripal_phylotree_dbxref', array('node' => $node)),
        '#tripal_toc_id'    => 'dbxref',
        '#tripal_toc_title' => 'Reference',
        '#weight' => -20,
      );
       $node->content['tripal_phylotree_phylonodes'] = array(
        '#markup' => theme('tripal_phylotree_phylonodes', array('node' => $node)),
        '#tripal_toc_id'    => 'phylonodes',
        '#tripal_toc_title' => 'phylonodes',
        '#weight' => -10,
      );
      */
    }
    
    if ($view_mode == 'teaser') {
      $node->content['tripal_phylotree_teaser'] = array(
        '#markup' => theme('tripal_phylotree_teaser', array('node' => $node)),
      );
    }
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
 */
function chado_phylotree_chado_node_default_title_format() {
  return '[phylotree.name]';
}
