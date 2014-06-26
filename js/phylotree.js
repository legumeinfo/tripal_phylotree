(function ($) {
   // drupal7 jquery outer context
  $(document).ready(function () {

    var width = 550;
    var height = 350;

    // create a color generator for organism_id. todo: only color the legume
    // species, not all species!
    var colors = d3.scale.category20();
    var organismColor = function(d) {
      var color = colors(d.organism_id);
      return color;
    };
    
    $.getJSON( phylogramJsonUrl, function(data) {
      
      d3.phylogram.build('#phylogram', data, {
	width: width,
	height: height,
	fill: organismColor
      });

      d3.phylogram.buildRadial('#phylotree-radial-graph', data, {
	width: width, 
	fill: organismColor	 
      });

      organismBubblePlot('#phylotree-organisms', data, {
	height: height,
	width: width,
	fill: organismColor
      });
      $('.phylogram-ajax-loader').remove();
    });
 // /jquery document.ready
 });
 // /drupal7 jquery outer context
})(jQuery);
