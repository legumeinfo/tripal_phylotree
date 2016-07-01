tripal_phylotree
================
A tripal module for Chado phylogeny tables, developed for
http://legumeinfo.org/ 

The Tripal Phylogeny module, http://tripal.info/extensions/modules/phylogeny ,  was initially based on this code. Tripal Phylogeny is more generic, and supports taxonomy trees as well. This LIS phylotree module is more oriented towards client side javascript features, for example using the BioJS MSA viewer.

Requirements
------------
* Tripal 2 http://tripal.info/
* Chado http://gmod.org/wiki/Chado_Phylogeny_Module
* Drupal 7 https://www.drupal.org/

Quickstart
----------
* git clone repos into drupal/sites/all/modules/tripal
* drush pm-enable tripal_phylotree (or enable via drupal ui admin interface)
* see in browser admin/modules, and enable tripal_phylotree permissions:
  add 'View Phylotree' permission to all Roles
  add 'Administer Phylotree' permission to Admin role(s)
* see in browser admin/tripal/extension/tripal_phylotree, go to Sync tab
* sync phylotrees
* sync organisms
* sync features of type = 'polypeptide'
* default view for searching and browsing is chado/phylotree

