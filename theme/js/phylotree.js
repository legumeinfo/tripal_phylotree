/* phylotree d3js graphs */

(function ($) {
  
  var width = 550;
  var height = 0; // will be dynamically sized in displayData
  var pane = null;
  var legumeColors = null;
  var phylogramOrganisms = {};
  
  function currentPane() {
    // parse the url hash, or the href query string to see which
    // sub-pane the navigation is on (the pane can appear in either
    // the hash or the query string)
    var url = window.location.href;
    var matches = url.match(/pane=(\w+)/i);
    if(! matches) {
      return 'base';
    }
    return matches[1];
  }

  function hiliteNodeNames(lowercase) {
    // Using the URI.js library parse the url query string for a one
    // or more hilite_node parameters. Return an object with
    // properties set for each node name to hilite.
    var uri = new URI(window.location.href);
    var query = uri.query(true);
    if(! query.hilite_node) {
      return {};
    }
    if(_.isArray(query.hilite_node)) {
      return _.zipObject(_.map(query.hilite_node, function(nodeName) {
	return lowercase ?
	  [nodeName.toLowerCase(), true] : [nodeName, true];
      }));
    }
    if(query.hilite_node.indexOf(',') >= 0) {
      // this appears to be a comma separated list
      var hiliteNodes =  query.hilite_node.split(',');
      return _.zipObject(_.map(hiliteNodes, function(nodeName) {
	return lowercase ?
	  [nodeName.toLowerCase(), true] : [nodeName, true];
      }));
    }
    // else generate a single item list
    return lowercase ?
      _.zipObject([query.hilite_node.toLowerCase(), true]) :
      _.zipObject([query.hilite_node, true]);
  }

  function d3GraphOnPane(pane) {
    return ['base',
	    'phylotree_circ_dendrogram',
	    'phylotree_organisms'].indexOf(pane) !== -1;
  }

  function displayLegend(organismColorData, forPane) {
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
      if(! a.data.order || ! b.data.order) {
	return a.label.localeCompare(b.label);
      }
      if(a.data.order > b.data.order) {
	return 1;
      }
      if(a.data.order < b.data.order) {
	return -1;
      }
      return 0;
    });
    var container = d3.selectAll('.organism-legend');
    container.selectAll('div').remove();
    
    var rows = container.selectAll('div')
	.data(organismList)
	.enter()
	.append('div')
        .attr('class', 'org-legend-row');
    rows.append('span')
      .attr('class', 'org-legend-color')
      .append('svg:svg')
      .attr('width', 14)
      .attr('height', 18)
      .append('svg:circle')
      .attr('class', 'legend-circle')
      .attr('cx', 7)
      .attr('cy', 18)
      .attr('r', 6)
      .attr('stroke', 'dimgrey')
      .attr('stroke-width', '1px')
      .attr('fill', function(d) { return d.color; });
    rows.append('span')
       .attr('class', 'org-legend-label')
      .html(function(d) { return d.label; });

    if(forPane !== 'phylotree_organisms') {
      var div = container.insert('div', ':first-child');
      div.attr('class', 'org-legend-row')
	.append('span')
	.attr('class', 'org-legend-color')
	.append('svg:svg')
	.attr('width', 14)
	.attr('height', 18)
	.append('svg:circle')
	.attr('cx', 7)
	.attr('cy', 18)
	.attr('r', 6)
	.attr('stroke', 'dimgrey')
	.attr('stroke-width', '2px')
	.attr('fill', 'white');
      div.append('span')
	.attr('class', 'org-legend-label')
	.html('internal node');

      div = container.insert('div', ':first-child');
      div.attr('class', 'org-legend-row')
	.append('span')
	.attr('class', 'org-legend-color')
	.append('svg:svg')
	.attr('width', 14)
	.attr('height', 18)
	.append('svg:circle')
	.attr('cx', 7)
	.attr('cy', 18)
	.attr('r', 6)
	.attr('stroke', 'black')
	.attr('stroke-width', '1px')
	.attr('fill', 'dimgrey');
      div.append('span')
	.attr('class', 'org-legend-label')
	.html('root node');

      var hilites = hiliteNodeNames(false);
      var warning = '';
      if(! $('.hilite-node').length) {
	warning = ' (FEATURE NOT FOUND)';
      }
      var hiliteNames = _.keys(hilites);
      if(hiliteNames && hiliteNames.length) {
	var div = container.append('div')
	    .attr('class', 'org-legend-row');
	div.append('span')
	  .attr('class', 'org-legend-color')
	  .append('svg:svg')
  	  .attr('width', 14)
	  .attr('height', 18)
	  .append('svg:rect')
	  .attr('x', 0)
	  .attr('y', 10)
	  .attr('width', 14)
	  .attr('height', 10)
	  .attr('fill', 'khaki');
	div.append('span')
	  .attr('class', 'org-legend-label')
	  .html('hilite: '+  hiliteNames.join(', ') + warning);
      }
    }
    
    var dialog = $('#organism-legend-dialog');
    //allows re-open of dialog, basically a toggle between this
    //element and the dialog being visible
    $('.organism-legend-show').click(function() {
      var dialog = $('#organism-legend-dialog');
      dialog.dialog('option', {
	position: {
	  my : 'left top',
	  at : 'right top',
	  of : $('.organism-legend-show'),
	}
      });
      dialog.dialog('open');
      $('.organism-legend-show').hide();
    });

    var positionOf = null, positionMy = null, positionAt = null;
    if(hiliteNames && hiliteNames.length) {
      // d3.phylogram.js *may* have added this class to an el.
      //      positionOf = $('.hilite-node').first();
      positionOf = topmostElementIn('.hilite-node');
      positionMy = 'left top';
      positionAt = 'right';
    }
    else {
      switch(forPane) {
      case 'base':
	positionOf = $('#phylogram');
	break;
      case 'phylotree_circ_dendrogram':
	positionOf = $('#phylotree-radial-graph');
	break;
      case 'phylotree_organisms':
	positionOf = $('#phylotree-organisms');
	break;
      }
      positionMy = 'right top';
      positionAt = 'right top';
    }
    var position = {
      my : positionMy,
      at : positionAt,
      of : positionOf,
      collision : 'fit flip',
      offset : '1000 -20',
    };
    dialog.dialog({
      title : 'Legend',
      closeOnEscape : true,
      modal : false,
      width : '300px',
      close: function() {
	$('.organism-legend-show').show();
      },
      position : position,
    });
  }

  function topmostElementIn(selector) {
    var top = Infinity;
    var topElem = null;
    jQuery(selector).each(function() {
      var offset = $(this).offset();
      if(offset.top > 0 && offset.top < top) {
	top = offset.top;
	topElem = $(this);
      }
    });
    return topElem;
  }

  function species5(d)  {
    // the 5 letter abbreviation -- YMMV
    var label = d.genus.substring(0, 3) + d.species.substring(0, 2);
    return label.toLowerCase();
  }

  // function to generate color based on the organism genus and species
  // on graph node d
  function organismColor(d) {
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
  }

  $(document).ready( function () {

    $('.phylogeny-help-btn').click(function() {
      $('#phylogeny-help-dlg').dialog( {
        title: 'Gene Family Help',
        closeOnEscape : true,
	width: '500px',
        modal: false,
        position: {
	  my: 'center top', at: 'top', of: window
	},
        show: { effect: 'blind', direction: 'down', duration: 200 }
      });
    });
    
    pane = currentPane();
    
    // when user navigates to a sub-panel without a graph, hide any popups
    // and redisplay the legend if applicable.
    $('.tripal_toc_list_item_link').click(function() {
      $('#organism-legend-dialog').dialog('close');
      var newPane = $(this).attr('id');
      if (d3GraphOnPane(newPane)) {
	// redisplay the legend only if there is a d3 graph on new panel
    	setTimeout(function() {
    	  // wait until new panel is displayed, to popup the legend
	  // because it needs to position wrt the current d3 graph
    	  displayLegend(legumeColors, newPane);
    	}, 100);
      }
      // always hide the Show Legend links by default (because Legend
      // appears by default)
      $('.organism-legend-show').hide();
      // always close the interior node dialog
      $('#phylonode_popup_dialog').dialog('close');
      // always close the help dialog
      $('#phylogeny-help-dlg').dialog('close');
      return false;
    });

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
	  var url = '/lis_context_viewer/index.html#/basic/' + d.phylonode_id;
          link.attr('href', url);
          link.text('View Genomic Contexts for genes in this subtree');
          link.show();
        }
        else {
          // this shouldn't happen but ok
          $('#phylonode_context_link').hide();
        }
        
        // show dialog content relevant for interior node
	// go_link not ready for prime time
        // $('#phylonode_go_link').show();
        $('#phylonode_go_link').hide();
        $('#phylonode_context_link').show();
        
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
        closeOnEscape : true,
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
			if(d3GraphOnPane(pane)) {
			  displayLegend(colorData, pane)
			}
			$('.phylogram-ajax-loader').remove();
		      });
	    });
    
    
    function displayData(treeData) {
      // draw the d3 graphs. in the current tripal pane
      // implementation, all content is drawn at page load, and then
      // shown/hidden with javascript. so all d3 graphs will get drawn
      // all the time.
      height = graphHeight(treeData);
      var hilites = hiliteNodeNames(true);
      d3.phylogram.build('#phylogram', treeData, {
        'width' : width,
        'height' : height,
        'fill' : organismColor,
	'hiliteNodes' : hilites,
        'nodeMouseOver' : nodeMouseOver,
        'nodeMouseOut' : nodeMouseOut,
        'nodeMouseDown' : nodeMouseDown
      });
      d3.phylogram.buildRadial('#phylotree-radial-graph', treeData, {
        'width' : width, // square graph 
        'fill' : organismColor,
	'hiliteNodes' : hilites,	
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
