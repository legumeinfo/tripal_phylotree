tripal_phylotree
================
A tripal module for Chado phylogeny tables, developed for
http://legumeinfo.org/ 

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

* remote change (test)
