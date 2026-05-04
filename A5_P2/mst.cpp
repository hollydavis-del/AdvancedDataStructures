#include "mst.h"
#include <vector>
#include "binaryHeap.h"       // Custom binary heap
#include "binomialHeap.h"   // Custom binomial heap

// ===== QuickUnion =====
QuickUnion::QuickUnion(int n) //constructor
{
    parent.resize(n); //make vector storing parents' size = num nodes
    for (int i = 0; i < parent.size(); i++)
    {
        parent[i] = i; //each node starts as its own parent
    }
}

int QuickUnion::find(int x) 
{
    int temp = x;
    while (parent[temp] != temp)
    {
        temp = parent[temp]; //move to root of tree
    }
    return temp; //returns node that is root
}

void QuickUnion::Union(int x, int y) 
{
    int a = find(x);
    int b = find(y);

    if ( a != b ) //not in same union already
    {
        if ( a > b )
        {
            parent[b] = a;//parent becomes node with greater number 
        }
        else if (b > a)
        {
            parent[a] = b;
        }
    }
}

// ===== UnionFind =====
UnionFind::UnionFind(int n) 
{
    parent.resize(n); //resize parent vector
    rank.resize(n); //resize vector storing order of each node

    for (int i = 0; i < n; i++)
    {
        parent[i] = i; //each node starts as its own parent
        rank[1] = 0; //each node starts as rank 1
    }
}

int UnionFind::find(int x) 
{
    int temp = x;
    while (parent[temp] != temp)
    {
        temp = parent[temp]; //move to root of tree
    }
    parent[x] = temp; //path compression, original node's parent is now directly root of tree
    return temp; //returns node that is root
}

void UnionFind::Union(int x, int y) 
{
    int a = find(x);
    int b = find(y);

    if ( a != b ) //not in same union already
    {
        if ( rank[a] > rank[b] ) //union by size
        {
            parent[b] = a;//parent becomes node with greater number 
            rank[a] += rank[b]; //a gains ranks equal to rank of b
        }
        else if ( rank[b] > rank[a] )
        {
            parent[a] = b;
            rank[b] += rank[a]; 
        }
    }
}

// ===== MST =====
MST::MST(int vertices)
{
    n = vertices;
}

void MST::addEdge(int u, int v, int weight) 
{
    edges.push_back({u, v, weight});
}

int MST::kruskalV1()//using binary heaps and quick union
{
    HEAP h(edges.size());//heap size of all edges
    for (int i = 0; i < edges.size(); i++ )
    {
        h.insertH(edges[i]); //add all edges to heap
    }

    QuickUnion q(n); //n stored in MST class, number of nodes

    vector<Edge> spanning;
    int spansum;

    while (spanning.size() != (n-1))
    {
        Edge temp = h.peek();//look at min edge in heap
        if (q.find(temp.dest) != q.find(temp.src))//if edge doesn't cause a cycle 
        {
            spansum += temp.weight;
            spanning.push_back(temp);
        }
        h.deleteMin(); //delete minnode regardless since we've looked at that edge
    }
    return spansum;
}

int MST::kruskalV2() //using binomial heaps and union with optimizations
{
    BinomialHeap h;
    for (int i = 0; i < edges.size(); i++)
    {
        h.insert(edges[i]);//add all edges to binomial heap
    }

    UnionFind u(n); 

    vector<Edge> spanning;
    int spansum;

    while (spanning.size() != (n-1)) //since mst will have edges equal to nodes - 1
    {
        Edge temp = h.findMin();//look at min edge in heap
        if (u.find(temp.dest) != u.find(temp.src))//if edge doesn't cause a cycle 
        {
            spansum += temp.weight;
            spanning.push_back(temp);
        }
        h.deleteMin(); //delete minnode regardless since we've looked at that edge
    }
    return spansum;
}
