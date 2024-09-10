// chip8.h

#define mem_start 0x200

extern unsigned char memory[4096];

extern unsigned char v[16];

extern unsigned short I;


extern unsigned char DT; // 0x3c = 60;
extern unsigned char ST;

extern unsigned short PC;
extern unsigned char SP;

extern unsigned short stack[16];

extern unsigned char display[64][32];
extern unsigned char keypad[16];
