/* show the bootstrap-tour, if user has not seen it already. */

'use strict';

(function() {
  var j = localStorage.getItem('lisTourVisited');
  if(!j || ! JSON.parse(j).phylotree) {
    // user has not seen phylotree tour; check for conflict with multi-page tours.
    if(! lisTours.active()) {
      lisTours.go('phylotree');
    }
  }
})();
