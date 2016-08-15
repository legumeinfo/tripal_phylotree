/* show the bootstrap-tour, if user has not seen it already. */

'use strict';

(function() {
  var j = localStorage.getItem('lisTourVisited');
  if(!j || ! JSON.parse(j).phylotree) {
    lisTours.go('phylotree');
  }
  
})();
