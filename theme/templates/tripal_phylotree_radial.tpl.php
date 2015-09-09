<div class="tripal_phylotree-data-block-desc tripal-data-block-desc">
<?php
$phylotree = $variables['node']->phylotree;
print $phylotree->name;
?>
<br/>
<?php
if( ! empty($phylotree->comment) ) {
  print $phylotree->comment;
}
?>
</div>

<div class="organism-legend-show" style="display: none">
	<a>Show Legend</a>
</div>

<div id="phylotree-radial-graph">
  <!-- d3 will add svg to this div -->
</div>
