/* implement http://msa.biojs.net for lis gene families */

var phylogeny_msa = {};

(function() {
  'use strict'; 

  var that = this;
  var fastaAPI = '/lis_gene_families/chado/msa/{f}-consensus/download/';
  // gff loading disabled until it can be debugged
  //var gffAPI = '/gff_export/{f}-consensus';
  
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
  
  this.toggle = function() {
    var btn = jQuery('#msa-toggle');
    if(! that.viewer) {
      that.load();
      btn.html('Hide Multiple Sequence Alignment (MSA)');
      return;
    }
    jQuery('#msa-viewer').empty();
    jQuery('.smenubar').remove();
    that.viewer = null;
    btn.html('View Multiple Sequence Alignment (MSA)');
  };
  
  this.load = function() {
    var msa = require('msa');
    
    jQuery('#organism-legend-dialog').dialog('close');
    
    var wrapper = jQuery('#msa-viewer-wrapper');
    var spinner = jQuery('#msa-spinner');
    var container = jQuery('#msa-viewer');
    wrapper.show();
    container.show();
    spinner.show();
    
    var params = {f: familyName};
    var url = fastaAPI.supplant(params);
    var div = container[0];
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

    // var gffParser = require('biojs-io-gff');
    // var xhr = require('xhr');
    // url = gffAPI.supplant(params);
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

    // workaround for biojs-msa menu divs possibly being effected by
    // leaking CSS rules in this context
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
