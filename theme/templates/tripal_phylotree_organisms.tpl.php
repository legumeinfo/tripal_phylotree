<?php
$phylotree = $variables['node']->phylotree;
?>

<div id="phylotree-organisms-graph">
  <!-- d3 will add svg to this div -->
</div>

<script>
 /***** d3js graph of phylotree organisms *****/
 
(function ($) {
   // drupal7 jquery outer context
    
 $(document).ready(function () {
 <?php
 print sprintf('var dataurl2 = "?q=chado_phylotree_organisms/%d/json";',
               $phylotree->phylotree_id );
 ?>
 
 var width = 550,
     height = 400,
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
