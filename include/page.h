#ifndef __ARIES_PAGE_H__
#define __ARIES_PAGE_H__

#include "c.h"

struct Page{
	uint32 space_id;
	uint32 page_no; /* page id */
	uint8  page_type; /* page type */
	pthread_mutex_t mutex;
}Page;

#endif

