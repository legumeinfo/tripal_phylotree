import {inject, TaskQueue} from 'aurelia-framework';
import {Api} from 'api';
import {FilterUpdated} from 'topics';


@inject(Api, TaskQueue)
export class App {

  treeData = null;
  msaData = null;

  constructor(api, tq) {
    this.api = api; // Api
    this.tq = tq;   // TaskQueue
  }

  attached() {
    this.tq.queueMicroTask( () => this.api.init() );
  }

}
