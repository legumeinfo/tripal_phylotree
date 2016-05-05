/* implement http://msa.biojs.net for lis gene families */

var msa = require('msa');
var phylogeny_msa = {};

(function() {
  'use strict'; 

  var that = this;
  var fastaAPI = '/lis_gene_families/chado/msa/{f}-consensus/download/';
  var gffAPI = '/gff_export/{f}-consensus';
  
  if (!String.prototype.supplant) {
    String.prototype.supplant = function (o) {
      return this.replace(
          /\{([^{}]*)\}/g,
	function (a, b) {
	  var r = o[b];
	  return typeof r === 'string' || typeof r === 'number' ? r : a;
	  
	}
      );
    };
  }

  this.load = function() {
    jQuery('#organism-legend-dialog').dialog('close');
    
    var wrapper = jQuery('#msa-viewer-wrapper');
    var spinner = jQuery('#msa-spinner');
    var container = jQuery('#msa-viewer');
    wrapper.show();
    container.show();
    spinner.show();
    
    if(that.viewer) {
      // TODO dispose of previous msa viewer somehow? cannot find in API
      console.log('cleanup previous msa viewer');
      container.empty();
      jQuery('.smenubar').remove();
    }
    
    var params = {f: familyName};
    var url = fastaAPI.supplant(params);
    var div = container[0];

    /* gff viewing disabled for now */
    
    //var gffParser = require('biojs-io-gff');
    //var xhr = require('xhr');
    var opts = {
      el: div,
      bootstrapMenu: true, 
      importURL: url,
      vis: {
	overviewbox: false,
      },
      zoomer: {
	labelNameLength: 120,
	labelFontsize: 9,
	labelIdLength: 24,
      }
    };
    that.viewer = new msa(opts);

    /* gff viewing disabled for now */
    
    //url = gffAPI.supplant(params);
    // xhr(url, function(err, request, body) {
    //   if(err || ! body) {
    // 	console.log(err);
    // 	spinner.hide();
    // 	return;
    //   }
    //   var features = gffParser.parseSeqs(body);
    //   that.viewer.seqs.addFeatures(features);
    //   spinner.hide();
    // });

    // workaround for biojs-msa menu divs apparently being misplaced
    // (at least in the LIS css context)
    jQuery('.smenubar_alink').live('click', function(evt) {
      setTimeout( function() {
	jQuery('.smenu-dropdown').css('top', '');
      });
    });

    function waitForImportURL() {
      // biojs-msa hides the callback fn to the importURL util. So
      // we dont know when it finishes loading
      if(jQuery('.biojs_msa_labelrow').length) {
	spinner.hide();
	return;
      }
      setTimeout(waitForImportURL, 100);
    }

    waitForImportURL();
    
  };
}.call(phylogeny_msa));
