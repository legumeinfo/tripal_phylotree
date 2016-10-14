import {bindable} from 'aurelia-framework';

let $ = jQuery;

export class Help {

	@bindable showDialog;
	
	attached() {
		this.dialog = $("#help-dialog");
	}
	
	showDialogChanged(newValue, oldValue) {
		// expect initial value is false (not shown)
		if(newValue || oldValue !== null) {
			this.toggle();
		}
	}
	
	// toggle visibility of help dialog
	toggle() {
		let opts = {
			title: 'Gene Family Help',
			closeOnEscape: true,
			modal: false,
			close: (event, ui) => this.closed()
		};
		this.dialog.dialog(opts);
		let action = this.showDialog ? 'open' : 'close';
		this.dialog.dialog(action);
	}

	closed() {
		this.showDialog = false;
	}

	onTour() {
		this.showDialog = false;
		lisTours.go('phylotree');
	}
}

