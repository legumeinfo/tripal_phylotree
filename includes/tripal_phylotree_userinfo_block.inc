<?php

/**
 *@ Userinfo block and content:
 *  A block (with guide text) at the top of module view with an optional link to detailed help in another drupal path.
 *  The detailed help drupal path is defined here via altering hook_menu and contents populated by a template by altering hook_theme.
 *  These routines avoid embedding any code inside the hook_menu and hook_theme in the .module file.
 *  Hence can stay independent of the main module code.
 */

/**
 * Implement hook_block_info().
 * Define a block to appear at the top of the module(tripal_phyloptree) view/page.
 */
function tripal_phylotree_block_info() {
  $blocks['tripal_phylotree_userinfo_block'] = array(  //the block-id (prefix with module name to avoid namespace conflict)
    'info' => t('User info block at top of tripal_phylotree view.'),
    'status' => TRUE,
    'region' => 'highlighted',
    'weight' => 0,
    'visibility' => BLOCK_VISIBILITY_LISTED, // appears only in listed paths
    'pages' => "search/phylotree", // block appears only in this path
  );
  return $blocks;
}    //end: _block_info


/**
 * Implements hook_block_view().
 *   The block content
 */
function tripal_phylotree_block_view($delta='') {
  $block = array();
  switch ($delta) {
    case 'tripal_phylotree_userinfo_block':  //if this is the block-ID
      //a heading
      $block['subject'] = t('<span align="center" style="font-size:100%">Phylotree: Gene Family Search</span>');
      // block text from f()
      $block['content'] = tripal_phylotree_userinfo_block_contents(); //prefix the fn name with block-id/module name to avoid namespace conflict
      break;
  }
  return $block;
}    //end: _block_view


/**
  * the userinfo block content
  * return string
  */
function tripal_phylotree_userinfo_block_contents() {
    return 'Search for gene families starting from a family ID (e.g. phytozome_10_2.59192412), using words from the family description (e.g., iron homeostasis or chlorophyll binding protein), or using the "count" filters to find families by size or species composition. [<a href="/search/phylotree/userinfo">More help</a>]';
}

/* End: block at the top with user help info*/

/* ************************************************************************ */
/* The detailed help path with contents */


//Userinfo details will be available at this path via hook_menu_alter (Adds another item to pre-existing hook_menu items; has the same effect as including in hook_menu)
function tripal_phylotree_menu_alter(&$items) {
     $items['search/phylotree/userinfo'] = array(
       'title' => 'Userinfo',
       'page callback' => 'userinfo_content',
       'access callback' => TRUE, //visible to anonymous users
     );
}


// The custom template file will be used for userinfo using this theme_registry_alter (has the same effect as adding in hook_theme)
function tripal_phylotree_theme_registry_alter(&$theme_registry) {
//Userinfo template
    // Very Imp that you declare the module path here. Path works differently in hook_theme vs. hook_theme_registry_alter
    $module_path = drupal_get_path('module', 'tripal_phylotree');
    $theme_registry['tripal_phylotree_userinfo'] = array(
      'variables' => array('node' => NULL),
      'template' => 'tripal_phylotree_userinfo',
      'path' => "$module_path/theme/templates", //"'$path/theme/templates'  fails"
    );
}


//Userinfo hook_menu callback to hook_theme
function userinfo_content() {
  return theme('tripal_phylotree_userinfo');
}

/* End: The detailed help path with contents  */
