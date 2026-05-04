#include "binomialHeap.h"
#include "mst.h"  

// Modify submission from A5_p1

//private
binomialNode* BinomialHeap::unionHeaps(binomialNode* heap1, binomialNode* heap2)
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
    //if ((tree1->edge->weight) > (tree2->edge->weight))
    {
        swap(tree1, tree2);
    }
    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

void BinomialHeap::linkTrees(binomialNode*& prev, binomialNode*& curr, binomialNode*& next)
{
    if (curr->degree != next->degree || next->sibling && next->sibling->degree == curr->degree)//no violation
    {
        prev = curr;
        curr = next;
        //next = cur->sibling;
    }
    else//actually linking trees
    {
        if(true)//curr->edge->weight < next->edge->weight)
        {
            curr->sibling = next->sibling;
            mergeTrees(curr, next);
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
            mergeTrees(next, curr);
            curr = next;
        }
    }
}

binomialNode* BinomialHeap::reverseList(binomialNode* node)
{
    if (node->sibling != nullptr)
    {
        reverseList(node->sibling);
        (node->sibling)->sibling = node;
    }
    else
        head = node;
}

binomialNode* BinomialHeap::findNode(binomialNode* node, Edge edge)
{

}

//public:
BinomialHeap::BinomialHeap()
{
    head = nullptr;
}

void BinomialHeap::insert(Edge edge)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(edge); // make heap with head node pointing to a new node
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
    head = unionHeaps(head, other.head); // union our heaps
    
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

Edge BinomialHeap::findMin()
{
    if (!head)
    {
        return -1;//empty heap
    }

    binomialNode* minNode = head;
    int minKey = (head->edge)->weight;

    binomialNode* temp = head->sibling;
    while(temp)//while nodes exist to walk to
    {
        if((temp->edge)->weight < minKey)
        {
            minKey = (temp->edge)->weight;
            minNode = temp;
        }
        else
        {
            temp = temp->sibling;//keep walking
        }
    }

    return minNode;
}

void BinomialHeap::deleteMin()
{
    if (!head)
    {
        //throw MyException("cannot delete from empty heap\n");
        return nullptr;
    }

    binomialNode* previous = nullptr;
    binomialNode* min = head;

    int minValue = (min->edge)->weight; //value starts as head->key
    binomialNode* mov = head;
    while (mov->sibling != nullptr)
    {
        if (((mov->sibling)->edge)->weight < minValue)
        {
            minValue = ((mov->sibling)->edge)->weight;
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
        reverseList(min->child);
        min->child->sibling = nullptr;
    }
    else
        head = nullptr;
    delete min;

    unionHeap(h, head);
}

void BinomialHeap::decreaseKey(Edge oldEdge, Edge newEdge)
{
    
}

void BinomialHeap::deleteKey(Edge edge)
{
    if (head == nullptr)
    {
        //throw MyException("cannot delete from empty heap, as key doesn't exist\n");
        return;
    }

    Edge newedge;
    newedge.dest = edge.dest;
    newedge.src = edge.dest;
    newedge.weight = -9999;
    decreaseKey(edge, newedge);
    deleteMin();//decreases value of node to be smallest value possible and then removes it
}

void BinomialHeap::printHeap()
{
    //out << "Binomial Heap:\n";
    binomialNode* current = head;


    while (current != nullptr) {
        //cout << endl << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }
}

void BinomialHeap::printTree(binomialNode* node, int space)
{
    if (node == nullptr) 
        return;

    //cout << setw(space * 2) << node->edge->weight << endl;

    binomialNode* child = node->child;
    while (child) {
        printTree(child, space + 3);
        child = child->sibling;
    }
}