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

  // EXPLANATION:  Typically for all Tripal modules the home administrative
  // page for the module contains a search form to help the adminstrator
  // locate records.  The following phylotree code adds a default View to
  // the page
  /*
  // Add the view
  $view = views_embed_view('tripal_phylotree_admin_phylotrees','default');
  if (isset($view)) {
    $output .= $view;
  }
  else {
    $output .= '<p>The Tripal Phylotree Module uses primarily views to provide an '
      . 'administrative interface. Currently one or more views needed for this '
      . 'administrative interface are disabled. <strong>Click each of the following links to '
      . 'enable the pertinent views</strong>:</p>';
    $output .= '<ul>';
      $output .= '<li>'.l('Phylotree Admin', 'admin/tripal/chado/tripal_phylotree/views/phylotrees/enable').'</li>';
    $output .= '</ul>';
  }
  */
  $output = 'TODO: typically a search view goes here. See Sync and Settings tabs above-right.';

  return $output;
}

/**
 * Administrative settings form
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_admin() {
  $form = array();

  $form['nothing'] = array(
    '#markup' => t('There are currently no settings to configure.')
  );

  /*
  // If your module is using the Chado Node: Title & Path API to allow
  // custom titles for your node type then you need to add the
  // configuration form for this functionality.
  $details = array(
    'module' => 'tripal_phylotree', // the name of the MODULE
                                    // implementing the content type
    
    // An array of options to use under "Page Titles" the key should
    // be the token and the value should be the human-readable
    // option
    'options' => array(
      '[phylotree.name]' => 'Phylotree Name Only',
        // there should always be one option matching the unique
        // constraint.  If you have a more human-readable constraint,
        // then that is preferrable.  See the tripal feature module
        // for a good example of this.
      '[phylotree.phylotree_id]' => 'Unique Contraint: The Chado ID for Phylotrees'
    ),
    
    // the token indicating the unique constraint in the options array
    'unique_option' => '[phylotree.phylotree_id]'
  );
  
  // This call adds the configuration form to your current form
  // This sub-form handles it's own validation & submit
  chado_add_admin_form_set_title($form, $form_state, $details);


  return system_settings_form($form);

  */
  return $form;
}



/**
 *
 * @ingroup tripal_phylotree
 */
function tripal_phylotree_admin_validate($form, &$form_state) {

}

