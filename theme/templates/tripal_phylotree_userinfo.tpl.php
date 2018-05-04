<!-- This is a test page under development -->

<h1>HOW TO: Gene Family Search</h1>
(<a href="/search/phylotree">Go to</a> gene family search)
<div>
<p>
This page lets you search for gene families starting from a legume gene family ID (e.g., legfed_v1_0.L_2951WH) or words from the description of the gene family (e.g., iron homeostasis or chlorophyll binding protein); family descriptions are derived from homology-based functional analysis of the hidden markov model representing the known sequences in the family, and include Interpro and Gene Ontology identifiers which may be included in the search. You may leave any of the search fields blank if you don't care about the field, or specify several criteria together (all filters will be ANDed together). The gene families were calculated in 2018, as part of the LegumeFederation project. The full family set is <a href="/data/public/Gene_families/">available here.</a>
</p>



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
    The root node click also gives an option to view the Multiple Sequence Alignment for this gene family.
  </li>
</ul>


<h2>The Analysis</h2>
<p>
The gene families at LegumeInfo were calculated as part of the <a href="https://legumefederation.org">NSF LegumeFederation</a> project, as follows (to be described in more detail in a paper anticipated for late 2018). Clustering was done on the basis of gene pairs filtered for per-species Ks values. These were clustered using Markov clustering. Sequence match scores of each sequence in a family were used to identify outliers, on the basis of score value relative to the median score for the family. Remaining sequences were re-clustered, added to the HMM set. Then all sequences were searched against all HMMs, realigned, re-screened relative to median match score, and finally used to generate alignments and phylogenetic trees (using RAxML). The trees are rooted, when possible, using the closest outgroup from among five outgroup species: Arabidopsis thaliana, Prunus persica, Cucumis sativa, Solanum lycopersicum, and Vitis vinifera.
</p>

<p>
Note: These current gene families at LegumeInfo replace (as of May, 2018) an older set families that were built on the Phytozome 10.2 Angiosperm-level gene family models. The Phytozome-based families are still accessible by direct URL (for researchers who may have referenced particular families). The URL form for such families has the form: /chado_phylotree/phytozome_10_2.59192412
</p>




</div>

<!--SCRATCH PAD-->
<!--
Other items that should appear in the help contents:

Ð 2 different simple/short use cases for illustrating benefit of this tool.

-->


