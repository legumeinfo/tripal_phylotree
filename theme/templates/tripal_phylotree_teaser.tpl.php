<?php
/* create a short description of the phylotree: list of organism
  abbreviations, number of features, this kind of thing. */

$node = $variables['node'];
$phylotree = $node->phylotree;
drupal_add_css( path_to_theme() . '/theme/css/teaser-fix.css');

$sql = 'SELECT n.phylonode_id, n.parent_phylonode_id, n.label AS name, '.
       'n.distance AS length, f.feature_id, f.name AS feature_name, '.
       'cvt.name AS cvterm_name, cvt.definition AS cvterm_definition, '.
       'o.organism_id, o.abbreviation, o.common_name, o.genus, o.species '.
       'FROM chado.phylonode n '.
       'LEFT OUTER JOIN chado.cvterm cvt ON n.type_id = cvt.cvterm_id '.
       'LEFT OUTER JOIN chado.feature f ON n.feature_id = f.feature_id ' .
       'LEFT OUTER JOIN chado.organism o on f.organism_id = o.organism_id '.
       'WHERE n.phylotree_id = :phylotree_id ' .
       'AND n.feature_id IS NOT NULL';
$args = array(':phylotree_id' => $phylotree->phylotree_id);
$result = chado_query($sql, $args);

// count the organisms and features in the result set
$featureCount = 0;
$organisms = array();
foreach($result as $r) {
  $featureCount++;
  $organisms[ $r->abbreviation ] = true;
}
?>
<div class="tripal_phylotree-teaser tripal-teaser"> 
  <div class="tripal-phylotree-teaser-title tripal-teaser-title"><?php 
    print l($node->title, "node/$node->nid", array('html' => TRUE));?>
  </div>
  <div class="tripal-phylotree-teaser-text tripal-teaser-text">
    <?php 
     foreach($organisms as $key => $val) {
      echo $key, ' ';
     }
     ?>
    (<?php echo $featureCount ?> features)
  </div>
</div>
