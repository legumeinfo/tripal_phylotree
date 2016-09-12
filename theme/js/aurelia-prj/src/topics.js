/* define message classes to be used for pub/sub event routing */


export class CrossfilterCreated {
    constructor(name, cf, sender) {
        this.name = name;
        this.crossfilter = cf;
        this.sender = sender;
    }
}
export class FilterUpdated {
  constructor(sender) {
    this.sender = sender;
  }
}
