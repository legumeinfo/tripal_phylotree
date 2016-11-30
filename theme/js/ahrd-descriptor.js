/* add some interactivity and explanations to the AHRD code in the
phylotree.comment */

'use strict';

jQuery(document).ready( function($) {
	var commentEl = $('#phylotree-comment');
	var comment = commentEl.html();
	var code = comment.match(/\s?[\*\-]{3,4}/);
	if(code) {
		var link = ', <a id="ahrd-help" href="#">'+
				'AHRD score for consensus seq <strong>(' + code + ')</strong>'+
				' <i class="fa fa-question-circle" aria-hidden="true"></i></a>' +
				', description from: ';
		commentEl.html(comment.replace(code, link));
		var linkEl = $('#ahrd-help');
		linkEl.click(function() {
			$('#ahrd-dialog').dialog({
				title: 'AHRD code: ' + code
			});
		})
	}
});
