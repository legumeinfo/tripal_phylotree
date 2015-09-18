<div class="tripal_phylotree-data-block-desc tripal-data-block-desc">
<p><b><?php
$phylotree = $variables['node']->phylotree;
print $phylotree->name;
?></b>:
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
</div>

<div id="phylotree-radial-graph">
  <!-- d3 will add svg to this div -->
</div>
