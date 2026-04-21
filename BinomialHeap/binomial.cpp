#include "binomial.h"

binomialNode::binomialNode(int k)
{
    key = k;
    degree = 0; //new node has no children
    parent = nullptr;
    child = nullptr;
    sibling = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------
binomialNode* BinomialHeap::unionHeap(binomialNode* heap1, binomialNode* heap2)
{
    if ( heap1 == nullptr)
    {
        return heap2;
    }
    if ( heap2 == nullptr)
    {
        return heap1;
    }

    binomialNode* newHead = nullptr;
    binomialNode** position = &newHead;

    while ( heap1 && heap2 )//both are not null
    {
        if (heap1->degree <= heap2->degree)
        {
            *position = heap1;
            heap1 = heap1->sibling;
        }
        else
        {
            *position = heap2;
            heap2 = heap2->sibling;
        }
        position = &((*position)->sibling); 
    }

    //add remaining trees if any
    *position = (heap1) ? heap1 : heap2;

    return newHead;
}


binomialNode* BinomialHeap::mergeTrees(binomialNode* tree1, binomialNode* tree2)
{
    //assume tree1 is the parent, so tree1 has smallest root (to maintain min heap properties)
    if (tree1->key > tree2->key)
    {
        swap(tree1, tree2);
    }
    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

void BinomialHeap::linkTrees(binomialNode*& prev, binomialNode*& cur, binomialNode*& next)
{
    if (cur->degree != next->degree || next->sibling && next->sibling->degree == cur->degree)//no violation
    {
        prev = cur;
        cur = next;
        //next = cur->sibling;
    }
    else//actually linking trees
    {
        if(cur->key < next->key)
        {
            cur->sibling = next->sibling;
            mergeTrees(cur, next);
        }
        else//if next->key is smaller
        {
            if( !prev ) // if previous pointer has a value
            {
                head = next;
            }
            else//prev has vlaue
            {
                prev->sibling = next;
            }
            mergeTrees(next, cur);
            cur = next;
        }
    }
}

// ---------------- PUBLIC FUNCTIONS ------------------------------
BinomialHeap::BinomialHeap()
{
    head = nullptr;
}

void BinomialHeap::insert(int key)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(key); // make heap with head node pointing to a new node
    head = unionHeap(head, tempHeap.head); //combine heaps

    if (!head || !head->sibling)
    {
        return; //no extra work, 1 node heap
    }
    //union of 2 trees required
    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while(next) 
    {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

void BinomialHeap::merge(BinomialHeap& other)
{
    head = unionHeap(head, other.head); // union our heaps
    
    if (!head || !head->sibling)
    {
        return; //no extra work, 1 node heap
    }
    //union of 2 trees required
    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while(next) 
    {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

int BinomialHeap::findMin() //log n complexity
{
    if (!head)
    {
        return -1;//empty heap
    }

    binomialNode* minNode = head;
    int minKey = head->key;

    binomialNode* temp = head->sibling;
    while(temp)//while nodes exist to walk to
    {
        if(temp->key < minKey)
        {
            minKey = temp->key;
            minNode = temp;
        }
        else
        {
            temp = temp->sibling;//keep walking
        }
    }

    return minKey;
}


binomialNode* BinomialHeap::deleteMin(binomialNode* h)
{
    if (!h)
    {
        throw MyException("cannot delete from empty heap\n");
        return nullptr;
    }

    binomialNode* previous = nullptr;
    binomialNode* min = head;

    int minValue = min->key; //value starts as head->key
    binomialNode* mov = h;
    while (mov->sibling != nullptr)
    {
        if ((mov->sibling)->key < minValue)
        {
            minValue = (mov->sibling)->key;
            previous = mov;
            min = mov->sibling;
        }
        mov = mov->sibling;
    }

    if (previous == nullptr && min->sibling == nullptr )//1 element heap
    {
        head = nullptr; 
    }
    else if (previous == nullptr)
    {
        head = min->sibling;
    }
    else//usual delete
        previous->sibling = min->sibling;

    if (min->child != nullptr)
    {
        revertList(min->child);
        min->child->sibling = nullptr;
    }
    else
        head = nullptr;
    delete min;

    unionHeap(h, head);    
}

int BinomialHeap::revertList(binomialNode* h)
{
    if (h->sibling != nullptr)
    {
        revertList(h->sibling);
        (h->sibling)->sibling = h;
    }
    else
        head = h;
}

void BinomialHeap::decreaseKey(binomialNode* node, int oldKey, int newKey)
{
    if(!node)
    {
        //throw MyException("value not present in heap\n");
        return;
    }

    if (node->key == oldKey)//if node is the one to be updated
    {
        node->key = newKey;
        binomialNode* parent = node->parent;

        while (parent != nullptr && node->key < parent->key)
        {
            int temp = node->key;
            node->key = parent->key;
            parent->key = temp;//swap node values
            node = parent;
            parent = parent->parent;
        }
    }
    else//recursive calls
    {
        decreaseKey(node->child, oldKey, newKey);
        decreaseKey(node->sibling, oldKey, newKey);
    }
}

void BinomialHeap::deleteKey(int key)
{
    if (head == nullptr)
    {
        throw MyException("cannot delete from empty heap, as key doesn't exist\n");
        return;
    }

    decreaseKey(head, key, -99999999);
    deleteMin(head);//decreases value of node to be smallest value possible and then removes it
}

void BinomialHeap::printHeap() {
    cout << "Binomial Heap:\n";
    binomialNode* current = head;


    while (current != nullptr) {
        cout << endl << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }

}

void BinomialHeap::printTree(binomialNode* node, int space) 
{
    if (node == nullptr) 
        return;

    cout << setw(space * 2) << node->key << endl;

    binomialNode* child = node->child;
    while (child) {
        printTree(child, space + 3);
        child = child->sibling;
    }
}