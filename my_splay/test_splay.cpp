#include <iostream>
#include "splay_tree.h"
using namespace std;

int main(void)
{
	TREE_NODE *t = NULL;
	tree_insert(&t,2,35);
    tree_insert(&t,0,6);
	tree_insert(&t,1,11);
	tree_insert(&t,3,97);
	tree_insert(&t,4,46);
	tree_print(&t);
	return 0;
}
