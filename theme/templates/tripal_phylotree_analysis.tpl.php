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
    <img src="/sites/all/modules/tripal/tripal_phylogeny/image/help.png">
    Gene Family Help
  </a>
  <a href="#" class="button organism-legend-show" style="display: none">
    Show Legend
  </a>
</div>
<br/>

<?php
/*
 * phylotree analyis template
 */
$node = $variables['node'];
$phylotree = $node->phylotree;

$header = array('Name', 'Description', 'Metadata');
$analysis = $phylotree->analysis_id;

$rows = array();

if($analysis) {
  $analysis = chado_expand_var($analysis, 'field', 'analysis.description');
  $rows[] = array($analysis->name, $analysis->description, 
      sprintf('%s %s %s %s %s %s %s',
          $analysis->program,
          $analysis->programversion,
          $analysis->algorithm,
          $analysis->sourcename,
          $analysis->sourceversion,
          $analysis->sourceuri,
          $analysis->timeexecuted));
}
else {
  // degrade gracefully if no analysis is present
  $rows[] = array('n/a','','');
}
   
$table = array(
  'header' => $header,
  'rows' => $rows,
  'attributes' => array(
    'id' => 'tripal_phylotree-table-analysis',
    'class' => 'tripal-data-table'
  ),
  'sticky' => FALSE,
  'caption' => '',
  'colgroups' => array(),
  'empty' => '',
);
print theme_table($table);
?>
