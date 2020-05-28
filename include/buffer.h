#ifndef __ARIES_BUFFER_H__
#define __ARIES_BUFFER_H__
#include "c.h"

#define DEFAULT_BUFFER_POOL_SIZE (1024*1024*1024) /*1GB*/
#define DEFAULT_PAGE_SIZE (8192) /*8K*/


/* buffer pool */
extern char *global_buffer_pool;
#endif
