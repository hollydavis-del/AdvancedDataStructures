This folder contains my code for the advanced data structures final project. p_bst_3.cpp contains implementation
for a persistent bst with the fat node method. 

My answer for task 4:

For BSTs, path copying is particular interesting as it merely does a bit of extra work while
we traverse a path, which we'd have to do anyways for any insert/delete performed on the tree. 
Since insert/delete run in log(n) time already, we're doing a bit more work that keeps the time
complexity in line with what we'd expect for BSTs. It also has high structural sharing, since it relies on 
keeping pointers to parts of the tree outside of the path.
However, with fat nodes the structural sharing is pretty much total since nodes have multiple values
within what is technically the same tree. The delete operation is still expensive for both, since it relies
changing multiple pointers and moving nodes in either persistence strategy. With fat nodes, 
it's more tricky to not waste space on storing values within nodes, but each value we store will 
have smaller space than a pointer to a node (like we store with path copying). 