<?php
$my_path = path_to_theme();
if(empty($my_path)) {
  // on lis-dev, path_to_theme() is returning empty string, just hardcoding it;
  // this is a problem on lis-stage too, probably all the lis servers. Would
  // be good to figure this out, as the recent rename of the git repos to
  // tripal_phylogeny broke this once, and in general we can't really know where
  // it will be installed
  $my_path = 'sites/all/modules/tripal/tripal_phylogeny';
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
print $phylotree->name;
?>
<br/>
<?php
if( ! empty($phylotree->comment) ) {
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
    
<p>
Click on colored terminal nodes to get more information about legume
genes. Click on internal (white) nodes to view the "genomic context" for
the genes included in the subtree for that node. Click on the root node (gray
with black outline at base of tree) for an option to get to the multiple
sequence alignment behind the tree. The genomic context
viewer shows flanking genes and allows exploration of syntenic regions
from all included legume genomes. You can access the Phytozome family from
which this tree was derived through the "Cross Reference" tab.
</p>
    
<div id="organism-legend-dialog" style="display: none">
    <!-- d3js will add content to this div -->
    <div class="organism-legend">
    </div>
</div>

<div id="phylonode_popup_dialog" style="display: none;">
  <!-- these links are for leaf nodes only -->
  <div><a id="phylonode_feature_link" href="" tabindex="-1"></a></div>
  <div id="linkout"></div>
  <div><a id="phylonode_context_search_link" href="" tabindex="-1"></a></div>
  <div><a id="msa_link" href="" tabindex="-1"></a></div>
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
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.5.6/d3.min.js',
          'external');

drupal_add_js('/'. $my_path . '/theme/js/d3.phylogram.js');
drupal_add_js('/'. $my_path . '/theme/js/organism-bubble-plot.js');
drupal_add_js('/'. $my_path . '/theme/js/phylotree.js');

drupal_add_library('system', 'ui.dialog');
?>


