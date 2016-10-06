import {inject, bindable, BindingEngine} from 'aurelia-framework';
import {Api} from 'api';

let $ = jQuery;


// TODO: dont filter on msa selection, instead hilight the feature in
// the tnt.tree.

@inject(Api, BindingEngine)
export class Msa {

  MAX_HEIGHT = 175;
	MSA_ZOOMER_WIDTH = 475;
	DIALOG_WIDTH = 650;
	
	@bindable familyName;
  selectedFeatureNames = {};
  selectedFeatureNum = 0;

  menu = false;
  msa = null; // msa viewer component

  seqs = [];
  index = {}
  _dim = null; // crossfilter dimension

  constructor(api, be) {
    this.api = api; // web api
    this.be = be;   // binding engine
  }

  attached() {
    this.subscribe();
  }
	
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

	initJqueryDialog() {
		this.dialog = $(this.msaEl);
		this.dialog.dialog({
			title: 'Multiple Sequence Alignment - ' + this.familyName,
			closeOnEscape: true,
			modal: false,
			width: this.DIALOG_WIDTH + 'px',
			position: {
				my: 'left', at: 'bottom'
			}
		});
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
		this.update();
	}
	
	onCfUpdated(msg) {
		if(msg.sender != this) {
			this.update();
		}
	}
	
  update() {
    let data = this._dim.top(Infinity);
    let seqs = _.map(data, d => d.msa);
    seqs = _.sortBy(seqs, d => d.name);
    seqs.unshift(this.api.getConsensusSeq());
    this.display(seqs);
		if(! this.dialog) {
			this.initJqueryDialog();
		}
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
    this.updateFilter();
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
    //this.updateFilter();
  }

  onClearSelection() {
      // this.msa.g.selcol.reset([]);    
      // this.selectedFeatureNum = 0;
      // this.selectedFeatureNames = {};
      // this._dim.filter(null);
  }

  // updateFilter() :  update crossfilter with the selected features.
  updateFilter() {
		// filters are additive per dimension, so clear previous.
    this._dim.filterAll();
    if(_.size(this.selectedFeatureNames)) {
      this._dim.filter(d => this.selectedFeatureNames[d]);
    }
  }

}
