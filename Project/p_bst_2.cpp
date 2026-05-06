// persistent BST
// path copying
// shared pointers - keep track of refcount  for delete()
// thread-safe

#include <iostream>
#include <memory>

using namespace std;

struct Node {
    int key;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    int refcount;  //it is not thread-safe 

    Node(int k, shared_ptr<Node> l = nullptr, shared_ptr<Node> r = nullptr)
    {
        key = k;
        left = l;
        right = r;
    }
};

class persistentBST {
    private:
        shared_ptr<Node> root;

        shared_ptr<Node> insert(shared_ptr<Node> node, int key)
        {
            if(!node)
                return make_shared<Node>(key);
            
            if(key < node->key)
            {
                return make_shared<Node>(node->key, insert(node->left, key), node->right);
            } else if (key > node->key)
            {
                return make_shared<Node>(node->key, node->left, insert(node->right, key));
            } else {
                return node;
            }
        }

        shared_ptr<Node> remove(shared_ptr<Node> node, int key)
        {
            if(!node) //value not in tree
                return nullptr;
            
            if(key < node->key)
            {
                return make_shared<Node>(node->key, remove(node->left, key), node->right);
            } else if (key > node->key)
            {
                return make_shared<Node>(node->key, node->left, remove(node->right, key));
            } else { //at node to be remoevd

                if (node->left == nullptr && node->right == nullptr) //leaf node
                    return nullptr;
                else if (node->left == nullptr)//right child only
                {
                    return make_shared<Node>(node->right->key, node->right->left, node->right->right);
                }
                else if (node->right == nullptr) //left child only
                {
                    return make_shared<Node>(node->left->key, node->left->left, node->left->right);
                }
                else //2 child case 
                {
                    shared_ptr<Node> mov = node->right;
                    while (mov->left->left != nullptr)
                    {
                        mov = mov->left; //get successor node
                    } //mov's left child is now successor
                    shared_ptr<Node> tmp = mov->left;
                    mov->left = nullptr;
                    return make_shared<Node>(tmp->key, node->left, node->right);
                }
            }
        }

        bool search(shared_ptr<Node> node, int key){
            if (!node)
                return false;
            if(key == node->key)
                return true;
            if (key < node->key)
                return search (node->left, key);
            return search(node->right, key);
        }

        void inorder(shared_ptr<Node> node)
        {
            if(!node)
                return;
            inorder(node->left);
            cout << node->key << " ";
            inorder(node->right);
        }

    public:
        persistentBST(){
            root = nullptr;
        }
        persistentBST(shared_ptr<Node> r){
            root = r;
        }

        persistentBST insert(int key)
        {
            return persistentBST(insert(root, key));
        }

        persistentBST remove(int key)
        {
            return persistentBST(remove(root, key));
        }
        bool search(int key)
        {
            return search(root, key);
        }

        void inorder()
        {
            inorder(root);
            cout<<endl;
        }
};

int main()
{
    persistentBST t1;

    auto t2 = t1.insert(10);
    auto t3 = t2.insert(20);
    auto t4 = t3.insert(30);
    auto t5 = t4.insert(40);
    auto t6 = t5.remove(40);
    auto t7 = t6.remove(10);

    cout << "t3: ";
    t3.inorder();

    cout << "t5: ";
    t5.inorder();

    cout << "t6: ";
    t6.inorder();

    cout << "t7: ";
    t7.inorder();

return 0;
}