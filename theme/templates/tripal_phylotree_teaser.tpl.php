<?php
$node = $variables['node'];
$phylotree = $variables['node']->phylotree; ?>

<div class="tripal_phylotree-teaser tripal-teaser"> 
  <div class="tripal-phylotree-teaser-title tripal-teaser-title"><?php 
    print l($node->title, "node/$node->nid", array('html' => TRUE));?>
  </div>
  <div class="tripal-phylotree-teaser-text tripal-teaser-text"><?php 
    print $node->name;
    print "... " . l("[more]", "node/$node->nid"); ?>
  </div>
</div>
