import {taxonChroma} from 'taxon-chroma';


export class Symbology {

	// we have 2 species of arachis which are not distinguishable in the
	// phylotree context, so force one to be a different shade, but same
	// hue.
	OPTS = {
		overrides : {
			'arachis ipaensis' : 'rgb(170, 171, 0)'
		}
	}
	
	color(taxon) {
		return taxonChroma.get(taxon, this.OPTS);
	}
	
}
