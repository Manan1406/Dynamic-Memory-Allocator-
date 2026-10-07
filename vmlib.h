/**
 * CSE 29: Systems Programming and Software Tools
 * Spring 2025
 * Programming Assignment 3
 *
 * vmlib.h
 * This header file defines the *public* interface for our memory allocation
 * system.
 *
 * Author: Jerry Yu <jiy066@ucsd.edu>
 * February 2023, Updated May 2024
 */

#ifndef VMLIB_H
#define VMLIB_H

#include <stddef.h>

/* Initializes an empty virtual heap */
int vminit(size_t sz);
/* Destroy the initialized virtual heap */
void vmdestroy();

/* For you to implement in vmalloc.c and vmfree.c */
void *vmalloc(size_t size);
void vmfree(void *ptr);

/* Print out the heap structure */
void vminfo();
/* Dump the heap into a file */
void vmdump(const char *filename);
/* Load a heap from a dump file */
int vmload(const char *filename);

#endif /* VMLIB_H */
