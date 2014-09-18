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

    // User View ("Search Biological Content") Remember, if you change
    // the name/path of this view, you also want to change it's
    // description in tripal_phylotree_search_biological_data_views()
  
    $view = default_view();
    $view = tripal_make_view_compatible_with_external($view);
    $views[$view->name] = $view;

    // TODO? an admin view could be added here
    //$view = tripal_phylotree_defaultview_admin_phylotrees();
    //$view = tripal_make_view_compatible_with_external($view);
    //$views[$view->name] = $view;
  
    return $views;
}

/**
 * Defines the phylotree user search default view.
 *
 * @ingroup tripal_phylotree
 */
function default_view() {

  $view = new view();
  $view->name = 'tripal_phylotree_user_phylotree';
  $view->description = 'Allows users to search phylotree data';
  $view->tag = 'tripal search';
  $view->base_table = 'phylotree';
  $view->human_name = 'Phylotree User Search';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  $handler = $view->new_display('default', 'phylotrees_all', 'default');
  
  // Now add the fields to the handler.
  // This code is copy/pasted from the views export.
  
/* Display: phylotrees_all */
  $handler = $view->new_display('default', 'phylotrees_all', 'default');
  $handler->display->display_options['title'] = 'Phylotree User Search';
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

  return $view;
}
