My observations of hash table performance are as follows:

Insertion operations on lists in method 1 outperformed AVL trees and vectors by as much as 50%. I thought this was interesting, as both vectors and lists have the ability to access both first and last elements, but lists outperformed vectors on deletion and searches as well. Between AVL trees and lists, their performance on searches and deletion were about the same with only a ~2000 microsecond discrepancy in search time, probably due to the fact that AVL trees have search running in log(n) time. 


In higher collision data sets, double hashing outperformed the other methods on insertion, likely due to its minimization of primary and secondary clustering. While double hashing has more overhead to run the second hashing function and is less cache efficient, it still outperformed linear and quadratic probing. 