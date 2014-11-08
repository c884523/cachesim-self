/* cache.c - cache module routines */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "cache.h"



/* create and initialize a general cache structure */
CACHE_T::CACHE_T(const char *name,int bsize,int assoc,int nsets,enum CACHE_POLICY policy)
{
	//cache format
	this->name	= name;
	this->bsize	= bsize;
	this->assoc	= assoc;
	this->nsets   = nsets;
	this->policy  = policy;
	//segment bits
	this->block_bits = log2(bsize);
	this->index_bits = log2(nsets);
	this->tag_bits   = 64-(this->block_bits + this->index_bits);
	//mask
	if(tag_bits == 64)
		this->tag_mask = 0;
	else
		this->tag_mask = (0xffffffffffffffff<<tag_bits)>>tag_bits;
	//per cache stat
	this->hits   = 0;
	this->misses = 0;
	this->writebacks    = 0;
	//cache block array
	this->blks = (CACHE_BLK_T*) malloc(sizeof(CACHE_BLK_T)*assoc*nsets);	
	for(int i=0 ; i < assoc*nsets ; i++){
		this->blks[i].valid = false;
		this->blks[i].dirty = false;
		this->blks[i].tag   = 0;
		this->blks[i].counter = 0;
	}
}
CACHE_T::~CACHE_T()
{
	free(blks);
}
/* access block function*/
HIT_MISS
CACHE_T::block_access(char cmd, uint64_t addr)
{
//===============decode the addr===============//
	uint64_t in_tag   = addr>>(64-tag_bits);
	uint64_t in_set   = (addr&tag_mask) >> block_bits;
	uint64_t in_index = in_set * assoc;
//===============search the block==============//
	bool is_find = false;
	//READ blcok	
	if(cmd == 'R'){
		for(int i=0; i < assoc ;i++,in_index++)
		{
			if(	blks[in_index].valid == true && 
			  	blks[in_index].tag == in_tag)//tag is the same & valid
			{
				is_find = true;
				blks[in_index].counter = 0;//go stack top
			}
			else 
				blks[in_index].counter++;//LRU counter+1
		}
		if(is_find){
			hits++;
			return READ_HIT;
		}
		//no valid & no same tag
		int i_victim_blk = choose_victim_blk(in_set*assoc);
		if(blks[i_victim_blk].dirty == true)  /*should do write back*/
			writebacks++;
		blks[i_victim_blk].tag     = in_tag;  /*update newest data*/
		blks[i_victim_blk].valid   = true;
		blks[i_victim_blk].dirty   = false;
		blks[i_victim_blk].counter = 0;
		misses++;
		return READ_MISS;
	}
	else{//WRITE block
		bool BACK;
		for(int i=0; i < assoc ;i++,in_index++)
		{
			if(	blks[in_index].valid == true && 
				blks[in_index].tag == in_tag)//tag is the same & valid
			{
				is_find = true;
				blks[in_index].counter = 0;//go stack top
				blks[in_index].dirty = true;
				/*if(blks[in_index].dirty == false){//no write back
					blks[in_index].dirty = true;
					BACK = false;
				}
				else{
					writebacks++;//writeback count + 1
					BACK = true;;
				}*/
			}
			else 
				blks[in_index].counter++;//LRU counter+1
		}
		if(is_find){
			hits++;
			return WRITE_HIT;	
			//if(!BACK)	return WRITE_HIT;
			//else 		return WRITE_HIT_BACK;
		}
		//no valid & no same tag
		int i_victim_blk = choose_victim_blk(in_set*assoc);
		if(blks[i_victim_blk].dirty == true)  /*should do write back*/
			writebacks++;
		blks[i_victim_blk].tag     = in_tag;  /*update newest data*/
		blks[i_victim_blk].valid   = true;
		blks[i_victim_blk].dirty   = true;	 /*write allocate,so also dirty*/
		blks[i_victim_blk].counter = 0;
		misses++;
		return WRITE_MISS;
	}
}


/*choose a victim by replacement policy*/
int CACHE_T::choose_victim_blk(uint64_t in_index)
{
	int out_index = in_index;
	switch(policy){
//================LRU policy implementation============//
		case LRU:{
			int max=0;
			for(int i = 0; i<assoc ; i++,in_index++){
				if(blks[in_index].valid == false){   /*invalid is first choose*/
					out_index = in_index;
					break;
				}
				else{ 							     /*valid, choose the max count*/
					if(blks[in_index].counter > max){/*if current blk's counter > max, record the index & update max*/
						max = blks[in_index].counter;
						out_index = in_index;
					}
				}	
			}
		}
		break;
//================FIFO policy implementation==========//
		case FIFO:{
			static int out_index=0;
			out_index = (out_index+1) % assoc;
		}
		break;
	}
	return out_index;
}

/*print the cache information*/
void dump_cache(CACHE_T *cp)
{
	printf("===========================\n");
	printf("%s cache:                  |\n",cp->name);
	printf("============================\n");
	printf("block size  = %-6dBytes  |\n",cp->bsize);
	printf("ways        = %-6d	   |\n",cp->assoc);
	printf("set numbers = %-6d       |\n",cp->nsets);
	printf("total size  = %-6.3fKB     |\n",float(cp->bsize*cp->assoc*cp->nsets)/1024);
	if(cp->policy == LRU)
		printf("policy      = LRU          |\n");
	else if(cp->policy == FIFO)
		printf("policy      = FIFO         |\n");
	printf("----------------------------\n");
	//segment bits
	printf("block_btis = %-2lu            |\n",cp->block_bits);
	printf("index_btis = %-2lu            |\n",cp->index_bits);
	printf("tag_btis   = %-2lu            |\n",cp->tag_bits);
	printf("----------------------------\n");
	//per cache stat
	printf("hits         = %-10d  |\n",cp->hits);
	printf("misses       = %-10d  |\n",cp->misses);
	printf("writebacks   = %-10d  |\n",cp->writebacks);
	printf("hit rate = %-10.3f      |\n",100*(float)cp->hits/(float)(cp->hits+cp->misses));
	printf("miss rate = %-10.3f     |\n",100*(float)cp->misses/(float)(cp->hits+cp->misses));
	printf("----------------------------\n\n\n");
}
