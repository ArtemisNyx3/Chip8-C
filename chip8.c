// chip.c
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

unsigned char fonts[5 * 16] =
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
    // initialize variables
    I = 0x0;
    PC = 0x200;
    SP = 0x0;

    DT = 0x3C;
    ST = 0;

    for (int i = 0; i < 16; i++)
    {
        v[i] = stack[i] = keypad[i] = 0x0;
    }

    for (int i = 0; i < 4096; i++)
        memory[i] = 0x0;

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            display[i][j] = 0x0;
        }
    }

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

    // decode & execute
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
        if (v[(opcode & 0x0F00) >> 8] == opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x4000: // 4XNN --- if (Vx != NN)
        if (v[(opcode & 0x0F00) >> 8] != opcode & 0x00FF)
            PC += 4;
        else
            PC += 2;
        break;

    case 0x5000: // 5XY0 --- if (Vx == Vy)
        if (v[(opcode & 0x0F00) >> 8] == v[(opcode & 0x00F0) >> 4])
            PC += 4;
        else
            PC += 2;
        break;

    case 0x6000: // 6XNN --- Vx = NN
        v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        PC += 2;
        break;

    case 0x7000: // 7XNN --- Vx += NN
        v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        PC += 2;
        break;

    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000: // 8XY0 --- Vx = Vy
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0001: // 8XY1 --- Vx = Vx | Vy
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x0F00) >> 8] | v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0002: // 8XY2 --- Vx = Vx & Vy
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x0F00) >> 8] & v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0003: // 8XY3 --- Vx = Vx ^ Vy
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x0F00) >> 8] ^ v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0004:                                                          // 8XY4 --- Vx = Vx + Vy
            if (v[(opcode & 0x0F00) >> 8] > 0xFF - v[(opcode & 0x00F0) >> 4]) // overflow cond
                v[0xF] = 1;
            else
                v[0xF] = 0;
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x0F00) >> 8] + v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0005:                                                   // 8XY5 --- Vx = Vx - Vy
            if (v[(opcode & 0x0F00) >> 8] < v[(opcode & 0x00F0) >> 4]) // undeflow cond
                v[0xF] = 0;
            else
                v[0xF] = 1;
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x0F00) >> 8] + v[(opcode & 0x00F0) >> 4];
            PC += 2;
            break;

        case 0x0006: // 8XY6 --- Vx >>= 1
                     // Shifts VX to the right by 1, then stores the least significant bit of VX prior to the shift into VF

            v[0xF] = v[(opcode & 0x0F00) >> 8] & 0x1;
            v[(opcode & 0x0F00) >> 8] >>= 1;
            PC += 2;
            break;

        case 0x0007:                                                   // 8XY7 --- Vx = Vy - Vx
            if (v[(opcode & 0x0F00) >> 8] > v[(opcode & 0x00F0) >> 4]) // undeflow cond
                v[0xF] = 0;
            else
                v[0xF] = 1;
            v[(opcode & 0x0F00) >> 8] = v[(opcode & 0x00F0) >> 4] - v[(opcode & 0x0F00) >> 8];
            PC += 2;
            break;

        case 0x000E: // 8XYE --- Vx <<= 1
                     // sets VF to Most Significant bit prior to bitshift
            v[0xF] = v[(opcode & 0x0F00) >> 8] >> 7;
            v[(opcode & 0x0F00) >> 8] <<= 1;
            PC += 2;
            break;

        default:
            break;
        }
        break;

    case 0x9000: // 9XY0 --- if (Vx != Vy)
        if (v[(opcode & 0x0F00) >> 8] != v[(opcode & 0x00F0) >> 4])
            PC += 4;
        else
            PC += 2;
        break;

    case 0xA000: // ANNN --- I = NNN
        I = opcode & 0x0FFF;
        PC += 2;
        break;

    case 0xB000: // BNNN --- PC = V0 + NNN
        PC = v[0x0] + (opcode & 0x0FFF);
        break;

    case 0xC000: // CXNN --- Vx = rand() & NN
        v[(opcode & 0x0F00) >> 8] = (rand() & 0xFF) & (opcode & 0x00FF);
        PC += 2;
        break;

    case 0xD000: // DXYN --- draw(Vx, Vy, N)
                 // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
                 // Each row of 8 pixels is read as bit-coded starting from memory location I;
                 // I value does not change after the execution of this instruction.
                 // As described above,
                 // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen.
        unsigned short x, y, n;
        x = v[(opcode & 0x0F00) >> 8];
        y = v[(opcode & 0x00F0) >> 4];
        n = opcode & 0x000F;
        unsigned char spriteRow;

        for (int i = 0; i < n; i++) // y coordinate
        {
            spriteRow = memory[I + i];
            for (int j = 0; j < 8; j++) // x coordinate
            {
                int a = x + j, b = y + i;
                unsigned char newVal = spriteRow & 0x80 >> 7; // get the MSB
                spriteRow <<= 1;                              // since we got the MSB, left shift to make the next one MSB

                if (display[a][b] == 1 && newVal == 0)
                    v[0xF] = 1;
                display[a][b] = newVal;
            }
        }
        break;

    case 0xE000:
        switch (opcode & 0x00FF)
        {
        case 0x009E: // EX9E --- if (key() == Vx)
                     // Skips the next instruction if the key stored in VX is pressed
            if (keypad[v[(opcode & 0x0F00) >> 8]] == 1)
                PC += 4;
            else
                PC += 2;
            break;

        case 0x00A1: // EXA1 --- if (key() != Vx)
                     // Skips the next instruction if the key stored in VX is not pressed
            if (keypad[v[(opcode & 0x0F00) >> 8]] != 1)
                PC += 4;
            else
                PC += 2;
            break;

        default:
            break;
        }
        break;

    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007: // FX07 --- Vx = get_delay()  ---  Sets VX to the value of the delay timer.
            v[(opcode & 0x0F00) >> 8] = DT;
            PC += 2;
            break;

        case 0x000A: // FX0A --- Vx = get_key()  ---  A key press is awaited, and then stored in VX
                     //(blocking operation, all instruction halted until next key event)
            short flg = 1;
            for (int i = 0; i < 16; i++)
            {
                if (keypad[i] == 1)
                {
                    v[(opcode & 0x0F00) >> 8] = i;
                    PC += 2;
                    flg = 0;
                }
            }
            if (flg)
                return;
            break;

        case 0x0015: // FX15 --- Sets the delay timer to VX
            DT = v[(opcode & 0x0F00) >> 8];
            PC += 2;
            break;

        case 0x0018: // FX18 --- Sets the sound timer to VX
            ST = v[(opcode & 0x0F00) >> 8];
            PC += 2;
            break;

        case 0x001E: // FX07 --- I += Vx  ---  Adds VX to I. VF is not affected
            unsigned short x = (opcode & 0x0F00) >> 8;
            if (x != 0xF)
            {
                I += x;
            }
            PC += 2;
            break;

        case 0x0029: // FX29 --- I = sprite_addr[Vx] --- Sets I to the location of the sprite for the character in VX.
                     // Characters 0-F (in hexadecimal) are represented by a 4x5 font
            I = v[(opcode & 0x0F00) >> 8] * 0x5;
            PC += 2;
            break;
        case 0x0033: // FX33
                     // set_BCD(Vx)
                     // *(I+0) = BCD(3);
                     // *(I+1) = BCD(2);
                     // *(I+2) = BCD(1);
                     // Stores the binary-coded decimal representation of VX, with the hundredth digit in memory at location in I,
                     // the tens digit at location I+1, and the ones digit at location I+2.
            memory[I] = v[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (v[(opcode & 0x0F00) >> 8] / 10);
            memory[I + 2] = v[(opcode & 0x0F00) >> 8] / 100;

            PC += 2;
            break;

        case 0x0055: // FX55 --- reg_dump(Vx, &I) --- Stores from V0 to VX (including VX) in memory, starting at address I
                     // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
            for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
            {
                memory[I + i] = v[i];
            }
            PC += 2;
            break;

        case 0x0065: // FX65 --- reg_load(Vx, &I)] --- Fills from V0 to VX (including VX) with values from memory, starting at address I.
                     // The offset from I is increased by 1 for each value read, but I itself is left unmodified.
            for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
            {
                v[i] = memory[I + i];
            }
            PC += 2;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

}