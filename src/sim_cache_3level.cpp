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
	int L2_WAY 		= atoi(argv[4]);
	int L2_SET		= atoi(argv[5]);
	int L3_WAY 		= atoi(argv[6]);
	int L3_SET		= atoi(argv[7]);
	FILE *fp = fopen(argv[1],"r");
	//Cache Create
	CACHE_T DL1("L1",LINE_SIZE,L1_WAY,L1_SET,LRU);//L1 Data Cache
	CACHE_T DL2("L2",LINE_SIZE,L2_WAY,L2_SET,LRU);//L2 Data Cache
	CACHE_T DL3("L3",LINE_SIZE,L3_WAY,L3_SET,LRU);//L2 Data Cache
	//FILE *fp = fopen("input/seqstrace","r");
	char mem_cmd;
	uint64_t addr;
	HIT_MISS L1H_M,L2H_M,L3H_M;
	while(fscanf(fp,"%c%lx\n",&mem_cmd,&addr) != EOF)
	{
		L1H_M = DL1.block_access(mem_cmd,addr);
		switch(L1H_M){
			case READ_MISS: 	//sould READ level-2
				L2H_M = DL2.block_access('R',addr);	
				switch(L2H_M){
					case READ_MISS:
						L3H_M = DL3.block_access('R',addr);
						break;
					case WRITE_MISS:
						L3H_M = DL3.block_access('R',addr);
						break;
				}
				break;
			case WRITE_MISS:	//sould READ level-2
				L2H_M = DL2.block_access('R',addr);	
				switch(L2H_M){
					case READ_MISS:
						L3H_M = DL3.block_access('R',addr);
						break;
					case WRITE_MISS:
						L3H_M = DL3.block_access('R',addr);
						break;
				}
				break;
		}
	}
	
	//sim_result
	char result_oname[64];
	sprintf(result_oname,"L3_sim_result_%d_%d_%d_%d_%d_%d",L1_WAY,L1_SET,L2_WAY,L2_SET,L3_WAY,L3_SET);
	FILE *rfp = fopen(result_oname,"w");
	fprintf(rfp,"%d\n",DL1.hits);	
	fprintf(rfp,"%d\n",DL1.misses);	
	fprintf(rfp,"%.3f\n",100*(float)DL1.hits/(float)(DL1.hits+DL1.misses));
	fprintf(rfp,"%.3f\n",100*(float)DL1.misses/(float)(DL1.hits+DL1.misses));
	fprintf(rfp,"%d\n",DL2.hits);	
	fprintf(rfp,"%d\n",DL2.misses);	
	fprintf(rfp,"%.3f\n",100*(float)DL2.hits/(float)(DL2.hits+DL2.misses));
	fprintf(rfp,"%.3f\n",100*(float)DL2.misses/(float)(DL2.hits+DL2.misses));
	fprintf(rfp,"%d\n",DL3.hits);	
	fprintf(rfp,"%d\n",DL3.misses);	
	fprintf(rfp,"%.3f\n",100*(float)DL3.hits/(float)(DL3.hits+DL3.misses));
	fprintf(rfp,"%.3f\n",100*(float)DL3.misses/(float)(DL3.hits+DL3.misses));
	//dump info.	
	dump_cache(&DL1);		
	dump_cache(&DL2);
	dump_cache(&DL3);
}
