/* phylotree d3js graphs */
(function ($) {
  
  var width = 550;
  var height = 0; // will be dynamically set
  
  $(document).ready( function () {

    $.when( $.getJSON(phylotreeDataURL),
	    $.getJSON(pathToTheme +'/js/legume-colors.json')
    ).then(displayData, ajaxFail);
    
    function displayData(treeDataResponse, legumeColorsResponse) {
      
      var legumeColors = legumeColorsResponse[0];
      var treeData = treeDataResponse[0];

      height = graphHeight(treeData);
      
      // generate height of graph from the number of leaf nodes
      var organismColor = function(d) {
	var color = legumeColors[d.abbreviation];
	if(color) { return color; }
	else { return 'grey'; }
      };
      
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
      $('.phylogram-ajax-loader').remove();
    }
    
    function ajaxFail(jqXHR, textStatus, errorThrown) {
      console.log('error: ' + textStatus);
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
