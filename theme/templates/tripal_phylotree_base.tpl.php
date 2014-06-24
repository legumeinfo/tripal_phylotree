<!-- this div is used by tripal to create a vertical tabs named by
   $node->content (created via _node_view) -->
<div class="tripal_phylotree-data-block-desc tripal-data-block-desc"></div>

<?php

/* this template depends on d3js, but not putting it into
tripal_phylotree.info scripts[] because that results in the script
getting loaded *on every request*. */
drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/d3/3.4.8/d3.min.js','external');

$phylotree = $variables['node']->phylotree;

$path_to_theme = path_to_theme();

?>

<style>

 .node rect {
  cursor: pointer;
  fill: #fff;
  fill-opacity: .5;
  stroke: #3182bd;
  stroke-width: 1.5px;
}

.node text {
  font: 11px sans-serif;
  pointer-events: none;
}

path.link {
  fill: none;
  stroke: #9ecae1;
  stroke-width: 1.5px;
 }

</style>

<div id="phylotree-organisms-graph">
  <!-- d3js will add to this div, and remove the loader gif -->
  <img src="<?php print $path_to_theme; ?>/image/ajax-loader.gif"
       class="ajax-loader"/>
</div>

<div id="phylotree-node-graph">
  <!-- d3js will add to this div -->
</div>

<script>

 /***** d3js graph of phylotree nodes *****/
 
(function ($) {
   // drupal7 jquery outer context
    
 $(document).ready(function () {
 <?php
 print sprintf('var dataUrl = "?q=chado_phylotree/%d/json";',
               $phylotree->phylotree_id );
 ?>

 var margin = {top: 30, right: 20, bottom: 30, left: 20},
    width = 960 - margin.left - margin.right,
    barHeight = 20,
    barWidth = width * .5;

var i = 0,
    duration = 400,
    root;

var tree = d3.layout.tree()
    .nodeSize([0, 20]);

var diagonal = d3.svg.diagonal()
    .projection(function(d) { return [d.y, d.x]; });

 var svg = d3.select("#phylotree-node-graph").append("svg")
	      .attr('id', 'phylotree-node-graph-svg')
	      .attr("width", width + margin.left + margin.right)
  .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

 d3.json(dataUrl, function(error, data) {
   if(error) {
     console.log(error);
     return;
   }
   
   data.x0 = 0;
   data.y0 = 0;
   
   update(root = data);
   $('.ajax-loader').remove();
   
 });

 // update is a separate function because it will be called when
 // nodes are expanded or collapsed.
function update(source) {

   // Compute the flattened node list. TODO use d3.layout.hierarchy.
  var nodes = tree.nodes(root);

  var height = Math.max(500, nodes.length * barHeight + margin.top + margin.bottom);

  d3.select("#phylotree-node-graph-svg").transition()
      .duration(duration)
      .attr("height", height);

  d3.select(self.frameElement).transition()
      .duration(duration)
      .style("height", height + "px");

  // Compute the "layout".
  nodes.forEach(function(n, i) {
    n.x = i * barHeight;
  });

  // Update the nodes…
  var node = svg.selectAll("g.node")
      .data(nodes, function(d) { return d.id || (d.id = ++i); });

  var nodeEnter = node.enter().append("g")
      .attr("class", "node")
      .attr("transform", function(d) { return "translate(" + source.y0 + "," + source.x0 + ")"; })
      .style("opacity", 1e-6);

  // Enter any new nodes at the parent's previous position.
  nodeEnter.append("rect")
      .attr("y", -barHeight / 2)
      .attr("height", barHeight)
      .attr("width", barWidth)
      .style("fill", color)
      .on("click", click);

  nodeEnter.append("text")
      .attr("dy", 3.5)
      .attr("dx", 5.5)
      .text(function(d) { return d.name; });

  // Transition nodes to their new position.
  nodeEnter.transition()
      .duration(duration)
      .attr("transform", function(d) { return "translate(" + d.y + "," + d.x + ")"; })
      .style("opacity", 1);

  node.transition()
      .duration(duration)
      .attr("transform", function(d) { return "translate(" + d.y + "," + d.x + ")"; })
      .style("opacity", 1)
    .select("rect")
      .style("fill", color);

  // Transition exiting nodes to the parent's new position.
  node.exit().transition()
      .duration(duration)
      .attr("transform", function(d) { return "translate(" + source.y + "," + source.x + ")"; })
      .style("opacity", 1e-6)
      .remove();

  // Update the links…
  var link = svg.selectAll("path.link")
      .data(tree.links(nodes), function(d) { return d.target.id; });

  // Enter any new links at the parent's previous position.
  link.enter().insert("path", "g")
      .attr("class", "link")
      .attr("d", function(d) {
        var o = {x: source.x0, y: source.y0};
        return diagonal({source: o, target: o});
      })
    .transition()
      .duration(duration)
      .attr("d", diagonal);

  // Transition links to their new position.
  link.transition()
      .duration(duration)
      .attr("d", diagonal);

  // Transition exiting nodes to the parent's new position.
  link.exit().transition()
      .duration(duration)
      .attr("d", function(d) {
        var o = {x: source.x, y: source.y};
        return diagonal({source: o, target: o});
      })
      .remove();

  // Stash the old positions for transition.
  nodes.forEach(function(d) {
    d.x0 = d.x;
    d.y0 = d.y;
  });
  
}

// Toggle children on click.
function click(d) {
  if (d.children) {
    d._children = d.children;
    d.children = null;
  } else {
    d.children = d._children;
    d._children = null;
  }
  update(d);
}

function color(d) {
  return d._children ? "#3182bd" : d.children ? "#c6dbef" : "#fd8d3c";
}

 /// jquery document.ready
});
 
 })(jQuery);
 
