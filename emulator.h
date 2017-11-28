/* emulator.h
 * Jason Iskenderian and Alexa Bosworth
 * 11/16/17
 * Provides definitions and headers for the Emulator Module
 */

#ifndef EMULATOR_INCLUDED
#define EMULATOR_INCLUDED

#define T Emulator_T
typedef struct T *T;

#include <stdio.h>

/* run
 * Purpose: Runs the um on the provided file
 * Arguments: Emulator_T - the emulator to run
 * Returns: none
 */
void run(T em);

/* new_em
 * Purpose: to create a new instance of the um emulator that is initialized
 * Arguments: FILE *fp - the file containing the program
 * Returns: Emulator_T - the initialized instance of the emulator
 */
T new_em(FILE *fp);

/* free_em
 * Purpose: To free the memory used by the emulator
 * Arguments: Emulator_T - the emulator to free
 * Returns: none
 */
void free_em(T em);



#undef T
#endif