import {inject} from 'aurelia-framework';
import {Api} from 'api';

let $ = jQuery;

@inject(Api)
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

  constructor(api) {
    this.api = api; // Api
  }

  attached() {
		// remove the static ajax spinner that was hardcoded in the php
		// template the loader-anim element will display the same spinner
		// while needed.
		$('#ajax-spinner').remove();
		
		// fetch the resources for this gene family
		this.api.init();
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
