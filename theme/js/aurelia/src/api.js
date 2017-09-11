/* a singleton/service for loading source data files and managing crossfilter
 * objects.
 * 
 * note: all http error handling is in the app.js http interceptor.
 */

import {inject, observable} from 'aurelia-framework';
import {HttpClient} from 'aurelia-fetch-client';
import * as crossfilter2 from 'crossfilter2';
let crossfilter = crossfilter2.crossfilter;
let fasta = msa.io.fasta; /* msa lib is loaded in <script> tag */

@inject(HttpClient)
export class Api {

  TREE_URL = API.tree; // global var was defined in drupal template
  MSA_URL =  API.msa;  // global var was defined in drupal template

	// the endpoint for the "family representative" links from the
	// tripal_linkout module.
	FAMREPS_LINKS_URL = '/famreps_links';
	LEAF_LINKS_URL = '/phylotree_links/'; // genus//speciesnode.feature_name/json

	// declare some observable properties for use by other vis. elements:
	@observable cf; // crossfilter object
	@observable cfUpdated; // a crossfilter updated message
	
  treeData = null;
  msaSeqs = null;
	loading = true;
  flatData = []; // same as tree leaves
  index = {};
	
  constructor(http) {
    this.http = http;
  }

  init() {
    // init() is called by app.js, after components are attached.
    let p1 = this.getTreeData();
    let p2 = this.getMsaData();
    // after both resources to load, then do more setup.
    let p3 = Promise.all([p1, p2]).then(() => {
			this.loading = false;
      this.postProcess();
      this.setupCrossFilter();
    });
		return p3;
  }
	
  // by convention, the consensus seq is the first in the msa seqs array.
  getConsensusSeq() {
    return (this.msaSeqs[0].name.endsWith("-consensus") ? this.msaSeqs[0] : undefined);
  }
	
  setupCrossFilter() {
    // create crossfilter from flat data, not from tree data.
    let msg = null;
		this.cf = crossfilter(this.flatData);
  }
	
  getTreeData() {
		if(treeData) {
			// have global var from the php template
			this.treeData = treeData;
			this.parseTree(treeData);
			return Promise.resolve(treeData);
		}
		else {
			let promise = this.http.fetch(this.TREE_URL)
					.then(res => res.json())
					.then(data => {
						this.treeData = data;
						this.parseTree(data);
						return this.treeData;
					});
			return promise;
		}
  }
	
  getMsaData() {
    let promise = this.http.fetch(this.MSA_URL)
        .then(res => res.text())
        .then(data => {
          this.msaFasta = data.trim(); // because ajax call to drupal has an initial empty line
          this.msaSeqs = fasta.parse(this.msaFasta);
          return this.msaSeqs;
        });
    return promise;
  }
	
  // postProcess() : add the corresponding MSA sequence to each record in the
  // flatData set.
  postProcess() {
    _.forEach(this.msaSeqs, d => {
      if (!d.seq || !d.name) {
        return;
      }
      let rec = this.index[d.name];
      if (rec) {
        rec.msa = d;
      }
    });
  }
	
  // parseTree() : build an index of names, add properties as needed, and
  // create a flattened version of the tree.
  parseTree(node) {
    let that = this;
		
    if (! node.name) {
      node.name = '';
    }
    else {
      this.index[node.name] = node;
    }
    if ('length' in node) {
       // add a property expected by tnt.tree- contrary to docs saying
       // it will work with the length property.
      node.branch_length = node.length;
    }
    if (! node.children || node.children.length === 0) {
      this.flatData.push(node);
      return;
    }
    _.each(node.children, function (d) {
      d.parent = node;
      that.parseTree(d);
    });
  }

	getLinkouts(node) {
		let d = node.data();
		if(node.is_leaf() && !node.is_collapsed()) {
			let url = `${this.LEAF_LINKS_URL}${d.genus}/${d.species}/${d.feature_name}/json`;
			let promise = this.http.fetch(url)
					.then(res => res.json());
			return promise;
		}
		else if(node.legumes.length > 0) {
			// for an interior node, request the familty representatives.
			// use POST because the number of features can exceed allowed
			// URL length. note: this is a bit of a hack because PHP 5.6
			// barfs when receiving a POST with application/json
			// content-type. As a workaround, send x-www-form-urlencoded
			// instead, and specify the body parameter.
			let url = this.FAMREPS_LINKS_URL;
			let query = 'famreps=' + node.legumes.map(n => n.data().feature_name).join(',');
			let promise = this.http.fetch(url, {
				method: 'post',
				body: query,
				headers: {
					'content-type': 'application/x-www-form-urlencoded'
				}
			})
					.then(res => res.json());
			return promise;
		}
		else {
			return Promise.resolve([]);
		}
	}
}
