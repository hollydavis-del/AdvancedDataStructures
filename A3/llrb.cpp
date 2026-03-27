#include "llrb.h"
#include "customErrorClass.h"

// ++++++++++++++++++++++++++++++ PUBLIC +++++++++++++++++++++++++++++++++++++++

RBTREE::RBTREE()
{
    TNULL = new Node(0);
    TNULL->color = BLACK;
    TNULL->right = TNULL;
    TNULL->left = TNULL;

    rotations = 0;

    root = TNULL;

}

RBTREE::~RBTREE()
{
    deleteSubtree(root);
}

void RBTREE::insert(int key)
{
    Node* node = nullptr;
    node = new Node(key);

    node->left = TNULL;
    node->right = TNULL;

    if (root == TNULL)//empty tree
    {
        root = node;
        root->color = BLACK;
        return;
    }

    insertR(root, node);
    insertFix(node);
}

//void RBTREE::remove(int key)
//{
//    removeR(root, key);
//}

void RBTREE::printTree()
{
    if(root == TNULL)
    {
        cout << "Tree is empty" << endl;
        return;
    }
    print(root, 0);
}

// ++++++++++++++++++++++++++++++ PRIVATE +++++++++++++++++++++++++++++++++++++++++++++

void RBTREE::leftRotate(Node* y)
{
    Node* x = y->right;
    if (x == TNULL || x == nullptr) return;  // Prevent invalid rotation
    y->right = x->left;  // adoption

    if(x->left != TNULL) // updating parent of adopted left child
        x->left->parent = y; 

    // update parent of x
    x->parent = y->parent;
    if(y->parent == nullptr)
        root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->left = y;
    y->parent = x;

    rotations++;
    
}

void RBTREE::rightRotate(Node* y)
{

    
    Node* x = y->left;
    if (x == TNULL || x == nullptr) return;  // Prevent invalid rotation
    y->left = x->right;  // adoption

    if(x->right != TNULL) // updating parent of adopted left child
        x->right->parent = y; 

    // update parent of x
    x->parent = y->parent;
    if(y->parent == nullptr)
        root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
    
    rotations++;
}

void RBTREE::insertR(Node*& root, Node*& node)
{
    if (node->data == root->data) //value already in tree
    {
        throw MyException("Value already in tree, insertion failed.");
    }
    else if(node->data < root->data)
    {
        if(root->left == TNULL)
        {
            root->left = node;
            node->parent = root;
        } else {
            insertR(root->left, node);
        }
    } 
    else { // if node->data > root->data
        if(root->right == TNULL)
        {
            root->right = node;
            node->parent = root;
        } else {
            insertR(root->right, node);
        }
    }

    if (root->right->color == RED && root->left->color != RED )
    {
        root->color = RED;
        root->right->color = BLACK;
        leftRotate(root);
        //root = root->parent;
    }
    if(root->left->color == RED && root->left->left->color == RED )
    {
        rightRotate(root);
        //root = root->parent;
    }
    if (root->left->color == RED && root->right->color == RED)
    {
        if (root->color == RED)
        {
            root->color = BLACK;
        }
        else
            root->color = RED;

        if (root->right->color == RED)
        {
            root->right->color = BLACK;
        }
        else
            root->right->color = RED;

        if (root->left->color == RED)
        {
            root->left->color = BLACK;
        }
        else
            root->left->color = RED;
        //flip colors of root and its children, can't have 2 red children
    }
    insertFix(root);
}

void RBTREE::insertFix(Node* k)
{
    root->color = BLACK; //just in case root was made red by the above loop
}


void RBTREE::removeR(Node*& node, int key)
{
    if (node == TNULL)//base case
    {
        return;//node not present in tree
    }
    if (key < node->data )
        removeR(node->left, key);
    else if ( key > node->data)
        removeR(node->right, key);//recursive calls on subtree key is in
    else{//key == node->data
        Node* z = node;//node we want to delete
        Node* y = node; //successor node, if any
        Node* x; //replacement node
        Color y_original_color = node->color;

        if (node->left == TNULL && node->right != TNULL)//right child only case
        {
            x = node->right; 
            transplant(node, node->right);

        }else if (node->left != TNULL && node->right == TNULL)//left child only case
        {
            x = node->left;
            transplant(node, node->left);
        }
        else if (node->left == TNULL && node->right == TNULL )//no child case
        {
            x = TNULL;
            transplant(node, TNULL);
        }
        else //2 child case
        {
            y = successor(y->right); //finds lowest value in right subtree of node
            y_original_color = y->color;
            x = y->right; // to avoid orphans, right child of successor
            if (y->parent != node )
            {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y; //update parent of transplanted node

            }
            y->left = node->left;
            y->left->parent = y;
            transplant(node, y);
            y->color = node->color;
        }
        if (y_original_color == BLACK )
        {
            deleteFix(x);
        }
        delete z; //frees node
    }
    root->color = BLACK;//root is always set to black
}

void RBTREE::deleteFix(Node* x) {
    Node* sibling;
    
    while(x != root && x->color == BLACK)//traverse up tree
    {
        if (x == x->parent->left)
        {
            sibling = x->parent->right;
            //case 1
            if( sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                sibling = x->parent->right; //update sibling after rotate
            }
            //case 2
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }
            else{//case 3 or 4
                if (sibling->right->color == BLACK)//case 3
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling);
                    sibling = x->parent->right;
                }
                //case 4
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else//is right child of parent
        {
            sibling = x->parent->left;
            //case 1
            if( sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                sibling = x->parent->left; //update sibling after rotate
            }
            //case 2
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }
            else{//case 3 or 4
                if (sibling->right->color == BLACK)//case 3
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling);
                    sibling = x->parent->left;
                }
                //case 4
                sibling->color = x->parent->color;
                x->parent->color = BLACK;
                sibling->right->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
    
}

void RBTREE::transplant(Node*& u, Node*& v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if(v != TNULL)
        v->parent = u->parent;
}

Node* RBTREE::successor(Node* node) {
    if (node == TNULL) 
        return TNULL;
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

void RBTREE::deleteSubtree(Node* node) {
    if (node != TNULL) {
        deleteSubtree(node->left);
        deleteSubtree(node->right);
        delete node;
    }
}


void RBTREE::print(Node* root, int space ) {
    if (root == TNULL || root == nullptr)
        return;

    space += 10;
    print(root->right, space);

    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";

        
    string color_code = (root->color == RED) ? RED_COLOR : BLACK_COLOR;
    cout << color_code << root->data << RESET_COLOR;
    cout << endl;
    print(root->left, space);
}
