/* um.c
 * Jason Iskenderian and Alexa Bosworth
 * 11/20/17
 * Runs the um and takes command line arguments
 */

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

int main(int argc, char *argv[])
{
        if (argc != 2) {
                printf("Usage:  ./um [filename]\n");
                return 1;
        }

        FILE *in = fopen(argv[1], "r");
        Emulator_T em = new_em(in);
        run(em);
        free_em(em);
        fclose(in);
        return 0;
}