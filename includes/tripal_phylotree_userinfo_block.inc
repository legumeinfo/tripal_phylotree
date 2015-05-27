<?php

/**
 *@ Userinfo block: A block at the top with userinfo/Help
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
  return '<span style="font-family:times">Search for gene families starting from a Phytozome family ID (phytozome_10_2.59192412) or a word from the gene description/name/function, e.g., iron homeostasis or chlorophyll binding protein.  </span>(<a href="/search/phylotree/userinfo" arget="_blank">More help</a>)';
}

/* End: block at the top with user help info*/



