/* cache.h - cache module interfaces */
#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <sys/types.h>
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

/* block status values */
#define CACHE_BLK_VALID		0x00000001	/* block in valid, in use */
#define CACHE_BLK_DIRTY		0x00000002	/* dirty block */

/* cache block (or line) definition */
struct CACHE_BLK_T
{
	bool 	 valid;
	bool 	 dirty;
	uint64_t tag;	/* data block tag value */
	uint8_t  *data;	/* cache block valuem, may multi-word*/	
};

/* cache definition */
struct CACHE_T
{
	/* parameters */
	char *name;			/* cache name */
	int bsize;			/* block size in bytes */
	int assoc;			/* cache associativity */
	int nsets;			/* number of sets */
	enum CACHE_POLICY policy;	/* cache replacement policy */

	/* miss/replacement handler*/
	unsigned int					/* latency of block access */
		(*blk_access_fn)(enum mem_cmd cmd,		/* block access command */
				uint64_t baddr,		/* program address to access */
				uint64_t bsize,			/* size of the cache block */
				struct CACHE_BLK_T *blk)	/* ptr to cache block struct */

	/* derived data, for fast decoding */
	uint64_t blk_mask;
	int set_shift;
	uint64_t set_mask;		/* use *after* shift */
	int tag_shift;
	uint64_t tag_mask;		/* use *after* shift */
	uint64_t tagset_mask;	/* used for fast hit detection */

	/* per-cache stats */
	uint32_t hits;		/* total number of hits */
	uint32_t misses;		/* total number of misses */
	uint32_t replacements;	/* total number of replacements at misses */
	uint32_t writebacks;		/* total number of writebacks at misses */
	uint32_t invalidations;	/* total number of external invalidations */
	/* data blocks */
	struct CACHE_BLK_T *blks;			/* pointer to data blocks allocation */
};

/* create and initialize a general cache structure */
struct cache_t *			/* pointer to cache created */
cache_create(char *name,		/* name of the cache */
		int nsets,			/* total number of sets in cache */
		int bsize,			/* block (line) size of cache */
		int assoc,			/* associativity of cache */
		enum CACHE_POLICY policy,	/* replacement policy w/in sets */
		/* block access function, see description w/in struct cache def */
		unsigned int (*blk_access_fn)(enum MEM_CMD cmd,
			uint64_t baddr, int bsize,
			struct CACHE_BLK_T *blk);

/* access a cache, perform a CMD operation on cache CP*/
unsigned int				/* latency of access in cycles */
cache_access(struct CACHE_T *cp,	/* cache to access */
		enum MEM_CMD cmd,		/* access type, Read or Write */
		uint64_t addr);		/* address of access */

#endif /* CACHE_H */
