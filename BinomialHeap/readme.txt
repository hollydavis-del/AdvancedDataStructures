
Answers for questions are as follows

- How do binomial heaps improve the efficiency of merge operations compared to binary heaps?

    Merging binomial heaps only merges two trees when they have the same degree. With normal binary heaps you have
    to merge each node, but with binomial heaps there are only at most log n trees to combine. 

- Explain the time complexity of various operations in binomial heaps.

    As mentioned above, merge operations on binomial heaps run in O(log n) time because the structure is split
    into multiple trees. The decrease key function runs in O( log n ) time, as do findMin, union, and deleting a given node.
    The paper in the following question also notably mentions the amortized cost of insert being O(1), making it efficient for large amounts of data. 
    

- Why do we use the left-child right-sibling representation in binomial heaps instead of traditional tree structures? How does it help with efficiency?
    Notably, this implemenation improves memory efficiency since each node doesn't need to have pointers to all of its children.
    This is especially important for binomial trees where the root of the tree has children equal to the degree of the tree. 
    Additionally, it's generally just easier to code. 

- What is your opinion on this

    The article discussing potential performance increases in using binomial heaps over rb trees in process scheduling.
    For the insert function in particular, the paper shows an improvement of 3x in time from rb trees to binomial heaps.
    The paper ultimately just uses simulation to measure insert and extractmin methods on the stuctures, 
    but it's definately promising and the need for a more efficient strucutre will arise as more processes
    need to be fairly scheduled in an OS. 
