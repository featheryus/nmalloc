/*
 * =====================================================================================
 *
 *       Filename:  nmalloc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/05/2014 01:44:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Su Yu ()
 *        Company:  N
 *
 * =====================================================================================
 */

#include "nmalloc.h"

// put the nmalloc info to data area, don't care the space, because we are testing.
nmalloc_info_t   nmalloc_info[NMALLOC_TOTAL]; 
unsigned long pagealign = 0;
unsigned int pagesize = 0;
unsigned int pointer_align = 0;
int nmalloc_index = 0;
char* nmalloc(int size)
{
    static int label = 0;
    static char * heap = 0;
    int mod = 0;
    int padding = 0;
    char * ptr = 0;
    char * ptr_r = 0;
    int index = 0;

    if(0 == label) {
        pagesize = sysconf(_SC_PAGE_SIZE);
        pagealign = ((unsigned long)0-1)<<12;
        pointer_align = ((unsigned int)0-1)<<4;
        label = 1;
        //TODO : forget it, make the prb busy is useful during the test. 
        //       this is only for test. so just use array, not hash table.
        //mem_hash = malloc(hash_size);
    }

    ptr = malloc(size+2*pagesize);
    if(NULL == ptr) {
        handle_error("malloc failed");
    }
    mod = ((long)ptr+size) & 0xFFF;
    if(mod != 0x0) {
        padding = pagesize - mod;
    }
    heap = (char*)((long)(ptr + size + padding) & pagealign);
    if (mprotect(heap, pagesize,PROT_READ) == -1) {
        handle_error("mprotect");
    }
    padding = padding & pointer_align ;
    ptr_r = ptr + padding;

    while (index < NMALLOC_TOTAL) {
        if(0 == nmalloc_info[nmalloc_index].ptr) {
            nmalloc_info[nmalloc_index].ptr = ptr;
            nmalloc_info[nmalloc_index].heap = heap;
            nmalloc_info[nmalloc_index].size = size;
            nmalloc_info[nmalloc_index].mem_ret = ptr_r;
            nmalloc_index++;
            nmalloc_index%=NMALLOC_TOTAL;
            index = 0;
            break;
        }
        else {
            nmalloc_index++;
            nmalloc_index%=NMALLOC_TOTAL;
            index++;
        }
    }
    if(index >= NMALLOC_TOTAL) {
        handle_error("nmalloc failed");
    }

    return ptr_r;
}
inline int get_real_ptr(char *ptr)
{
    int i = -1;
    char * heap;

    for(i = 0; i < NMALLOC_TOTAL; i++) {
       if(nmalloc_info[i].mem_ret == ptr) {
           heap = nmalloc_info[i].heap;
           if (mprotect(heap, pagesize,PROT_READ|PROT_WRITE) == -1) {
                    handle_error("mprotect");
           }
           return i;
       } 
    }
    return -1;
}

void nfree(char *ptr)
{
    char * real_ptr = NULL;
    int index = 0;

    index = get_real_ptr(ptr);
    if(index >= 0) {
    	real_ptr = nmalloc_info[index].ptr;
	    free(real_ptr);
	    memset(&nmalloc_info[index], 0x0, sizeof(nmalloc_info_t));
    }
    else {
        handle_error("double free?");
    }
}

