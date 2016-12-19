/* add linkout to intepro IPRxxxxxx codes in the description. */

'use strict';

jQuery(document).ready( function($) {
	var commentEl = $('#phylotree-comment');
	var comment = commentEl.html();
	var matches = comment.match(/IPR\d{6,6}/gi);
	matches.forEach(function(code) {
		var link = '<a id="interpro-help" href="'+
					'http://www.ebi.ac.uk/interpro/entry/'+ code + '">'+code + '</a>';
		comment = comment.replace(code, link);
	});
	commentEl.html(comment);
});
