#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <iostream>
#include <iomanip>
#include "customErrorClass.h"

using namespace std;

struct binomialNode
{
    int key;
    int degree;
    binomialNode* sibling; //with left child, right sibling representation
    binomialNode* child; //leftmost child
    binomialNode* parent; 

    binomialNode(int k);
};

class BinomialHeap
{
    private:
        binomialNode* unionHeap(binomialNode* heap1, binomialNode* heap2);
        binomialNode* mergeTrees(binomialNode* tree1, binomialNode* tree2);
        void linkTrees(binomialNode*& prev, binomialNode*& cur, binomialNode*& next);

    public:
        BinomialHeap();

        binomialNode* head;

        void insert(int key);
        void merge(BinomialHeap& other);
        int findMin();

        binomialNode* deleteMin(binomialNode* h);
        void decreaseKey(binomialNode* node, int oldKey, int newKey);
        void deleteKey(int key);
        int revertList(binomialNode* h);

        void printHeap();
        void printTree(binomialNode* node, int space);
};

#endif