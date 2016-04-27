/* implement http://msa.biojs.net for lis gene families */

var msaWrapper = require('msa');

(function() {
  'use strict';
  var that = this;
  var fastaAPI = '/lis_gene_families/chado/msa/{f}-consensus/download/';
  var gffAPI = '/legumeinfo.org/gff_export/{f}-consensus';
  
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

    var wrapper = jQuery('#msa-viewer-wrapper');
    var spinner = jQuery('#msa-spinner');
    var container = jQuery('#msa-viewer');
    wrapper.show();
    container.show();
    spinner.show();
    
    if(that.viewer) {
      // TODO dispose of previous msa viewer somehow? cannot find in API
      container.empty();
      jQuery('.smenubar').remove();
    }
    
    var params = {f: familyName};
    var url = fastaAPI.supplant(params);
    var div = container[0];
    var gffParser = require('biojs-io-gff');
    var xhr = require('xhr');
    var opts = {
      el: div,
      bootstrapMenu: true, 
      importURL: url,
      vis : { overviewbox: true},
    };
    that.viewer = new msa(opts);
    url = gffAPI.supplant(params);
    xhr(url, function(err, request, body) {
      if(err || ! body) {
	console.log(err);
	spinner.hide();
	return;
      }
      var features = gffParser.parseSeqs(body);
      that.viewer.seqs.addFeatures(features);
      spinner.hide();
    });
  };
      
}.call(msaWrapper));
