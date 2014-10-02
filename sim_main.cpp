#include<stdio.h>
#include<iostream>
#include"cache.h"
using namespace std;
/*block size, n-ways, n-sets*/
CACHE_T DL1("L1",64,L1_W,1,LRU);//32KB,fully-associate,LRU
#if SET_L2==1
CACHE_T DL2("L2",64,L2_W,1,LRU);//256KB,fully-associate,LRU
#endif
int main(int argc,char **argv)
{
	//FILE *fp = fopen("nput/cpu2006_int/429.mcf/output/mcf_traceaa","r");
	FILE *fp = fopen(argv[1],"r");
	//FILE *fp = fopen("input/seqstrace","r");
	char mem_cmd;
	uint64_t addr;
	HIT_MISS L1H_M,L2H_M;

	while(!feof(fp)){
		fscanf(fp,"%c%lx\n",&mem_cmd,&addr);
		L1H_M = DL1.block_access(mem_cmd,addr);
	
		#if SET_L2==1
		switch(L1H_M){
			case READ_MISS: 	//READ level-2
				L2H_M = DL2.block_access('R',addr);	
				break;
			case WRITE_MISS:	//READ level-2
				L2H_M = DL2.block_access('R',addr);	
				break;
			case WRITE_HIT_BACK://WRITE leve-2
				L2H_M = DL2.block_access('W',addr);	
				break;
		}
		#endif
	}
	dump_cache(&DL1);		
	#if SET_L2==1
	dump_cache(&DL2);
	#endif
}
