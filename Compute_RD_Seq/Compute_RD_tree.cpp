#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<iostream>
#include<map>
#include"splay_tree.h"
using namespace std;

map<uint64_t,uint64_t> HASH_TABLE;//store last access time
uint64_t now_t = 1;
int main(int argc,char*argv[])
{
	FILE *fp = fopen(argv[1],"r");
	if(!fp){
		printf("開檔失敗\n");
		exit(1);
	}
	//==========Variable section==========
	TREE_NODE *tree = NULL;//splay tree
	map<uint64_t,uint64_t> ReuseDistance;
	uint64_t m_Address;
	uint64_t infinite_dist=0,max_dist=0; 	
	char c;
	//==========Compute reuse distance==========
	while(fscanf(fp,"%c%lx\n",&c,&m_Address) != EOF ){
		//counts++;
		//printf("%c %lx\n",c,m_Address);//pipe file for monitor 
		uint64_t dist=0;
		uint64_t last_t = HASH_TABLE[m_Address];
		//dist = infinite 
		if( last_t == 0 ){
			infinite_dist++;
			tree_insert(&tree,now_t,m_Address);
		}
		else{ 
			dist = distance_compute(&tree,last_t);
			tree_insert(&tree,now_t,m_Address);
			ReuseDistance[dist]++;
			if( dist > max_dist)
				max_dist = dist;
		}
		HASH_TABLE[m_Address] = now_t;
		now_t++;
	}
	fclose(fp);
	//==========Output the result to file==========
	char oname[30];
	if( argv[2][1] == '1')	//L1's trace file
		sprintf(oname,"reuse_dist_1");
	else 				  	//L2's trace file
		sprintf(oname,"reuse_dist_2_%s",argv[3]);
	fp = fopen(oname,"w");
	for(uint64_t i= 0; i <= max_dist ; i++)
		fprintf(fp,"%-6lu %-6lu\n", i, ReuseDistance[i]);
	fprintf(fp,"-1 %-6lu\n", infinite_dist);
}
