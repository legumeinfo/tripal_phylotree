import {inject, bindable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {Symbology} from 'symbology';
import {App} from 'app';

let $ = jQuery;

@inject(App, Api, Symbology, BindingEngine)
export class Taxon {
	
  ANIM_MS = 50; // nvd3 defaults to 250ms
	DOUBLECLICK_MS = 500;
	DIALOG_WIDTH = '520px';
	
	@bindable familyName = null;
	@bindable showDialog; // two-way databinding for toggling dialog in app.js
	
  _chart = null;
  _cf = null;
  _dim = null; // crossfilter dimension
  _grp = null; // crossfilter group

  chartDirty = false; // a flag for whether the pie chart can be reset.

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
		let that = this;
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
      chart.options({
				legendPosition : 'right',
				duration: this.ANIM_MS
			});
      chart.margin({ left: 0, right: 0, top: 0, bottom: 0 });
      chart.dispatch.on('stateChange', evt => {
				// prevent double-click from stacking up state change events
				if(that.timeoutId) {
					clearTimeout(that.timeoutId);
					that.timeoutId = null;
				}
				// dont publish state change until the pie chart's transition
				// is finished.
        that.timeoutId = setTimeout(
					() => this.onTaxonStateChange(evt), this.ANIM_MS
				);
      });
      return chart;
		});
	}
	
  subscribe() {
		this.be.propertyObserver(this.api, 'cf')
		 	.subscribe( o => this.onCfCreated(o));
		this.be.propertyObserver(this.api, 'cfUpdated')
		 	.subscribe( o => this.onCfUpdated(o));
  }

  setupCrossfilter() {
    // create a dimension for the taxon
    this._dim = this._cf.dimension(
      d => d.species ? d.genus + ' ' + d.species : ''
    );
    this._grp = this._dim.group();
  }
	
  onTaxonStateChange(event) {
		// the state object from the nvd3 diagram is an array of booleans
		// (having true for disabled) which may not match the full list of
		// taxa names.
		this.chartDirty = false;
		let disabledTaxa = {};
		_.each(event.disabled, (b, i) => {
			let chartItem = this._data[i];
			let taxon = chartItem.label;
			disabledTaxa[taxon] = b;
			if(b) {
				// have at least one disabled item, so chart is reset-able.
				this.chartDirty = true;
			}
		});
    this._dim.filter(null); // clear filter, they are additive per dimension.
    this._dim.filter( d => {
			let test = disabledTaxa[d];
			if(test === undefined || test === false) {
				return true;
			}
			return false;
		});
		this.api.cfUpdated = { sender: this };
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
		}
	}

	updateDialog() {
		if(this.showDialog) {
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
				my: 'left', at: 'bottom'
			},
			close: (event, ui) => this.closed() 
    };
		if(this.dialogWasOpened) {
			delete opts.position; // allow dialog to re-open in previous location
		}
		this.dialog.dialog(opts);
		let action = this.showDialog ? 'open' : 'close';
		this.dialog.dialog(action);
		this.dialogWasOpened = true;
	}
	
	showDialogChanged(newValue, oldValue) {
		this.showDialog = newValue;
		// ignore changed events if attached() has not run yet (based on
		// existence of dialog jquery object)
		if(this.dialog) {
			this.updateDialog();
		}
	}

	closed() {
		this.onResetChart();
		this.showDialog = false;
	}
	
  updateTaxaChart() {
    // get the current results from our group
    let data = this._grp.all();
    // filter out taxa having 0 value (count)
    data = _.filter(data, d => d.value > 0 );
    // transform to objects expected by nvd3 chart
    this._data = _.map(data, d => {
      return { label: d.key, value: d.value };
    });
    d3.select(this.taxonSvgEl)
      .datum(this._data)
			.transition().duration(this.ANIM_MS)
      .call(this._chart);
    this._chart.legend.updateState();
		this.chartDirty = false;
  }

  onResetChart() {
    this._dim.filter(null);
    this.updateTaxaChart();
		this.api.cfUpdated = { sender: this };		
  }

};
