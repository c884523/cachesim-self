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
	this->word_bits  = 3;
	this->block_bits = log2(bsize/sizeof(uint64_t));
	this->index_bits = log2(nsets);
	this->tag_bits   = 64-(this->word_bits + this->block_bits + this->index_bits);
	//mask
	this->tag_mask   = (0xffffffffffffffff<<tag_bits)>>tag_bits;
	//per cache stat
	this->hits   = 0;
	this->misses = 0;
	this->replacements  = 0;
	this->writebacks    = 0;
	this->invalidations = 0;
	//cache block array
	this->blks = (CACHE_BLK_T*) malloc(sizeof(CACHE_BLK_T)*assoc*nsets);	
	for(int i=0 ; i < assoc*nsets ; i++){
		this->blks[i].valid = false;
		this->blks[i].dirty = false;
		this->blks[i].tag   = 0;
		this->blks[i].data  = (uint64_t*) calloc(bsize/sizeof(uint64_t), sizeof(uint64_t));
		this->blks[i].counter = 0;
	}
}
CACHE_T::~CACHE_T()
{
	for(int i=0 ; i < assoc*nsets; i++)
		free(blks[i].data);	
	free(blks);
}
/* access block function*/
HIT_MISS
CACHE_T::block_access(char cmd, uint64_t addr)
{
	/*decode the addr*/
	uint64_t in_tag   = addr>>(64-tag_bits);
	uint64_t in_set   = addr&tag_mask >> (word_bits+block_bits);	
	uint64_t in_index = in_set * assoc;
	/*search the block*/
	//READ blcok	
	if(cmd == 'R'){
		for(int i=0; i < assoc ;i++,in_index++)
		{
			if(blks[in_index].valid == true && 
					blks[in_index].tag == in_tag)//tag is the same & valid
			{
				blks[in_index].counter = 0;//go stack top
				hits++;
				return READ_HIT;
			}
		}
		//no valid & no same tag
		CACHE_BLK_T *victim_blk = choose_victim_blk(in_set*assoc);
		victim_blk->tag = in_tag;  /*update newest data*/
		victim_blk->valid = true;
		victim_blk->dirty = false;
		victim_blk->counter = 0;
		misses++;
		return READ_MISS;
	}
	else{//WRITE block
		for(int i=0; i < assoc ;i++,in_index++)
		{
			if(blks[in_index].valid == true && 
					blks[in_index].tag == in_tag)//tag is the same & valid
			{
				blks[in_index].counter = 0;//go stack top
				hits++;
				if(blks[in_index].dirty == false){//no write back
					blks[in_index].dirty = true;
					return WRITE_HIT;
				}
				else{
					blks[in_index].dirty = true;
					return WRITE_HIT_BACK;
				}
			}
		}
		//no valid & no same tag
		CACHE_BLK_T *victim_blk = choose_victim_blk(in_set*assoc);
		victim_blk->tag = in_tag;  /*update newest data*/
		victim_blk->valid = true;
		victim_blk->dirty = true;  /*write allocate,so also dirty*/
		victim_blk->counter = 0;
		misses++;
		return WRITE_MISS;
	}
}


/*choose a victim by replacement policy*/
CACHE_BLK_T* 
CACHE_T::choose_victim_blk(uint64_t in_index)
{
	switch(policy){
		case LRU:{
			int max=0,out_index=in_index;
			for(int i=0; i<assoc ; i++,in_index++){
				if(blks[in_index].valid == false){   /*invalid is first choose*/
					out_index = in_index;
					break;
				}
				else{ 							     /*valid, choose the max count*/
					if(blks[in_index].counter > max){/*if current blk's counter > max, record the index & update max*/
						max = blks[in_index].counter;
						out_index = in_index;
					}
					blks[in_index].counter++;        /*if <= max , current counter+1*/
				}	
			}
			return &blks[out_index];
		}
		break;
	}
}

/*print the cache information*/
void dump_cache(CACHE_T *cp)
{
	printf("============================\n");
	printf("%s cache:                  |\n",cp->name);
	printf("============================\n");
	printf("block size  = %-6dBytes  |\n",cp->bsize);
	printf("ways        = %-6d	   |\n",cp->assoc);
	printf("set numbers = %-6d       |\n",cp->nsets);
	printf("total size  = %-6dKB     |\n",(cp->bsize*cp->assoc*cp->nsets)/1024);
	printf("policy      = LRU          |\n");
	printf("----------------------------\n");
	//segment bits
	printf("word_btis  = %-2lu           |\n",cp->word_bits);
	printf("block_btis = %-2lu           |\n",cp->block_bits);
	printf("index_btis = %-2lu           |\n",cp->index_bits);
	printf("tag_btis   = %-2lu           |\n",cp->tag_bits);
	printf("----------------------------\n");
	//per cache stat
	printf("hits         = %-10d  |\n",cp->hits);
	printf("misses       = %-10d  |\n",cp->misses);
	printf("hit rate = %-10.3f      |\n",100*(float)cp->hits/(float)(cp->hits+cp->misses));
	printf("miss rate = %-10.3f     |\n",100*(float)cp->misses/(float)(cp->hits+cp->misses));
	//printf("replacements = %-10d  |\n",cp->replacements);
	//printf("writebacks   = %-10d  |\n",cp->writebacks);
	//printf("invalidations= %-10d  |\n",cp->invalidations);
	printf("----------------------------\n\n\n");
}
