<?php
drupal_add_css( path_to_theme() . '/theme/css/teaser-fix.css');
$node = $variables['node'];
$phylotree = $node->phylotree;
$summary = phylotree_feature_summary($phylotree->phylotree_id);
$feature_count = 0;
?>
<div class="tripal_phylotree-teaser tripal-teaser"> 
  <div class="tripal-phylotree-teaser-title tripal-teaser-title"><?php 
    print l($node->title, "node/$node->nid", array('html' => TRUE));?>
  </div>
  <div class="tripal-phylotree-teaser-text tripal-teaser-text">
    <?php
    foreach($summary as $organism_abbrev => $count) {
      echo $organism_abbrev, ' ';
      ++$feature_count;
    }
    ?>
    (<?php echo $feature_count ?> features)
  </div>
</div>
