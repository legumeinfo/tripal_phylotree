<!-- this div is used by tripal to create a vertical tabs named by
   $node->content (created via _node_view) -->
<div class="tripal_phylotree-data-block-desc tripal-data-block-desc"></div>

<script>
 <?php
 $path_to_theme = path_to_theme();
 
 // write a js var having URL of json data source for charting
 $phylotree = $variables['node']->phylotree; 
 printf('var phylogramJsonUrl = "?q=chado_phylotree/%d/json";',
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

<?php
/* this template depends on d3js, but i am not putting it into
tripal_phylotree.info scripts[] because that results in the script
getting loaded *on every drupal request*! */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js','external');

drupal_add_js( $path_to_theme . '/js/d3.phylogram.js');
drupal_add_js( $path_to_theme . '/js/organism-bubble-plot.js');
drupal_add_js( $path_to_theme . '/js/phylotree.js');
?>
