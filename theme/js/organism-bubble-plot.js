
/* d3js graph of phylotree organisms 

selector : jquery selector for div to insert svg content
data : json data
options : {  fill : color function for each node, 
             width :  svg width & height,
             height : svg height }
*/

function organismBubblePlot( selector, data, options ) {
  
  /* flatten the tree structure to sum organisms. the bubble layout
   * expects a name property, but the organismColor function expects
   * the abbreviation property to exist as well. */
  var organisms = {};
  var countOrganisms = function(node, i, arr) {
    if (node.organism_id) {
      if( ! organisms[ node.organism_id ] ) {
        // copy only the organisms related metadata, discarding feature info
        organisms[ node.organism_id ] = {
          'organism_id' : node.organism_id,
          'name' : node.abbreviation,
          'abbreviation' : node.abbreviation,
          'genus' : node.genus,
          'species' : node.species,
          'common_name' : node.common_name,
          'organism_node_id' : node.organism_node_id,
          'value' : 1 // count
        };
      }
      else {
        organisms[ node.organism_id ]['value']++;
      }
    }
    if(node.children) {
      node.children.forEach(countOrganisms);
    }
  }
  data.children.forEach(countOrganisms);

  // flatten counts hash into an array
  var organismsArr = [];
  for (var organismId in organisms) {
    organismsArr.push( organisms[organismId] );
  }

  options = options || {};
  var fill = options.fill || function(node) {
    return 'cyan';
  };
  var nodeMouseOver = options.nodeMouseOver || function(d) {};
  var nodeMouseOut = options.nodeMouseOut || function(d) {};
  var nodeMouseDown = options.nodeMouseDown || function(d) {};
  
  var w = options.width || d3.select(selector).style('width') || d3.select(selector).attr('width'),
  h = options.height || d3.select(selector).style('height') || d3.select(selector).attr('height'),
  w = parseInt(w),
  h = parseInt(h);
  
  var format = d3.format(",d");
  
  var bubble = d3.layout.pack()
    .sort(null)
    .size([w, h]);
  
  var vis = d3.select(selector).append("svg:svg")
    .attr("width",  w +'px')
    .attr("height", h +'px')
    .attr("class", "bubble");
  
  var node = vis.selectAll("g.node")
    .data(bubble.nodes( { 'children': organismsArr } )
          .filter(function(d) { return !d.children; }))
    .enter().append("svg:g")
    .attr("class", "node")
    .on('click', nodeMouseDown)
    .on('mouseover', nodeMouseOver)
    .on('mouseout', nodeMouseOut)    
    .attr("transform", function(d) { return "translate(" + d.x + "," + d.y + ")"; });
  
  node.append("svg:title")
    .text(function(d) { return d.name + ": " + format(d.value); });
  
  node.append("svg:circle")
    .attr("r", function(d) { return d.r; })
    .style("fill", function(d) { return fill(d); });
  
  node.append("svg:text")
    .attr("text-anchor", "middle")
    .attr('font-size', '90%')
    .attr("dy", ".3em")
    .text(function(d) {
      return d.name + ' (' + d.value +')';
    });
  d3.select(self.frameElement).style("height", h + "px");
}
