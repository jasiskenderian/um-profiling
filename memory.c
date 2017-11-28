/* memory.c
 * Jason Iskenderian and Alexa Bosworth
 * 11/12/17
 * Purpose: Implements the memory of the um which meets all the segment 
 * requirements
 */

#include "memory.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bitpack.h>
#include "stack.h"
#include <stdint.h>

/* Memory struct */
 struct Memory_T {
        Stack_T available;
        uint32_t **segments;
        unsigned num_segs;
};


Memory_T new_mem(int num_segs)
{
        Memory_T mem = malloc(sizeof(struct Memory_T));
        mem->available = Stack_new();
        mem->segments = malloc(sizeof(uint32_t *) * num_segs);
        
        /* fills the memory stack with available memory locations */ 
        for (int i = 0; i < num_segs; i++) {
                mem->segments[i] = NULL;
                if (i != 0) {
                        Stack_push(mem->available, (void  *)(uint64_t)i);
                }
        }

        mem->num_segs = num_segs;

        return mem;
}


void free_mem(Memory_T mem)
{
        for (unsigned i = 0; i < mem->num_segs; i++) {
               
                if (mem->segments[i] != NULL){
                        free(mem->segments[i]);
                }
        }

        free(mem->segments);
        Stack_free(&(mem->available));
        free(mem);

}

uint32_t get_word(Memory_T mem, uint32_t id, uint32_t word_num)
{
        return mem->segments[id][word_num];

}

void set_word(Memory_T mem, uint32_t id, uint32_t word_num, uint32_t word)
{
        mem->segments[id][word_num] = word;

}

uint32_t map_seg(Memory_T mem, uint32_t num_words)
{
        /* Resizes the memory if memory stack has no availble locations */
        if (Stack_empty(mem->available)) {
                mem->segments = realloc(mem->segments, sizeof(uint32_t *) * 
                                mem->num_segs * 2);
                for (int i = mem->num_segs; (unsigned) i < mem->num_segs * 2; 
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

void unmap_seg(Memory_T mem, uint32_t id)
{
        Stack_push(mem->available, (void *)(uint64_t)id);
        free(mem->segments[id]);
        mem->segments[id] = NULL;

}

void load_file(Memory_T mem, FILE *file)
{
        uint32_t word = 0;
        uint32_t new_char = 0;
        int num_words = 0;
        int count = 3;
        int size = 20;

       
        mem->segments[0] = malloc(sizeof(uint32_t) * 20);

        while((int)new_char != EOF) {
                /* adds new char to word */
                new_char = getc(file);
                word = word | (new_char << ((count) * 8));
                /* resizes segment if necessary */
                if (num_words == size) {
                        mem->segments[0] = realloc(mem->segments[0], 
                                        sizeof(uint32_t) * size * 2);
                        size *= 2;
                }
                /* word is complete, moves to next word */
                if(count == 0) {
                        mem->segments[0][num_words] = word;
                        word = 0;
                        count = 4;
                        num_words++;
                }
                count--;

        }
        mem->segments[0] = realloc(mem->segments[0], sizeof(uint32_t) * 
                        (num_words + 1));
        mem->segments[0][num_words] = 0xF0000000;

}

void change_program(Memory_T mem, uint32_t id)
{
        if (id == 0) {
                return;
        }

        uint32_t curr = mem->segments[id][0];
        uint64_t count = 1;
        /* seeks to end of segment */
        while (curr != 0xF0000000) {
                curr = mem->segments[id][count];
                count++;
        }

        uint32_t *temp = malloc(sizeof(uint32_t) * count);

        for (unsigned i = 0; i < count; i++) {
                temp[i] = mem->segments[id][i];

        }
        free(mem->segments[0]);
        mem->segments[0] = temp;

}

void check_avail(Memory_T mem)
{
        uint32_t *curr;
        Stack_T new = Stack_new();
       
        while(!Stack_empty(mem->available)) {
                curr = Stack_pop(mem->available);
                printf(" %lu,",(uint64_t)curr);
                Stack_push(new, curr);
        }
      
        Stack_free(&(mem->available));
        mem->available = new;
}









