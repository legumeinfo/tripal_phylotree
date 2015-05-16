<?php
/**
 * @file
 * This file contains the functions used for administration of the module
 *
 */

function tripal_phylotree_admin_phylotrees_listing() {
  $output = '';

  // set the breadcrumb
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  $breadcrumb[] = l('Tripal', 'admin/tripal');
  $breadcrumb[] = l('Chado', 'admin/tripal/chado');
  $breadcrumb[] = l('Phylotrees', 'admin/tripal/extension/tripal_phylotree');
  drupal_set_breadcrumb($breadcrumb);

  // Add the view
  $view = views_embed_view('tripal_phylotree_admin_phylotree','default');
  if (isset($view)) {
    $output .= $view;
  }
  else {
    $output .= '<p>The Phylotree module uses primarily views to provide an '
      . 'administrative interface. Currently one or more views needed for this '
      . 'administrative interface are disabled. <strong>Click each of the following links to '
      . 'enable the pertinent views</strong>:</p>';
    $output .= '<ul>';
      $output .= '<li>'.l('Phylotree View', 'admin/tripal/chado/tripal_phylotree/views/phylotree/enable').'</li>';
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Administrative settings form
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_admin() {
  $form = array();

  $form['tree_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tree Defaults'),
    '#description' => t('You can customize the way trees  by setting the
      following options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $form['tree_settings']['phylogram_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Phylogram Width',
    '#description' => 'Please specify the width in pixels for the phylogram',
    '#default_value' => variable_get('tripal_phylotree_default_phylogram_width', 350),
    '#element_validate' => array(
      'element_validate_integer_positive'
    ),
    '#size' => 5,
  );
  $form['tree_settings']['dendrogram_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Dendrogram Width',
    '#description' => 'Please specify the width in pixels for the circular dendrogram',
    '#default_value' => variable_get('tripal_phylotree_default_dendrogram_width', 500),
    '#element_validate' => array(
      'element_validate_integer_positive'
    ),
    '#size' => 5,
  );
  $form['tree_settings']['bubble_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Bubble Plot Width',
    '#description' => 'Please specify the width in pixels for the organism bubble plot',
    '#default_value' => variable_get('tripal_phylotree_default_bubble_width', 500),
    '#element_validate' => array(
      'element_validate_integer_positive'
    ),
    '#size' => 5,
  );

  $form['tree_settings']['root_node_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Root Node Size',
    '#description' => 'Please specify a size for the root node size. If set to zero, the node will not appear.',
    '#default_value' => variable_get('tripal_phylotree_default_root_node_size', 6),
    '#element_validate' => array(
      'element_validate_integer'
    ),
    '#size' => 3,
  );
  $form['tree_settings']['interior_node_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Interor Node Size',
    '#description' => 'Please specify a size for the interior node size. If set to zero, the node will not appear.',
    '#default_value' => variable_get('tripal_phylotree_default_interior_node_size', 6),
    '#element_validate' => array(
      'element_validate_integer'
    ),
    '#size' => 3,
  );
  $form['tree_settings']['leaf_node_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Leaf Node Size',
    '#description' => 'Please specify a size for the leaf node size. If set to zero, the node will not appear.',
    '#default_value' => variable_get('tripal_phylotree_default_leaf_node_size', 6),
    '#element_validate' => array(
      'element_validate_integer'
    ),
    '#size' => 3,
  );


  // PHYLOTREE NODE TITLES
  // If your module is using the Chado Node: Title & Path API to allow
  // custom titles for your node type then you need to add the
  // configuration form for this functionality.
  $details = array(
    'module' => 'tripal_phylotree',
    'content_type' => 'chado_phylotree',
    // An array of options to use under "Page Titles"
    // the key should be the token and the value should be the human-readable option
    'options' => array(
      '[phylotree.name]' => 'Tree Name Only',
      // there should always be one options matching the unique constraint.
      '[phylotree.phylotree_id]' => 'The Chado ID for Phylotrees'
    ),
    // the token indicating the unique constraint in the options array
    'unique_option' => '[phylotree.phylotree_id]'
  );

  // This call adds the configuration form to your current form
  // This sub-form handles it's own validation & submit
  chado_add_admin_form_set_title($form, $form_state, $details);


  // PHYLOTREE NODE URL
  // Using the Chado Node: Title & Path API
  $details = array(
    'module' => 'tripal_phylotree',
    'content_type' => 'chado_phylotree',
    // An array of options to use under "Page URL"
    // the key should be the token and the value should be the human-readable option
    'options' => array(
      '/tree/[phylotree.name]' => 'Tree Name Only',
      // there should always be one options matching the unique constraint.
      '/tree/[phylotree.phylotree_id]' => 'The Chado ID for Phylotrees',
    )
  );
  // This call adds the configuration form to your current form
  // This sub-form handles it's own validation & submit
  chado_add_admin_form_set_url($form, $form_state, $details);

  return system_settings_form($form);

}



/**
 * Validate the phylotree settings forms
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_admin_validate($form, &$form_state) {

  variable_set('tripal_phylotree_default_phylogram_width', $form_state['values']['phylogram_width']);
  variable_set('tripal_phylotree_default_dendrogram_width', $form_state['values']['dendrogram_width']);
  variable_set('tripal_phylotree_default_bubble_width', $form_state['values']['bubble_width']);

  variable_set('tripal_phylotree_default_root_node_size', $form_state['values']['root_node_size']);
  variable_set('tripal_phylotree_default_interior_node_size', $form_state['values']['interior_node_size']);
  variable_set('tripal_phylotree_default_leaf_node_size', $form_state['values']['leaf_node_size']);

}

