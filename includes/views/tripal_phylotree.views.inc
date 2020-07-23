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
        'medtr_count' => 'medtr_count',
        'pissa_count' => 'pissa_count',
        'tripr_count' => 'tripr_count',
        'lotja_count' => 'lotja_count',
        'cicar_count' => 'cicar_count',
        'glyma_count' => 'glyma_count',
        'phavu_count' => 'phavu_count',
        'phalu_count' => 'phalu_count',
        'cajca_count' => 'cajca_count',
        'vigun_count' => 'vigun_count',
        'vigra_count' => 'vigra_count',
        'vigan_count' => 'vigan_count',
        'aradu_count' => 'aradu_count',
        'araip_count' => 'araip_count',
        'arahy_count' => 'arahy_count',
        'lupan_count' => 'lupan_count',
        'lupal_count' => 'lupal_count',
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
        'total_count' => array(
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
        'pissa_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'tripr_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'lotja_count' => array(
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
        'phavu_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'phalu_count' => array(
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
        'arahy_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'lupan_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'lupal_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'cajca_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'vigun_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'vigra_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'vigan_count' => array(
            'sortable' => 1,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
        ),
        'cicar_count' => array(
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
    /* Field: phylotree_count: Medtr count */
    $handler->display->display_options['fields']['medtr_count']['id'] = 'medtr_count';
    $handler->display->display_options['fields']['medtr_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['medtr_count']['field'] = 'medtr_count';
    $handler->display->display_options['fields']['medtr_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['medtr_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=medtr';
    $handler->display->display_options['fields']['medtr_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Pissa count */
    $handler->display->display_options['fields']['pissa_count']['id'] = 'pissa_count';
    $handler->display->display_options['fields']['pissa_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['pissa_count']['field'] = 'pissa_count';
    $handler->display->display_options['fields']['pissa_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['pissa_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=pissa';
    $handler->display->display_options['fields']['pissa_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Tripr count */
    $handler->display->display_options['fields']['tripr_count']['id'] = 'tripr_count';
    $handler->display->display_options['fields']['tripr_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['tripr_count']['field'] = 'tripr_count';
    $handler->display->display_options['fields']['tripr_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['tripr_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=tripr';
    $handler->display->display_options['fields']['tripr_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Lotja count */
    $handler->display->display_options['fields']['lotja_count']['id'] = 'lotja_count';
    $handler->display->display_options['fields']['lotja_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['lotja_count']['field'] = 'lotja_count';
    $handler->display->display_options['fields']['lotja_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['lotja_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=lotja';
    $handler->display->display_options['fields']['lotja_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Cicar count */
    $handler->display->display_options['fields']['cicar_count']['id'] = 'cicar_count';
    $handler->display->display_options['fields']['cicar_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['cicar_count']['field'] = 'cicar_count';
    $handler->display->display_options['fields']['cicar_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['cicar_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=cicar.CDCFrontier';
    $handler->display->display_options['fields']['cicar_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Glyma count */
    $handler->display->display_options['fields']['glyma_count']['id'] = 'glyma_count';
    $handler->display->display_options['fields']['glyma_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['glyma_count']['field'] = 'glyma_count';
    $handler->display->display_options['fields']['glyma_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['glyma_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=glyma';
    $handler->display->display_options['fields']['glyma_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Phavu count */
    $handler->display->display_options['fields']['phavu_count']['id'] = 'phavu_count';
    $handler->display->display_options['fields']['phavu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['phavu_count']['field'] = 'phavu_count';
    $handler->display->display_options['fields']['phavu_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['phavu_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=phavu';
    /* Field: phylotree_count: Phalu count */
    $handler->display->display_options['fields']['phalu_count']['id'] = 'phalu_count';
    $handler->display->display_options['fields']['phalu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['phalu_count']['field'] = 'phalu_count';
    $handler->display->display_options['fields']['phalu_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['phalu_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=phalu';
    /* Field: phylotree_count: Cajca count */
    $handler->display->display_options['fields']['cajca_count']['id'] = 'cajca_count';
    $handler->display->display_options['fields']['cajca_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['cajca_count']['field'] = 'cajca_count';
    $handler->display->display_options['fields']['cajca_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['cajca_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=cajca';
    /* Field: phylotree_count: Vigun count */
    $handler->display->display_options['fields']['vigun_count']['id'] = 'vigun_count';
    $handler->display->display_options['fields']['vigun_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['vigun_count']['field'] = 'vigun_count';
    $handler->display->display_options['fields']['vigun_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['vigun_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=vigun';
    /* Field: phylotree_count: Vigra count */
    $handler->display->display_options['fields']['vigra_count']['id'] = 'vigra_count';
    $handler->display->display_options['fields']['vigra_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['vigra_count']['field'] = 'vigra_count';
    $handler->display->display_options['fields']['vigra_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['vigra_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=vigra';
    /* Field: phylotree_count: Vigan count */
    $handler->display->display_options['fields']['vigan_count']['id'] = 'vigan_count';
    $handler->display->display_options['fields']['vigan_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['vigan_count']['field'] = 'vigan_count';
    $handler->display->display_options['fields']['vigan_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['vigan_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=vigan';
    /* Field: phylotree_count: Aradu count */
    $handler->display->display_options['fields']['aradu_count']['id'] = 'aradu_count';
    $handler->display->display_options['fields']['aradu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['aradu_count']['field'] = 'aradu_count';
    $handler->display->display_options['fields']['aradu_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['aradu_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=aradu';
    $handler->display->display_options['fields']['aradu_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Araip count */
    $handler->display->display_options['fields']['araip_count']['id'] = 'araip_count';
    $handler->display->display_options['fields']['araip_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['araip_count']['field'] = 'araip_count';
    $handler->display->display_options['fields']['araip_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['araip_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=araip';
    $handler->display->display_options['fields']['araip_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Arahy count */
    $handler->display->display_options['fields']['arahy_count']['id'] = 'arahy_count';
    $handler->display->display_options['fields']['arahy_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['arahy_count']['field'] = 'arahy_count';
    $handler->display->display_options['fields']['arahy_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['arahy_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=arahy';
    $handler->display->display_options['fields']['arahy_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Lupan count */
    $handler->display->display_options['fields']['lupan_count']['id'] = 'lupan_count';
    $handler->display->display_options['fields']['lupan_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['lupan_count']['field'] = 'lupan_count';
    $handler->display->display_options['fields']['lupan_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['lupan_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=lupan';
    $handler->display->display_options['fields']['lupan_count']['alter']['absolute'] = TRUE;
    /* Field: phylotree_count: Lupal count */
    $handler->display->display_options['fields']['lupal_count']['id'] = 'lupal_count';
    $handler->display->display_options['fields']['lupal_count']['table'] = 'phylotree_count';
    $handler->display->display_options['fields']['lupal_count']['field'] = 'lupal_count';
    $handler->display->display_options['fields']['lupal_count']['alter']['make_link'] = TRUE;
    $handler->display->display_options['fields']['lupal_count']['alter']['path'] = 'search/gene?gene_family=[phylotree_name]&abbreviation=lupal';
    $handler->display->display_options['fields']['lupal_count']['alter']['absolute'] = TRUE;
    /* Filter criterion: phylotree_count: Family ID */
    $handler->display->display_options['filters']['phylotree_name']['id'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phylotree_name']['field'] = 'phylotree_name';
    $handler->display->display_options['filters']['phylotree_name']['group'] = 1;
    $handler->display->display_options['filters']['phylotree_name']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phylotree_name']['expose']['operator_id'] = 'phylotree_name_op';
    $handler->display->display_options['filters']['phylotree_name']['expose']['label'] = 'Family ID';
    $handler->display->display_options['filters']['phylotree_name']['expose']['description'] = '(e.g. [is equal to]: legfed_v1_0.L_2951WH)';
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
    $handler->display->display_options['filters']['total_count']['expose']['label'] = 'Gene count';
    $handler->display->display_options['filters']['total_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['total_count']['expose']['operator'] = 'total_count_op';
    $handler->display->display_options['filters']['total_count']['expose']['identifier'] = 'total_count';
    $handler->display->display_options['filters']['total_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Medtr count */
    $handler->display->display_options['filters']['medtr_count']['id'] = 'medtr_count';
    $handler->display->display_options['filters']['medtr_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['medtr_count']['field'] = 'medtr_count';
    $handler->display->display_options['filters']['medtr_count']['group'] = 1;
    $handler->display->display_options['filters']['medtr_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['medtr_count']['expose']['operator_id'] = 'medtr_count_op';
    $handler->display->display_options['filters']['medtr_count']['expose']['label'] = 'Medtr count';
    $handler->display->display_options['filters']['medtr_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['medtr_count']['expose']['operator'] = 'medtr_count_op';
    $handler->display->display_options['filters']['medtr_count']['expose']['identifier'] = 'medtr_count';
    $handler->display->display_options['filters']['medtr_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Pissa count */
    $handler->display->display_options['filters']['pissa_count']['id'] = 'pissa_count';
    $handler->display->display_options['filters']['pissa_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['pissa_count']['field'] = 'pissa_count';
    $handler->display->display_options['filters']['pissa_count']['group'] = 1;
    $handler->display->display_options['filters']['pissa_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['pissa_count']['expose']['operator_id'] = 'pissa_count_op';
    $handler->display->display_options['filters']['pissa_count']['expose']['label'] = 'Pissa count';
    $handler->display->display_options['filters']['pissa_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['pissa_count']['expose']['operator'] = 'pissa_count_op';
    $handler->display->display_options['filters']['pissa_count']['expose']['identifier'] = 'pissa_count';
    $handler->display->display_options['filters']['pissa_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Tripr count */
    $handler->display->display_options['filters']['tripr_count']['id'] = 'tripr_count';
    $handler->display->display_options['filters']['tripr_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['tripr_count']['field'] = 'tripr_count';
    $handler->display->display_options['filters']['tripr_count']['group'] = 1;
    $handler->display->display_options['filters']['tripr_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['tripr_count']['expose']['operator_id'] = 'tripr_count_op';
    $handler->display->display_options['filters']['tripr_count']['expose']['label'] = 'Tripr count';
    $handler->display->display_options['filters']['tripr_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['tripr_count']['expose']['operator'] = 'tripr_count_op';
    $handler->display->display_options['filters']['tripr_count']['expose']['identifier'] = 'tripr_count';
    $handler->display->display_options['filters']['tripr_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Lotja count */
    $handler->display->display_options['filters']['lotja_count']['id'] = 'lotja_count';
    $handler->display->display_options['filters']['lotja_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['lotja_count']['field'] = 'lotja_count';
    $handler->display->display_options['filters']['lotja_count']['group'] = 1;
    $handler->display->display_options['filters']['lotja_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['lotja_count']['expose']['operator_id'] = 'lotja_count_op';
    $handler->display->display_options['filters']['lotja_count']['expose']['label'] = 'Lotja count';
    $handler->display->display_options['filters']['lotja_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['lotja_count']['expose']['operator'] = 'lotja_count_op';
    $handler->display->display_options['filters']['lotja_count']['expose']['identifier'] = 'lotja_count';
    $handler->display->display_options['filters']['lotja_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Cicar count */
    $handler->display->display_options['filters']['cicar_count']['id'] = 'cicar_count';
    $handler->display->display_options['filters']['cicar_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['cicar_count']['field'] = 'cicar_count';
    $handler->display->display_options['filters']['cicar_count']['group'] = 1;
    $handler->display->display_options['filters']['cicar_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['cicar_count']['expose']['operator_id'] = 'cicar_count_op';
    $handler->display->display_options['filters']['cicar_count']['expose']['label'] = 'Cicar (CDCFrontier) count';
    $handler->display->display_options['filters']['cicar_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['cicar_count']['expose']['operator'] = 'cicar_count_op';
    $handler->display->display_options['filters']['cicar_count']['expose']['identifier'] = 'cicar_count';
    $handler->display->display_options['filters']['cicar_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Glyma count */
    $handler->display->display_options['filters']['glyma_count']['id'] = 'glyma_count';
    $handler->display->display_options['filters']['glyma_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['glyma_count']['field'] = 'glyma_count';
    $handler->display->display_options['filters']['glyma_count']['group'] = 1;
    $handler->display->display_options['filters']['glyma_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['glyma_count']['expose']['operator_id'] = 'glyma_count_op';
    $handler->display->display_options['filters']['glyma_count']['expose']['label'] = 'Glyma count';
    $handler->display->display_options['filters']['glyma_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['glyma_count']['expose']['operator'] = 'glyma_count_op';
    $handler->display->display_options['filters']['glyma_count']['expose']['identifier'] = 'glyma_count';
    $handler->display->display_options['filters']['glyma_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Phavu count */
    $handler->display->display_options['filters']['phavu_count']['id'] = 'phavu_count';
    $handler->display->display_options['filters']['phavu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phavu_count']['field'] = 'phavu_count';
    $handler->display->display_options['filters']['phavu_count']['group'] = 1;
    $handler->display->display_options['filters']['phavu_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phavu_count']['expose']['operator_id'] = 'phavu_count_op';
    $handler->display->display_options['filters']['phavu_count']['expose']['label'] = 'Phavu count';
    $handler->display->display_options['filters']['phavu_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['phavu_count']['expose']['operator'] = 'phavu_count_op';
    $handler->display->display_options['filters']['phavu_count']['expose']['identifier'] = 'phavu_count';
    $handler->display->display_options['filters']['phavu_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Phalu count */
    $handler->display->display_options['filters']['phalu_count']['id'] = 'phalu_count';
    $handler->display->display_options['filters']['phalu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['phalu_count']['field'] = 'phalu_count';
    $handler->display->display_options['filters']['phalu_count']['group'] = 1;
    $handler->display->display_options['filters']['phalu_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['phalu_count']['expose']['operator_id'] = 'phalu_count_op';
    $handler->display->display_options['filters']['phalu_count']['expose']['label'] = 'Phalu count';
    $handler->display->display_options['filters']['phalu_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['phalu_count']['expose']['operator'] = 'phalu_count_op';
    $handler->display->display_options['filters']['phalu_count']['expose']['identifier'] = 'phalu_count';
    $handler->display->display_options['filters']['phalu_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Cajca count */
    $handler->display->display_options['filters']['cajca_count']['id'] = 'cajca_count';
    $handler->display->display_options['filters']['cajca_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['cajca_count']['field'] = 'cajca_count';
    $handler->display->display_options['filters']['cajca_count']['group'] = 1;
    $handler->display->display_options['filters']['cajca_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['cajca_count']['expose']['operator_id'] = 'cajca_count_op';
    $handler->display->display_options['filters']['cajca_count']['expose']['label'] = 'Cajca count';
    $handler->display->display_options['filters']['cajca_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['cajca_count']['expose']['operator'] = 'cajca_count_op';
    $handler->display->display_options['filters']['cajca_count']['expose']['identifier'] = 'cajca_count';
    $handler->display->display_options['filters']['cajca_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Vigun count */
    $handler->display->display_options['filters']['vigun_count']['id'] = 'vigun_count';
    $handler->display->display_options['filters']['vigun_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['vigun_count']['field'] = 'vigun_count';
    $handler->display->display_options['filters']['vigun_count']['group'] = 1;
    $handler->display->display_options['filters']['vigun_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['vigun_count']['expose']['operator_id'] = 'vigun_count_op';
    $handler->display->display_options['filters']['vigun_count']['expose']['label'] = 'Vigun count';
    $handler->display->display_options['filters']['vigun_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['vigun_count']['expose']['operator'] = 'vigun_count_op';
    $handler->display->display_options['filters']['vigun_count']['expose']['identifier'] = 'vigun_count';
    $handler->display->display_options['filters']['vigun_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Vigra count */
    $handler->display->display_options['filters']['vigra_count']['id'] = 'vigra_count';
    $handler->display->display_options['filters']['vigra_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['vigra_count']['field'] = 'vigra_count';
    $handler->display->display_options['filters']['vigra_count']['group'] = 1;
    $handler->display->display_options['filters']['vigra_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['vigra_count']['expose']['operator_id'] = 'vigra_count_op';
    $handler->display->display_options['filters']['vigra_count']['expose']['label'] = 'Vigra count';
    $handler->display->display_options['filters']['vigra_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['vigra_count']['expose']['operator'] = 'vigra_count_op';
    $handler->display->display_options['filters']['vigra_count']['expose']['identifier'] = 'vigra_count';
    $handler->display->display_options['filters']['vigra_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Vigan count */
    $handler->display->display_options['filters']['vigan_count']['id'] = 'vigan_count';
    $handler->display->display_options['filters']['vigan_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['vigan_count']['field'] = 'vigan_count';
    $handler->display->display_options['filters']['vigan_count']['group'] = 1;
    $handler->display->display_options['filters']['vigan_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['vigan_count']['expose']['operator_id'] = 'vigan_count_op';
    $handler->display->display_options['filters']['vigan_count']['expose']['label'] = 'Vigan count';
    $handler->display->display_options['filters']['vigan_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['vigan_count']['expose']['operator'] = 'vigan_count_op';
    $handler->display->display_options['filters']['vigan_count']['expose']['identifier'] = 'vigan_count';
    $handler->display->display_options['filters']['vigan_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Aradu count */
    $handler->display->display_options['filters']['aradu_count']['id'] = 'aradu_count';
    $handler->display->display_options['filters']['aradu_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['aradu_count']['field'] = 'aradu_count';
    $handler->display->display_options['filters']['aradu_count']['group'] = 1;
    $handler->display->display_options['filters']['aradu_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['aradu_count']['expose']['operator_id'] = 'aradu_count_op';
    $handler->display->display_options['filters']['aradu_count']['expose']['label'] = 'Aradu count';
    $handler->display->display_options['filters']['aradu_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['aradu_count']['expose']['operator'] = 'aradu_count_op';
    $handler->display->display_options['filters']['aradu_count']['expose']['identifier'] = 'aradu_count';
    $handler->display->display_options['filters']['aradu_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Araip count */
    $handler->display->display_options['filters']['araip_count']['id'] = 'araip_count';
    $handler->display->display_options['filters']['araip_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['araip_count']['field'] = 'araip_count';
    $handler->display->display_options['filters']['araip_count']['group'] = 1;
    $handler->display->display_options['filters']['araip_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['araip_count']['expose']['operator_id'] = 'araip_count_op';
    $handler->display->display_options['filters']['araip_count']['expose']['label'] = 'Araip count';
    $handler->display->display_options['filters']['araip_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['araip_count']['expose']['operator'] = 'araip_count_op';
    $handler->display->display_options['filters']['araip_count']['expose']['identifier'] = 'araip_count';
    $handler->display->display_options['filters']['araip_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Arahy count */
    $handler->display->display_options['filters']['arahy_count']['id'] = 'arahy_count';
    $handler->display->display_options['filters']['arahy_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['arahy_count']['field'] = 'arahy_count';
    $handler->display->display_options['filters']['arahy_count']['group'] = 1;
    $handler->display->display_options['filters']['arahy_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['arahy_count']['expose']['operator_id'] = 'arahy_count_op';
    $handler->display->display_options['filters']['arahy_count']['expose']['label'] = 'Arahy count';
    $handler->display->display_options['filters']['arahy_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['arahy_count']['expose']['operator'] = 'arahy_count_op';
    $handler->display->display_options['filters']['arahy_count']['expose']['identifier'] = 'arahy_count';
    $handler->display->display_options['filters']['arahy_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Lupan count */
    $handler->display->display_options['filters']['lupan_count']['id'] = 'lupan_count';
    $handler->display->display_options['filters']['lupan_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['lupan_count']['field'] = 'lupan_count';
    $handler->display->display_options['filters']['lupan_count']['group'] = 1;
    $handler->display->display_options['filters']['lupan_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['lupan_count']['expose']['operator_id'] = 'lupan_count_op';
    $handler->display->display_options['filters']['lupan_count']['expose']['label'] = 'Lupan count';
    $handler->display->display_options['filters']['lupan_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['lupan_count']['expose']['operator'] = 'lupan_count_op';
    $handler->display->display_options['filters']['lupan_count']['expose']['identifier'] = 'lupan_count';
    $handler->display->display_options['filters']['lupan_count']['expose']['remember_roles'] = array(
        2 => '2',
        1 => 0,
        4 => 0,
    );
    /* Filter criterion: phylotree_count: Lupal count */
    $handler->display->display_options['filters']['lupal_count']['id'] = 'lupal_count';
    $handler->display->display_options['filters']['lupal_count']['table'] = 'phylotree_count';
    $handler->display->display_options['filters']['lupal_count']['field'] = 'lupal_count';
    $handler->display->display_options['filters']['lupal_count']['group'] = 1;
    $handler->display->display_options['filters']['lupal_count']['exposed'] = TRUE;
    $handler->display->display_options['filters']['lupal_count']['expose']['operator_id'] = 'lupal_count_op';
    $handler->display->display_options['filters']['lupal_count']['expose']['label'] = 'Lupal count';
    $handler->display->display_options['filters']['lupal_count']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['lupal_count']['expose']['operator'] = 'lupal_count_op';
    $handler->display->display_options['filters']['lupal_count']['expose']['identifier'] = 'lupal_count';
    $handler->display->display_options['filters']['lupal_count']['expose']['remember_roles'] = array(
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
