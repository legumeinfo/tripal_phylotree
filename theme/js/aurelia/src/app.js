import {inject, TaskQueue} from 'aurelia-framework';
import {Api} from 'api';

let $ = jQuery;

@inject(Api, TaskQueue)
export class App {

	// currently shown tools in the ui
	tools = {
		msa : true,
		taxon : true,
		help : false
	};
	
  treeData = null;
  msaData = null;
	familyName = FAMILY_NAME; // is global var in php template

  constructor(api, tq) {
    this.api = api; // Api
    this.tq = tq;   // TaskQueue
  }

  attached() {
    this.tq.queueMicroTask( () => this.api.init() );
		// remove the ajax spinner that was hardcoded in the php template
		$('#ajax-spinner').remove();
  }

	toggleHelp() {
		this.tools.help = ! this.tools.help;
	}

	toggleTaxon() {
		this.tools.taxon = ! this.tools.taxon;
		let dialog = $("#taxon-dialog");
		let action = this.tools.taxon ? 'open' : 'close';
		dialog.dialog(action);
	}

	toggleMSA() {
		this.tools.msa = ! this.tools.msa;
		let dialog = $("#msa-dialog");
		let action = this.tools.msa ? 'open' : 'close';
		dialog.dialog(action);
	}
}
