#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include"cache.h"
using namespace std;
/*block size, n-ways, n-sets*/
int main(int argc,char **argv)
{
	//Analyze the argv
	int LINE_SIZE	= 64;
	int L1_WAY 		= atoi(argv[2]);
	int L1_SET 		= atoi(argv[3]);
	FILE *fp = fopen(argv[1],"r");
	//Cache Create
	CACHE_T DL1("L1",LINE_SIZE,L1_WAY,L1_SET,LRU);//L1 Data Cache
	char mem_cmd;
	uint64_t addr;
	HIT_MISS L1H_M;
	while(fscanf(fp,"%c%lx\n",&mem_cmd,&addr) != EOF)
	{
		L1H_M = DL1.block_access(mem_cmd,addr);
	}
	
	//sim_result
	char result_oname[64];
	sprintf(result_oname,"L1_sim_result_%d_%d",L1_WAY,L1_SET);
	FILE *rfp = fopen(result_oname,"w");
	fprintf(rfp,"%d\n",DL1.hits);	
	fprintf(rfp,"%d\n",DL1.misses);	
	fprintf(rfp,"%.3f\n",100*(float)DL1.hits/(float)(DL1.hits+DL1.misses));
	fprintf(rfp,"%.3f\n",100*(float)DL1.misses/(float)(DL1.hits+DL1.misses));
	//dump info.	
	dump_cache(&DL1);		
}
