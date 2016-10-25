import {inject} from 'aurelia-framework';
import {HttpClient} from 'aurelia-fetch-client';
import {Api} from 'api';

let $ = jQuery;

@inject(Api, HttpClient)
export class App {

	// currently shown tools in the ui
	tools = {
		taxon : true, // taxon doubles as a legend, so show it by default.
		msa : false, // hide by default- for consistency with past versions.
		help : false // hide help dialog by default
	};
	
  treeData = null;
  msaData = null;
	familyName = FAMILY_NAME; // is global var in php template

  constructor(api, http) {
    this.api = api; // Api
		this.http = http; // fetch client
  }

  attached() {
		// remove the static ajax spinner that was hardcoded in the php
		// template the loader-anim element will display the same spinner
		// while needed.
		$('#ajax-spinner').remove();

		this.configureHttpClient();
		
		// fetch the resources for this gene family
		this.api.init();
  }

	configureHttpClient() {
		this.http.configure(config => {
			config
			  .withDefaults({
					credentials: 'same-origin',
					headers: {
						'Accept': 'application/json',
						'X-Requested-With': 'Fetch',
						'Cache-Control': 'no-cache',
						'Pragma' : 'no-cache',
						// internet explorer workaround to prevent caching
						'If-Modified-Since': 'Mon, 26 Jul 1997 05:00:00 GMT'
					}
				})
			  .withInterceptor({
					// request(request) {
					//     console.log(`Requesting ${request.method} ${request.url}`);
					//     return request; // you can return a modified Request, or you can short-circuit the request by returning a Response
					// },
					response(response) {
						// check for failed http status codes here. For explanation, see
						// https://www.tjvantoll.com/2015/09/13/fetch-and-errors/
						if(! response.ok) {
							let msg =  `HTTP ${response.status}: ${response.statusText} [ ${response.url} ]`;
							console.error(msg);
							alert(msg);
							throw response; // cause reject
						}
						return response;
					},
					requestError(request) {
						// TypeError is, strangely, what the fetch api raises when
						// the browser is actually saying connection refused
						if(response instanceof TypeError) {
							// display something understandable
							let msg = `A network error was encountered: browser offline, DNS lookup failure, or connection refused: [${response}]`;
							console.error(msg);
							alert(msg);
						}
						else {
							let msg = 'Fetch request error: ' + request;
							console.error(msg);
							alert(msg);
						}
						throw request;
					},
					responseError(response) {
						// TypeError is, strangely, what the fetch api raises when
						// the browser is actually saying connection refused
						if(response instanceof TypeError) {
							// display something understandable
							let msg =  `A network error was encountered: browser offline, DNS lookup failure, or connection refused: [${response}]`;
							console.error(msg);
							alert(msg);
						}
						else {
							let msg = 'Fetch response error: ' + response;
							console.error(msg);
							alert(msg);
						}
						throw response;
					}
				});
		});
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
