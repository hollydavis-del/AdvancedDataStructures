// Base Code - linked binary search tree

#include <iostream>
#include "customErrorClass.h"
using namespace std;


// tree node for linked BST
struct treenode
{
    int dat;
    treenode *lchild;
    treenode *rchild;
};


// Class for binary search tree
// Private will be treenode * to the root

class bstree
{
 public:
	bstree();

	void in( );
	void pre( );
	void post( );

	void insert( int x );
	void remove( int x );
	bool search( int x );

	int numnodes(); // must compute, not a variable in class
	int height();   // must compute, not a variable in class
	int heightleft();
	int heightright();
	int height(int x);
	int getMax();
	bool isWorstCase();
	bool isBalanced();


 private:
	treenode *root;
	void in ( treenode *node );
	void pre ( treenode *node );
	void post ( treenode *node );

	void insert ( int x, treenode *st );
	void remove ( int x, treenode *st );
	treenode * remove ( treenode *st, int x );
	bool search ( int x, treenode *st );

	int numnodes ( treenode *st );
	int height ( treenode *st );
	int heightleft( treenode *st );
	int heightright ( treenode *st );
	int getMax( treenode *st );
};

