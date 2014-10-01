/* cache.h - cache module interfaces */
#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stdint.h>
/*All self "typedef" */

/* cache replacement policy */
enum CACHE_POLICY {
	LRU,		/* replace least recently used block (perfect LRU) */
	RAMDOM,		/* replace a random block */
	FIFO		/* replace the oldest block in the set */
};
/* access hit or miss*/
enum HIT_MISS{
	READ_HIT,
	WRITE_HIT,
	WRITE_HIT_BACK,
	READ_MISS,
	WRITE_MISS
};
/* cache block (or line) definition */
struct CACHE_BLK_T
{
	bool 	 valid;
	bool 	 dirty;
	uint64_t tag;	/* data block tag value */
	uint64_t *data;	/* cache block valuem, may multi-word*/	
	//LRU Hardware unit
	uint32_t counter; 
};

/* cache definition */
struct CACHE_T
{
	//construtor,destructor
	CACHE_T(const char *name,int bsize,int assoc,int nsets,enum CACHE_POLICY policy);
	~CACHE_T();

	/* parameters */
	const char *name;   /* cache name */
	int bsize;			/* block size in bytes */
	int assoc;			/* cache associativity */
	int nsets;			/* number of sets */
	enum CACHE_POLICY policy;	/* cache replacement policy */
	/*segment bit*/
	uint64_t word_bits;
	uint64_t block_bits;
	uint64_t index_bits;
	uint64_t tag_bits;
	/*mask bits*/
	uint64_t tag_mask;
	/* per-cache stats */
	uint32_t hits;		/* total number of hits */
	uint32_t misses;		/* total number of misses */
	uint32_t replacements;	/* total number of replacements at misses */
	uint32_t writebacks;		/* total number of writebacks at misses */
	uint32_t invalidations;	/* total number of external invalidations */

	/* data blocks */
	struct CACHE_BLK_T *blks;			/* pointer to data blocks allocation */

	/* data access function*/
	HIT_MISS block_access(char cmd,uint64_t addr);
	/* miss/replacement handler*/
	CACHE_BLK_T*  choose_victim_blk(uint64_t in_index);
};

void dump_cache(CACHE_T *cp);	

#endif /* CACHE_H */
