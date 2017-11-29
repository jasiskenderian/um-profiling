/* emulator.c
 * Jason Iskenderian and Alexa Bosworth
 * 11/16/17
 * Implements the emulator module and corresponding functions
 */

#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"
#include "memory.h"
#include <stdint.h>
#include "assert.h"

static const uint32_t OP_MASK = 0xF0000000;
static const uint32_t REGC_MASK = 0x00000007;
static const uint32_t REGB_MASK = 0x00000038;
static const uint32_t REGA_MASK = 0x000001C0;
static const uint32_t LOAD_A = 0x0E000000;
static const uint32_t LOAD_VAL = 0x01FFFFFF;
static const unsigned OP_SHIFT = 28;
static const unsigned A_SHIFT = 6;
static const unsigned B_SHIFT = 3;
static const unsigned A_LOAD_SHIFT = 25;
static const uint64_t MAX = (uint64_t) 1 << 32;

struct Emulator_T {
        uint32_t *registers;
        uint32_t pc;
        Memory_T mem;
};

/* load_program
 * Purpose: initializes memory[0][0] with the program
 * Arguments: Emulator_T * - the emulator
 *            uint32_t - register B
 *            uint32_t - register C
 * Returns: none
 */
void load_program(Emulator_T em, uint32_t regB, uint32_t regC);

/* take_input
 * Purpose: receives input as part of the I/O device
 * Arguments: Emulator_T * - the emulator
 *            uint32_t - register C
 * Returns: none
 */
void take_input(Emulator_T em, uint32_t regC);

/* output
 * Purpose: completes the output instruction of the I/O device
 * Arguments: Emulator_T * - the emulator
 *            uint32_t - register C
 * Returns: none
 */
void output(Emulator_T em, uint32_t regC);

/* map
 * Purpose: initializes the memory segment in regB with contents of regC
 * Arguments: Emulator_T * - the emulator
 *            uint32_t - register B
 *            uint32_t - register C
 * Returns: none
 */
void map(Emulator_T em, uint32_t regB, uint32_t regC);

/* nand
 * Purpose: stores the complement of regB ^ regC in regA
 * Arguments: Emulator_T * - the emulator
 *            uint32_T - registers A, B and C
 * Returns: none
 */
void nand(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC);

/* store
 * Purpose: uses set_word() to store the word in regC into memory[regA][regB]
 * Arguments: Emulator_T * - the emulator
 *            uint32_T - registers A, B and C
 * Returns: none
 */
void store(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC);

/* load
 * Purpose: loads the word found in memory[regB][regC] into regA
 * Arguments: Emulator_T * - the emulator
 *            uint32_T - registers A, B and C
 * Returns: none
 */
void load(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC);

/* cond_move
 * Purpose: if the contents of regC do not equal zero, store regB into regA
 * Arguments: Emulator_T * - the emulator
 *            uint32_T - registers A, B and C
 * Returns: none
 */
void cond_move(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC);


Emulator_T new_em(FILE *fp)
{
        Emulator_T new = malloc(sizeof(struct Emulator_T));
        new->registers = malloc(sizeof(uint32_t) * 8);
        new->mem = new_mem(10);
        for (int i = 0; i < 8; i++) {
                new->registers[i] = 0;
        }
        new->pc = 0;
        load_file(new->mem, fp);
        return new;


}

void free_em(Emulator_T em)
{
        free(em->registers);
        free_mem(em->mem);
        free(em);

}


void run(Emulator_T em)
{
        unsigned opcode = 1;
        uint32_t instruction;
        uint32_t regA;
        uint32_t regB;
        uint32_t regC;
        uint32_t val;

        while (opcode != 15) {
                instruction = get_word(em->mem, 0, em->pc);
                opcode = (instruction & OP_MASK) >> OP_SHIFT;

                switch(opcode) {
                        /* sentinel value */
                        case 15:
                                break;
                        /* Load Value*/
                        case 13:
                                regA = (LOAD_A & instruction) >> A_LOAD_SHIFT;
                                val = (LOAD_VAL & instruction);
                                em->registers[regA] = val;
                                em->pc++;
                                break;
                        /* Load Program */
                        case 12:
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                //fprintf(stderr, "REGB: %u REGC: %u\n", regB,
                                //regC);
                                load_program(em, regB, regC);
                                break;
                        /* Input */
                        case 11:
                                regC = (REGC_MASK & instruction);
                                take_input(em, regC);
                                em->pc++;
                                break;
                        /* Output */
                        case 10:
                                regC = (REGC_MASK & instruction);
                                output(em, regC);
                                em->pc++;
                                break;
                        /* Unmap Segment */
                        case 9:
                                regC = (REGC_MASK & instruction);
                                unmap_seg(em->mem, em->registers[regC]);
                                em->pc++;
                                break;
                        /* Map Segment */
                        case 8:
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                map(em, regB, regC);
                                em->pc++;
                                break;
                        /* Halt */
                        case 7:
                                return;
                        /* Bitwise NAND */
                        case 6:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                nand(em, regA, regB, regC);
                                em->pc++;
                                break;
                        /* Division */
                        case 5:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                em->registers[regA] = (em->registers[regB] /
                                                em->registers[regC]);
                                em->pc++;
                                break;
                        /* Multiplication */
                        case 4:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                em->registers[regA] = (em->registers[regB] *
                                                em->registers[regC]) % MAX;
                                em->pc++;
                                break;
                        /* Addition */
                        case 3:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                em->registers[regA] = (em->registers[regB] +
                                                em->registers[regC]) % MAX;
                                em->pc++;
                                break;
                        /* Addition */
                        case 2:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                store(em, regA, regB, regC);
                                em->pc++;
                                break;
                        /* Segmented Store */
                        case 1:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                load(em, regA, regB, regC);
                                em->pc++;
                                break;
                        /* Segmented Load */
                        case 0:
                                regA = (REGA_MASK & instruction) >> A_SHIFT;
                                regB = (REGB_MASK & instruction) >> B_SHIFT;
                                regC = (REGC_MASK & instruction);
                                cond_move(em, regA, regB, regC);
                                em->pc++;
                                break;
                }

        }
}


void load_program(Emulator_T em, uint32_t regB, uint32_t regC)
{
        em->pc = em->registers[regC];

        if (em->registers[regB] == 0) {
                return;
        } else {
                change_program(em->mem, em->registers[regB]);
        }
}

void take_input(Emulator_T em, uint32_t regC)
{
        unsigned in = getchar();
        if ((int)in == EOF) {
                em->registers[regC] = ~0;
                return;
        } else {
                assert(in <= 255);
                em->registers[regC] = in;
        }
}

void output(Emulator_T em, uint32_t regC)
{
        assert(em->registers[regC] <= 255);
        printf("%c", em->registers[regC]);
}

void map(Emulator_T em, uint32_t regB, uint32_t regC)
{
        uint32_t temp;
        temp = map_seg(em->mem, em->registers[regC]);
        em->registers[regB] = temp;
}

void nand(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC)
{
        uint32_t temp;
        temp = ~(em->registers[regB] & em->registers[regC]);
        em->registers[regA] = temp;
}

void store(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC)
{
        set_word(em->mem, em->registers[regA], em->registers[regB],
                        em->registers[regC]);
}

void load(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC)
{
        em->registers[regA] = get_word(em->mem, em->registers[regB],
                        em->registers[regC]);
}

void cond_move(Emulator_T em, uint32_t regA, uint32_t regB, uint32_t regC)
{
        if (em->registers[regC] != 0) {
                em->registers[regA] = em->registers[regB];
        }
}
