import {inject, bindable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';

let $ = jQuery;

@inject(Api, BindingEngine, Symbology)
export class Tree {

  WIDTH = window.innerWidth - 100;
  DURATION_MS = 300;

  selectedLayout = 'vertical';
	@bindable familyName;

	hilitedNodes = [];
	node = null; // clicked node for expand/collapse/other dialog options
	
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
    this.subscribe();
  }

  subscribe() {
		this.be.propertyObserver(this.api, 'cf')
		 	.subscribe( o => this.onCfCreated(o));
		this.be.propertyObserver(this.api, 'cfUpdated')
		 	.subscribe( o => this.onCfUpdated(o));
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
		nd.fill( node => this.getNodeColor(node) );
		this._tree.node_display(nd);

		// override the default labelling in increase verticalspacing
		let labeler = this._tree.label();
		labeler.height(18);
		this._tree.label(labeler);
		
		// add event handler for node clicks
		this._tree.on('click', node => this.onTreeNodeClick(node) );

		// display the tree
    this._tree(this.phylogramElement);
  }

	// get the fill color of each node
	getNodeColor(node) {
		if(node.is_leaf()) {
			let d = node.data();
			return this.symbology.color(d.genus + ' ' + d.species);
		}
		// else the color will be defined by tree.css, not by a fill attribute.
	}
	
  onTreeNodeClick(node) {
		this.node = node;
		let nodeSelector = '#tnt_tree_node_tree-chart_'+ node.id();
		let nodeEl = $(nodeSelector);
		let dialog = $(this.nodeDialogEl);
		let opts = {
			title: null,
      closeOnEscape: true,
      modal: false,
			position: {
				my: 'center', at: 'center', of: nodeEl
			},
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
		dialog.dialog(opts);
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
