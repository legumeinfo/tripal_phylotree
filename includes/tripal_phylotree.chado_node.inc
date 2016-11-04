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
    $node->content['tripal_phylotree_references'] = array(
      '#markup' => theme('tripal_phylotree_references', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_references',
      '#tripal_toc_title' => 'Cross References',
      '#weight' => -80,
    );
     $node->content['tripal_phylotree_analysis'] = array(
      '#markup' => theme('tripal_phylotree_analysis', array('node' => $node)),
      '#tripal_toc_id'    => 'phylotree_analysis',
      '#tripal_toc_title' => 'Analysis',
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
    $phylotree = chado_generate_var(
      'phylotree',
      array('phylotree_id' => $phylotree_id));
    
    // expand the comment field, tripal omits it by default
    $phylotree = chado_expand_var($phylotree, 'field', 'phylotree.comment');
    
    $nodes[$nid]->phylotree = $phylotree;
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
 * @param int $name
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
    drupal_set_title(sprintf('Gene Family | %s', $name));
    $html = drupal_render(node_view(node_load($nid))); 
    print drupal_render_page($html);
}

/**
 * phylotree_redirect_by_gene
 *
 * Lookup family by gene and redirect to the URL of the gene family
 * tree, with hilite_node set to the matching polypeptide name. Note:
 * we are querying on both the feature.uniquename and feature.name,
 * for flexibility in the different naming conventions. We are pretty
 * sure this a sane thing to do considering how the chado database is
 * loaded, i.e. feature.name should still be unique although the
 * database does not enforce that with a unique key.
 *
 * @return nothing: we redirect the client
 * @ingroup tripal_phylotree
 */
function phylotree_redirect_by_gene() {
    $q = drupal_get_query_parameters();
    $family = $q['family'];
    $gene_name = $q['gene_name'];
    if(empty($gene_name)) {
        // handle the case where the URL ends with /geneName instead
        // of a query string ?gene_name=...
        $url_parts = explode('/', $_SERVER['REQUEST_URI']);
        $gene_name = array_pop($url_parts);
    }
    if(strpos($gene_name, ',') !== FALSE) {
        // a comma separate list was requested
        $gene_names = explode(',', $gene_name);
    }
    else {
        // a single gene name was requested
        $gene_names = array($gene_name);
    }
    //in some contexts, it's easy to make the target family explicit; doing
    //so makes it easier to get the right tree when the gene is not explcitly
    //part of the tree building or we don't store the gene info explicitly
    //(non-legumes).
    if (!$family) {
        // query for the gene family name by gene name (taking only the
        // first gene name from the list)
        $sql = <<<SQL
          SELECT fp.value
          FROM chado.featureprop fp, chado.feature f
          WHERE fp.type_id = (SELECT cvterm_id FROM chado.cvterm
                  WHERE NAME = 'gene family')
          AND (f.uniquename = :gene OR f.name = :gene)
          AND fp.feature_id = f.feature_id
SQL;
        $args = array(':gene' => $gene_names[0]);
        $result = chado_query($sql, $args);
        $family = $result->fetchField();
        if(! $family) {
        drupal_add_http_header('Status', '404 Not Found');
        print("Error: gene family not found\n");
        drupal_exit();
        }
    }
    $pp_names = array();
    foreach ($gene_names as $n) {
        // query for the polypeptide (node name) for each gene name
        $sql = <<<SQL
          SELECT fp.value
          FROM chado.featureprop fp, chado.feature f
          WHERE fp.type_id = (SELECT cvterm_id FROM chado.cvterm 
                              WHERE NAME = 'family representative')
          AND (f.uniquename = :gene OR f.name = :gene)
          AND fp.feature_id = f.feature_id
SQL;
        $args = array(':gene' => $n);
        $result = chado_query($sql, $args);
        $name = $result->fetchField();
        if ($name) {
        $pp_names[] = $name;
        }
        else{
        //probably won't be listed in tree, but at least we'll get the message
        $pp_names[] = $n;
        }
    }
    // multiple family representatives appear to resolve to same
    // polypeptide, so uniquify the array
    $pp_names = array_unique($pp_names);
    $url = sprintf('/chado_phylotree/%s', $family);
    $encoded_names = array();
    foreach(array_unique($pp_names) as $name) {
        $encoded_names[] = rawurlencode($name);
    }
    if(count($encoded_names) == 1) {
        $url .= sprintf('?hilite_node=%s', $encoded_names[0]);
    }
    else {
        $url .= '?hilite_node=' . join(',', $encoded_names);
    }
    // don't use drupal_goto() here because it will urlencode the
    // entire query string, which is not going to work. instead
    // redirect header directly.
    header('Location: '. $url, TRUE, 302);
    drupal_exit();
}

/**
 * phylotree_by_genename_text
 *
 * Lookup family by gene
 *
 * @param string $gene_name
 * @return string $family_name
 * @ingroup tripal_phylotree
 */
function phylotree_by_genename_text($gene) {
    
    $sql = <<<SQL
 SELECT fp.value 
 FROM chado.featureprop fp, chado.feature g
 WHERE g.uniquename = :gene
 AND fp.feature_id=g.feature_id
 AND fp.type_id=(select cvterm_id from chado.cvterm where name='gene family')
SQL;

    $args = array(':gene' => $gene);
    $result = chado_query( $sql, $args );
    $family = $result->fetchField();
    return $family;
}

/**
 * phylotree_by_id
 *  Get json representation of a phylotree id.
 *
 * @param int $phylotree_id
 * @return data structure of tree
 * @ingroup tripal_phylotree
 */
function phylotree_by_id($phylotree_id) {
  
  //getting the name of the tree will allow construction of links to MSAs
  //TODO: might be better to have this function accept the name as its param
  $sql = <<<SQL
  SELECT name from chado.phylotree where phylotree_id = :phylotree_id
SQL;

  $args = array(':phylotree_id' => $phylotree_id);
  $result = chado_query( $sql, $args );
  $phylotree_name = '';
  foreach ($result as $r) {
    $phylotree_name = $r->name;
  }

  // reminder: the chado_xxxx tables are tripal/drupal's linking tables
  $sql = <<<SQL
  SELECT n.phylonode_id, n.parent_phylonode_id, n.label AS name,
  distance AS length, f.feature_id, f.name AS feature_name,
  cvt.name AS cvterm_name, o.organism_id, o.common_name, o.abbreviation,
  o.genus, o.species, cf.nid AS feature_node_id, co.nid AS organism_node_id
  FROM chado.phylonode n
  LEFT OUTER JOIN chado.cvterm cvt ON n.type_id = cvt.cvterm_id
  LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id
  LEFT OUTER JOIN chado_feature cf ON cf.feature_id = f.feature_id
  LEFT OUTER JOIN chado.organism o ON f.organism_id = o.organism_id
  LEFT OUTER JOIN chado_organism co ON co.organism_id = o.organism_id
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
      $root_phylonode_ref['phylotree_name'] = $phylotree_name;
    }
  }
  return $root_phylonode_ref;
}


