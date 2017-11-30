/* memory.h
 * Jason Iskenderian and Alexa Bosworth
 * 11/12/17
 * Purpose: Provides interface function headers and descriptions for the memory
 * module
 */

#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED



#include "memory.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bitpack.h>
#include "stack.h"
#include <stdint.h>

 typedef struct Memory_T {
        Stack_T available;
        uint32_t **segments;
        unsigned num_segs;
}*Memory_T;

/* new_mem
 * Purpose: to allocate a new memory space
 * Arguments: int - the number of segments in the new mem
 * Returns: Memory_T - a pointer to the new memory space
 */
Memory_T new_mem(int num_segs);

/* free_mem
 * Purpose: to free the memory used by the memory space
 * Arguments: Memory_T *- a pointer to the memory space
 * Returns: none
 */
void free_mem(Memory_T mem);


/* get_word
 * Purpose: To retrieve a specified word from an instance of Memory_T
 * Arguments: Memory_T *- the memory instance to access
 *            uint32_t - the segment id of the segment to access
 *            int - the offset of the word in the segment
 * Returns: uint32_t - the word found at the specified location
 */
inline uint32_t get_word(Memory_T mem, uint32_t id, uint32_t word_num)
{
        return mem->segments[id][word_num];
}

/* set_word
 * Purpose: sets a word at the specified location in the memory
 * Arguments: Memory_T *- the instance of memory to access
 *            uint32_t - the segment id
 *            int - the offset of the word to change
 *            uint32_t - the new value of that word
 * Returns: none
 */
void set_word(Memory_T mem, uint32_t id, uint32_t word_num, uint32_t word);

/* map_seg
 * Purpose: to create a new mapped segment with an unused id and a specified
 * number of words
 *          with all words initialized to 0
 * Arguments: Memory_T *- the instance of memory to map a new segment into
 *            uint32_t - the number of words that will be in that segment
 * Returns: int - the id of the segment
 */
inline uint32_t map_seg(Memory_T mem, uint32_t num_words)
{
        /* Resizes the memory if memory stack has no available locations */
        Stack_T stack = mem->available;

        if (Stack_empty(stack)) {
                mem->segments = realloc(mem->segments, sizeof(uint32_t *) *
                                mem->num_segs * 2);
                unsigned segs = mem->num_segs;
                for (int i = segs; (unsigned) i < segs * 2;
                                i++) {
                        mem->segments[i] = NULL;
                        Stack_push(mem->available, (void *)(uint64_t)i);
                }
                mem->num_segs *= 2;
        }
        /* id is initialized with an available location from the memory stack */
        uint32_t id = (uint32_t)(uint64_t)Stack_pop(mem->available);
        mem->segments[id] = malloc(sizeof(uint32_t) * (num_words + 1));
        for (int i = 0; (unsigned)i < num_words; i++) {
                mem->segments[id][i] = 0;
        }
        /* sets a sentinel for end of segment*/
        mem->segments[id][num_words] = 0xF0000000;


        return id;
}

/* unmap_seg
 * Purpose: to "free" a segment and allow access to the id
 * Arguments: Memory_T *- the memory to access
 *            uint32_t - the id of the segment to unmap
 * Returns: none
 */
void unmap_seg(Memory_T mem, uint32_t id);

/* load_file
 * Purpose: To read in a um binary and load the program into the first segment
 * Arguments: Memory_T *- the memory to access
 *            FILE * - A pointer to the file to read from
 * Returns: none
 */
void load_file(Memory_T mem, FILE *file);

/* change_program
 * Purpose: To duplicate the segment specified by the id and place it in the
 * program $m[0]
 * Arguments: Memory_T * - the memory to access
 *            uint32_T - the id to be duplicated
 * Returns: none
 */
void change_program(Memory_T mem, uint32_t id);

/* check_avail
 * Purpose: a function for testing by printing elements of the memory stack
 * Arguments: Memory_T * - the memory to access
 * Returns: none
 */
void check_avail(Memory_T mem);

#undef Memory_T
#endif
