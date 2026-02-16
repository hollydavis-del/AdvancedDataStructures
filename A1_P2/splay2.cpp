#include "splay.h"

SplayTree::SplayTree() 
{
    root = nullptr; 
}

// aka Zig
SplayTree::Node* SplayTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// aka Zag
SplayTree::Node* SplayTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splaying :)
//top down
SplayTree::Node* SplayTree::splay(Node* root, int key) {
    if (root == nullptr || root->key == key)
        return root;//empty tree or key is at root
    
    //key in left subtree
    if( root->key > key)
    {
        if(root->left == nullptr )//base case
        {
            return root;//value not in tree
            throw MyException("Value already present. No insertion performed.");
        }
        //zigzig
        if(key < root->left->key)
        {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } 
        //zigzag
        else if (key > root->left->key)
        {
            root->left->right = splay(root->left->right, key);
            if(root->left->right != nullptr)
                root->left = rotateLeft(root->left);
        }
        if(root->left == nullptr)
        {
            return root;
        }
        else
        {
            return rotateRight(root);
        }
    }
    //key in right subtree
    if( root->key < key)
    {
        if(root->right == nullptr )//base case
        {
            throw MyException("Value already present. No insertion performed.");
            return root;//value not in tree
        }
        //zagzag
        if(key > root->right->key)
        {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        } 
        //zagzig
        else if (key < root->right->key)
        {
            root->right->left = splay(root->right->left, key);
            if(root->right->left != nullptr)
                root->right = rotateRight(root->right);
        }
        if(root->right == nullptr)
        {
            return root;
        }
        else
        {
            return rotateLeft(root);
        }
    }

}

//1pass
SplayTree::Node* SplayTree::insertNode(Node* root, int key) {
    if ( root == nullptr )
    {
        return new Node(key);
    }
    root = splay(root, key); //if value is already in tree, root->key == key. Otherwise closest nodekey becomes root
    
    if (root->key == key)
    {  
         throw MyException("Value already present. No insertion performed.");
        return root;//value already in tree
    }
    Node* newNode = new Node(key);
    if ( key < root->key)
    {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr; 
    }
    else
    {
        newNode->left = root;
        newNode->right = root->left;
        root->right = nullptr;
    }
    return newNode; //returns new root, newNode
}


SplayTree::Node* SplayTree::deleteNode(Node* root, int key) {
    if (root == nullptr)
    {
        throw MyException("empty tree, no deletion possible");
        return root;//exception handling, empty tree
    }
    
    Node* mov = root;
    while ( mov->left->key != key && mov->right->key != key)
    {
        if ( mov->key > key)
            mov = mov->left;
        else
            mov = mov->right;
    }//mov points to parent of target node to be deleted

    int parentKey = mov->key;
    splay(root, key);//splay target node to top of tree

    Node* leftsubtree = root->left;
    Node* rightsubtree = root->right;
    if ( leftsubtree == nullptr )
        splay(root, parentKey);
        return rightsubtree;
    else if (rightsubtree == nullptr)
        splay(root, parentKey);
        return leftsubtree;
    else//both subtrees are non-empty
    {
        Node* tmp = rightsubtree;
        while (tmp->left != null)
            tmp = tmp->left;
        tmp->left = leftsubtree;
    }
    splay(root, parentKey);//splays innitial parent to top of tree

}

Node* SplayTree::weightedSplay(Node* root, int key)
{
if (root == nullptr || root->key == key)
        return root;//empty tree or key is at root
    
    //key in left subtree
    if( root->key > key)
    {
        if(root->left == nullptr )//base case
        {
            return root;//value not in tree
            throw MyException("Value already present. No insertion performed.");
        }
        //zigzig
        if(key < root->left->key && root->left->left->weight >= root->left->weight)//rotations performed onlu if child weight >= parent weight
        {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } 
        //zigzag
        else if (key > root->left->key && root->left->right->weight >= root->left->weight)
        {
            root->left->right = splay(root->left->right, key);
            if(root->left->right != nullptr)
                root->left = rotateLeft(root->left);
        }
        if(root->left == nullptr)
        {
            return root;
        }
        else
        {
            if(root->weight >= root->left->weight)
                return rotateRight(root);
        }
    }
    //key in right subtree
    if( root->key < key)
    {
        if(root->right == nullptr )//base case
        {
            throw MyException("Value already present. No insertion performed.");
            return root;//value not in tree
        }
        //zagzag
        if(key > root->right->key && root->right->right->weight >= root->right->weight)
        {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        } 
        //zagzig
        else if (key < root->right->key && root->right->left->weight >= root->right->weight)
        {
            root->right->left = splay(root->right->left, key);
            if(root->right->left != nullptr)
                root->right = rotateRight(root->right);
        }
        if(root->right == nullptr)
        {
            return root;
        }
        else
        {
            if(root->weight >= root->right->weight)
                return rotateLeft(root);
        }
    }
    root->wegiht++;
}

Node* SplayTree::semiSplay(Node* root, int key, int rotations);
{
    if (root == nullptr || root->key == key)
        return root;//empty tree or key is at root

    if ( rotations > 4)
    {
        return root; //stop semi splay after defined number of rotations
    }
    
    //key in left subtree
    if( root->key > key)
    {
        if(root->left == nullptr )//base case
            return root;//value not in tree
        //zigzig
        if(key < root->left->key)
        {
            root->left->left = semiSplay(root->left->left, key, rotations);
            rotations++;
            root = rotateRight(root);
        } 
        //zigzag
        else if (key > root->left->key)
        {
            root->left->right = semiSplay(root->left->right, key, rotations);
            if(root->left->right != nullptr)
            {
                rotations++;
                root->left = rotateLeft(root->left);
            }
        }
        if(root->left == nullptr)
        {
            return root;
        }
        else
        {
            rotations++;
            return rotateRight(root);
        }
    }
    //key in right subtree
    if( root->key < key)
    {
        if(root->right == nullptr )//base case
        {
            throw new exception("Value not present in tree, splay cancelled.");
            return root;//value not in tree
        }
        //zagzag
        if(key > root->right->key)
        {
            root->right->right = semiSplay(root->right->right, key, rotations);
            rotations++;
            root = rotateLeft(root);
        } 
        //zagzig
        else if (key < root->right->key)
        {
            root->right->left = semiSplay(root->right->left, key, rotations);
            if(root->right->left != nullptr)
            {
                rotations++;
                root->right = rotateRight(root->right);
            }
        }
        if(root->right == nullptr)
        {
            return root;
        }
        else
        {
            rotations++;
            return rotateLeft(root);
        }
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
