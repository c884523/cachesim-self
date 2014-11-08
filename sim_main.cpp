#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include"cache.h"
using namespace std;
/*block size, n-ways, n-sets*/
int main(int argc,char **argv)
{
	//Analyze the argv
	int LINE_SIZE	= 1;
	int L1_WAY 		= atoi(argv[2]);
	int L1_SET 		= atoi(argv[3]);
	int L2_WAY 		= atoi(argv[4]);
	int L2_SET		= atoi(argv[5]);
	//FILE *fp = fopen("nput/cpu2006_int/429.mcf/output/mcf_traceaa","r");
	char l2_oname[30];
	sprintf(l2_oname,"L2_trace_%d",L2_WAY);
	FILE *fp = fopen(argv[1],"r");
	FILE *l2_fp=fopen(l2_oname,"w");
	//Cache Create
	CACHE_T DL1("L1",LINE_SIZE,L1_WAY,L1_SET,LRU);//L1 Data Cache
	CACHE_T DL2("L2",LINE_SIZE,L2_WAY,L2_SET,LRU);//L2 Data Cache
	//FILE *fp = fopen("input/seqstrace","r");
	char mem_cmd;
	uint64_t addr;
	HIT_MISS L1H_M,L2H_M;
	while(fscanf(fp,"%c%lx\n",&mem_cmd,&addr) != EOF)
	{
		L1H_M = DL1.block_access(mem_cmd,addr);
		switch(L1H_M){
			case READ_MISS: 	//sould READ level-2
				fprintf(l2_fp,"R %lx\n",addr);
				L2H_M = DL2.block_access('R',addr);	
				break;
			case WRITE_MISS:	//sould READ level-2
				fprintf(l2_fp,"R %lx\n",addr);
				L2H_M = DL2.block_access('R',addr);	
				break;
			case WRITE_HIT_BACK://sould WRITE leve-2
				L2H_M = DL2.block_access('W',addr);	
				break;
		}
	}
	dump_cache(&DL1);		
	dump_cache(&DL2);
}
