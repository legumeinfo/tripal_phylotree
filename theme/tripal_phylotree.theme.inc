<?php
/**
 * Implements hook_preprocess_hook()
 * 
 * @param unknown $variables
 */
function tripal_phylotree_preprocess_tripal_phylotree_phylogram(&$variables) {
  $phylotree = $variables['node']->phylotree;
  $module_path = drupal_get_path('module', 'tripal_phylotree');
  
  drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js', 'external');
  
  drupal_add_js("$module_path/theme/js/d3.phylogram.js");
  drupal_add_js("$module_path/theme/js/organism-bubble-plot.js");
  drupal_add_js("$module_path/theme/js/tripal_phylotree.js");
  drupal_add_css("$module_path/theme/css/tripal_phylotree.css");
  
  drupal_add_library('system', 'ui.dialog');
  
  drupal_add_js("
      // var having URL of json data source for charting
      var phylotreeDataURL = \"/chado_phylotree/$phylotree->phylotree_id/json\";
      // var with path to our theme, for use by javascript functions.
      var pathToTheme = \"/$module_path/theme\";
    ",
    'inline'
  );
}

