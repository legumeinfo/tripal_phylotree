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

  /* toggle visibility of msa viewer, saving state, lazily load fasta */
  var visible = false;
  
  this.toggle = function() {
    var wrapper = jQuery('#msa-viewer-wrapper');
    if(! visible) {
      if(! that.viewer) {
	that.load();
      }
      wrapper.show();
    }
    else {
      wrapper.hide();
    }
    visible = ! visible;
    var btn = jQuery('#msa-toggle');
    var label = visible ? 'Hide' : 'Hide';
    btn.html(label + ' Multiple Sequence Alignment (MSA)');
  };
  
  this.load = function() {
    jQuery('#organism-legend-dialog').dialog('close');
    var msa = require('msa');
    var spinner = jQuery('#msa-spinner');
    var container = jQuery('#msa-viewer');
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
	labelId: false,
      },
      zoomer: {
	labelNameLength: 150,
	labelFontsize: 9,
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
