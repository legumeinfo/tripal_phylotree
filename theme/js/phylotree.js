/* phylotree d3js graphs */

(function ($) {
  
  var width = 550;
  var height = 0; // will be dynamically sized
  
  $(document).ready( function () {
    
    var legumeColors = null;
    var phylogramOrganisms = {};
    
    // function to generate color based on the organism genus and species
    // on graph node d
    var organismColor = function(d) {
      // create map of species in this graph, for use in legend later.
      phylogramOrganisms[d.common_name] = true;
      
      var organism = legumeColors[d.common_name];
      if(! organism) {
	return legumeColors['default'].color;
      }
      if( ! organism.color) {
	return legumeColors['default'].color;	
      }
      return organism.color;
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

      // remove previously generated external links for nodes with different phylonode_id:
      $("div#linkout a[id!='^phylonode_linkout_"+d.phylonode_id+"']").remove();
      $("div#linkout br").remove();

      if(d.children) {
        // interior node
        if(d.phylonode_id) {
          var link = $('#phylonode_context_link');
          //note that the trailing slash is somewhat important to avoid apparent hanging due to the way django handles url pattern matching
          link.attr('href', '/lis_gene_families/chado/context_viewer/' + d.phylonode_id + '/');
          link.text('View Genomic Contexts for genes in this subtree');
          link.show();
        }
        else {
          // this shouldn't happen but ok
          $('#phylonode_context_link').hide();
        }
        if(d.phylotree_name) {
          var link = $('#msa_link');
          link.attr('href', '/lis_gene_families/chado/msa/'+ d.phylotree_name +'-consensus/');
          link.text('View Multiple Sequence Alignment for this Gene Family');
          link.show();
        }
        else {
          // this shouldn't happen but ok
          $('#msa_link').hide();
        }
        
        // show dialog content relevant for interior node
	// go_link not ready for prime time
        // $('#phylonode_go_link').show();
        $('#phylonode_go_link').hide();
        $('#phylonode_context_link').show();
        $('#msa_link').show();
        
        // hide dialog content which is only applicable to leaf nodes
        $('#phylonode_organism_link').hide();
        $('#phylonode_feature_link').hide();
      }
      else {
        // leaf node

        // show dialog content relevant for leaf node
        $('#phylonode_organism_link').show();
        $('#phylonode_feature_link').show();
        
        // hide dialog content which is only applicable to interior nodes
        $('#phylonode_go_link').hide();
        $('#phylonode_context_link').hide();
        $('#msa_link').hide();
        
        if(d.feature_node_id) {
          var link = $('#phylonode_feature_link');
          link.attr('href', '?q=node/' + d.feature_node_id);
          link.text('view feature: ' + d.feature_name);
          link.show();
        }
        else {
          // this shouldn't happen but ok
          $('#phylonode_feature_link').hide();
        }

        // view organism bof
        if(d.organism_node_id) {
          var link = $('#phylonode_organism_link');
          link.attr('href', d.organism_node_id ?
                    '?q=node/' + d.organism_node_id : '#');
          link.text('view organism: ' + d.genus + ' '+
                    d.species + ( d.common_name ? ' (' + d.common_name + ')' : '' ) );
          link.show();
        } else {
          $('#phylonode_organism_link').hide();
        } // view organism eof


        // Linkout bof:
	if (d.feature_name) {

          //FIXME: hack depending on typical naming conventions. we can certainly do better
          var transcript = d.feature_name.replace(/^.....\./, "");
          var gene = transcript.replace(/\.\d+$/, "");
          // The ajax call should be at the end after generating #phylonode_feature_link, #phylonode_organism_link, etc.
          $.ajax({
            type: "GET",
            url: window.location.origin+"/phylotree_links/"+d.genus+"/"+d.species+"/"+gene+"/"+transcript+"/json",
            success:function(data) {

              if (data.length > 0) {
                $.each(data, function( index, value ) {
                  var existinglink = $("a#phylonode_linkout_"+d.phylonode_id+"_"+index);
                  
                  if (existinglink.length == 0) {
                    var $link = $("<a id='phylonode_linkout_"+d.phylonode_id+"_"+index+"' href='"+value.href+"' tabindex='-1'>"+value.text+"</a></br>");
                    $("div#linkout").append($link);
                  }
                });
              }
            },
          });

        } //linkout eof


      }
    
      dialog.dialog( {
        title : title,
        closeOnEscape : false,
        modal : false,
        position : { my : 'center center', at : 'center center', of : el },
        show : { effect: 'blind', direction: 'down', duration: 200 }
      });
    };

    // colors.json and phylotree data json could be fetched in
    // parallel with promises, but for now, fetch them sequentially
    d3.json(pathToTheme +'/theme/js/colors.json',
	    function(error, colorData) {
	      if(error) { return console.warn(error); }
	      legumeColors = colorData;
	      d3.json(phylotreeDataURL,
		      function(error, treeData) {
			if(error) { return console.warn(error); }
			displayData(treeData);
			displayLegend(colorData)
			$('.phylogram-ajax-loader').remove();
		      });
	    });
    
    function displayLegend(organismColorData) {
      // first convert to array for d3 use
      var organismList = [];
      for(var key in organismColorData) {
	if(key !== 'default' && key !== 'comment' ) {
	  var org = organismColorData[key];
	  if(org.common_name in phylogramOrganisms) {
	    // only list in legend those organisms appear
	    var o = organismColorData[key];
	    var litem  = {
	      'label' : species5(o) + ' (' + o.genus + ' '+ o.species +
		', ' + o.common_name + ')',
	      'color' : organismColor(o),
	      'data' : o,
	    };
	    organismList.push(litem);
	  }
	}
      }

      organismList.sort( function(a,b) {
	return a.label.localeCompare(b.label);
      });
      
      var container = d3.selectAll('.organism-legend');
      var rows = container.selectAll('div')
	  .data(organismList)
	  .enter()
	  .append('div')
          .attr('class', 'organism-legend-row');
      rows.append('span')
	.attr('class', 'legend-organism-color')
  	.html("&nbsp;&nbsp;&nbsp;")
	.attr('style', function(d) {
	  return 'background-color: '+ d.color;
	});
      rows.append('span')
	.attr('class', 'legend-organism-label')
  	.html(function(d) { return d.label; });
    }

    function species5(d)  {
      // the 5 letter abbreviation -- YMMV
      var label = d.genus.substring(0, 3) + d.species.substring(0, 2);
      return label.toLowerCase();
    }
    
    function displayData(treeData) {
      height = graphHeight(treeData);
      d3.phylogram.build('#phylogram', treeData, {
        'width' : width,
        'height' : height,
        'fill' : organismColor,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
      });
      d3.phylogram.buildRadial('#phylotree-radial-graph', treeData, {
        'width' : width, // square graph 
        'fill' : organismColor,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
      });
      organismBubblePlot('#phylotree-organisms', treeData, {
        'height' : width, // square graph
        'width' : width, 
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
