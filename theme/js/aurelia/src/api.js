/* a singleton/service for loading source data files and managing crossfilter
   objects. In a real app, this would talk to a rest API and not load static
   data files */
import {inject} from 'aurelia-framework';
import {HttpClient} from 'aurelia-fetch-client';
import {EventAggregator} from 'aurelia-event-aggregator';

import {CrossfilterCreated, FilterUpdated, DimensionAdded} from 'topics';
import * as crossfilter2 from 'crossfilter2';
let crossfilter = crossfilter2.crossfilter;
let fasta = msa.io.fasta; /* msa lib is loaded in <script> tag */

@inject(HttpClient, EventAggregator)
export class Api {

    TREE_URL = API.tree; // global var was defined in drupal template
    MSA_URL =  API.msa;  // global var was defined in drupal template

    _cf = null;
    _dims = {};

    treeData = null;
    msaSeqs = null;
    flatData = []; // same as tree leaves
    index = {};

    constructor(http, ea) {
        this.http = http;
        this.ea = ea;
    }

    init() {
        // init() is called by app.js, after components are attached.
        let p1 = this.getTreeData();
        let p2 = this.getMsaData();
        // after both resources to load, then do more setup.
        Promise.all([p1, p2]).then(() => {
            this.postProcess();
            this.setupCrossFilter();
        });
    }

    // by convention, the consensus seq is the first in the msa seqs array.
    getConsensusSeq() {
      return this.msaSeqs[0];
    }

    setupCrossFilter() {
        // create crossfilter from flat data, not from tree data.
        let msg = null;
        let cf = crossfilter(this.flatData);
        this.cf = cf;
        //
        // // create a dimension by feature name. share with all consumers of this
        // // crossfilter.
        // let dim = cf.dimension(d => d.name);
        // this._dims.featureName = dim;

        msg = new CrossfilterCreated('demoGeneFamily', cf);
        this.ea.publish(msg);
        //
        // msg = new DimensionAdded('featureName', dim);
        // this.ea.publish(msg);
    }

    getTreeData() {
        let promise = this.http.fetch(this.TREE_URL)
            .then(res => res.json())
            .then(data => {
              this.treeData = data;
              this.parseTree(data);
              console.log('# tree leaves: ' + this.flatData.length);
              return this.treeData;
            })
            .catch(err => {
                console.warn(err);
            });
        return promise;
    }

    getMsaData() {
        let promise = this.http.fetch(this.MSA_URL)
            .then(res => res.text())
            .then(data => {
                this.msaFasta = data;
                this.msaSeqs = fasta.parse(data);
                return this.msaSeqs;
            })
            .catch(err => {
                console.warn(err);
            });
        return promise;
    }

    // postProcess() : add the corresponding MSA sequence to each record in the
    // flatData set.
    postProcess() {
        console.log('# msa seqs: ' + this.msaSeqs.length);
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

        if (node.length) {
          // add a property expected by tnt.tree
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
}
