/* em_test
 * Jason Iskenderian and Alexa Bosworth
 * 11/16/17
 * Defines and runs tests for the emulator module
 */

#include "emulator.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
void test_free(Emulator_T em);


int main()
{
        printf("Emulator Tests");
        FILE *infile = fopen("cat.um", "r");
        printf("\nAttempting to create Emulator:   ");
        Emulator_T em = new_em(infile);
        printf("SUCCESS\n\n");
        printf("Running Emulator on cat.um\n\n");
        run(em);
        printf("\n\n");
        test_free(em);
        fclose(infile);
        return 0;
}

void test_free(Emulator_T em)
{
        printf("Attempting to free emulator:   ");
        free_em(em);
        printf("SUCCESS\n\n");
}