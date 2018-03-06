<?php
/**
 * @file
 * Describe default phylotree views
 */

require_once 'handlers/tripal_phylotree.handler.inc';

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

    $view = tripal_phylotree_defaultvalue_user_phylotrees();
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
function tripal_phylotree_defaultvalue_user_phylotrees() {
   $sql_count ="select count(f.feature_id) as count
   from {feature} f left join {cvterm} cvt
   on f.type_id=cvt.cvterm_id
   where cvt.name = :name";

   $args = array(':name' => 'polypeptide_domain');
   $result = chado_query($sql_count, $args)->fetchObject();

   $view = new view();
   $view->name = 'phylotree';
   $view->description = '';
   $view->tag = 'default';
   $view->base_table = 'phylotree_count';
   $view->human_name = 'Phylotree';
   $view->core = 7;
   $view->api_version = '3.0';
   $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

   /* Display: Master */
   $handler = $view->new_display('default', 'Master', 'default');
   $handler->display->display_options['title'] = 'Phylotree';
   $handler->display->display_options['use_more_always'] = FALSE;
   $handler->display->display_options['access']['type'] = 'none';
   $handler->display->display_options['cache']['type'] = 'none';
   $handler->display->display_options['query']['type'] = 'views_query';
   $handler->display->display_options['exposed_form']['type'] = 'basic';
   $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
   $handler->display->display_options['pager']['type'] = 'full';
   $handler->display->display_options['pager']['options']['items_per_page'] = '10';
   $handler->display->display_options['style_plugin'] = 'table';
   $handler->display->display_options['style_options']['columns'] = array(
     'phylotree_name' => 'phylotree_name',
     'phylotree_comment' => 'phylotree_comment',
     'total_count' => 'total_count',
  );

  $organisms = tripal_phylotree_get_orgs();

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
        'total_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
    );

    /* Footer: Global: Result summary */
    $handler->display->display_options['footer']['result']['id'] = 'result';
    $handler->display->display_options['footer']['result']['table'] = 'views';
    $handler->display->display_options['footer']['result']['field'] = 'result';
    $handler->display->display_options['footer']['result']['label'] = 'Result summary';
    $handler->display->display_options['footer']['result']['content'] = 'Displaying @start - @end of @total records found.';
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['area']['id'] = 'area';
    $handler->display->display_options['empty']['area']['table'] = 'views';
    $handler->display->display_options['empty']['area']['field'] = 'area';
    $handler->display->display_options['empty']['area']['label'] = 'No records found';
    $handler->display->display_options['empty']['area']['empty'] = TRUE;
    $handler->display->display_options['empty']['area']['content'] = 'No records found. Please select your search options, then press "Apply".';
    $handler->display->display_options['empty']['area']['format'] = '3';
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
    if ($result->count > 0) {
        /* Field: phylotree_count: Domains/Interpro term */
        $handler->display->display_options['fields']['domains']['id'] = 'domains';
        $handler->display->display_options['fields']['domains']['table'] = 'phylotree_count';
        $handler->display->display_options['fields']['domains']['field'] = 'domains';
        $handler->display->display_options['fields']['domains']['exclude'] = TRUE;

       /*Field: phylotree_count: Domains/Interpro terms for family consensus */
        $handler->display->display_options['fields']['domains_con']['id'] = 'domains_con';
        $handler->display->display_options['fields']['domains_con']['table'] = 'phylotree_count';
        $handler->display->display_options['fields']['domains_con']['field'] = 'domains_con';
        $handler->display->display_options['fields']['domains_con']['exclude'] = TRUE;

     }
    /* Field: phylotree_count: Gene count */
    $handler->display->display_options['fields']['total_count']['id'] = 'total_count';
    $handler->display->display_options['fields']['total_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['total_count']['field'] = 'total_count';
    $handler->display->display_options['fields']['total_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['total_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]';

    /* Field: phylotree_count: Gene count by organism */
    $handler->display->display_options['fields']['counts_by_org']['id'] = 'total_count';
    $handler->display->display_options['fields']['counts_by_org']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['counts_by_org']['field'] = 'counts_by_org';

    /* Filter criterion: phylotree_count: Family ID */
    $handler->display->display_options['filters']['phylotree_name']['id'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phylotree_name']['field'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['group'] = 1;
    $handler->display->display_options['filters']['phylotree_name']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phylotree_name']['expose']['operator_id'] = 'phylotree_name_op';
    $handler->display->display_options['filters']['phylotree_name']['expose']['label'] = 'Family ID';
    $handler->display->display_options['filters']['phylotree_name']['expose']['description'] = '(e.g. [is equal to]: phytozome_10_2.59192412)';
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
    $handler->display->display_options['filters']['phylotree_comment']['expose']['label'] = 'Description';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['description'] = '(e.g. [Contains all words]: iron homeostasis) ';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['phylotree_comment']['expose']['operator'] = 'phylotree_comment_op';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['identifier'] = 'phylotree_comment';
    $handler->display->display_options['filters']['phylotree_comment']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    $handler->display->display_options['filters']['phylotree_comment']['max_length'] = '40';
    if ($result->count > 0) {
        /* Filter criterion: gene: Domains/Interpro term */
       /*
        $handler->display->display_options['filters']['domains']['id'] = 'domains';
        $handler->display->display_options['filters']['domains']['table'] = 'phylotree_count';
        $handler->display->display_options['filters']['domains']['field'] = 'domains';
        $handler->display->display_options['filters']['domains']['operator'] = 'contains';
        $handler->display->display_options['filters']['domains']['group'] = 1;
        $handler->display->display_options['filters']['domains']['exposed'] = TRUE;
        $handler->display->display_options['filters']['domains']['expose']['operator_id'] = 'domains_op';
        $handler->display->display_options['filters']['domains']['expose']['label'] = 'Domains/Interpro term';
        $handler->display->display_options['filters']['domains']['expose']['description'] = '(e.g. [Contains]: PF05659 ,[Contains]:IPR027278)';
        $handler->display->display_options['filters']['domains']['expose']['use_operator'] = TRUE;
        $handler->display->display_options['filters']['domains']['expose']['operator'] = 'domains_op';
        $handler->display->display_options['filters']['domains']['expose']['identifier'] = 'domains';
        $handler->display->display_options['filters']['domains']['expose']['remember_roles'] = array(
            2 => '2',
            1 => 0,
            4 => 0,
        );
        $handler->display->display_options['filters']['domains']['max_length'] = '40';
        */
     /*Filter criterion : Domains/Interpro terms for family consensus*/
        $handler->display->display_options['filters']['domains_con']['id'] = 'domains_con';
        $handler->display->display_options['filters']['domains_con']['table'] = 'phylotree_count';
        $handler->display->display_options['filters']['domains_con']['field'] = 'domains_con';
        $handler->display->display_options['filters']['domains_con']['operator'] = 'contains';
        $handler->display->display_options['filters']['domains_con']['group'] = 1;
        $handler->display->display_options['filters']['domains_con']['exposed'] = TRUE;
        $handler->display->display_options['filters']['domains_con']['expose']['operator_id'] = 'domains_con_op';
        $handler->display->display_options['filters']['domains_con']['expose']['label'] = 'Domains/Interpro terms for family consensus';
        $handler->display->display_options['filters']['domains_con']['expose']['description'] = '(e.g. [Contains all words]: PF00931 PF00560, [Contains]: IPR027278)';
        $handler->display->display_options['filters']['domains_con']['expose']['use_operator'] = TRUE;
        $handler->display->display_options['filters']['domains_con']['expose']['operator'] = 'domains_con_op';
        $handler->display->display_options['filters']['domains_con']['expose']['identifier'] = 'domains_con';
        $handler->display->display_options['filters']['domains_con']['expose']['remember_roles'] = array(
            2 => '2',
            1 => 0,
            4 => 0,
        );
        $handler->display->display_options['filters']['domains_con']['max_length'] = '40';


     }
    /* Filter criterion: phylotree_count: Gene count */
    $handler->display->display_options['filters']['total_count']['id'] = 'total_count';
    $handler->display->display_options['filters']['total_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['total_count']['field'] = 'total_count';
    $handler->display->display_options['filters']['total_count']['group'] = 1;
    $handler->display->display_options['filters']['total_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['total_count']['expose']['operator_id'] = 'total_count_op';
    $handler->display->display_options['filters']['total_count']['expose']['label'] = 'Total gene count';
    $handler->display->display_options['filters']['total_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['total_count']['expose']['operator'] = 'total_count_op';
    $handler->display->display_options['filters']['total_count']['expose']['identifier'] = 'total_count';
    $handler->display->display_options['filters']['total_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );

    /* Display: Phylotree */
    $handler = $view->new_display('page', 'Phylotree', 'page');
    $handler->display->display_options['path'] = 'search/phylotree';

    // End of exported code here

    return $view;

}


function tripal_phylotree_views_pre_view(&$view, &$display_id, &$args) {
  if ($view->name == 'phylotree') {

    $organisms = tripal_phylotree_get_orgs();

    foreach($organisms as $org_id => $org) {
      $view->add_item($view->current_display, 'field', 'phylotree_count', $org_id.'_count', array(
        'label' => $organisms[$org_id]['genus'].' '.$organisms[$org_id]['species'].' count',
        'table' => 'phylotree_count',
        'field' => $org_id.'_count',
        'counts_by_org' => '[counts_by_org]',
        'alter' => array(
          'make_link' => TRUE,
          'path' => 'chado_phylotree/[phylotree_name]'
        )
      ), $org_id.'_count');
    }
  }
}

function tripal_phylotree_views_pre_render(&$view) {
  // Hide field containing json data
  if ($view->name == 'phylotree') {
    $view->field['counts_by_org']->options['exclude'] = TRUE;
  }
}

function tripal_phylotree_views_data_alter(&$data) {

  $organisms = tripal_phylotree_get_orgs();

  foreach($organisms as $org_id => $org) {
    $data['phylotree_count'][$org_id.'_count'] = array(
        'title' => $organisms[$org_id]['genus'].' '.$organisms[$org_id]['species'].' count',
        'real field' => 'counts_by_org',
        'field' => array(
          'handler' => 'tripal_phylotree_views_handler_field_org_count',
          // As these fields are auto generated, there's no way to sort it at sql level
          // https://drupal.stackexchange.com/questions/150169/sort-handler-for-pseudo-field-in-views
          'click sortable' => FALSE,
        ),
    );
  }
}

function tripal_phylotree_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'tripal_phylotree') . '/views/handlers',
    ),
    'handlers' => array(
      'tripal_phylotree_views_handler_field_org_count' => array(
        'parent' => 'views_handler_field',
      ),
    )
  );
}

function tripal_phylotree_get_orgs() {
    // List organisms with at least 1 feature in a tree
    $sql = 'SELECT o.abbreviation, o.genus, o.species, o.organism_id '.
      'FROM chado.phylonode n '.
      'LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id '.
      'LEFT OUTER JOIN chado.organism o ON f.organism_id = o.organism_id '.
      'WHERE n.feature_id IS NOT NULL '.
      'GROUP BY o.organism_id';
    $results = chado_query($sql, array());
    $organisms = array();
    foreach($results as $r) {
      $organisms[$r->organism_id] = array('genus' => $r->genus, 'species' => $r->species, 'abbr' => $r->abbreviation);
    }

    return $organisms;
}
