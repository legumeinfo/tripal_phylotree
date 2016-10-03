/* use the chroma.js library to colorize taxons consistently and
 * predictably. Requires chroma.js https://github.com/gka/chroma.js/
 *
 * usage example: (always returns same hue for Arachis, but scaled
 * lightness depending on the Species -- completely abitrarily, but
 * consistently)
 *
 * taxonChroma.get(taxon, options);
 *
 * // examples: 
 * taxonChroma.get('Arachis hypogaea');
 * taxonChroma.get('Arachis burkartii');
 *
 * // make 20% lighter overall
 * taxonChroma.get(someTaxon, { lightnessFactor: 1.2 } );
 * // override some taxon
 * taxonChroma.get(acc.properties.taxon, {
 * 'lightnessFactor' : 1.2,
 *   'overrides' : {
 *   'phaseolus lunatus' : 'green',
 *  }
 * });
 */

var taxonChroma = {};

(function() {

  var colorCache = {};
  var LIGHTNESS_FACTOR = 1; // default lightness factor (1= don't post-adjust)
  var MIN_LIGHTNESS = 0.3;
  var moreBrewerColors = chroma.brewer.Set2; 
  
  this.defaultColor = '#d3d3d3'; // used for non-legume genera

  // some of these colors are carried over from the colors.json file
  // from the pholylotree module. they are all cbrewer classification
  // colors.
  this.legumeGenera = {
    apios :        moreBrewerColors[0],
    arachis :      '#bcbd22',
    cajanus :      '#ffbb78',
    chamaecrista : moreBrewerColors[5],
    cicer :        '#2ca02c',
    glycine :      '#1f77b4',
    lens :         '#98df8a',
    lotus :        '#17becf',
    lupinus :      '#ff9896',
    medicago :     '#8c564b',
    phaseolus :    '#e377c2',
    pisum :        '#f7b6d2',
    trifolium :    moreBrewerColors[2],
    vicia :        moreBrewerColors[4],
    vigna :        '#d62728',
  };

  this.get = function(taxon, options) {
    taxon = taxon.toLowerCase();
    
    // options is an object w/ keys lightnessFactor, overrides
    if (! options) {
      options = {};
    }
    if (options.lightnessFactor === undefined) {
      options.lightnessFactor = LIGHTNESS_FACTOR;
    }
    if (options.overrides === undefined) {
      options.overrides = {};
    }
    if(options.overrides[taxon] !== undefined) {
      return options.overrides[taxon];
    }
    if(colorCache[taxon] !== undefined) {
      return colorCache[taxon];
    }
    var color = null;
    var parts = taxon.split(' ');
    var genus = parts[0];
    var species = parts[1];

    if(genus in this.legumeGenera ) {
      var genusColor = this.legumeGenera[genus];
      var hue = chroma(genusColor).hsl()[0];
      var lightness = MIN_LIGHTNESS +
	             (fnv32a(species, 1000) / 1000) * (1 - 2 *MIN_LIGHTNESS);
      color = chroma(hue, 1, lightness * options.lightnessFactor, 'hsl').hex();
    }
    else {
      color = this.defaultColor;
    }
    colorCache[taxon] = color;
    return color;
  };
  
  function fnv32a(str, hashSize) {
    /* a consistent hashing algorithm
       https://gist.github.com/vaiorabbit/5657561
       http://isthe.com/chongo/tech/comp/fnv/#xor-fold
    */
    var FNV1_32A_INIT = 0x811c9dc5;
    var hval = FNV1_32A_INIT;
    for ( var i = 0; i < str.length; ++i ) {
      hval ^= str.charCodeAt(i);
      hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
    }
    return (hval >>> 0) % hashSize;
  }
  
}.call(taxonChroma));

export { taxonChroma };

