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
    // fetch
    
    // decode
    // execute
}