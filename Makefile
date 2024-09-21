all:
	g++ -I src/include -L src/lib -o emulator main.c chip8.c -lmingw32 -lSDL2main -lSDL2