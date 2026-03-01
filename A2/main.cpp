#include "dsw.h"

int main()
{
    BST bst;

    bst.insert(2);
    bst.insert(4);
    bst.insert(5);
    bst.insert(1);
    bst.insert(6);
    bst.insert(3);
    bst.insert(0);
    bst.insert(12);
    bst.insert(13);
    bst.display();

    bst.dswBalance();

    

    
    return 0;
}