<?php
$my_path = path_to_theme();
if(empty($my_path)) {
  // on lis-dev, path_to_theme() is returning empty string, just hardcoding it
  $my_path = 'sites/all/modules/tripal/tripal_phylotree';
}
drupal_add_css( $my_path . '/theme/css/phylogram.css');
$phylotree = $variables['node']->phylotree;
?>
<script>
<?php
// write js var having URL of json data source for charting
printf('var phylotreeDataURL = "/chado_phylotree/%d/json";',
  $phylotree->phylotree_id );
// write js var with path to our theme, for use below by javascript functions.
// prefix path to theme with / because drupal returns it as a relative URL.
printf('var pathToTheme = "/%s";', $my_path);
?>
</script>

<div class="tripal_phylotree-data-block-desc tripal-data-block-desc">
<?php
/*
 * note: if the comment field contains newick formatted tree data, then the
 * database content needs to be updated with a relevant description (per adf)
 */
if( ! empty($phylotree->comment) && $phylotree->comment[0] != '(') {
  // doesnt appear to be newick data starting with paren
  print $phylotree->comment;
}
?>
</div>

<div id="phylogram">
    <!-- d3js will add svg to this div, and remove the loader gif
     prefix with / for absolute url -->
  <img src="/<?php print $my_path ?>/image/ajax-loader.gif"
       class="phylogram-ajax-loader"/>
</div>

<p>Phylogenies are essential to any analysis of evolutionary gene
  sequences collected across a group of organisms. A <b>phylogram</b>
  is a phylogenetic tree that has branch spans proportional to the
  amount of character change.
</p>

<div id="phylonode_popup_dialog" style="display: none;">
  <!-- these links are for leaf nodes only -->
  <div><a id="phylonode_feature_link" href="" tabindex="-1"></a></div>
  <div><a id="phylonode_organism_link" href="" tabindex="-1"></a></div>
  
  <!-- these links are for interior nodes only -->
  <div><a id="phylonode_go_link" href="?block=phylotree_go" class="tripal_toc_list_item_link"  tabindex="-1">
    View Gene Ontology</a></div>
  <!-- removed tripal_toc_list_item_link from context link, at least while it is a link off the site -->
  <div><a id="phylonode_context_link" href="?block=phylotree_context" class="" tabindex="-1">
    View Context</a></div>
</div>

<?php
/* this template depends on d3js, but i am not putting it into
tripal_phylotree.info scripts[] because that results in the script
getting loaded *on every drupal request*! */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js',
          'external');

drupal_add_js('/'. $my_path . '/theme/js/d3.phylogram.js');
drupal_add_js('/'. $my_path . '/theme/js/organism-bubble-plot.js');
drupal_add_js('/'. $my_path . '/theme/js/phylotree.js');

drupal_add_library('system', 'ui.dialog');
?>


