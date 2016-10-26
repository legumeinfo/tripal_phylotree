import {inject, bindable, observable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';
import {App} from 'app';

let $ = jQuery;


@inject(App, Api, BindingEngine)
export class Msa {

  MAX_HEIGHT = 175;
	MSA_ZOOMER_WIDTH = 475;
	DIALOG_WIDTH = 650;
	
	@bindable familyName;
	@bindable showDialog; // two-way databinding for toggling dialog in app.js
  @observable selectedFeatureNames = {};
  @observable selectedFeatureNum = 0; // count of selected features
	
  menu = false;
  msa = null; // msa viewer component
	msaDirty = false; // flag for whether the msa component should be reloaded
	dialog = null;

  seqs = [];
  index = {}
  _dim = null; // crossfilter dimension

  constructor(app, api, be) {
		this.app = app; // app.js singleton
    this.api = api; // web api
    this.be = be;   // binding engine
  }

  attached() {
		// the dom is ready, so use jquery to get reference to dialog div.
		this.dialog = $("#msa-dialog");
    this.subscribe();
  }

	showDialogChanged(newValue, oldValue) {
		this.showDialog = newValue;
		if(this.showDialog && ! this.msa) {
			// lazily create the msa component
			this.init();
			this.updateMsa();
		}
		if(this.dialog) {
			// hide or show the dialog as necessary
			this.updateDialog();			
		}
	}

	// initialize the msa component
  init() {
    this.seqs = this.api.msaSeqs;
    let opts = {
      el: this.msaElement,
      bootstrapMenu: true,
      vis: {
        overviewbox: false,
        labelId: false,
      },
      zoomer: {
        labelNameLength: 150,
        labelFontsize: 9,
				alignmentWidth: this.MSA_ZOOMER_WIDTH
      }
    };
    this.msa = msa(opts);
    let callbacks = {
      reset: this.onMsaSelectionReset,
      add: this.onMsaSelectionAdd,
			//  all: function (a, b, c) {}
    };
    this.msa.g.selcol.on(callbacks, this);

    // workaround for biojs-msa menu divs possibly being effected by
    // leaking CSS rules in this context
    $('.smenubar_alink').live('click', evt => {
      setTimeout( () => {
				let el = $('.smenu-dropdown');
				el.css('top', '');
				el.css('left', '');
			});
		});
	}

	updateDialog() {
		if(this.showDialog && this.msaDirty) {
			// lazy update the msa component
			this.updateMsa();
		}
		let opts = {
			title: 'Multiple Sequence Alignment - ' + this.familyName,
			closeOnEscape: true,
			modal: false,
			width: this.DIALOG_WIDTH + 'px',
			position: {
				my: 'right', at: 'bottom'
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

	// callback for close dialog event
	closed() {
		this.onClearSelection();
		this.showDialog = false;
	}

	// setup observers for crossfilter data
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
	}
	
	onCfUpdated(msg) {
		if(msg.sender != this) {
			if(this.showDialog) {
				this.updateMsa();
			}
			else {
				this.msaDirty = true;
			}
		}
	}
	
  updateMsa() {
    let data = this._dim.top(Infinity);
    let seqs = _.map(data, d => d.msa);
    seqs = _.sortBy(seqs, d => d.name);
    seqs.unshift(this.api.getConsensusSeq());
    this.display(seqs);
		this.msaDirty = false;
  }

  display(seqs) {
    this.msa.seqs.reset(seqs);
    this.msa.render();
    let x = seqs.length * 15;
    let height = (x > this.MAX_HEIGHT) ? this.MAX_HEIGHT : x;
    this.msa.g.zoomer.set('alignmentHeight', height);
    if(this.menu) {
      $('div.smenubar').show();// msa component does not clean up  menu bar
    }
    else {
        $('div.smenubar').hide();
    }
  }

  onToggleMenu() {
    this.menu = ! this.menu;
    if(this.menu) {
      $('div.smenubar').show();// msa component does not clean up  menu bar
    }
    else {
      $('div.smenubar').hide();
    }
  }

  // onMsaSelectionAdd() : add means there is more than 1 feature selected
  onMsaSelectionAdd(newModel, prevModel) {
    // must translate from the msa's "seqId" which is an integer,
    // into the actual fasta subject
    let seqIds = _.map(newModel.collection.models, d => d.attributes.seqId);
    let res = this.msa.seqs.filter(el => seqIds.indexOf(el.id) !== -1);
    let names = _.map(res, d => d.attributes.name);
    this.selectedFeatureNames = {};
    _.each(names, n => this.selectedFeatureNames[n] = true);
    this.selectedFeatureNum  = _.size(this.selectedFeatureNames);
  }

  // onMsaSelectionReset() : reset means there is a single feature selected
  onMsaSelectionReset(newModel, prevModel) {
    if(newModel.models.length) {
      let seqId = newModel.models[0].attributes.seqId;
      let res = this.msa.seqs.filter(el => el.id === seqId);
      if(res.length) {
        let n = res[0].attributes.name;
        this.selectedFeatureNames = {};
        this.selectedFeatureNames[n] = true;
      }
    }
    this.selectedFeatureNum  = _.size(this.selectedFeatureNames);
  }

  onClearSelection() {
    this.msa.g.selcol.reset([]);    
    this.selectedFeatureNum = 0;
    this.selectedFeatureNames = {};
  }

}
