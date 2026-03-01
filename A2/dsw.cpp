#include "dsw.h"
#include "customErrorClass.h"

// ----------------- PRIVATE ----------------------------------

// when left-heavy tree
void BST::rotateRight(Node*& node) // passing the parent
{
    if(node == nullptr || node->left == nullptr)
    {
        //throw MyException("cannot rotate null nodes, rotation cancelled\n");
        return;
    }
    // get the node to rotate R
    Node* leftChild = node->left; 
    // 1) leftnode's right child is going to become parent's left child
    node->left = leftChild->right;
    // 2) parent is going to be right child of node that is rotated
    leftChild->right = node;
    
    node = leftChild;
}

// when right-heavy
void BST::rotateLeft(Node*& node)
{
    if(node == nullptr || node->right == nullptr)
    {
        //throw MyException("cannot rotate null nodes, rotation cancelled\n");
        return;
    }
    // get the node to rotate L
    Node* rightChild = node->right;
    // 1) rightnode's left child is going to become parent's right child
    node->right = rightChild->left;
    // 2) parent is going to be left child of node that is rotated
    rightChild->left = node;

    node = rightChild;
}

int BST::size(Node* node)//recursive function for finding height for a given subtree with root of node
{
    if (node == nullptr)
    {
        return 0;
    }
    else
        return 1 + size(node->left) + size(node->right);
}

// Phase 1 - left skewed linked list tree
void BST::createVine()
{
    if (root == nullptr){
        throw MyException("no nodes in tree, cannot create vine \n");
        return; //nothing to rotate
    }
    Node* grandparent = nullptr;
    Node* parent = root;
    Node* child = root->right;


    while ( parent != nullptr )
    {
        if( child != nullptr && size(child) >= 2)//rchild exists and has a subtree size >=2
        {
            rotateLeft(parent);

            if(grandparent == nullptr )
            {
                root = child;
            }
            else{
                grandparent->left = child;
            }
            parent = child;//what was the child node is now in parent's position
            child = parent->right;
        }
        else//no left child
        {
            grandparent = parent;
            parent = parent->left;
            if (parent != nullptr )
                child = parent->right;
        }
    }
}

void BST::rebuildTree(int size) //find how many left roations we need
{
    int h = (int) log2(size + 1 ); //height necessary for perfect subtree
    int m = ( 1 << h ) - 1; //nodes in perfect subtree
    //(1 << h) = 2^h
    int extra = size - m; //total size - nodes in perfect subtree

    //innitial rotation
    //cout << "performing innitial rotations with extra nodes: " << extra <<"\n";
    performRotation(extra);

    //subsequent roations
    for ( size = m / 2; size > 0; size/=2)
    {
        //cout << "performing rotation on size " << size << "\n";
        performRotation(size);
    }
}

// left rotate every second node based count
void BST::performRotation(int count)
{
    Node* grandparent = nullptr;
    Node* tmp;
    Node* parent = root;

    for ( int i = 0; i < count && parent != nullptr && parent->left != nullptr; i++)
    {
        //if even position, rotate
        if ( i % 2 == 0)
        {
            //cout << "rigth rotation performed on node " << parent->data << "\n";
            tmp = parent->left; //temp address of parent's left child
            rotateRight(parent);
            //update pointers
            if ( grandparent != nullptr )
            {
                grandparent->left = tmp;
                parent = grandparent->left;
            }
            else//parent is root of tree
            {
                root = tmp;
                //parent = root;
            }
        }
        //if even, keep moving
        else
        {
            grandparent = parent;
            parent = parent->left;
        }
    }
}

void BST::printTree(Node* root, int space) {
    const int COUNT = 10; 

    if (root == nullptr) {
        //throw MyException("cannot print empty tree");
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
    cout << root->data << endl;

    // Print the left child
    printTree(root->left, space);
}

// --------------------- PUBLIC ------------------
BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    deleteTree(root);
}

void BST::deleteTree(Node*& node)
{
    if(node == nullptr)
    {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

void BST::insert(int val)
{
    Node* newNode = new Node(val);
    if(root == nullptr){
        root = newNode;
        return;
    }
    
    Node* curr = root;
    Node*parent = nullptr;

    while(curr != nullptr)
    {
        parent = curr;
        if(val < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if(val < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

void BST::dswBalance()
{
    if ( root == nullptr){
        //throw MyException("cannot balance empty tree \n");
        return;
    }
    //phase 1
    createVine();

    cout<< "After phase 1: ";
    display();

    int size = 0;
    Node* temp = root;
    while (temp != nullptr)
    {
        size++;
        temp = temp->left;
    }

    //cout<< size << "<- size is \n";
    //phase 2
    rebuildTree(size);
    cout<< "After phase 2: ";
    display();
}

void BST::display()
{
    cout << endl;
    printTree(root, 0);
    cout << endl;
}