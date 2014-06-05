/*
 * =====================================================================================
 *
 *       Filename:  nmalloc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/05/2014 01:46:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Su Yu ()
 *        Company:  N
 *
 * =====================================================================================
 */
#ifndef nmalloch
#define nmalloch
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

char* nmalloc(int size);
void nfree(char *ptr);
inline int get_page_num(char* ptr);

typedef struct nmalloc_info_t {
    char * ptr;
    char * heap;
    char * mem_ret;
    unsigned int  size;
}nmalloc_info_t;
#define NMALLOC_TOTAL 4096
//TODO: modify it according to your system
#define handle_error(msg) \
           do {printf("%s", msg);*(int*)0=1; } while (0)
#endif
