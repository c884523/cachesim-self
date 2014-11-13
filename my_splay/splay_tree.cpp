#include "splay_tree.h"
#include<stdio.h>
using namespace std;

map<uint64_t,uint64_t> HASH_TABLE;//store last access time

TREE_NODE* tree_create(uint64_t i_time,uint64_t i_addr)
{
	TREE_NODE *tmp = new TREE_NODE;
	tmp->left  = NULL;
	tmp->right = NULL;
	tmp->parent= NULL;
	tmp->time  = i_time; 
	tmp->addr  = i_addr;
	tmp->size  = 1;
	return tmp;
}

uint64_t distance_compute(TREE_NODE **root,uint64_t f_time)
{
	TREE_NODE *cur = *root;
	uint64_t d = 0;
	while(true)
	{
		if(f_time > cur->time)
			cur = cur->right;
		else if(f_time < cur->time){
			d++;
			if(cur->right != NULL)
				d += cur->right->size;
			cur = cur->left;
		}
		else{
			if(cur->right != NULL)
				d += cur->right->size;
			//do tree delete op
			if((*root)->left != NULL || (*root)->right != NULL)
				*root = tree_rotate(&cur);
			tree_delete(root);//pass a tree
			return d;
		}
		
	}

}

void tree_insert(TREE_NODE **root,uint64_t i_time, uint64_t i_addr)
{
	TREE_NODE *new_node = tree_create(i_time, i_addr); 
	
	cout<<"new_node="<<new_node->time<<",&="<<new_node<<endl;
	//if tree is empty
	if(*root == NULL){
		*root = new_node;
		(*root)->parent = *root;//root's parent is itself
	}
	else{
		TREE_NODE *cur = *root;
		while(true){
			if( new_node->time > cur->time){
				if( cur->right != NULL)	//go right path	
					cur = cur->right;
				else{					//assign new to cur's rirght
					cur->right = new_node;
					new_node->parent = cur;
					*root = tree_rotate(&new_node);//adjust tree
					break;//insert is done
				}
			}
			else{
				if( cur->left != NULL)	//go left path	
					cur = cur->left;
				else{					//assign new to cur's left
					cur->left = new_node;
					new_node->parent = cur;
					*root = tree_rotate(&new_node);//adjust tree
					break;//insert is done
				}
			}
		}
	}
	return;			
}

void tree_delete(TREE_NODE **root)
{
	TREE_NODE *cur = *root;
	if(cur->left == NULL && cur->right == NULL){//tree only a root
		delete cur;
		*root = NULL;
	}
	else if(cur->left  != NULL){ //=====find left  subtree's max to be root=====
		TREE_NODE *tmp1 = cur->left;
		TREE_NODE *tmp2 = cur->left;
		while(tmp2->right != NULL)
			tmp2 = tmp2->right;
		//tmp2 == tmp1
		if(tmp2 == tmp1){
			tmp2->right = cur->right;
			if(tmp2->right != NULL)
				tmp2->right->parent = tmp2;
			tmp2->parent = tmp2;
		}
		//tmp2 != tmp1
		else{
			tmp1->parent = tmp1;//self root
			tmp2 = tree_rotate(&tmp2);
			/*if(tmp2->left == NULL)   //max is a leaf node
				tmp2->parent->right = NULL;
			else{					 //max has left subtree , should assign to max's parent
				tmp2->parent->right = tmp2->left;
				tmp2->left->parent = tmp2->parent;
			}*/
			//do delete
			//tmp2->left   = cur->left;
			tmp2->right  = cur->right;
			tmp2->parent = tmp2;
			//tmp2->left->parent = tmp2;
			if(cur->right != NULL)
				cur->right->parent = tmp2;
			delete cur;
		}
		*root = tmp2;
	}
	else if(cur->right != NULL){ //=====find right subtree's min to be root=====
		TREE_NODE *tmp1 = cur->right;
		TREE_NODE *tmp2 = cur->right;
		while(tmp2->left != NULL)
			tmp2 = tmp2->left;
		//tmp2 == tmp1
		if(tmp2 == tmp1){
			tmp2->left = cur->left;
			if(tmp2->left != NULL)
				cur->left->parent = tmp2;
			tmp2->parent = tmp2;
		}
		//tmp2 != tmp1
		else{
			tmp1->parent = tmp1;
			tmp2 = tree_rotate(&tmp2);
			/*if(tmp2->right == NULL)   //max is a leaf node
				tmp2->parent->left = NULL;
			else{					 //max has left subtree , should assign to max's parent
				tmp2->parent->left = tmp2->right;
				tmp2->right->parent = tmp2->parent;
			}*/
			//do delete
			tmp2->left   = cur->left;
			//tmp2->right  = cur->right;
			tmp2->parent = tmp2;
			//cur->right->parent = tmp2;
			if(cur->left != NULL)
				cur->left->parent = tmp2;
			delete cur;
		}
		*root = tmp2;
	}
}

