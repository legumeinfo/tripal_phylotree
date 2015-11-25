/* show the hopscotch tour, if user has not seen it already. */

(function($) {

  if(hopscotch.getState()) {
    // user has already seen tour, so don't automatically reveal it.
    return;
  }

  // wait for the dom to be ready
  $(document).ready(waitForPhylogram);
  
  function waitForPhylogram() {
    // dont start the tour until the d3 tree is finished rendering
    // because the tour needs to attach to the dom.
    if(! $('#phylogram svg').length || ! $('.leaf circle').length) {
      setTimeout(waitForPhylogram, 100);
      return;
    }
    showHopscotchTour();
  }
})(jQuery);

function forceTripalNavigation() {
  /* tripal doesn't expose navigation state of it's sub-panes, so use
   * jquery to ensure that the phylogram (base pane) is visible */
  (function($) {
    if($('#base-tripal-data-pane').is(':visible')) {
      return;
    }
    $('.tripal-data-pane').hide();
    $('#base-tripal-data-pane').show();
    window.location.hash = 'pane=base';
  })(jQuery);
}

function showHopscotchTour() {
  
  forceTripalNavigation();
  
  var tour = {
    id: 'lis-phylogeny-tour',
    steps: [
      {
      	title: 'Welcome',
      	content: 'This quick tour will acquaint you with the phylogeny tree \
               viewer and other resources available in this section.',
      	target: 'base',
      	placement: 'bottom',
      },
      {
      	title: 'Gene family name',
      	content: 'The Gene family name and description are shown in the \
         header. Family descriptions are derived from homology-based  \
         functional analysis of the hidden markov model representing the \
         known sequences in the family, and include Interpro and Gene \
         Ontology identifiers.',
      	target: document.querySelector('.tripal-data-block-desc'),
      	placement: 'bottom',
      },
      {
      	title: 'Phylogram',
	content:  'The phylogram view displays a phylogenetic tree with \
                  branch spans proportional to the amount of character \
                  change, including both legumes and selected non-legumes.',
      	target: document.querySelector('svg'),
      	placement: 'top',
      },
      {
        title: 'Terminal Nodes',
        content: 'The tree nodes on the right are terminal nodes\
                  Click on a (colored) node to \
                  see more information about legume genes including links to \
                  organism, gene, genomic context, and various \
                  resources and viewers. For example, the genomic context \
                  viewer shows flanking genes and allows exploration of \
                  syntenic regions from all included legume genomes.',
        target: document.querySelector('.legume-leaf-node'),
        placement: 'top',
      },
      {
        title: 'Interior Nodes',
        content: 'The other tree nodes are \'interior\' nodes. Click on \
               a (white) interior node to view genomic context and genomic \
                  distribution for the node\'s sub-tree.',
        target: document.querySelector('.inner circle'),
        placement: 'top',
      },
      {
        title: 'Root Node',
        content: 'The root node is also an interior node, although it \
                  is colored differently for reference. (Note: this root is \
                  only one of several possible root choices, and may not be \
                  the oldest common ancestor. It is the result of midpoint \
                  rooting of the tree.)',
        target: document.querySelector('.root circle'),
        placement: 'top',
      },
      {
        title: 'MSA',
        content: 'The multiple sequence aligment for the family \
                  is available via this link.',
        target: 'msa-link',
        placement: 'top',
      },
      {
        title: 'Circular Dendrogram',
        content: 'You can view an alternate representation of the same \
         phylogram data via this link. This view will not allow you \
         to see the fine details, but does give a concise overview \
         of the topology of the tree and how gene content from different \
         species is distributed throughout.',
        target: 'phylotree_circ_dendrogram',
        placement: 'bottom',
      },
      {
        title: 'Organisms',
        content: 'You can display the membership counts per species in a \
          diagram illustrating their relative abundances (helpful for \
          very large families)',
        target: 'phylotree_organisms',
        placement: 'bottom',
      },
      {
        title: 'Cross References',
        content: 'You can access the Phytozome family from which this tree \
                   was derived via this link.',
        target: 'phylotree_references',
        placement: 'bottom',
      },
      {
        title: 'Analysis',
        content: 'You can access the Analysis description and metadata  \
                  via this link.',
        target: 'phylotree_analysis',
        placement: 'bottom',
      },
      {
      	title: 'Gene Family Search',
      	content: 'You can search for other gene families via the Search menu.',
      	target: document.querySelector('ul li a[title="Search"]'),
      	placement: 'bottom',
      },
      {
        title: 'Getting Help',
        content: 'You can restart this tour anytime from the Gene Family Help \
          button. In addition there is a more detailed Help page: \
          <a href="/search/phylotree/userinfo" target="_blank" \
            style="text-decoration: underline"> \
            View more help on searching gene families...</a>',
        target: document.querySelector('.phylogeny-help'),
        placement: 'top',
      },
    ],
    showPrevButton: true,
  };
  hopscotch.startTour(tour, 0);
}
