// polyfill support for older browsers and IE especially.
// aurelia natively supports only evergreen browsers (chrome, firefox, safari)
// http://aurelia.io/hub.html#/doc/article/aurelia/framework/latest/app-configuration-and-startup/3

// Promises polyfill (bluebird) is prepended via aurelia.json

// Fetch polyfill
import 'fetch';

// requestAnimationFrame polyfill
import * as raf from 'raf';
raf.polyfill();

// MutationObserver polyfill
import './MutationObserver';

