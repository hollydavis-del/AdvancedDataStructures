#include "splay.h"

SplayTree::SplayTree() 
{
    root = nullptr; 
}

// aka Zig
SplayTree::Node* SplayTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    //--------
    if( y->right )
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent)
    {
        if ( x->parent->left = x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
   //--------
    y->right = x;
    //-------
    x->parent = y;
    //-------
    return y;
}

// aka Zag
SplayTree::Node* SplayTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    //---------
    if( y->left )
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent)
    {
        if ( x->parent->left = x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    //---------
    y->left = x;

    //---------
    x->parent = y;
    //---------
    return y;
}

// Splaying :)
//bottom-up 
void SplayTree::splay(Node* x) {
    if (!x)
        return;
    while ( x-> parent != nullptr)
    {
        Node* p = x->parent;//parent
        Node* g = x->parent->parent;//grandparent

        //SINGLE ROTATION CASES, zig/zag
        if (!g)//parent is root
        {
            if(p->left == x)
                rotateRight(p);//zig
            else
                rotateLeft(p);//zag
        }
        //double rotations, zig zig or zag zag
        else if( g->left == p && p->left = x)
        {
            rotateRight(g);
            rotateRight(p);
        }
        else if( g->right == p && p->right = x)
        {
            rotateLeft(g);
            rotateLeft(p);
        }

        //opposite sides. zig zag or zag zig
        else if ( g->left == p && p->right == x)
        {
            rotateLeft(p);
            rotateRight(g);
        }
        else if (g->right == p && p->left == x)
        {
            rotateRight(p);
            rotateLeft(g);
        }
    }
    root = x; //x is now root of tree
}


void SplayTree::insertNode(int key) {
    if ( ! root ) //empty tree
    {
        root = new Node(key);
        return;
    }
    Node* curr = root;
    Node* parent = nullptr;

    while(curr != nullptr)
    {
        parent = curr;
        if(key == curr->key)
            return;//value already in tree
        if ( key < curr->key )
        {
            curr = curr->left;
        }
        else
            curr = curr->right;
    }

    Node* n = new Node(key);
    n->parent = parent;
    if (key < parent->key)
        parent->left = n;
    else
        parent->right = n;
    splay(n);//splays around added node
}


SplayTree::Node* SplayTree::deleteNode(Node* root, int key) {
    Node* mov = root;
    while ( mov->left->key != key && mov->right->key != key)
    {
        if ( mov->key > key)
            mov = mov->left;
        else
            mov = mov->right;
    }//mov now points to parent of node to be deleted
    //leaf cases
    if (mov->left->key == key && mov->left->left == nullptr && mov->left-right == nullptr )
    {
        free(mov->left);
        mov->left = nullptr;
        splay(mov);
    }
    else if (mov->right->key == key && mov->right->right == nullptr && mov->left->right == nullptr)
    {
        free( mov->right);
        mov->right = nullptr;
        splay(mov);
    }
    //2 child cases
    else if (mov->right->key == key && mov->right->right != nullptr && mov->right->left != nullptr)
    {
        node* temp = mov->right;
        mov->right = tmp->right;
        while (temp->left != null)
            temp = temp->left;
        temp->left = mov->right->left;
        free(temp);//free node to be deleted
        splay(mov);
    }
     else if (mov->left->key == key && mov->left->right != nullptr && mov->left->left != nullptr)
    {
        node* temp = mov->left;
        mov->left = tmp->left;
        while (temp->right != null)
            temp = temp->right;
        temp->right = mov->left->right;
        free(temp);//free node to be deleted
        splay(mov);
    }
}


void SplayTree::insert(int key) {
    root = insertNode(root, key);
}


void SplayTree::remove(int key) {
    root = deleteNode(root, key);
}


bool SplayTree::search(int key) {
    root = splay(root, key);
    return (root && root->key == key);
}


void SplayTree::printTree(Node* root, int space) {
    const int COUNT = 10; 

    if (root == nullptr) {
        return;
    }

    // Increase the distance between levels
    space += COUNT;

    // Print the right child first (to appear on top)
    printTree(root->right, space);

    // Print the current node after right child

    for (int i = COUNT; i < space; i++) {
        cout << " "; // Indentation for tree depth
    }
    cout << root->key << endl;

    // Print the left child
    printTree(root->left, space);
}

void SplayTree::display() {
    printTree(root, 0);
    cout << endl;
}
