#include <iostream>
#include <fstream>
using namespace std;

#include "bst.h"
#include <cmath>



// allocate a new node
//   not part of the class
treenode *makeatreenode( int x )
{
	treenode *box = new treenode;
	box->dat = x;
	box->lchild = NULL;
	box->rchild = NULL;
	return box;
}


bstree::bstree()
{
	root = NULL;
}

void bstree::in()
{
	cout << "In order: ";
	in (root);
	cout << endl;
}

void bstree::in ( treenode *st )
{
	if ( st == NULL )
	{
		return;
	}
	in ( st->lchild );
	cout << st->dat << " ";
	in ( st->rchild );
}

void bstree::pre()
{
	cout << "Pre order: ";
	pre ( root );
	cout << endl;
}

void bstree::pre ( treenode *st )
{
	if ( st == NULL )
	{
		return;
	}
	cout << st->dat << " ";
	pre ( st->lchild );
	pre ( st->rchild );
}

void bstree::post()
{
	cout << "Post order: ";
	post ( root );
	cout << endl;
}

void bstree::post ( treenode *st )
{
	if ( st == NULL )
	{
		return;
	}
	post ( st->lchild );
	post ( st->rchild );
	cout << st->dat << " ";
}



void bstree::insert( int val )
{
	insert (val, root );
}

void bstree::insert ( int x, treenode *st )
{
	if ( st == NULL )//st will only be null when empty bst is passed
	{
		treenode *box = makeatreenode ( x );
		root = box;
		return;
	}
	if ( x == st->dat )//value to be inserted is already in tree
	{
		throw MyException("Value already present. No insertion performed.");
		return;
	}
	if ( st->dat < x )//value to be inserted is greater than current node
	{
		if ( st->rchild == NULL )
		{
			treenode *box = makeatreenode ( x );
			st->rchild = box;
		}
		else
			insert ( x, st->rchild );
	}
	else if (st->dat > x )//value to be inserted is less than current node
	{
		if ( st->lchild == NULL )
		{
			treenode *box = makeatreenode ( x );
			st->lchild = box;
		}
		else
			insert ( x, st->lchild );
	}
}

void bstree::remove ( int val )
{
	remove ( val, root );
}

treenode * bstree::remove (treenode *st, int x )
{	//base case
	if ( st == NULL )
	{
		return st;
	}
	//recursive calls
	else if ( st->dat < x )
	{
		st->rchild = remove( st->rchild, x );
	}
	else if ( st->dat > x )
	{
		st->lchild = remove( st->lchild, x );
	}
	else{
		//perform deletes
		if ( st->lchild == NULL && st->rchild == NULL )//node is a leaf
		{
			free( st );
			st = NULL;
		}
		else if ( st->lchild == NULL )//we know value in st->right must not be null
		{
			treenode *tmp = st;
			st = st->rchild;
			free( tmp );
		}
		else if ( st->rchild == NULL )
		{
			treenode *tmp = st;
			st = st->lchild;
			free( tmp );
		}
		else //2 child case
		{
			treenode *mov = st->rchild;
			while (mov->lchild != NULL )
			{
				mov = mov->lchild;
			}
			st->dat = mov->dat;
			st->rchild = remove( st->rchild, mov->dat );
		}
	}
	return st;

}

void bstree::remove ( int x, treenode *st )
{
	if ( root == NULL )//tree has no nodes
	{
		throw MyException("Tree has no nodes to remove");
		return;
	}
		if ( root->rchild == NULL && root->lchild == NULL )//one node tree
	{
		free( root );
		root = NULL;
		return;
	}
	if ( root->dat == x && root->lchild == NULL )
	{
		treenode *tmp = root;
		root = root->rchild;
		free(tmp);
		return;
	}
	if ( root->dat == x && root->rchild == NULL )
	{
		treenode *tmp = root;
		root = root->lchild;
		free(tmp);
		return;
	}
	if ( !( search( x ) ) ){//if value is not in tree
		cout << "Value not present in tree. " << endl;
		return;
	}
	
	remove ( st, x );//calls separate remove with a different return type making recursion easier
}

bool bstree::search( int val )
{	//return true or false
	return search ( val, root );
}

bool bstree::search ( int x, treenode *st )
{
	if ( st == NULL )//base cases
	{
		return false;
	}
	if ( x == st->dat )
	{
		return true;
	}
	if ( x < st->dat )
	{
		return search ( x, st->lchild );
	}
	else
	{
		return search ( x, st->rchild );
	}
}

int bstree::numnodes( )
{
	return numnodes ( root );
}

int bstree::numnodes ( treenode *st )
{
	if (st == NULL )
	{
		return 0;	
	}
	int left = numnodes ( st->lchild );
	int right = numnodes ( st->rchild );
	int sum = 1 + left + right;
	return sum;
}

int bstree::height( int x ) //height for a given node
{
	if (!search(x))
	{
		return -1;//exception, node not in tree
	}
	else
	{
		int h = 0 ;
		treenode *mov = root;
		while ( mov->dat != x )
		{
			if ( mov->dat < x)
			{
				mov = mov->rchild;
				h++;
			}
			else if ( mov->dat > x) //must move left to find node
			{
				mov = mov->lchild;
				h++;
			}
		}
		return h;
	}
}

int bstree::height( )
{
	return height ( root );
}

bool bstree::isWorstCase( )
{
	if (height() == numnodes())
	{
		printf("Worst case scenario.");
		return true;
	}
	else 
		return false;
}

bool bstree::isBalanced()
{
	if ( heightleft(root) == heightright(root) && numnodes(root->lchild) == numnodes(root->rchild)) // tree is balanced if height is logarithmically proportional to the number of nodes
		return true;
	return false;
}

int bstree::height ( treenode *st )
{
	if ( st == NULL )
	{
		return 0;
	}
	if (height (st->lchild ) > height (st->rchild))
	{
		return 1 + height( st->lchild );
	}
	else//works in the case that they're both equal
	{
		return 1 + height( st->rchild );
	}
}

int bstree::heightright()
{
	return heightright( root );
}

int bstree::heightright ( treenode *st )
{
	return height( st->rchild );
}


int bstree::heightleft()
{
	return heightleft( root );
}

int bstree::heightleft ( treenode *st )
{
	return height( st->lchild );
}

int bstree::getMax( )
{
	return getMax( root );
}

int bstree::getMax( treenode *st )
{
	if ( root == NULL )
	{
		return 0;
	}
	treenode *tmp = root;
	while ( tmp->rchild != NULL )
	{
		tmp = tmp->rchild;
	}
	return tmp->dat;
}
