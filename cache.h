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
/* memory comand*/
enum MEM_CMD{
	READ,
	WRITE
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
};

/* cache definition */
struct CACHE_T
{
	//construtor,destructor
	CACHE_T(char *name,int bsize,int assoc,int nsets,enum CACHE_POLICY policy);
	~CACHE_T();

	/* parameters */
	char *name;			/* cache name */
	int bsize;			/* block size in bytes */
	int assoc;			/* cache associativity */
	int nsets;			/* number of sets */
	enum CACHE_POLICY policy;	/* cache replacement policy */
	/*segment bit*/
	int word_bits;
	int block_bits;
	int index_bits;
	int tag_bits;
	/* per-cache stats */
	uint32_t hits;		/* total number of hits */
	uint32_t misses;		/* total number of misses */
	uint32_t replacements;	/* total number of replacements at misses */
	uint32_t writebacks;		/* total number of writebacks at misses */
	uint32_t invalidations;	/* total number of external invalidations */

	/* data blocks */
	struct CACHE_BLK_T *blks;			/* pointer to data blocks allocation */

	/* miss/replacement handler*/
	HIT_MISS block_access(enum MEM_CMD cmd,uint64_t addr);
};


/* access a cache, perform a CMD operation on cache CP*/
void 
cache_access(struct CACHE_T *cp,	/* cache to access */
		enum MEM_CMD cmd,		/* access type, Read or Write */
		uint64_t addr);		/* address of access */

#endif /* CACHE_H */
