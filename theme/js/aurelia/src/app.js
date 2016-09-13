import {inject} from 'aurelia-framework';
import {EventAggregator} from 'aurelia-event-aggregator';


import {Api} from 'api';
import {FilterUpdated} from 'topics';


@inject(Api, EventAggregator)
export class App {

  treeData = null;
  msaData = null;

  constructor(api, ea) {
    this.api = api;
    this.ea = ea;
  }

  attached() {
    setTimeout(() => this.api.init());
  }

}
