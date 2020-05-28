#include "buffer.h"

char *global_buffer_pool = NULL;

void
init_buffer_pool()
{

	global_buffer_pool = (char*)mmap(NULL, DEFAULT_BUFFER_POOL_SIZE, 
					PROT_READ | PROT_WRITE,
					MAP_SHARED | MAP_ANONYMOUS,-1,0);


	if(NULL){
		printf("%s\n","cannot alloc buffer pool");
	}
}
