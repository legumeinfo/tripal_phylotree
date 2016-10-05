import {inject, bindable, BindingEngine, TaskQueue} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';

let $ = jQuery;

@inject(Api, Symbology, TaskQueue, BindingEngine)
export class Taxon {
	
  DURATION_MS = 500

  hiddenTaxaCount = 0;
	
	@bindable() familyName = null;
	
  _chart = null;
  _cf = null;
  _dim = null; // crossfilter dimension
  _grp = null; // crossfilter group

  disabledTaxaNum = 0;

  constructor(api, sym, tq, be) {
    this.api = api;       // web api
		this.symbology = sym; // symbology
		this.tq = tq;         // task queue
    this.be = be;         // binding engine
  }

  created() {
    this.subscribe();
  }

  attached() {
		this.initNvD3Graph();
  }

	initNvD3Graph() {
    let that = this;
    nv.addGraph(function () {
      let chart = that._chart = nv.models.pieChart()
					.x(function (d) { return d.label })
					.y(function (d) { return d.value })
					.showLabels(true)
					.labelThreshold(.05)
					.labelType('percent')
					.donut(true)
					.donutRatio(0.35)
					.color( d => that.symbology.color(d.label) );
      chart.options({ legendPosition : 'right' });
      chart.margin({ left: 0, right: 0, top: 0, bottom: 0 });
      chart.dispatch.on('stateChange', evt => {
        setTimeout(() => that.onTaxonStateChange(evt), this.DURATION_MS);
      })
      return chart;
    });
	}

	initJqueryDialog() {
		this.dialog = $(this.taxonEl);
    this.dialog.dialog({
      title: 'Taxon',
      closeOnEscape: true,
      modal: false,
      width: '450px'
    });
		// TODO position, close properties
    //this.dialog.dialog('open');
	}
	
  onTaxonStateChange(event) {
    // the event.disabled property is a list of boolean values for
    // which taxa are active vs inactive.
    let data = this._grp.all();
    let disabled = event.disabled; // array of true/false
    let disabledTaxa = {};
    this.disabledTaxaNum = 0;
    for (var i = 0; i < disabled.length; i++) {
      let taxon = data[i].key;
      let b = disabled[i];
      disabledTaxa[taxon] = b;
      if(b) {
        this.disabledTaxaNum ++;
      }
    }
    this._dim.filter(null); // filters are additive per dimension.
    this._dim.filter( d => ! disabledTaxa[d] );
		
		this.api.cfUpdated = { sender: this };
  }

	
  subscribe() {
		this.be.propertyObserver(this.api, 'cf')
		 	.subscribe( o => this.onCfCreated(o));
		this.be.propertyObserver(this.api, 'cfUpdated')
		 	.subscribe( o => this.onCfUpdated(o));
  }

	onCfCreated(cf) {
    this._cf = cf;
    this.setupCrossfilter();
    this.update();
	}

	onCfUpdated(msg) {
    if(msg.sender != this) {
      this.update();
    }
	}

  setupCrossfilter() {
    // create a dimension for the taxon
    this._dim = this._cf.dimension(
      d => d.species ? d.genus + ' ' + d.species : ''
    );
    this._grp = this._dim.group();
  }

  update() {
    // get the current results from our group
    let data = this._grp.all();
    // filter out taxa having 0 value (count)
    data = _.filter(data, d => d.value > 0 );
    // transform to objects expected by nvd3 chart
    data = _.map(data, d => {
      return { label: d.key, value: d.value };
    });
    this.display(data);

		if(! this.dialog) {
			this.initJqueryDialog();
		}
  }

  display(data) {
    d3.select(this.taxonSvgEl)
      .datum(data)
      .transition().duration(this.DURATION_MS)
      .call(this._chart);
    this._chart.legend.updateState();
  }

  onClearSelection() {
    this.disabledTaxaNum = 0;
    this._dim.filter(null);
    this.update();
		this.api.cfUpdated = { sender: this };		
  }

};