</script>

<style>

 .bubble {
   background: #eee;
 }

</style>

<script>
 /***** d3js graph of phylotree organisms *****/
 
(function ($) {
   // drupal7 jquery outer context
    
 $(document).ready(function () {
 <?php
 print sprintf('var dataurl2 = "?q=chado_phylotree_organisms/%d/json";',
               $phylotree->phylotree_id );
 ?>
 
 var width = 600,
     height = 300,
     format = d3.format(",d"),
     fill = d3.scale.category20();
 
 var bubble = d3.layout.pack()
     .sort(null)
     .size([width, height]);
 
 var vis = d3.select("#phylotree-organisms-graph").append("svg:svg")
     .attr("width", width + 'px')
     .attr("height", height +'px')
     .attr("class", "bubble");
 
 d3.json(dataurl2, function(error,data) {
   if(error) {
     console.log(error);
     return;
   }
   if(! data || data.length == 0) {
     // this may occur if there  is no organism data! (e.g. ncbi taxon tree)
     $('#phylotree-organisms-graph').hide();
     return;
   }
   var min = d3.min(data, function(d) {
     return d.value;
   });
   var max = d3.max(data, function(d) {
     return d.value;
   });

   /*
   var radiusScale = d3.scale.log()
			     .domain([min,max])
			     .range([30, 100]);
    */
   
      var node = vis.selectAll("g.node")
		    .data(bubble.nodes( { 'children' : data } )
				.filter(function(d) { return !d.children; }))
		    .enter().append("svg:g")
		    .attr("class", "node")
		    .attr("transform", function(d) { return "translate(" + d.x + "," + d.y + ")"; });
 
   node.append("svg:title")
       .text(function(d) { return d.name + ": " + format(d.value); });
 
   node.append("svg:circle")
       .attr("r", function(d) {
//     console.log(d);
     return d.r;
//     return radiusScale(d.value);
   })
       .style("fill", function(d) { return fill(d.organism_id); });
 
   node.append("svg:text")
       .attr("text-anchor", "middle")
       .attr("dy", ".3em")
       .text(function(d) {
         return d.name + ' (' + d.value +')';
   });
 });

  d3.select(self.frameElement).style("height", height + "px");
 
 // /jquery document.ready
 });
 // /drupal7 jquery outer context
})(jQuery);
 
</script>
