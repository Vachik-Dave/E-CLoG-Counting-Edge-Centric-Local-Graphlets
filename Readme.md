# E-CLoG: Counting Edge-Centric Local Graphlets.

### Authors: [Vachik S. Dave](https://www.linkedin.com/in/vachik-dave), Nesreen Ahmed and Mohammad Al Hasan.

This code provides frequency of local graphlets upto size 5 for the given graph. 

Vachik S. Dave, Nesreen Ahmed and Mohammad Al Hasan. “[E-CLoG: Counting Edge-Centric Local Graphlets](https://ieeexplore.ieee.org/document/8257974)”, IEEE International Conference on Big Data (IEEE BigData), Dec-2017.
 

### Compile: makefile is provided for compilation of the code. 
$ make

### Requirements:
It needs c++ compiler and openMP support for the c++.

### Run: make command creates output file "E_CLoG_omp".
$ ./E_CLoG_omp -i *input_graph_filename* \[-edgefile <*edgelist_filename*>\] \[-EdgeAtt\] \[-t #threads\] \[-local or -unique or -all\]

### Options:
* -i : With this option next argument should be *input_graph_filename*.
  - Format: First row specify "#nodes #edges". 
   - From the 2nd row, each row specify an edge in space delimited format: "node_id1 node_id2".
   - Node_id need to be integer and starting node_id is 0.

* -edgefile : With this option next argument should be *edgelist_filename*. E-CLoG will return graphlet counts for this set of edges.
  - Format: First row of the file specify #edges (number of edges in the file).
   - From the 2nd row, each row specify an edge in space delimited format: "node_id1 node_id2".
   - Node_id need to be integer and starting node_id is 0.
   - Note: Order of the edges is not maintian in output file.



* -EdgeAtt : This option specify the input format of the *input_graph_filename*. If the *input_graph_file* has 3rd column specifying some edge attribute, with this flag it can read the file, however during graphlet counting our method ignores the value of the attribute.
  - Format: First row specify "#nodes #edges #attribVals". 
   - From the 2nd row, each row specify an edge in space delimited format: "node_id1 node_id2 attrib_val".
   - Node_id need to be integer and starting node_id is 0.

* -t : with '-t' flag provide number of threads (#threads (int)), program will try to get #threads based on availability on the host machine. 

* -local : this option provide counts of all local graphlets. (Not Local: g3,g7,g12,g19) [Default]
* -unique : this option provide counts of all graphlts structures (ignoring the orbit) only once to calculate global graphlet count.
* -all: this option provide counts of all graphlts shown in the figures.


### Output: 
- The E-CLoG code will create an output file with same named (same directory) as *input_graph_filename* OR *edgelist_filename* with
post-fix "_local_graphlet_frequency_5_omp.txt".
- For "-all" OR "-unique" options the "local" is replaced by the option.

### The output format:
node1 node2: f0 f1 f2 ... f45