TREE_NODE* tree_rotate(TREE_NODE **check_node){
	TREE_NODE *cur,*father,*grandpa;
	cur = *check_node;
	father  = cur->parent;
	grandpa = father->parent;
	
	//cout<<cur->time<<endl<<father->time<<endl<<grandpa->time<<endl;
	//do rotation
	while(true){
		//cout<<cur->time<<",";
		if(father == grandpa){	//father is root
			if(cur == father->left){	//zig
				//cout<<"zig"<<endl;
				father->left = cur->right;
				if(cur->right != NULL)
					cur->right->parent = father;
				cur->right   = father;
			}
			else{ 						//zag
				//cout<<"zag"<<endl;
				father->right = cur->left;
				if(cur->left != NULL)
					cur->left->parent = father;
				cur->left	  = father;
			}
			father->parent = cur;
			cur->parent    = cur;
			break;
		}
		else{					
			bool is_root = false;
			if(grandpa->parent == grandpa)//grandpa is root
				is_root = true;
			if(!is_root){//isn't root , so grandpa should be assgined first
				if(grandpa->parent->left == grandpa)
					grandpa->parent->left  = cur;
				else 
					grandpa->parent->right = cur;
				cur->parent                = grandpa->parent;//should first assign to cur 
			}
			//start to rotate
			if(cur == father->left && father == grandpa->left){//zig-zig
				//cout<<"zig-zig"<<endl;
				//subtree change
				grandpa->left             = father->right;
				if(father->right != NULL)
					father->right->parent = grandpa;
				father->left              = cur->right;
				if(cur->right != NULL)
					cur->right->parent    = father;
				//rotate
				father->right     = grandpa;
				grandpa->parent   = father; 
				cur->right        = father;
				father->parent    = cur;
			}
			else if(cur == father->right && father == grandpa->right){//zag-zag
				//cout<<"zag-zag"<<endl;
				//subtree change
				grandpa->right           = father->left;
				if(father->left != NULL)
					father->left->parent = grandpa;
				father->right            = cur->left;
				if(cur->left != NULL)
					cur->left->parent    = father;
				//rotate
				father->left      = grandpa;
				grandpa->parent   = father; 
				cur->left         = father;
				father->parent    = cur;
			}
			else if(cur == father->right && father == grandpa->left){//zig-zag
				//cout<<"zig-zag"<<endl;
				//subtree change
				grandpa->left         = cur->right;
				if(cur->right != NULL)
					cur->right->parent= grandpa;
				father->right         = cur->left;
				if(cur->left != NULL)
					cur->left->parent = father;
				//rotate
				cur->left         = father;
				cur->right        = grandpa;
				grandpa->parent   = cur; 
				father->parent    = cur;
				
			}
			else if(cur == father->left && father == grandpa->right){//zag-zig
				//cout<<"zag-zig"<<endl;
				//subtree change
				grandpa->right         = cur->left;
				if(cur->left != NULL)
					cur->left->parent  = grandpa;
				father->left           = cur->right;
				if(cur->right != NULL)
					cur->right->parent = father;
				//rotate
				cur->left         = grandpa;
				cur->right        = father;
				grandpa->parent   = cur; 
				father->parent    = cur;
			}
			//if grandpa is root , break
			if(is_root){
				cur->parent = cur;
				break;
			}
			//assign new father,grandpa
			father  = cur->parent;
			grandpa = father->parent;
		}
	}
	return cur;
}

int level=1;
void tree_print(TREE_NODE **t)
{
	if(*t != NULL)
	{
		level++;
		tree_print(&((*t)->left));
		cout<<"key="<<(*t)->time<<",level="<<level<<endl;
		level++;
		tree_print(&((*t)->right));
	}
	level--;
}
