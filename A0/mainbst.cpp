#include "bst.h"
//#include "customErrorClass.h"
using namespace std;


int main()
{
    bstree tree;

    MyException excp("MyError");
    try{
        tree.remove(5);    
    } catch( MyException &excp){
        cerr<<excp.what()<<endl;
    }

    tree.bstree::insert(5);
    tree.bstree::insert(12);
    tree.bstree::insert(1);
    tree.bstree::in();
    printf("%b\n", tree.isBalanced());
    tree.remove(5);
    tree.in();

    bstree tree2;   
    tree2.insert(5);
    tree2.insert(6);
    tree2.insert(7);
    printf("7 is at height: %d\n", tree2.height(7));
    tree2.pre();
    tree2.isWorstCase();

    return 0;
}