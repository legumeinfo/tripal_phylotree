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

<?php
/*
 * cross references template
 */

$node = $variables['node'];
$phylotree = $node->phylotree;

$header = array('Database', 'Accession');

$db = sprintf('%s %s',
     $phylotree->dbxref_id->db_id->name,
     $phylotree->dbxref_id->db_id->description);

if($phylotree->dbxref_id->db_id->url) {
  $db_linkout = sprintf('<a href="%s" target="_blank">%s</a>', $phylotree->dbxref_id->db_id->url, $db);
}

//not really sure about this, but leaving as alex had it for now
$accession = sprintf('%s %s %s',
      $phylotree->dbxref_id->accession,
      $phylotree->dbxref_id->version,
      $phylotree->dbxref_id->description);

if($phylotree->dbxref_id->db_id->urlprefix) {
  $url = sprintf('%s%s',
      $phylotree->dbxref_id->db_id->urlprefix,
      $accession);
  $item_linkout = sprintf('<a href="%s" target="_blank">%s</a>', $url, $accession);
}

$rows = array(
  array( 'data' => array(
    (!empty($db_linkout) ? $db_linkout : $db),
    (!empty($item_linkout) ? $item_linkout : $accession)
  )));

$table = array(
  'header' => $header,
  'rows' => $rows,
  'attributes' => array(
    'id' => 'tripal_phylotree-table-references',
    'class' => 'tripal-data-table'
  ),
  'sticky' => FALSE,
  'caption' => '',
  'colgroups' => array(),
  'empty' => '',
);
print theme_table($table);

// allow site admins to see the phylotree id
if (user_access('view ids')) {
  print sprintf('<div>phylotree_id = %d</div>',
        $phylotree->phylotree_id );
}
?>
