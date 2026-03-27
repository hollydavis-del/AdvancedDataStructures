#include "llrb.h"
#include "customErrorClass.h"

int main() {
    RBTREE rb;

        rb.insert(1);  
        rb.insert(2);
        rb.insert(3);  
        rb.insert(4);
        rb.insert(5);  
        rb.insert(6);
        rb.insert(7);  
        rb.insert(8);
        rb.insert(9);  
        try 
        {
            rb.insert(9);
        }
        catch( MyException &excp)
        {
            cerr<<excp.what()<<endl;
        }


        rb.printTree();

    return 0;
}
