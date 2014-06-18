<?php

// remove view and edit tabs!
// http://press.take88.com/drupal-how-to-hide-view-and-edit-tabs-on-node-page/
function tripal_phylotree_menu_local_tasks_alter (&$data, $router_item, $root_path) {
  if(! user_access('administer')) {
    $data['tabs'][0]['count'] = 0;
  }
}


