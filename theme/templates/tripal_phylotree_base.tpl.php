<?php
$my_path = path_to_theme();
if(empty($my_path)) {
  // on lis-dev, path_to_theme() is returning empty string. this is a
  // problem on lis-stage too, probably all the lis servers. Would be
  // good to figure this out, as the recent rename of the git repos to
  // tripal_phylogeny broke this once, and in general we can't really
  // know where it will be installed. workaround: hardcode the path to
  // the theme.
  $my_path = 'sites/all/modules/tripal/tripal_phylogeny';
  // note: there is no leading '/' because that is the format used by
  // path_to_theme(), even though this is effectively an absolute url.
}
$phylotree = $variables['node']->phylotree;
?>

<script>
<?php
// write js var with gene family name
printf("var FAMILY_NAME = '%s';\n", $phylotree->name);

// write js var with path to our theme, for use below by javascript functions.
// prefix path to theme with / because drupal returns it as a relative URL.
printf("var THEME_PATH = '/%s';\n", $my_path);

// write js var having URL of json and gff data sources
printf("var API = {
  tree: \"/chado_phylotree/%s/json\",
  msa: \"/lis_gene_families/chado/msa/%s-consensus/download/\"
};\n",
       $phylotree->phylotree_id,
       $phylotree->name
);
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

<div id="organism-legend-dialog" style="display: none">
  <div class="organism-legend">
  </div>
</div>

<div id="phylonode_popup_dialog" style="display: none;">
</div>

<div id="au-content" aurelia-app="main">
</div>

<?php

drupal_add_css(
    '//cdnjs.cloudflare.com/ajax/libs/loaders.css/0.1.2/loaders.min.css',
    array('type' => 'external', 'group' => CSS_THEME)
);

    
/*
 * this template depends on a few javascript libraries, but we are not
 * putting it into tripal_phylotree.info scripts[] because that
 * results in the script getting loaded *on every drupal request*
 * across the site, which is waste of resources.
 */


// TODO: is the old drupal jquery sufficient version?
drupal_add_js(
    '//cdn.bio.sh/msa/1.0/msa.min.gz.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);
drupal_add_js(
    '//cdnjs.cloudflare.com/ajax/libs/d3/3.5.17/d3.min.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);
drupal_add_js(
    '//cdnjs.cloudflare.com/ajax/libs/nvd3/1.8.4/nv.d3.min.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);
drupal_add_js(
    '//cdnjs.cloudflare.com/ajax/libs/URI.js/1.18.1/URI.min.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);
drupal_add_js(
    '//cdnjs.cloudflare.com/ajax/libs/lodash.js/4.14.2/lodash.min.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);
drupal_add_js(
    '//cdnjs.cloudflare.com/ajax/libs/chroma-js/1.2.1/chroma.min.js',
    array('type' => 'external', 'group' => JS_LIBRARY)
);

drupal_add_library('system', 'ui.dialog');


//
// default level javascripts (loads after library level)
//
$js_dir = '/'. $my_path . '/theme/js';
drupal_add_js(
    $js_dir . '/tour-autolauncher.js',
    array('type' => 'file', 'group' => JS_DEFAULT)
);

// finally, use a regular script tag to inject the aurelia boostrapper
// it will populate the aurelia-app div, above.
printf('<script src="%s/aurelia/scripts/vendor-bundle.js" data-main="aurelia-bootstrapper"></script>',
       $js_dir)

?>

