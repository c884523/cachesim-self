#include <iostream>
#include "splay_tree.h"
using namespace std;

int main(void)
{
	TREE_NODE *t = NULL;
    tree_insert(&t,8,1);
	tree_insert(&t,4,2);
	tree_insert(&t,5,0);
	tree_insert(&t,6,3);
	tree_insert(&t,11,0);
	tree_insert(&t,7,0);
	tree_insert(&t,10,0);
	tree_insert(&t,3,0);
	tree_insert(&t,9,0);
	tree_insert(&t,1,0);
	tree_insert(&t,2,0);
	/*distance_compute(&t,7);
	distance_compute(&t,3);
	distance_compute(&t,10);
	distance_compute(&t,6);
	distance_compute(&t,5);
	distance_compute(&t,9);
	distance_compute(&t,11);
	distance_compute(&t,4);
	distance_compute(&t,2);
	distance_compute(&t,1);
	distance_compute(&t,8);*/
	tree_print(&t);
	tree_free(&t);
	return 0;
}
