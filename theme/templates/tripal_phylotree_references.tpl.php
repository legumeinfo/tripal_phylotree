<?php
/*
 there must be a better way of representing cross references in tripal.
 right now every tripal module pretty much duplicates dbxref template code.
 see my question here:
 http://sourceforge.net/p/gmod/mailman/message/32481909/
 */

$phylotree = $variables['node']->phylotree;

$header = array('Database', 'Accession', 'Link Out');

if($phylotree->dbxref_id->db_id->url) {
  $url = sprintf('%s%s%s',
      $phylotree->dbxref_id->db_id->urlprefix,
      $phylotree->dbxref_id->db_id->url,
      $phylotree->dbxref_id->accession);
  $link_out = sprintf('<a href="%s" target="_blank">%s</a>', $url, $url);
}
else {
  $link_out = 'n/a';
}

$rows = array(
  array( 'data' => array(
    sprintf('%s %s',
     $phylotree->dbxref_id->db_id->name,
     $phylotree->dbxref_id->db_id->description),
    sprintf('%s %s %s',
      $phylotree->dbxref_id->accession,
      $phylotree->dbxref_id->version,
      $phylotree->dbxref_id->description),
    $link_out
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
