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
  <p><b><?php print $phylotree->name ?></b>:
<?php
if( ! empty($phylotree->comment) ) {
  print $phylotree->comment;
}
?>
</p>
</div>

<div>
  <a href="#" class="button phylogeny-help phylogeny-help-btn">
    <img src="/sites/all/modules/tripal/tripal_phylogeny/image/help.png"></img>
    Gene Family Help
  </a>
  <a href="#" class="button organism-legend-show" style="display: none">
    Show Legend
  </a>
  <a href="/lis_gene_families/chado/msa/<?php print $phylotree->name ?>-consensus/">
    View Multiple Sequence Alignment for this gene family
  </a>
</div>

<div id="phylogram">
    <!-- d3js will add svg to this div, and remove the loader gif
     prefix with / for absolute url -->
  <img src="/<?php print $my_path ?>/image/ajax-loader.gif"
       class="phylogram-ajax-loader"/>
</div>

<div id="phylogeny-help-dlg" style="display: none">
  <p style="font-size: 0.9rem">
    Click on colored terminal nodes to get more information about
    legume genes. Click on internal (white) nodes to view the "genomic
    context" for the genes included in the subtree for that
    node. Click on the root node (gray with black outline at base of
    tree) for an option to get to the multiple sequence alignment
    behind the tree. The genomic context viewer shows flanking genes
    and allows exploration of syntenic regions from all included
    legume genomes. You can access the Phytozome family from which
    this tree was derived through the Cross References tab at left.
    You can access the Analysis description and metadata through the
    Analysis tab at left.
  <a href="/search/phylotree/userinfo" target="_blank"
     style="text-decoration: underline">
    View more help on searching gene families...</a>
  </p>
</div>
    
<div id="organism-legend-dialog" style="display: none">
    <!-- d3js will add content to this div -->
    <div class="organism-legend">
    </div>
</div>

<div id="phylonode_popup_dialog" style="display: none;">
</div>

<?php
/* 
 * this template depends on a few javascript libraries, but i am not
 * putting it into tripal_phylotree.info scripts[] because that results
 * in the script getting loaded *on every drupal request* which is wasteful 
 */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.5.9/d3.min.js',
          'external');
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/URI.js/1.17.0/URI.min.js',
              'external');
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/lodash.js/3.10.1/lodash.js',
              'external');
drupal_add_js('/'. $my_path . '/theme/js/d3.phylogram.js');
drupal_add_js('/'. $my_path . '/theme/js/organism-bubble-plot.js');
drupal_add_js('/'. $my_path . '/theme/js/phylotree.js');

drupal_add_library('system', 'ui.dialog');

?>


