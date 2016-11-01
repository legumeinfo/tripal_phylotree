import {inject, bindable, parseQueryString, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';

let $ = jQuery;

@inject(Api, BindingEngine, Symbology)
export class Tree {

  WIDTH = window.innerWidth - 200; // tripal layout has a gutter, plus margins
  DURATION_MS = 300;
	LABEL_HEIGHT = '10px';
	LABEL_BASELINE_SHIFT = '30%';
	AXIS_TICKS = 12;
	AXIS_SAMPLE_PX = 30; // pixels
	
	@bindable familyName; // family-name attribute of <tree> element
	@bindable msaEl;      // reference to <msa> element
	@bindable showDialog; // two-way databinding for toggling dialog in app.js
	
	msa = null;           // msa view-model
	selectedLayout = 'vertical';
	hiliteFeatures = {};
	node = null;       // clicked node for expand/collapse/other dialog options.
	loading = false;       // loading flag for use by tree node popup dialog.
	
	rootNodeDirty = false; // flag for has user refocused the tree on some node.
  _rootNode = null; // initially _rootNode is api.treeData, but my be
										// updated by onNodeFocusTree.
  _tree = null;
  _cf = null;
  _dims = {};
  _grps = {};

  constructor(api, be, sym) {
    this.api = api;       // web api
		this.be = be;         // binding engine
		this.symbology = sym; // symbology
  }

  attached() {
		// aurelia bound the <msa> element to a variable, but we need the
		// msa's view-model (msa.js)
		this.msa = this.msaEl.au.controller.viewModel;
		this.initHiliteFeatures();
    this.subscribe();
  }

	// parse the url query string into an object keyed by feature name
	// to hilite. aurelias parser will transform this into an array:
	// ?hilite_node=xxx&hilite_node=yyy but wont parse this, so we have
	// to check for this nonstandard encoding of parameters:
	// ?hilite_node=x,y,z
	initHiliteFeatures() {
		let q = parseQueryString(window.location.search);
		if(! 'hilite_node' in q) {
			this.hiliteFeatures = {};
			this.hiliteFeaturesCount = 0;
			return;
		}
		// convert hilite_node into an array.
		if(typeof q.hilite_node === 'string') {
			if(q.hilite_node.indexOf(',') !== -1) {
				q.hilite_node = q.hilite_node.split(',');
			}
			else {
				q.hilite_node = [ q.hilite_node ];
			}
		}
		let keys = _.map(q.hilite_node, n => n.toLowerCase());
		let vals = _.map(keys, n => true);
		this.hiliteFeatures = _.zipObject(keys, vals);
		this.hiliteFeaturesCount = keys.length;
	}

	// the leaf nodes need to have a css selector, so lisTours can
	// attach a tour step to a legume leaf node.
	decorateLeafNodes() {
		d3.selectAll('#phylogram .leaf.tnt_tree_node')
			.filter((d) => this.isLegume(d))
			.classed('legume', true);
	}
	
  subscribe() {
		this.be.propertyObserver(this.api, 'cf')
		 	.subscribe(o => this.onCfCreated(o));
		this.be.propertyObserver(this.api, 'cfUpdated')
		 	.subscribe(o => this.onCfUpdated(o));
		this.be.propertyObserver(this.msa, 'selectedFeatureNames')
			.subscribe(o => this.onMsaSelectionChange(o));
  }

	onMsaSelectionChange(newValue, oldValue) {
		// val is like {arath.AT2G14835.1: true, glyma.Glyma.20G133500.1: true}
		let names = _.keys(newValue);
		names = _.map(names, n => n.toLowerCase());
		names = _.filter(names, n => {
			// filter out the consensus sequence because it is not shown in tree view
			return n.indexOf('consensus') === -1;
		});
		let vals = _.map(names, () => true);
		this.hiliteFeatures = _.zipObject(names, vals);
		this.hiliteFeaturesCount = names.length;
		this._tree.update_nodes(); // use tree api to refersh tree nodes & labels.
		this.updateLeafNodeHilite(false); 
	}

	// tnt.tree api does not support selections or hiliting that I can
	// see, so use d3 to decorate the currently selected features with a 
	// hilited css style.
	updateLeafNodeHilite(scroll) {
		let that = this;
		d3.selectAll('text.tnt_tree_label')
			.filter((d) => d.name.toLowerCase() in that.hiliteFeatures)
			.each( function(d) {
				d.bbox = this.getBBox();
			});
		let top = Infinity;
		// the text labels bounding box was set in d.bbox, so use that to
		// draw a rect with the hilite color.
		d3.selectAll('g.tnt_tree_node')
			.filter((d) => d.name.toLowerCase() in that.hiliteFeatures)
			.insert('svg:rect', ':first-child')
			.attr('x', (d) => {
				if(d.textAnchor === 'end') {
					// textAnchor was set dynamically for the radial layout
					return d.bbox.x + d.bbox.width + 10;
				}
				return d.bbox.x + 10;
			})
			.attr('y', (d) => d.bbox.y/2)
			.attr('width', (d) => d.bbox.width + 2)
			.attr('height', (d) => d.bbox.height + 1)
			.attr('class', 'hilite-node')
		  .each(function() {
				var offset = $(this).offset();
				if(offset.top > 0 && offset.top < top) {
					top = offset.top;
				}
			});
		if(scroll && _.keys(this.hiliteFeatures).length) {
			$('html,body').attr('scrollTop',  top - 100);
		}
	}
	
	/*
	 * onScrollToHilite() : use jquery to scroll to the tree element
	 * with selector like #tnt_tree_node_phylogram_{id} (id is the
	 * tnt.tree generated it)
	 */
	onScrollToHilite(featureName) {
		let node = this._tree.root().find_node( node => {
			return node.node_name().toLowerCase() === featureName;
		});
		let nodeId = node.property('_id');
		let selector = '#tnt_tree_node_phylogram_'+ nodeId;
		let offset = $(selector).offset();
		$('html,body').attr('scrollTop',  offset.top - 100);
	}
	
	onCfCreated(cf) {
    this._cf = cf;
		// create a dimension by name (keep our own instance of this dimension)
    this._dim = cf.dimension(d => d.name);
    this.init();
	}
	
	onCfUpdated(msg) {
		if(msg.sender != this) {
			this.update();
		}
	}
	
  // init() : create the tnt.tree chart
  init() {
		let that = this;
    this._tree = tnt.tree()
      .data(this.api.treeData)
      .duration(this.DURATION_MS)
      .layout(tnt.tree.layout.vertical()
							.width(this.WIDTH)
							.scale(true));
		this._rootNode = this._tree.root();
		// override the default node display props
		let nd = this._tree.node_display();
		nd.size(6);
		nd.fill(node => this.getNodeColor(node));
		this._tree.node_display(nd);
		// optimize the labeling, code adapted from
		// https://github.com/tntvis/tnt.tree/blob/master/src/label.js
		let labeler = tnt.tree.label.text();
		labeler.display (  function (node, layout_type) {
			let d = node.data();
			let hilite = d.name.toLowerCase() in that.hiliteFeatures;
			let l = d3.select(this) // note: this is the d3js "this"
			    .append('text')
			    .text((d) => d.name)
			    .style('font-size', that.LABEL_HEIGHT)
					.style('font-weight', (d) => hilite ? 'bold' : 'normal')
					.style('baseline-shift', that.LABEL_BASELINE_SHIFT)
			    .style('fill', '#000')
			    .attr('text-anchor', (d) => {
						if (layout_type === 'radial') {
							d.textAnchor = (d.x%360 < 180) ? 'start' : 'end';
						}
						else {
							d.textAnchor = 'start';
						}
						return d.textAnchor; // d.textAnchor also used in label hiliting
					});
			return l;
		});
		this._tree.label(labeler);

		// add event handlers for node interaction
		this._tree.on('click', node => this.onTreeNodeClick(node) );
		this._tree.on('mouseover', node => this.onNodeMouseover(node));
		this._tree.on('mouseout', node => this.onNodeMouseout(node));
		
		// display the tree with msa tnt.tree component
    this._tree(this.phylogramElement);

		// use tnt.tree api to calculate intergenic distance over some screen dist.
		let distance = this._tree.scale_bar(this.AXIS_SAMPLE_PX, 'pixel');
		if(distance === undefined) {
			// this occurs for some trees
			console.error('failed to lookup intergenic distance for tree:');
			console.log(this.api.treeData);
		}
		else {
			d3.select(this.phylogramAxisElement)
				.insert('svg')
				.attr('width', this.WIDTH)
				.attr('height', 40)
				.append('g')
				.attr('transform', 'translate(20,20)')
				.attr('class', 'x axis')
				.call(this.getXAxis(distance));
		}
		
		// perform final ui tweaks after rendering the tree
		this.decorateLeafNodes();
		if(_.keys(this.hiliteFeatures).length) {
			this.updateLeafNodeHilite(true);
		}
  }

	getXAxis(d) {
		let distance = d || this._tree.scale_bar(this.AXIS_SAMPLE_PX, 'pixel');
		let scale = d3.scale.linear()
				.domain([0, distance * this.WIDTH/this.AXIS_SAMPLE_PX ])
				.range([0, this.WIDTH]);
		let axis = d3.svg.axis()
				.scale(scale)
				.ticks(this.AXIS_TICKS)
				.orient('bottom');
		return axis;
	}
	
	updateXAxis() {
		d3.selectAll('#phylogram-axis g.x.axis').call(this.getXAxis());
	}
	
	// lookup the node with jquery and return a jquery element
	node2jquery(node) {
		let nodeSelector = this.node2jquerySelector(node);
		let nodeEl = $(nodeSelector);
		return nodeEl;
	}

	node2jquerySelector(node) {
		return '#tnt_tree_node_phylogram_'+ node.id();		
	}
	
	onNodeMouseover(node) {
		let el = this.node2jquery(node);
    el.attr('cursor', 'pointer');
	}

	onNodeMouseout(node) {
		let el = this.node2jquery(node);
    el.attr('cursor', 'default');		
	}
	
	// get the fill color of each node
	getNodeColor(node) {
		if(node.is_leaf() && ! node.is_collapsed()) {
			let d = node.data();
			return this.symbology.color(d.genus + ' ' + d.species);
		}
		// else the color will be defined by tree.css, not by a fill attribute.
	}

	isLegume(d) {
		if( ! d.genus) { return false; }
		let legumeGenera = this.symbology.legumes;
		return d.genus.toLowerCase() in legumeGenera;
	}
	
  onTreeNodeClick(node) {
		this.loading = true;
		let that = this;
		this.node = node;
		this.node.legumes = _.filter(node.get_all_leaves(true), n => {
			return this.isLegume(n.data());
		});
		this.api.getLinkouts(node)
			.then(data => {
				that.node.linkouts = data;
				that.loading = false;
			});
		let dialog = $(this.nodeDialogEl);
		let opts = {
			title: null,
      closeOnEscape: true,
      modal: false,
			position: {
				my: 'center', at: 'center', of: this.node2jquery(node)
			},
			show : { effect: 'blind', direction: 'down', duration: 100 },
			//close: (event, ui) => {}
		};
		if(node.is_collapsed()) {
			opts.title = 'collapsed subtree';
		}
		else if(node.is_leaf()) {
			opts.title = node.node_name();
		}
		else {
			opts.title = 'interior node';
		}
		// aurelia updates the dom asynchronously, so delay before showing
		// the jquery.ui dialog
		setTimeout( () => dialog.dialog(opts) );
	}

	// toggle node state, and refresh display of tree
	onNodeToggle() {
		let dialog = $(this.nodeDialogEl);
		dialog.dialog('close');
		this.node.toggle();
		this._tree.update();
		this.updateLeafNodeHilite(false);
		setTimeout(() => this.updateFilter(), this.DURATION_MS);
	}

	// onNodeFocusTree() : replace the tree viewer with the subtree
	onNodeFocusTree() {
		let dialog = $(this.nodeDialogEl);
		dialog.dialog('close');
		// expand all children of the current node
		let node = this.node;
		node.apply( n => {
			if(n.is_collapsed()) {
				n.toggle();
			}
		});
		// extract the subtree and re-root the tree visualization
		let subtree = this._tree.root().subtree(node.get_all_leaves(true));
		this._tree.data(subtree.data());
		this._rootNode = this._tree.root();
		this._tree.update();
		this.updateLeafNodeHilite(false);
		this.rootNodeDirty = true;
		this.hiddenLeavesNum = 0;
		setTimeout(() => this.updateFilter(), this.DURATION_MS);
	}

  updateFilter() {
    // update crossfilter with currently visible nodes
    let root = this._tree.root();
    // get leaves from the tnt.tree api. leaves counts collapsed node as leaf.
    let allLeaves = root.get_all_leaves(true);
    let visibleLeaves = root.get_all_leaves(false); // dont traverse collapsed nodes.
    this.hiddenLeavesNum = allLeaves.length - visibleLeaves.length;
		if(this.hiddenLeavesNum > 0) {
			// the visibleLeaves count includes 1 collapsed node
			this.hiddenLeavesNum += 1;
		}
		
    let visibleNodes = {};
    _.forEach(visibleLeaves, (node) => {
      let n = node.data().name;
      if(n) {
        visibleNodes[n] = true;
      }
    });
    this._dim.filterAll(); // filters are additive per dimension, so clear out previous
    this._dim.filterFunction((d) => visibleNodes[d]);
		
		this.api.cfUpdated = { sender: this };
  }

  /* update the view-model with current crossfilter results */
  update() {
    // restore full tree from current root node
    this._tree.data(this._rootNode.data());
    // get list of featurenames from crossfilter
    let data = this._dim.top(Infinity);
    let featureNames = _.map(data, d => d.name);
    let hash = _.keyBy(featureNames, n => n);
    // generate the tree leaves matching selected feature names
    let root = this._tree.root();
    let leaves = root.find_all(n => hash[n.node_name()], true);
    // use tnt.tree api to display only the selected subtree
    let subtree = root.subtree(leaves);
    this._tree.data(subtree.data());
    this._tree.update();
		this.updateLeafNodeHilite(false);
  }

  onLayout() {
    var layout = null;
    if (this.selectedLayout === 'vertical') {
      layout = tnt.tree.layout.vertical();
    }
    else if (this.selectedLayout === 'radial') {
      layout = tnt.tree.layout.radial();
    }
    layout.width(this.WIDTH).scale(true);
    this._tree.layout(layout);
    this._tree.update();
		this.updateLeafNodeHilite(false);
		setTimeout(() => this.updateXAxis(), this.DURATION_MS);
  }

	// restore original root node, and reload the data, update the ui.
  onReset() {
    this._tree.data(this.api.treeData);
    let root = this._rootNode = this._tree.root();
    let nodes = root.get_all_nodes(true);
    _.forEach(nodes, n => {
      if(n.is_collapsed()) {
        n.toggle();
      }
    });
    this.hiddenLeavesNum = 0;
		this.rootNodeDirty = false;
    this._tree.update();
		this.updateLeafNodeHilite(true);
    setTimeout(() => this.updateFilter(), this.DURATION_MS);
  }

}

export class KeysValueConverter {
	toView(obj) {
		return Reflect.ownKeys(obj);
	}
}
