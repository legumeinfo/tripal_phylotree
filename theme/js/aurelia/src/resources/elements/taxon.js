import {inject, bindable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';
import {App} from 'app';

let $ = jQuery;

@inject(App, Api, Symbology, BindingEngine)
export class Taxon {
	
  DURATION_MS = 500;
	DIALOG_WIDTH = '520px';
	
	@bindable familyName = null;
	@bindable showDialog; // two-way databinding for toggling dialog in app.js
	
  _chart = null;
  _cf = null;
  _dim = null; // crossfilter dimension
  _grp = null; // crossfilter group

  disabledTaxaNum = 0;

  constructor(app, api, sym, be) {
		this.app = app;       // app.js
    this.api = api;       // web api
		this.symbology = sym; // symbology
    this.be = be;         // binding engine
  }

  attached() {
		this.dialog = $(this.taxonEl);
		this.initNvD3Graph();
    this.subscribe();
  }

	initNvD3Graph() {
    nv.addGraph( () => {
      let chart = this._chart = nv.models.pieChart()
					.x( d => d.label )
					.y( d => d.value )
					.showLabels(true)
					.labelThreshold(.05)
					.labelType('percent')
					.donut(true)
					.donutRatio(0.35)
					.color( d => this.symbology.color(d.label) );
      chart.options({ legendPosition : 'right' });
      chart.margin({ left: 0, right: 0, top: 0, bottom: 0 });
      chart.dispatch.on('stateChange', evt => {
        setTimeout(() => this.onTaxonStateChange(evt), this.DURATION_MS);
      });
      return chart;
		});
	}

	showDialogChanged(newValue, oldValue) {
		this.showDialog = newValue;
		// ignore changed events if attached() has not run yet (based on
		// existence of dialog jquery object)
		if(this.dialog) {
			this.updateDialog();
		}
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
    this.updateTaxaChart();
		this.updateDialog();
	}

	onCfUpdated(msg) {
    if(msg.sender != this) {
			if(this.showDialog) {
				this.updateTaxaChart();
			}
			else {
				this._chart.dirty = true;
			}
		}
	}

  setupCrossfilter() {
    // create a dimension for the taxon
    this._dim = this._cf.dimension(
      d => d.species ? d.genus + ' ' + d.species : ''
    );
    this._grp = this._dim.group();
  }

	updateDialog() {
		if(this.showDialog && this._chart.dirty) {
			// lazy update the taxa component
			this.updateTaxaChart();
		}
		let that = this;
		let opts = {
      title: 'Taxa - ' + this.familyName,
      closeOnEscape: true,
      modal: false,
      width: this.DIALOG_WIDTH,
			position: {
				my: 'right', at: 'bottom'
			},
			close: (event, ui) => this.closed() 
    };
		this.dialog.dialog(opts);
		let action = this.showDialog ? 'open' : 'close';
		this.dialog.dialog(action);
	}

	closed() {
		this.onClearSelection();
		this.showDialog = false;
	}
	
  updateTaxaChart() {
    // get the current results from our group
    let data = this._grp.all();
    // filter out taxa having 0 value (count)
    data = _.filter(data, d => d.value > 0 );
    // transform to objects expected by nvd3 chart
    data = _.map(data, d => {
      return { label: d.key, value: d.value };
    });
    this.display(data);
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
    this.updateTaxaChart();
		this.api.cfUpdated = { sender: this };		
  }

};
