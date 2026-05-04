This folder contains my work for assignment 5 part 2, using different structures in kruskal mst algortithms

My answers for task 8 are as follows:

Since kruskal relies on repeatedly finding the smallest edge, the structure we use to store them makes
a big difference. Compared to the standard priority queue, binary heaps aim to be more efficient by offering
O(1) access to the smallest weight edge. While binomial heaps can also offer O(1) access to the lowest weighted
edge, they have more efficient delete operations. 

Firstly, union-find with path compression means that any calls to find (which happens twice per every edge we look at)
will run more efficiently over time. This becomes especially apparent when the numbers of edges increases, 
as find() runs closer to O(1) on average. This is a big advantage over quickunion, which will have more glaring
shortcomings when there are lots of edges since it has to trace all the way back up to the node's parent each time.
Union by size is also more important as the number of nodes increases, and makes sure that the depth of trees
stays low, making each first find() run quicker before the path is compressed.

The differences in time complexity between algortithms show themselves more when considering large numbers of nodes and edges,
which results in a larger amount of find() and union() operations. While both heaps have similar amortized time complexity,
(o(1) for insert, log(n) for deletemin, o(1) for findmin) the optimations in union by size with path compression 
show a clear increase in efficiency that is especially evident with a large number of nodes. 