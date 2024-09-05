// chip.c
#include <stdio.h>
#include <stdlib.h>

// Memory
//  +---------------+= 0xFFF (4095) End of Chip-8 RAM
//  |               |
//  |               |
//  |               |
//  |               |
//  |               |
//  | 0x200 to 0xFFF|
//  |     Chip-8    |
//  | Program / Data|
//  |     Space     |
//  |               |
//  |               |
//  |               |
//  |               |
//  |               |
//  |               |
//  +---------------+= 0x200 (512) Start of most Chip-8 programs
//  | 0x000 to 0x1FF|
//  | Reserved for  |
//  |  interpreter  |
//  +---------------+= 0x000 (0) Start of Chip-8 RAM

unsigned char memory[4096] = {0x0};

// 16 general purpose 8-bit registers, usually referred to as Vx, where x is a hexadecimal digit (0 through F)
unsigned char v[16] = {0x0};

// 16-bit register called I. This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
unsigned short I = 0x0;

// The VF register should not be used by any program, as it is used as a flag by some instructions
unsigned char vf = 0x0;

// Chip-8 also has two special purpose 8-bit registers, for the delay and sound timers.
unsigned char DT = 0x3c; // 0x3c = 60;
unsigned char ST = 0x0;

// "pseudo-registers"
// program counter (PC) should be 16-bit, and is used to store the currently executing address.
unsigned short PC = 0x200;
// The stack pointer (SP) can be 8-bit, it is used to point to the topmost level of the stack.
unsigned char SP = 0x0;

// The stack is an array of 16 16-bit values, used to store the address that the interpreter shoud return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.

unsigned short stack[16] = {0};

unsigned char display[64][32] = {0x0};

unsigned char fonts[5][16] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init(void)
{
    // load sprites into intrepretor region
    // region is from 0x000 to 0x200 i.e 0 to 512
    memcpy(memory, fonts, sizeof(fonts));

    // load rom
    FILE *rom = NULL;
    rom = fopen("game", "r"); // game is place holder

    int memptr = 0x200;
    unsigned char x;
    if (rom != NULL){
        while(rom != EOF){
            x = fgetc(rom);
            memory[memptr++] = x;
        }
    }

    
}

void emulation_cycle(void)
{
}