<!-- this div is used by tripal to create a vertical tabs named by
   $node->content (created via _node_view) -->
<div class="tripal_phylotree-data-block-desc tripal-data-block-desc"></div>

<script>
 <?php
 $path_to_theme = path_to_theme();

 drupal_add_css( $path_to_theme . '/theme/css/phylogram.css');
 
 // write a js var having URL of json data source for charting
 $phylotree = $variables['node']->phylotree; 
 printf('var phylotreeDataURL = "?q=chado_phylotree/%d/json";',
       $phylotree->phylotree_id );
// write a js var with path to our theme, for use below by javascript functions.
 printf('var pathToTheme = "%s";', $path_to_theme);
 ?>
 
</script>

<div id="phylogram">
   <!-- d3js will add svg to this div, and remove the loader gif -->
  <img src="<?php print path_to_theme(); ?>/image/ajax-loader.gif"
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
  <div><a id="phylonode_go_link" href="?block=phylotree_go" tabindex="-1">
    view Gene Ontology</a></div>
  <div><a id="phylonode_context_link" href="?block=phylotree_context" tabindex="-1">
    view Context</a></div>
</div>

<?php
/* this template depends on d3js, but i am not putting it into
tripal_phylotree.info scripts[] because that results in the script
getting loaded *on every drupal request*! */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js','external');

drupal_add_js( $path_to_theme . '/theme/js/d3.phylogram.js');
drupal_add_js( $path_to_theme . '/theme/js/organism-bubble-plot.js');
drupal_add_js( $path_to_theme . '/theme/js/phylotree.js');
drupal_add_library('system', 'ui.dialog');
?>


