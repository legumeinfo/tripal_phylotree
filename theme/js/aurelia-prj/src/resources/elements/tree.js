import {inject, bindable} from 'aurelia-framework';
import {EventAggregator} from 'aurelia-event-aggregator';

import {Api} from 'api';
import {CrossfilterCreated, FilterUpdated} from 'topics';


@inject(Api, EventAggregator)
export class Tree {

  WIDTH = window.innerWidth - 100;
  DURATION_MS = 300;

  loading = true;
  selectedLayout = 'vertical';
  _rootNode = null;
  _tree = null;
  _cf = null;
  _dims = {};
  _grps = {};

  constructor(api, ea) {
    this.api = api;
    this.ea = ea;
  }

  attached() {
    this.subscribe();
  }

  subscribe() {
    this.ea.subscribe(CrossfilterCreated, msg => {
      let cf = this._cf = msg.crossfilter;
      this._dim = cf.dimension(d => d.name);
      this.init();
    });
    this.ea.subscribe(FilterUpdated, msg => {
      if(msg.sender != this) {
        this.update();
      }
    });
  }

  // init() : create the tnt.tree chart
  init() {
    this._rootNode = this.api.treeData;
    this._tree = tnt.tree()
      .data(this._rootNode)
      .duration(this.DURATION_MS)
      .layout(tnt.tree.layout.vertical()
        .width(this.WIDTH)
        .scale(true)
      );
    this._tree.on('click', (node) => this.onToggleTreeNode(node) );
    this._tree(this.chartElement);
    this.loading = false;
  }

  onToggleTreeNode(node) {
      // toggle node state, and refresh display of tree
      node.toggle();
      //this.update();
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
    this.ea.publish(new FilterUpdated(this));
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
