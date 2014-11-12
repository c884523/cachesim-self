#include <iostream>
#include "splay_tree.h"
using namespace std;

int main(void)
{
	TREE_NODE *t = NULL;
	tree_insert(&t,5,0);
    tree_insert(&t,8,1);
	tree_insert(&t,4,2);
	tree_insert(&t,6,3);
	tree_insert(&t,11,0);
	tree_insert(&t,7,0);
	tree_insert(&t,10,0);
	tree_insert(&t,3,0);
	tree_insert(&t,9,0);
	tree_insert(&t,1,0);
	tree_insert(&t,2,0);
	tree_insert(&t,12,0);
	tree_print(&t);
	return 0;
}
