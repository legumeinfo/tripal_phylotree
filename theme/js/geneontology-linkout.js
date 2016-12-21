/* add linkout to intepro IPRxxxxxx codes in the description. */

'use strict';

jQuery(document).ready( function($) {
	var commentEl = $('#phylotree-comment');
	var comment = commentEl.html();
	var matches = comment.match(/GO:\d{7,7}/gi);
	matches.forEach(function(code) {
		var link = '<a href="http://amigo.geneontology.org/amigo/term/'+ code +
				        '">'+code + '</a>';
		comment = comment.replace(code, link);
	});
	commentEl.html(comment);
});