/**
 * phylotree_json_endpoint
 *  Get json representation of a phylotree id, send http response.
 *
 * @param int $phylotree_id
 * @return string json
 * @ingroup tripal_phylotree
 */
function phylotree_json_endpoint($phylotree_id) {
  $tree = phylotree_by_id($phylotree_id);
  drupal_json_output($tree);
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
  
  $sql = 'SELECT o.abbreviation, COUNT(o.organism_id) AS count '.
    'FROM chado.phylonode n '.
    'LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id '.
    'LEFT OUTER JOIN chado.organism o ON f.organism_id = o.organism_id '.
    'WHERE n.phylotree_id = :phylotree_id '.
    'AND n.feature_id IS NOT NULL '.
    'GROUP BY o.organism_id';
  $args = array(':phylotree_id' => $phylotree_id);
  $result = chado_query($sql, $args);
  $summary = array();
  foreach($result as $r) {
    $summary[$r->abbreviation] = $r->count;
  }
  return $summary;
}

/**
 * phylotree_static_scripts
 * workaround for drupal having terrible support for serving up static content.
 * js/aurelia/scripts is the build folder for `au build` script.
 *
 * @param string filename
 * @ingroup tripal_phylotree
 */
function phylotree_static_scripts($file) {
    $my_path = drupal_get_path('module', 'tripal_phylotree');
    $file = "$my_path/theme/js/aurelia/scripts/$file";
    drupal_add_http_header('Content-Type', 'application/javascript');
    readfile($file);
    drupal_exit();
}
