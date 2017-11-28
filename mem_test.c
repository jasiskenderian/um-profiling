 /* Unit Tests for Segment Module
 * By Jason Iskenderian and
 * Alexa Bosworth
 * HW #6
 */

#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


/*
 * Accepts a Memory_T struct
 * If the index queue is empty, we call the resize function.
 * we test that each word matches those in the original struct
 * and return 1 if there is a discrepancy.
 */
int overflow_test(Memory_T seg_ray);

/* new_mem_test
 * Tests for succesful creation of a mem struct.
 * Returns the newly created Memory_T
 */
Memory_T new_mem_test();


/* mem_free_test
 * Tests the free function for Memory_T
 */
void  free_mem_test(Memory_T mem);

void check_stack(Memory_T mem);


/*
 * Accepts a Memory_T struct and an array of testwords
 * creates a new segment with an array of words
 * prints the words, identifier and the number of words, once instantiated
 */
uint32_t new_seg_test(Memory_T seg_ray, int32_t* testwords, uint32_t num_words);

 /*
  * Acepts a Memory_T struct
  * maps a NULL value to all segment values
  * prints index and corresponding segment values
  */

uint32_t zero_out_seg_test(Memory_T seg_ray);

  /*
   * Instantiates a seg_array to one above the maximum overhead and asserts
   */

void out_of_mem_test();

/*
 * Takes a Memory_T struct, an array of words and number of words
 * Prints the words and identifier once they have been set
 *
 */

void simple_set_get(Memory_T seg_ray);


/*
 * Takes a Memeory_T struct, an array of words and number of words
 * prints the segments words and its identifier
 *
 */

void test_map(Memory_T mem);

/*
 * Takes a Memeory_T struct, an array of words and number of words
 * prints the segments words, which should be NULL, and its identifier
 *
 */

void test_unmap(Memory_T mem);

void large_map(Memory_T mem);

void load_file_test(Memory_T mem, FILE *fp);



int main()
{
        FILE *fp = fopen("hello.um", "r");

      /*uint32_t* testwords = NULL;
      uint32_t num_words = 1;
      uint32_t identifier = 1;*/

      printf("Beginning Memory_T unit tests:\n\n");
      Memory_T mem = new_mem_test();
      printf("Testing Tag Stack:\n\tExpecting Tags 1-9\n\t");
      check_stack(mem);
      test_map(mem);
      printf("Testing Tag Stack:\n\tExpecting Tags 2-9\n\t");
      check_stack(mem);
      test_unmap(mem);
      printf("Testing Tag Stack:\n\tExpecting Tags 9,8,7,6,5,4,3,2,1,\n\t");
      check_stack(mem);
      load_file_test(mem, fp);
      printf("Mapping 1000 segments:   ");
      large_map(mem);

      /*simple_set_get(mem);*/
      free_mem_test(mem);
      fclose(fp);
      /*printf("Sentinel: 0x%X\n", (uint32_t) 15 << 28);*/

  /*
        overflow_test(seg_ray);
        new_seg_test(seg_ray, testwords, num_words);
        zero_out_seg_test(seg_ray);
        out_of_mem_test();
        test_set(seg_ray, (uint32_t *)testwords, num_words);
        test_get(seg_ray, identifier);
        test_map(seg_ray, (uint32_t *)testwords, num_words);
        test_unmap(seg_ray, identifier);*/
      return 0;
}

Memory_T new_mem_test()
{
      printf("Attempting to create a new Memory_T:   ");
      Memory_T mem = new_mem(10);
      printf("SUCCESS\n\n");
      return mem;

}

void free_mem_test(Memory_T mem)
{
      printf("Attempting to free the Memory used by Memory_T:   ");
      free_mem(mem);
      printf("SUCCESS\n\n");
}


int overflow_test(Memory_T seg_ray){
      (void) seg_ray;
      return 1;

}

void check_stack(Memory_T mem)
{
        check_avail(mem);

}

uint32_t new_seg_test(Memory_T seg_ray, int32_t* testwords, uint32_t num_words){
      (void) seg_ray;
      (void) testwords;
      (void) num_words;
      return 0;
}

uint32_t zero_out_seg_test(Memory_T seg_ray){
      (void) seg_ray;
      return 0;

}

void out_of_mem_test(){

}

void simple_set_get(Memory_T mem)
{
      printf("Simple test of set and get functions:\n");
      uint32_t word = 123456;
      set_word(mem, 0, 0, word);
      printf("\tSet (0,0) to 123456\n");
      word = get_word(mem, 0, 0);
      printf("\tThe value at (0,0) is %u\n\n",(unsigned)word);

}



void test_map(Memory_T mem)
{
        uint32_t id;
        printf("Attempting to map segment. Expected id of 0:   ");
        id = map_seg(mem, 20);
        printf("SUCCESS with id of %u\n\n", (unsigned)id);
}

void test_unmap(Memory_T mem)
{
        printf("Testing unmap on segment 0:   ");
        unmap_seg(mem, 1);
        printf("SUCCESS\n\n");
}

void large_map(Memory_T mem)
{
        for (int i = 0; i < 1000; i++) {
                map_seg(mem, 20);
        }
        printf("SUCCESS\n\n");
}

void load_file_test(Memory_T mem, FILE *fp)
{
        printf("Attempting to read program from file:   ");
        load_file(mem, fp);
        printf("SUCCESS\n\n");
}






