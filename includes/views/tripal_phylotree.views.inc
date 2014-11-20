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
    $view->name = 'phylotree_search';
    $view->description = 'Allows users to search phylotree data';
    $view->tag = 'tripal search';
    $view->base_table = 'phylotree_count';
    $view->human_name = 'Phylotree Search';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Phylotree Search';
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
    $handler->display->display_options['style_options']['columns'] = array(
        'phylotree_name' => 'phylotree_name',
        'phylotree_comment' => 'phylotree_comment',
        'aradu_count' => 'aradu_count',
        'araip_count' => 'araip_count',
        'glyma_count' => 'glyma_count',
        'medtr_count' => 'medtr_count',
        'phavu_count' => 'phavu_count',
        'total_count' => 'total_count',
    );
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
        'phylotree_name' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'phylotree_comment' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'aradu_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'araip_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'glyma_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'medtr_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'phavu_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'total_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
    );
/* Header: Global: Result summary */
    $handler->display->display_options['header']['result']['id'] = 'result';
    $handler->display->display_options['header']['result']['table'] = 'views';
    $handler->display->display_options['header']['result']['field'] = 'result';
/* Header: Global: Text area */
    $handler->display->display_options['header']['area']['id'] = 'area';
    $handler->display->display_options['header']['area']['table'] = 'views';
    $handler->display->display_options['header']['area']['field'] = 'area';
    $handler->display->display_options['header']['area']['format'] = '1';
/* Field: phylotree_count: Family ID */
    $handler->display->display_options['fields']['phylotree_name']['id'] = 'phylotree_name';
    $handler->display->display_options['fields']['phylotree_name']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['phylotree_name']['field'] = 'phylotree_name';
    $handler->display->display_options['fields']['phylotree_name']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['phylotree_name']['alter']['path'] = 'chado_phylotree/[phylotree_name]';
/* Field: phylotree_count: Description */
    $handler->display->display_options['fields']['phylotree_comment']['id'] = 'phylotree_comment';
    $handler->display->display_options['fields']['phylotree_comment']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['phylotree_comment']['field'] = 'phylotree_comment';
/* Field: phylotree_count: Aradu count */
    $handler->display->display_options['fields']['aradu_count']['id'] = 'aradu_count';
    $handler->display->display_options['fields']['aradu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['aradu_count']['field'] = 'aradu_count';
/* Field: phylotree_count: Araip count */
    $handler->display->display_options['fields']['araip_count']['id'] = 'araip_count';
    $handler->display->display_options['fields']['araip_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['araip_count']['field'] = 'araip_count';
/* Field: phylotree_count: Glyma count */
    $handler->display->display_options['fields']['glyma_count']['id'] = 'glyma_count';
    $handler->display->display_options['fields']['glyma_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['glyma_count']['field'] = 'glyma_count';
/* Field: phylotree_count: Medtr count */
    $handler->display->display_options['fields']['medtr_count']['id'] = 'medtr_count';
    $handler->display->display_options['fields']['medtr_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['medtr_count']['field'] = 'medtr_count';
/* Field: phylotree_count: Phavu count */
    $handler->display->display_options['fields']['phavu_count']['id'] = 'phavu_count';
    $handler->display->display_options['fields']['phavu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['phavu_count']['field'] = 'phavu_count';
/* Field: phylotree_count: Total count */
    $handler->display->display_options['fields']['total_count']['id'] = 'total_count';
    $handler->display->display_options['fields']['total_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['total_count']['field'] = 'total_count';
/* Filter criterion: phylotree_count: Family ID */
    $handler->display->display_options['filters']['phylotree_name']['id'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phylotree_name']['field'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['group'] = 1;
    $handler->display->display_options['filters']['phylotree_name']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phylotree_name']['expose']['operator_id'] = 'phylotree_name_op';
    $handler->display->display_options['filters']['phylotree_name']['expose']['label'] = 'Family ID';
    $handler->display->display_options['filters']['phylotree_name']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['phylotree_name']['expose']['operator'] = 'phylotree_name_op';
    $handler->display->display_options['filters']['phylotree_name']['expose']['identifier'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    $handler->display->display_options['filters']['phylotree_name']['max_length'] = '40';
/* Filter criterion: phylotree_count: Description */
    $handler->display->display_options['filters']['phylotree_comment']['id'] = 'phylotree_comment';
    $handler->display->display_options['filters']['phylotree_comment']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phylotree_comment']['field'] = 'phylotree_comment';
    $handler->display->display_options['filters']['phylotree_comment']['operator'] = 'allwords';
    $handler->display->display_options['filters']['phylotree_comment']['group'] = 1;
    $handler->display->display_options['filters']['phylotree_comment']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phylotree_comment']['expose']['operator_id'] = 'phylotree_comment_op';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['label'] = 'Tree Description';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['phylotree_comment']['expose']['operator'] = 'phylotree_comment_op';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['identifier'] = 'phylotree_comment';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    $handler->display->display_options['filters']['phylotree_comment']['max_length'] = '40';

/* Display: Page */
    $handler = $view->new_display('page', 'Page', 'page');
    $handler->display->display_options['path'] = 'search/phylotree-search';
}
