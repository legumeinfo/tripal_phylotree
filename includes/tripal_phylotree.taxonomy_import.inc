<?php

/**
 *
 * @param unknown $form
 * @param unknown $form_state
 * @return unknown
 */
function tripal_phylotree_taxonomy_load_form($form, &$form_state) {
  $form['instructions'] = array(
    '#type' => 'item',
    '#markup' => t('The NCBI Taxonmic Importer examines the organisms
      currently present into the database and queries NCBI for the
      taxonomic details.  those details are then added to the phylogeny
      tables of Chado where taxonomic data is housed.  A list of organisms
      that are not mapped to the NCBI taxonomic database will appears
      below.'),
  );
  $form['sync_ncbi'] = array(
    '#type' => 'submit',
    '#name' => 'ncbi_sync',
    '#value' => 'Import NCBI taxonomic data'
  );
  return $form;
}

/**
 *
 * @param unknown $form
 * @param unknown $form_state
 */
function tripal_phylotree_taxonomy_load_form_submit($form, &$form_state) {
  global $user;
  $args = array();
  tripal_add_job("Import NCBI Taxonomy", 'tripal_phylotree', 'tripal_phylotree_ncbi_taxonomy_import', $args, $user->uid);
}

/**
 *
 * @param unknown $job_id
 */
function tripal_phylotree_ncbi_taxonomy_import($job_id) {

  print "\nNOTE: Importing of NCBI taxonomy data is performed using a database transaction. \n" .
    "If the load fails or is terminated prematurely then the entire set of \n" .
    "insertions/updates is rolled back and will not be found in the database\n\n";

  $transaction = db_transaction();
  try {
    // TDDO: there should be an API function named tripal_insert_analysis().
    // But until then we have to insert the analysis manually.
    // Get the version of this module for the analysis record:
    $info = system_get_info('module', 'tripal_phylotree');
    $version = $info['version'];
    $analysis_name = 'NCBI Taxonomy Tree Import';

    // If the analysis record already exists then don't add it again.
    $analysis = chado_select_record('analysis', array('*'), array('name' => $analysis_name));
    if (count($analysis) == 0) {
      $values = array(
        'name' => 'NCBI Taxonomy Tree Import',
        'description' => 'Used to import NCBI taxonomy details for organisms in this database.',
        'program' => 'Tripal Phylogeny Module NCBI Taxonomy Importer',
        'programversion' => $version,
        'sourcename' => 'NCBI Taxonomy',
        'sourceuri' => 'http://www.ncbi.nlm.nih.gov/taxonomy',
      );
      $analysis = chado_insert_record('analysis', $values);
      if (!$analysis) {
        throw new Exception("Cannot add NCBI Taxonomy Tree Import Analysis.");
      }
    }
    else {
      $analysis = $analysis[0];
    }

    // If the tree already exists then don't insert it again
    $tree_name = 'NCBI Taxonomy Tree';
    $phylotree = chado_select_record('phylotree', array('*'), array('name' => $tree_name));
    if (count($phylotree) == 0) {
      // Add the taxonomic tree.
      $options = array(
        'name' => 'NCBI Taxonomy Tree',
        'description' => 'The Taxonomy Database is a curated classification and nomenclature for all of the organisms in the public sequence databases.',
        'leaf_type' => 'taxonomy',
        'analysis_id' => $analysis->analysis_id,
        'tree_file' => '/dev/null',
        'format' => 'taxonomy',
        'no_load' => TRUE,
      );
      $phylotree = tripal_insert_phylotree($options);
      if (!$phylotree) {
        throw new Exception("Cannot add the NCBI Taxonomy Tree phylotree record.");
      }
    }
    else {
      $phylotree = $phylotree[0];
    }

    // Get the list of orgnaisms that haven't yet been imported.
    $sql = "
      SELECT O.*
      FROM {organism} O
        LEFT JOIN {organism_dbxref} ODBX on O.organism_id = ODBX.organism_id
      WHERE ODBX.dbxref_id IS NULL
    ";
    $organisms = chado_query($sql);
    while ($organism = $organisms->fetchObject()) {
      // Build the query string to get the information about this species.
      $term = $organism->genus . ' ' . $organism->species;
      $term = urlencode($term);
      $search_url = "http://www.ncbi.nlm.nih.gov/entrez/eutils/esearch.fcgi?".
        "db=taxonomy" .
        "&term=$term";

      // Get the search response from NCBI.
      $rfh = fopen($search_url, "r");
      $xml_text = '';
      while (!feof($rfh)) {
        $xml_text .= fread($rfh, 255);
      }
      fclose($rfh);

      // Parse the XML to get the taxonomy ID
      $tree = array();
      $xml = new SimpleXMLElement($xml_text);
      if ($xml) {
        $taxid = (string) $xml->IdList->Id;
        if ($taxid) {
          print "$taxid\t$organism->genus $organism->species\n";
          // If we have a taxonomy ID we can now get the details.
          $fetch_url = "http://www.ncbi.nlm.nih.gov/entrez/eutils/efetch.fcgi?".
            "db=taxonomy" .
            "&id=$taxid";
          // Get the search response from NCBI.
          $rfh = fopen($fetch_url, "r");
          $xml_text = '';
          while (!feof($rfh)) {
            $xml_text .= fread($rfh, 255);
          }
          fclose($rfh);

          $xml = new SimpleXMLElement($xml_text);
          if ($xml) {
            $taxon = $xml->Taxon;

            // Add in the organism properties
            $lineage = (string) $taxon->Lineage;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'lineage', $lineage);

            $genetic_code = (string) $taxon->GeneticCode->GCId;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'genetic_code', $genetic_code);

            $genetic_code_name = (string) $taxon->GeneticCode->GCName;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'genetic_code_name', $genetic_code_name);

            $mito_genetic_code = (string) $taxon->MitoGeneticCode->MGCId;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'mitochondrial_genetic_code', $mito_genetic_code);

            $mito_genetic_code_name = (string) $taxon->MitoGeneticCode->MGCName;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'mitochondrial_genetic_code_name', $mito_genetic_code_name);

            $division = (string) $taxon->Division;
            tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'division', $division);

            $name_ranks = array();
            foreach ($taxon->OtherNames->children() as $child) {
              $type = $child->getName();
              $name = (string) $child;
              if (!array_key_exists($type, $name_ranks)) {
                $name_ranks[$type] = 0;
              }
              switch ($type) {
                case 'GenbankCommonName':
                  tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'genbank_common_name', $name, $name_ranks[$type]);
                  break;
                case 'Synonym':
                  tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'synonym', $name, $name_ranks[$type]);
                  break;
                case 'CommonName':
                case 'Includes':
                  tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'other_name', $name, $name_ranks[$type]);
                  break;
                case 'EquivalentName':
                  tripal_phylotree_taxonomy_add_organism_property($organism->organism_id, 'equivalent_name', $name, $name_ranks[$type]);
                case 'Name':
                  // skip the Name stanza
                  break;
                default:
                  print "NOTICE: Skipping unrecognzed name type: $type\n";
                  // do nothing for unrecognized types
              }
              $name_ranks[$type]++;
            }

            // Generate a nested array structure that can be used for importing the tree.
            $parent = (string) $taxon->ParentTaxId;
            $rank = (string) $taxon->Rank;
            $sci_name = (string) $taxon->ScientificName;
            $i = 0;
            $parent = NULL;
            foreach ($taxon->LineageEx->children() as $child) {
              $tid = (string) $child->TaxID;
              $name = (string) $child->ScientificName;
              $cvterm = tripal_get_cvterm(array(
                'name' => (string) $child->Rank,
                'cv_id' => array(
                  'name' => 'taxonomy',
                ),
              ));
              $node = array(
                'name' => $name,
                'depth' => $i,
                'is_root' => $i == 0 ? TRUE : FALSE,
                'is_leaf' => FALSE,
                'is_internal' => $i > 0 ? TRUE : FALSE,
                'descendents' => 0,
                'left_index' => 0,
                'right_index' => 0,
                'type' => $cvterm->cvterm_id,
                'xref' => "taxonomy:$tid",
              );
              $i++;
            }

            // Now add in the leaf node
            $cvterm = tripal_get_cvterm(array(
              'name' => $rank,
              'cv_id' => array(
                'name' => 'taxonomy',
              ),
            ));
            $node = array(
              'name' => $sci_name,
              'depth' => $i,
              'is_root' => FALSE,
              'is_leaf' => TRUE,
              'is_internal' => FALSE,
              'descendents' => 0,
              'left_index' => 0,
              'right_index' => 0,
              'type' => $cvterm->cvterm_id,
              'xref' => "taxonomy:$tid",
            );
          }
        }
      }
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    print "\n"; // make sure we start errors on new line
    watchdog_exception('tripal_phylogeny', $e);
    print "FAILED: Rolling back database changes...\n";
  }
}

/**
 *
 * @param unknown $organism_id
 * @param unknown $term_name
 * @param unknown $value
 */
function tripal_phylotree_taxonomy_add_organism_property($organism_id, $term_name, $value, $rank = 0) {
  if (!$value) {
    return;
  }

  $record = array(
    'table' => 'organism',
    'id' => $organism_id
  );
  $property = array(
    'type_name' => $term_name,
    'cv_name' => organism_property,
    'value' => $value
  );
  // Delete all properties of this type if the rank is zero.
  if ($rank == 0) {
    chado_delete_property($record, $property);
  }
  chado_insert_property($record, $property);
}
