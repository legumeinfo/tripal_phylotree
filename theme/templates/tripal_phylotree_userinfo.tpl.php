<!-- This is a test page under development -->

<h1>HOW TO: Gene Family Search</h1>
(<a href="/search/phylotree">Go to</a> gene family search)
<div style="font-family:times">
<p>This page lets you search for gene families starting from a Phytozome gene family ID (e.g., phytozome_10_2.59192412) or words from the description of the gene family (e.g., iron homeostasis or chlorophyll binding protein); family descriptions are derived from homology-based functional analysis of the hidden markov model representing the known sequences in the family, and include Interpro and Gene Ontology identifiers which may be included in the search.  You may leave any of the search fields blank if you don't care about the field, or specify several criteria together (all filters will be ANDed together). The gene families are based on <a href="http://phytozome.jgi.doe.gov/pz/portal.html" target="_blank">Phytozome version 10.2</a>.  You can further refine or restrict the search results by family size or species composition by choosing the count fields under different species (e.g. the maximum number of members in a particular species).</p>

<h2>The result table</h2>
<p>The resulting table shows the Family ID and description along with the membership counts in different species. The table can be sorted by clicking on the column headers (clicking a header again will reverse the order of the sort).
<ul>
  <li>
    The <u>Family ID links to a page displaying a phylogram</u> of all its members (a phylogenetic tree that has branch spans proportional to the amount of character change), including both legumes and non-legumes, and helps reveal a probable evolutionary history leading to the current set of genes in the family (e.g. orthology and paralogy relationships).
  </li>
  <li>
    The <u>counts under a given species are linked to the 'Genes page'</u> and lists the gene IDs (gene models) of the species that are members of this gene family. There is an opportunity to build a gene list or basket of gene IDs here for further analysis (in future).
  </li>
  <li>
    The <u>total counts</u> are also linked to the 'Genes page' listing only the legume genes that are currently in the LIS database (note that the number displayed for the total count includes the genes for all species included in the tree, and will typically be somewhat larger than the count of the legume genes retrieved in this way).
  </li>
</ul>

<h2>The phylogram</h2>
The <u>phylogram</u> page displays a phylogenetic tree that has branch spans proportional to the amount of character change, including both legumes and selected non-legumes. Alternatively, you can also display the tree in the form of a <u>circular dendrogram</u> (when a large number of genes are in the tree, this view will not allow you to see the fine details, but does give a concise overview of the topology of the tree and how gene content from different species is distributed throughout). Click the <u>Organism</u> link to display the membership counts per species in a diagram illustrating their relative abundances (helpful for very large families). You can <u>access the Phytozome family through the "Cross Reference" tab</u>. 

<ul>
  <li>
    Click on colored terminal nodes to get more information about legume genes at LIS including its sequence, genome browser view, links to information at external sites, etc. Non-legume genes have minimal information stored in LIS and so only provide links to external sites.
  </li>
  <li>
    Click on internal (white) nodes to view the "genomic context" for the legume genes included in the subtree for that node. The genomic context viewer shows flanking genes colored by gene family assignment and allows exploration of syntenic regions from all included legume genomes. The genomic context view works only if there are legume genes in the subtree.
  </li>
  <li>
    The root node click also gives an option to view the Multiple Sequence Alignment** for this gene family.
  </li>
</ul>
**The MSA viewer (Jalview) requires Java WebStart support.
<h2>The Analysis</h2>
The gene families at LegumeInfo were built on the Phytozome 10.2 Angiosperm-level gene family models. Sequences from each species were placed in families based on best Hidden Markov Model match (using hmmsearch from the Hmmer package, v 3.1b2), with a minimum E-value match threshold of 0.1. Sequences in each family were realigned to the family's HMM using hmmalign, and then trimmed to include only match-state characters. Trees were generated using FastTree, and descriptors for the families were created using AHRD (Automatic assignment of human readable descriptions) on the consensus representation of the family generated with hmmemit.

</div>

<!--SCRATCH PAD-->
<!--
Other items that should appear in the help contents:

Ð 2 different simple/short use cases for illustrating benefit of this tool.

-->


