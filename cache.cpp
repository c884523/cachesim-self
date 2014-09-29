/* cache.c - cache module routines */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "cache.h"



/* create and initialize a general cache structure */
CACHE_T::CACHE_T(char *name,int bsize,int assoc,int nsets,enum CACHE_POLICY policy)
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
	//per cache stat
	this->hits = 0;
	this->misses=0;
	this->replacements=0;
	this->writebacks = 0;
	this->invalidations=0;
	//cache block array
	this->blks = (CACHE_BLK_T*) malloc(sizeof(CACHE_BLK_T)*assoc*nsets);	
	for(int i=0 ; i < assoc*nsets ; i++){
		this->blks[i].valid = false;
		this->blks[i].dirty = false;
		this->blks[i].tag   = 0;
		this->blks[i].data  = (uint64_t*) calloc(bsize/sizeof(uint64_t), sizeof(uint64_t));
	}
}
/* access block function*/
HIT_MISS
CACHE_T::block_access(enum MEM_CMD cmd, uint64_t addr)
{
	/*decode the addr*/
	uint64_t in_tag   = addr>>(64-tag_bits);
	uint64_t in_set   = (addr<<tag_bits)>>(word_bits+block_bits);	
	uint64_t in_index = in_set * assoc;
	
	/*check hit or miss*/
	if(blks[in_index].valid == false)//invlaid
	{
		misses++;
		if(cmd == READ)
			return READ_MISS;
		else 
			return WRITE_MISS;
	}	
	else//valid
	{
		if(blks[in_index].tag != in_tag){//tag not the same
			misses++;
			if(cmd == READ)
				return READ_MISS;
			else 
				return WRITE_MISS;
		}
		else{//tag is the same
			hits++;
			if(cmd == READ)
				return READ_HIT;
			else{
				if(blks[in_index].dirty == false){//no write back
					blks[in_index].dirty = true;
					return WRITE_HIT;			
				}
				else
					return WRITE_HIT_BACK;//should write back
			}
		}	
	}

}
