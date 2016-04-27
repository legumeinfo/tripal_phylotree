/* implement http://msa.biojs.net for lis gene families */

var msaWrapper = require('msa');

(function() {
  'use strict';
  var that = this;
  var fastaAPI = 'http://legumeinfo.org/lis_gene_families/chado/msa/{f}-consensus/download/';
  var gffAPI = 'https://cors-anywhere.herokuapp.com/legumeinfo.org/gff_export/{f}-consensus';
  
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
    var container = jQuery('#msa-viewer');
    container.empty();
    container.show();
    var params = {f: familyName};
    var url = fastaAPI.supplant(params);
    var div = container[0];
    var gffParser = require('biojs-io-gff');
    var xhr = require('xhr');
    var opts = {
      el: div,
      bootstrapMenu: true, 
      importURL: url,
    };
    that.viewer = new msa(opts);
    url = gffAPI.supplant(params);
    xhr(url, function(err, request, body) {
      var features = gffParser.parseSeqs(body);
      that.viewer.seqs.addFeatures(features);
      //jQuery('#msa-spinner').hide();
    });
  };
      
}.call(msaWrapper));
