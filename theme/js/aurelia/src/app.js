import {inject, TaskQueue} from 'aurelia-framework';
import {Api} from 'api';


@inject(Api, TaskQueue)
export class App {

  treeData = null;
  msaData = null;
	familyName = FAMILY_NAME; // is global var in php template

  constructor(api, tq) {
    this.api = api; // Api
    this.tq = tq;   // TaskQueue
  }

  attached() {
    this.tq.queueMicroTask( () => this.api.init() );
  }

}
