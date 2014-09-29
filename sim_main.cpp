#include<stdio.h>
#include<iostream>
#include"cache.h"
using namespace std;

CACHE_T DL1("L1",64,512,1,LRU);//32KB,fully-associate,LRU
CACHE_T DL2("L2",64,16,256,LRU);
int main(int argc,char **argv)
{
	FILE *fp = fopen("input","r");
	char mem_cmd;
	uint64_t addr;
	while(!feof(fp)){
		fscanf(fp,"%c%lx\n",&mem_cmd,&addr);
		DL1.block_access(mem_cmd,addr);	
	}
	dump_cache(&DL1);		
	dump_cache(&DL2);
}
