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
drupal_add_css($my_path . '/theme/css/phylogram.css');
drupal_add_css('//cdn.biojs.net/msa/latest/msa.min.gz.css', 'external');
drupal_add_css(
    '//cdnjs.cloudflare.com/ajax/libs/hopscotch/0.2.5/css/hopscotch.min.css',
    'external');

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

printf('var familyName = "%s";', $phylotree->name);
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
    <img src="/sites/all/modules/tripal/tripal_phylogeny/image/help.png"/>
    Gene Family Help
  </a>
  <a id="msa-toggle" href="#" class="button" onclick="phylogeny_msa.toggle()">
    View Multiple Sequence Alignment (MSA)
  </a>
  <a href="#" class="button organism-legend-show" style="display: none">
    Show Legend
  </a>
</div>

<div id="msa-viewer-wrapper" style="display: none; padding: 10px;">
    <div id="msa-spinner">
    <img src="/<?php print $my_path ?>/image/ajax-loader.gif"/>
    </div>
    <div id="msa-viewer">
        <!-- biojs msa viewer will load div -->
    </div>
</div>

<div id="phylogram">
    <!-- d3js will add svg to this div, and remove the loader gif
     prefix with / for absolute url -->
  <img src="/<?php print $my_path ?>/image/ajax-loader.gif"
       class="phylogram-ajax-loader"/>
</div>

<div style="display: none" id="phylogeny-help-dlg">
  <p style="font-size: 0.9rem" id="hopscotch-tour-link">
    <a href="#"
       onclick="showHopscotchTour(); jQuery('#phylogeny-help-dlg').dialog('close');"
       style="text-decoration: underline">
    View Quick Tour
    </a>
  </p>
    
  <p style="font-size: 0.9rem">
    <a href="/search/phylotree/userinfo" target="_blank"
    style="text-decoration: underline">
    View complete help on searching gene families...</a>
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
drupal_add_js('//cdn.biojs.net/msa/latest/msa.min.gz.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.5.16/d3.min.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/URI.js/1.17.1/URI.min.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/lodash.js/3.10.1/lodash.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/hopscotch/0.2.5/js/hopscotch.min.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/js-cookie/2.1.0/js.cookie.min.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/chroma-js/1.1.1/chroma.min.js',
              array(
                  'type' => 'external',
                  'group' => JS_LIBRARY,
              ));
drupal_add_js('/'. $my_path . '/theme/js/taxon-chroma.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));
drupal_add_js('/'. $my_path . '/theme/js/d3.phylogram.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));
drupal_add_js('/'. $my_path . '/theme/js/organism-bubble-plot.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));
drupal_add_js('/'. $my_path . '/theme/js/phylotree.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));
drupal_add_js('/'. $my_path . '/theme/js/tour.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));
drupal_add_js('/'. $my_path . '/theme/js/phylogeny-msa.js',
              array(
                  'type' => 'file',
                  'group' => JS_DEFAULT,
              ));

drupal_add_library('system', 'ui.dialog');

?>
