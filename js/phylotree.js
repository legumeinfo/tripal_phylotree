(function ($) {
   // drupal7 jquery outer context
  $(document).ready(function () {

    var width = 550;
    var height = 350;

    $.getJSON( pathToTheme +'/js/legume-colors.json', function(legumeColors) {
      var organismColor = function(d) {
	var color = legumeColors[d.abbreviation];
	if(color) { return color; }
	else { return 'grey'; }
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
      
    });
 // /jquery document.ready
 });
 // /drupal7 jquery outer context
})(jQuery);
