<!-- this div is used by tripal to create a vertical tabs named by
   $node->content (created via _node_view) -->
<div class="tripal_phylotree-data-block-desc tripal-data-block-desc"></div>

<?php

/* this template depends on d3js, but i am not putting it into
tripal_phylotree.info scripts[] because that results in the script
getting loaded *on every drupal request*! */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js','external');
drupal_add_js( path_to_theme() . '/js/d3.phylogram.js' );

?>

<div id="phylogram">
   <!-- d3js will add svg to this div, and remove the loader gif -->
  <img src="<?php print path_to_theme(); ?>/image/ajax-loader.gif"
       class="phylogram-ajax-loader"/>
</div>

<p>Phylogenies are essential to any analysis of evolutionary gene
  sequences collected across a group of
  organisms. A <strong>phylogram</strong> is a phylogenetic tree that
  has branch spans proportional to the amount of character change.
</p>
  
<script>
 <?php
 $phylotree = $variables['node']->phylotree; 
 print sprintf('var phylogramJsonUrl = "?q=chado_phylotree/%d/json";',
               $phylotree->phylotree_id );
 ?>
 
 (function ($) {
   // drupal7 jquery outer context
   // use jquery $() here...

   $(document).ready( function () {
     $.getJSON( phylogramJsonUrl, function(data) {
       
       d3.phylogram.build('#phylogram', data, {
	 width: 550,
	 height: 400
       });

       d3.phylogram.buildRadial('#phylotree-radial-graph', data, {
	 width: 550
       });

       $('.phylogram-ajax-loader').remove();       
     });
  // jquery ready     
   });
   // drupal jquery context
})(jQuery);
 
</script>
