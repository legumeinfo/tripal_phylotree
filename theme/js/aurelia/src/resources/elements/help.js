import {bindable} from 'aurelia-framework';

let $ = jQuery;

export class Help {

	@bindable showDialog;

	
	showDialogChanged(newValue, oldValue) {
		this.showDialog = newValue;
		// expect initial value is false (not shown)
		if(newValue || oldValue !== null) {
			this.update();
		}
	}
	
	update() {
		if(! this.dialog) {
			this.dialog = $("#help-dialog");			
		}
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

