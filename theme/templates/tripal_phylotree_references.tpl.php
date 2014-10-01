<?php
/*
 * cross references template
 */

$node = $variables['node'];
$phylotree = $node->phylotree;

$header = array('Database', 'Accession');

//not really sure about this, but leaving as alex had it for now
$accession = sprintf('%s %s %s',
      $phylotree->dbxref_id->accession,
      $phylotree->dbxref_id->version,
      $phylotree->dbxref_id->description);

if($phylotree->dbxref_id->db_id->url) {
  $url = sprintf('%s%s',
      $phylotree->dbxref_id->db_id->urlprefix,
      $accession);
  $linkout = sprintf('<a href="%s" target="_blank">%s</a>', $url, $accession);
}

$rows = array(
  array( 'data' => array(
    sprintf('%s %s',
     $phylotree->dbxref_id->db_id->name,
     $phylotree->dbxref_id->db_id->description),
    ($linkout ? $linkout : $accession)
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
