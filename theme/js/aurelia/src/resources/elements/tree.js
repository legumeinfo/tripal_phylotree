import {inject, bindable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';

let $ = jQuery;

@inject(Api, BindingEngine, Symbology)
export class Tree {

  WIDTH = window.innerWidth - 100;
  DURATION_MS = 300;
	LABEL_HEIGHT = '10px';
	LABEL_BASELINE_SHIFT = '30%';
	HILITE_COLOR = 'goldenrod';
	
	@bindable familyName; // family-name attribute of <tree> element
	@bindable msaEl;      // reference to <msa> element
	@bindable showDialog; // two-way databinding for toggling dialog in app.js
	
	msa = null;           // msa view-model
	selectedLayout = 'vertical';
	hilitedFeatures = {};
	node = null; // clicked node for expand/collapse/other dialog options.
	loading = false; // loading flag for use by tree node dialog.
	
  _rootNode = null;
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
    this.subscribe();
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
		this.hilitedFeatures = newValue;
		this._tree.update_nodes(); // use tree api to refersh tree nodes & labels.
		this.updateLeafNodeHilite(); 
	}

	// tnt.tree api does not support selections or hiliting that I can
	// see, so use d3 to decorate the currently selected features with a 
	// hilited css style.
	updateLeafNodeHilite() {
		let that = this;
		let hilite = {
			padding: 5, width: 200, height: 20,
			color: 'yellow'
		};
		d3.selectAll('text.tnt_tree_label')
			.filter((d) => d.name in that.hilitedFeatures)
			.each( function(d) {
				d.bbox = this.getBBox();
			});
		// the text labels bounding box was set in d.bbox, so use that to
		// draw a rect with the hilite color.
		d3.selectAll('g.tnt_tree_node')
			.filter((d) => d.name in that.hilitedFeatures)
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
			.attr('class', 'hilite-node');
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
    this._rootNode = this.api.treeData;
    this._tree = tnt.tree()
      .data(this._rootNode)
      .duration(this.DURATION_MS)
      .layout(tnt.tree.layout.vertical()
							.width(this.WIDTH)
							.scale(true));
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
			let hilite = d.name in that.hilitedFeatures;
			let l = d3.select(this) // note: this is the d3js "this"
			    .append('text')
			    .text((d) => d.name)
			    .style('font-size', that.LABEL_HEIGHT)
					.style('font-weight', (d) => hilite ? 'bold' : 'normal')
					.style('baseline-shift', that.LABEL_BASELINE_SHIFT)
			    .style('fill', '#000')
			    .attr('id', 'tree-label-'+ d.name)
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
		
		// display the tree
    this._tree(this.phylogramElement);
  }
	
	// lookup the node with jquery
	node2jQuery(node) {
		let nodeSelector = '#tnt_tree_node_tree-chart_'+ node.id();
		let nodeEl = $(nodeSelector);
		return nodeEl;
	}
	
	onNodeMouseover(node) {
		let el = this.node2jQuery(node);
    el.attr('cursor', 'pointer');
	}

	onNodeMouseout(node) {
		let el = this.node2jQuery(node);
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
	
  onTreeNodeClick(node) {
		this.loading = true;
		let that = this;
		this.node = node;
		let legumeGenera = this.symbology.legumes;
		 this.node.legumes = _.filter(node.get_all_leaves(true), n => {
			 let d = n.data();
			 if(! d.genus) { return false; }
			 return d.genus.toLowerCase() in legumeGenera;
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
				my: 'center', at: 'center', of: this.node2jQuery(node)
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
		setTimeout(() => this.updateFilter(), this.DURATION_MS);
	}

  updateFilter() {
    // update crossfilter with currently visible nodes
    let root = this._tree.root();
    // get leaves from the tnt.tree api. leaves counts collapsed node as leaf.
    let allLeaves = root.get_all_leaves(true);
    let visibleLeaves = root.get_all_leaves(false); // dont traverse collapsed nodes.
    this.hiddenLeavesNum = allLeaves.length - visibleLeaves.length;
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
    // restore full tree from original root node
    this._tree.data(this._rootNode);
    // get list of featurenames from crossfilter
    let data = this._dim.top(Infinity);
    let featureNames = _.map(data, d => d.name );
    let hash = {};
    _.each(featureNames, n => hash[n] = true);
    // generate the tree leaves matching selected feature names
    let root = this._tree.root();
    let leaves = root.find_all( (n) => hash[n.node_name()], true);
    // use tnt.tree api to display only the selected subtree
    let subtree = root.subtree(leaves);
    this._tree.data(subtree.data());
    this._tree.update();
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
		this.updateLeafNodeHilite();
  }

  onReset() {
    this._tree.data(this._rootNode);
    let root = this._tree.root();
    let nodes = root.get_all_nodes(true);
    _.forEach(nodes, n => {
      if(n.is_collapsed()) {
        n.toggle();
      }
    });
    this.hiddenLeavesNum = 0;
    this._tree.update();
    setTimeout(() => this.updateFilter(), this.DURATION_MS);
  }

}
