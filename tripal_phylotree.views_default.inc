<?php
/**
 * @file
 * Describe default phylotree views
 */

/**
 * Implements hook_views_default_views().
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_views_default_views() {
  $views = array();

  // User View ("Search Biological Content") 
  // Remember, if you change the name/path of this view, you also want to 
  // change it's description in tripal_phylotree_search_biological_data_views()
  $view = tripal_phylotree_defaultvalue_user_phylotrees();
  $view = tripal_make_view_compatible_with_external($view);
  $views[$view->name] = $view;

  /// Admin view
  $view = tripal_phylotree_defaultview_admin_phylotree();
  $view = tripal_make_view_compatible_with_external($view);
  $views[$view->name] = $view;

  return $views;
}

/**
 * The default phylotree administration view.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_defaultview_admin_phylotree() {
  $view = new view();
  $view->name = 'tripal_phylotree_admin_phylotree';
  $view->description = 'DO NOT DISABLE';
  $view->tag = 'tripal admin';
  $view->base_table = 'phylotree';
  $view->human_name = 'Phylotree Admin';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'phylotree_all', 'default');
  $handler->display->display_options['title'] = 'Phylogeny';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access chado_phylotree content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Action Links */
  $handler->display->display_options['header']['action_links_area']['id'] = 'action_links_area';
  $handler->display->display_options['header']['action_links_area']['table'] = 'views';
  $handler->display->display_options['header']['action_links_area']['field'] = 'action_links_area';
  $handler->display->display_options['header']['action_links_area']['label'] = 'Action Links';
  $handler->display->display_options['header']['action_links_area']['link-1'] = array(
    'label-1' => 'Add Phylogenetic Tree',
    'path-1' => 'node/add/tripal-phylotree',
  );
  $handler->display->display_options['header']['action_links_area']['link-2'] = array(
    'label-2' => '',
    'path-2' => '',
  );
  $handler->display->display_options['header']['action_links_area']['link-3'] = array(
    'label-3' => '',
    'path-3' => '',
  );
  $handler->display->display_options['header']['action_links_area']['link-4'] = array(
    'label-4' => '',
    'path-4' => '',
  );
  /* Relationship: Phylotree: Phylotree => Node */
  $handler->display->display_options['relationships']['phylotree_id']['id'] = 'phylotree_id';
  $handler->display->display_options['relationships']['phylotree_id']['table'] = 'chado_phylotree';
  $handler->display->display_options['relationships']['phylotree_id']['field'] = 'phylotree_id';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'phylotree_id';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Chado Phylotree: Phylotree Id */
  $handler->display->display_options['fields']['phylotree_id']['id'] = 'phylotree_id';
  $handler->display->display_options['fields']['phylotree_id']['table'] = 'phylotree';
  $handler->display->display_options['fields']['phylotree_id']['field'] = 'phylotree_id';
  $handler->display->display_options['fields']['phylotree_id']['exclude'] = TRUE;
  /* Field: Chado Phylotree: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'phylotree';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Tree Name';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'node/[nid]';
  /* Field: Chado Phylotree: Phylotree Id */
  $handler->display->display_options['fields']['phylotree_id_1']['id'] = 'phylotree_id_1';
  $handler->display->display_options['fields']['phylotree_id_1']['table'] = 'phylotree';
  $handler->display->display_options['fields']['phylotree_id_1']['field'] = 'phylotree_id';
  $handler->display->display_options['fields']['phylotree_id_1']['exclude'] = TRUE;
  /* Field: Chado Analysis: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'analysis';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Analysis';
  /* Field: Chado Cvterm: Name */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'cvterm';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  $handler->display->display_options['fields']['name_2']['label'] = 'Leaf Node Type';
  /* Field: Chado Phylotree: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'phylotree';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  $handler->display->display_options['fields']['comment']['label'] = 'Description';
  /* Sort criterion: Chado Phylotree: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'phylotree';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Chado Phylotree: Name */
  $handler->display->display_options['filters']['name_1']['id'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['table'] = 'phylotree';
  $handler->display->display_options['filters']['name_1']['field'] = 'name';
  $handler->display->display_options['filters']['name_1']['group'] = 1;
  $handler->display->display_options['filters']['name_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator_id'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['label'] = 'Tree Name';
  $handler->display->display_options['filters']['name_1']['expose']['description'] = 'Provide the name of a phylogenetic tree';
  $handler->display->display_options['filters']['name_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name_1']['expose']['operator'] = 'name_1_op';
  $handler->display->display_options['filters']['name_1']['expose']['identifier'] = 'name_1';
  $handler->display->display_options['filters']['name_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['name_1']['max_length'] = '40';
  /* Filter criterion: Chado Analysis: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'analysis';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['value'] = array();
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Analysis';
  $handler->display->display_options['filters']['name']['expose']['description'] = 'Select one or more analyses to filter trees.';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['name']['values_form_type'] = 'select';
  $handler->display->display_options['filters']['name']['select_multiple'] = TRUE;
  $handler->display->display_options['filters']['name']['select_optional'] = TRUE;
  $handler->display->display_options['filters']['name']['max_length'] = '40';
  /* Filter criterion: Chado Cvterm: Name */
  $handler->display->display_options['filters']['name_2']['id'] = 'name_2';
  $handler->display->display_options['filters']['name_2']['table'] = 'cvterm';
  $handler->display->display_options['filters']['name_2']['field'] = 'name';
  $handler->display->display_options['filters']['name_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_2']['expose']['operator_id'] = 'name_2_op';
  $handler->display->display_options['filters']['name_2']['expose']['label'] = 'Leaf node type';
  $handler->display->display_options['filters']['name_2']['expose']['description'] = 'Provide the type of leaf node (e.g. polypeptide, taxonomic, etc).';
  $handler->display->display_options['filters']['name_2']['expose']['operator'] = 'name_2_op';
  $handler->display->display_options['filters']['name_2']['expose']['identifier'] = 'name_2';
  $handler->display->display_options['filters']['name_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['name_2']['max_length'] = '40';
  /* Filter criterion: Chado Phylotree: Comment */
  $handler->display->display_options['filters']['comment']['id'] = 'comment';
  $handler->display->display_options['filters']['comment']['table'] = 'phylotree';
  $handler->display->display_options['filters']['comment']['field'] = 'comment';
  $handler->display->display_options['filters']['comment']['exposed'] = TRUE;
  $handler->display->display_options['filters']['comment']['expose']['operator_id'] = 'comment_op';
  $handler->display->display_options['filters']['comment']['expose']['label'] = 'Description';
  $handler->display->display_options['filters']['comment']['expose']['description'] = 'Find a tree using words in the tree description.';
  $handler->display->display_options['filters']['comment']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['comment']['expose']['operator'] = 'comment_op';
  $handler->display->display_options['filters']['comment']['expose']['identifier'] = 'comment';
  $handler->display->display_options['filters']['comment']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/tripal/chado/tripal_phylotree/phylotree';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Phylogenetic Tree';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  
  return $view;
}
/**
 * Defines the phylotree user search default view.
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_defaultvalue_user_phylotrees() {

  $view = new view();
  $view->name = 'tripal_phylotree_user_phylotree';
  $view->description = 'Allows users to search phylotree data';
  $view->tag = 'tripal search';
  $view->base_table = 'phylotree';
  $view->human_name = 'Phylogeny User Search';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  $handler = $view->new_display('default', 'phylotrees_all', 'default');

  // Now add the fields to the handler.
  // This code is copy/pasted from the views export.

  /* Display: phylotrees_all */
  $handler = $view->new_display('default', 'phylotrees_all', 'default');
  $handler->display->display_options['title'] = 'Phylogeny User Search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access chado_phylotree content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'Search for a gene family by submitting annotation terms, PFAM IDs, or GO terms in the "Tree Description" field (e.g.cytochrome, IPR008914, homeobox leucine zipper, or GO:0003677) ... or by "Family ID" (e.g. 54689426; these IDs correspond to the Phytozome v10 gene families).
';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Chado Phylotree: Phylotree Id */
  $handler->display->display_options['fields']['phylotree_id']['id'] = 'phylotree_id';
  $handler->display->display_options['fields']['phylotree_id']['table'] = 'phylotree';
  $handler->display->display_options['fields']['phylotree_id']['field'] = 'phylotree_id';
  $handler->display->display_options['fields']['phylotree_id']['exclude'] = TRUE;
  /* Field: Chado Phylotree: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'phylotree';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Family ID';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = TRUE;
  /* Field: Chado Phylotree: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'phylotree';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  $handler->display->display_options['fields']['comment']['label'] = 'Description';
  $handler->display->display_options['fields']['comment']['alter']['max_length'] = '240';
  $handler->display->display_options['fields']['comment']['alter']['trim'] = TRUE;
  /* Filter criterion: Chado Phylotree: Comment */
  $handler->display->display_options['filters']['comment']['id'] = 'comment';
  $handler->display->display_options['filters']['comment']['table'] = 'phylotree';
  $handler->display->display_options['filters']['comment']['field'] = 'comment';
  $handler->display->display_options['filters']['comment']['operator'] = 'allwords';
  $handler->display->display_options['filters']['comment']['group'] = 1;
  $handler->display->display_options['filters']['comment']['exposed'] = TRUE;
  $handler->display->display_options['filters']['comment']['expose']['operator_id'] = 'comment_op';
  $handler->display->display_options['filters']['comment']['expose']['label'] = 'Tree Description';
  $handler->display->display_options['filters']['comment']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['comment']['expose']['operator'] = 'comment_op';
  $handler->display->display_options['filters']['comment']['expose']['identifier'] = 'comment';
  $handler->display->display_options['filters']['comment']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['comment']['expose']['remember_roles'] = array(
      2 => '2',
      1 => '1',
      3 => '3',
  );
  /* Filter criterion: Chado Phylotree: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'phylotree';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Family ID';
  $handler->display->display_options['filters']['name']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
      2 => '2',
      1 => '1',
      3 => '3',
  );
  $handler->display->display_options['filters']['name']['max_length'] = '30';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'chado/phylotree';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Phylogeny';
  $handler->display->display_options['menu']['description'] = 'Gene trees, species trees, etc.';
  $handler->display->display_options['menu']['weight'] = '-10';
  
  return $view;
}
