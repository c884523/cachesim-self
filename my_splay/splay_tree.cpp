#include "splay_tree.h"
#include<stdio.h>
using namespace std;

uint64_t my_timer;
map<uint64_t,uint64_t> HASH_TABLE;//store last access time

TREE_NODE* tree_create(uint64_t i_time,uint64_t i_addr)
{
	TREE_NODE *tmp = new TREE_NODE;
	tmp->left  = NULL;
	tmp->right = NULL;
	tmp->parent= NULL;
	tmp->time  = i_time; 
	tmp->addr  = i_addr;
	tmp->size  = 0;
	return tmp;
}

void tree_insert(TREE_NODE **root,uint64_t i_time, uint64_t i_addr)
{
	TREE_NODE *new_node = tree_create(i_time, i_addr); 
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

void tree_free(TREE_NODE *t)
{
	if(t==NULL) return;
    tree_free(t->right);
    tree_free(t->left);
    tree_free(t);
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
