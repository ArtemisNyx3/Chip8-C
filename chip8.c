// chip.c
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

void init(void)
{
    // load sprites into intrepretor region
    // region is from 0x000 to 0x200 i.e 0 to 512
    for (int i = 0; i < 5 * 16; i++)
    {
        memory[i] = fonts[i];
    }

    // load rom
    FILE *rom = NULL;
    rom = fopen("game", "r"); // game is place holder
    int memptr = mem_start;
    unsigned char x;
    if (rom != NULL)
    {
        while (rom != EOF)
        {
            x = fgetc(rom);
            memory[memptr++] = x;
        }
    }
}

void executeCPU(void)
{
    unsigned short opcode;
    // fetch
    opcode = memory[PC] << 8 | memory[PC + 1]; // Since each opcode is 2 bytes long, we left shift 8 bits to make msb

    // decode
    switch (opcode & 0xF000) // isolate the first digit
    {
    case 0x0000:
        switch (opcode)
        {
        case 0x00E0: // clear display
            for (int i = 0; i < 64; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    display[i][j] = 0;
                }
            }
            PC += 2;
            break;

        case 0x00EE:        // return
            SP--;           // prev layer of stack
            PC = stack[SP]; // update pc
            PC += 2;        // next instruction
            break;
        }

    case 0x1000: // goto NNN
        PC = opcode & 0x0FFF;
        break;

    case 0x2000: // calls subroutine at NNN
        stack[SP] = PC;
        SP++;
        PC = opcode & 0x0FFF;
        break;

    case 0x3000:
        if (v[opcode & 0x0F00 >> 8] == opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x4000:
        if (v[opcode & 0x0F00 >> 8] != opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x5000:
        if (v[opcode & 0x0F00 >> 8] == v[opcode & 0x00F0 >> 4])
            PC += 4;
        else
            PC += 2;
        break;

    case 0x6000:
        v[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
        PC += 2;
        break;

    case 0x7000:
        v[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
        PC += 2;
        break;

    default:
        break;
    }

    // execute
}