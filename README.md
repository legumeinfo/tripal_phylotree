# tripal_phylotree
A tripal module for Chado phylogeny schema, developed for LIS http://legumeinfo.org.

An example gene family:
http://legumeinfo.org/chado_phylotree/phytozome_10_2.59027077

The Tripal Phylogeny module,
http://tripal.info/extensions/modules/phylogeny , was initially based
on this code. Tripal Phylogeny is more generic, and supports taxonomy
trees as well. This LIS phylotree module is more oriented towards
client side javascript features, for example using the BioJS MSA
viewer.

## Server Requirements
* Tripal 2 http://tripal.info/
* Chado http://gmod.org/wiki/Chado_Phylogeny_Module
* Drupal 7 https://www.drupal.org/

## Server Quickstart
* git clone repository into drupal/sites/all/modules/tripal
* drush pm-enable tripal_phylotree (or enable via drupal ui admin interface)
* see in browser admin/modules, and enable tripal_phylotree permissions:
  add 'View Phylotree' permission to all Roles
  add 'Administer Phylotree' permission to Admin role(s)
* see in browser admin/tripal/extension/tripal_phylotree, go to Sync tab
* sync phylotrees
* sync organisms
* sync features of type = 'polypeptide'
* browse to /chado_phylotree/{family}, e.g. /chado_phylotree/phytozome_10_2.59026827

## Javascript Build Steps
The coordinated UI views of the phylogram, the taxa pie chart, and the msa
visualization are implemented using several javascript libraries. To build and
bundle the javascript application you will need:

* Node 4 or newer
* Npm 3 or newer
* Git

```
# install aurelia-cli so the au command is on your path
npm install aurelia-cli -g

# install the javascript dependencies from the package.json manifest
cd theme/js/aurelia/
npm install

# build for development
au run --watch

# build for production (produces minified javascript)
au build --env prod
```
warning: on freebsd, you may have to repeat the `npm install` 2-3
times until all the packages are in place.
