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

    case 0x3000: // 3XNN --- if (Vx == NN)
        if (v[opcode & 0x0F00 >> 8] == opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x4000: // 4XNN --- if (Vx != NN)
        if (v[opcode & 0x0F00 >> 8] != opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x5000: // 5XY0 --- if (Vx == Vy)
        if (v[opcode & 0x0F00 >> 8] == v[opcode & 0x00F0 >> 4])
            PC += 4;
        else
            PC += 2;
        break;

    case 0x6000: // 6XNN --- Vx = NN
        v[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
        PC += 2;
        break;

    case 0x7000: // 7XNN --- Vx += NN
        v[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
        PC += 2;
        break;

    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000: // 8XY0 --- Vx = Vy
            /* code */
            break;

        case 0x0001: // 8XY1 --- Vx = Vx | Vy
            /* code */
            break;

        case 0x0002: // 8XY2 --- Vx = Vx & Vy
            /* code */
            break;

        case 0x0003: // 8XY3 --- Vx = Vx ^ Vy
            /* code */
            break;

        case 0x0004: // 8XY4 --- Vx = Vx + Vy
            /* code */
            break;

        case 0x0005: // 8XY5 --- Vx = Vx - Vy
            /* code */
            break;

        case 0x0006: // 8XY6 --- Vx >>= 1
                     // Shifts VX to the right by 1, then stores the least significant bit of VX prior to the shift into VF
            /* code */
            break;

        case 0x0007: // 8XY7 --- Vx = Vy - Vx
            /* code */
            break;

        case 0x000E: // 8XYE --- Vx <<= 1
            /* code */
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    // execute
}