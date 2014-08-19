/* phylotree d3js graphs */

(function ($) {
  
  var width = 550;
  var height = 0; // will be dynamically sized
  
  $(document).ready( function () {
    var legumeColors = null;
    
    var organismColor = function(d) {
      var color = legumeColors[d.abbreviation];
      if(color) { return color; }
      else { return 'grey'; }
    };

    $.getJSON(pathToTheme +'/js/legume-colors.json', function(colorData) {
      legumeColors = colorData;
      $.getJSON(phylotreeDataURL, function(treeData) {
	displayData(treeData);
	$('.phylogram-ajax-loader').remove();
      });
    });
		     
    function displayData(treeData) {
      height = graphHeight(treeData);
      d3.phylogram.build('#phylogram', treeData, {
	width: width,
	height: height,
	fill: organismColor
      });
      d3.phylogram.buildRadial('#phylotree-radial-graph', treeData, {
	width: width, 
	fill: organismColor	 
      });
      organismBubblePlot('#phylotree-organisms', treeData, {
	height: height,
	width: width,
	fill: organismColor
      });
    }

    /* graphHeight() generate graph height based on leaf nodes */
    function graphHeight(data) {
      function countLeafNodes(node) {
	if(! node.children) {
	  return 1;
	}
	var ct = 0;
	node.children.forEach( function(child) {
	  ct+= countLeafNodes(child);
	});
	return ct;
      }
      var leafNodeCt = countLeafNodes(data);
      return 22 * leafNodeCt;
    }
  });
})(jQuery);
