/* phylotree d3js graphs */

(function ($) {
  
  var phylogram_width = 350;
  var dendrogram_width = 500;
  var bubble_width = 500;
  //var width = 250;
  var height = 0; // will be dynamically sized
  
  $(document).ready( function () {
    
    var colors = null;

    // function to generate color based on the organism genus and species
    // on graph node d
    var organismColor = function(d) {
      var color = colors[d.genus + ' ' + d.species];
      if(color) { return color; }
      else { return 'grey'; }
    };

    // callback for mouseover event on graph node d
    var nodeMouseOver = function(d) {
      var el =$(this);
      el.attr('cursor', 'pointer');
      var circle = el.find('circle');
      // highlight in yellow no matter if leaf or interior node
      circle.attr('fill', 'yellow');
      if(! d.children) {
        // only leaf nodes have descriptive text
        var txt = el.find('text');
        txt.attr('font-weight', 'bold');
      }
    };
    
    // callback for mouseout event on graph node d
    var nodeMouseOut = function(d) {
      var el = $(this);
      el.attr('cursor', 'default');
      var circle = el.find('circle');
      if(! d.children) {
        // restore the color based on organism id for leaf nodes
        circle.attr('fill', organismColor(d));
        var txt = el.find('text');
        txt.attr('font-weight', 'normal');
      }
      else {
        // restore interior nodes to white
        circle.attr('fill', 'white');
      }
    };
    
    // callback for mousedown/click event on graph node d
    var nodeMouseDown = function(d) {
      var el = $(this);
      var dialog = $('#phylonode_popup_dialog');
      var title = (! d.children ) ? d.name : 'interior node ' + d.phylonode_id;

      if(d.children) {
        // interior node

        // hide dialog content which is only applicable to leaf nodes
        $('#phylonode_organism_link').hide();
        $('#phylonode_feature_link').hide();
      } 
      else {
        // leaf node

        // show dialog content relevant for leaf node
        $('#phylonode_organism_link').show();
        $('#phylonode_feature_link').show();
	  
        if(d.feature_node_id) {
          var link = $('#phylonode_feature_link');
          link.attr('href', '?q=node/' + d.feature_node_id);
          link.text('view feature: ' + d.feature_name);
          link.show();

        }
        else {
          $('#phylonode_feature_link').hide();
        }
        if(d.organism_id) {
          var link = $('#phylonode_organism_link');
          link.attr('href', d.organism_node_id ?
                    '?q=node/' + d.organism_node_id : '#');
          link.text('view organism: ' + d.genus + ' '+
                    d.species + ( d.common_name ? ' (' + d.common_name + ')' : '' ) );
          link.show();
        } else {
          // there should always be an organism id, but degrade gracefully
          $('#phylonode_organism_link').hide();
        }
	}
    
      dialog.dialog( {
        title : title,
        closeOnEscape : false,
        modal : false,
        position : { my : 'center center', at : 'center center', of : el },
        show : { effect: 'blind', direction: 'down', duration: 200 }
      });
    };

    $.getJSON(pathToTheme +'/js/colors.json', function(colorData) {
      colors = colorData;
      $.getJSON(phylotreeDataURL, function(treeData) {
        displayData(treeData);
        $('.phylogram-ajax-loader').remove();
      });
    });
                     
    function displayData(treeData) {
      height = graphHeight(treeData);
      d3.phylogram.build('#phylogram', treeData, {
        'width' : phylogram_width,
        'height' : phylogram_width,
        'fill' : organismColor,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
      });
      d3.phylogram.buildRadial('#phylotree-radial-graph', treeData, {
        'width' : dendrogram_width, // square graph 
        'fill' : organismColor,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
      });
      organismBubblePlot('#phylotree-organisms', treeData, {
        'height' : bubble_width, // square graph
        'width' : bubble_width, 
        'fill' : organismColor,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
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
