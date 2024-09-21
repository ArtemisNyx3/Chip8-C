#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

int WIDTH = 64;
int HEIGHT = 32;
int SCALE = 10;
int main(int argc, char *argv[])
{
    printf("In main!!\n");
    init();
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH * SCALE, HEIGHT * SCALE, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderDrawPoint(renderer, WIDTH / 2, HEIGHT / 2);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    // draw display

    while (1)
    {
        printf("Draw Display loop\n");
        SDL_RenderClear(renderer);
        for (int i = 0; i < WIDTH; i++)
        {
            for (int j = 0; j < HEIGHT; j++)
            {
                if (display[i][j])
                    SDL_RenderDrawPoint(renderer, i, j);
            }
        }

        // input
        while (1)
        {
            if (SDL_PollEvent(&event))
            {
                if (SDL_QUIT == event.type)
                    break;

                if (SDL_KEYDOWN == event.type)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_1: // 1
                        keypad[0x1] = 1;
                        break;

                    case SDLK_2: // 2
                        keypad[0x2] = 1;
                        break;

                    case SDLK_3: // 3
                        keypad[0x3] = 1;
                        break;

                    case SDLK_4: // C
                        keypad[0xC] = 1;
                        break;

                    case SDLK_q: // 4
                        keypad[0x4] = 1;
                        break;

                    case SDLK_w: // 5
                        keypad[0x5] = 1;
                        break;

                    case SDLK_e: // 6
                        keypad[0x6] = 1;
                        break;

                    case SDLK_r: // D
                        keypad[0xD] = 1;
                        break;

                    case SDLK_a: // 7
                        keypad[0x7] = 1;
                        break;

                    case SDLK_s: // 8
                        keypad[0x8] = 1;
                        break;

                    case SDLK_d: // 9
                        keypad[0x9] = 1;
                        break;

                    case SDLK_f: // E
                        keypad[0xE] = 1;
                        break;

                    case SDLK_z: // A
                        keypad[0xA] = 1;
                        break;

                    case SDLK_x: // 0
                        keypad[0x0] = 1;
                        break;

                    case SDLK_c: // B
                        keypad[0xB] = 1;
                        break;

                    case SDLK_v: // F
                        keypad[0xF] = 1;
                        break;

                    default:
                        break;
                    }
                }

                if (SDL_KEYUP == event.type)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_1: // 1
                        keypad[0x1] = 0;
                        break;

                    case SDLK_2: // 2
                        keypad[0x2] = 0;
                        break;

                    case SDLK_3: // 3
                        keypad[0x3] = 0;
                        break;

                    case SDLK_4: // C
                        keypad[0xC] = 0;
                        break;

                    case SDLK_q: // 4
                        keypad[0x4] = 0;
                        break;

                    case SDLK_w: // 5
                        keypad[0x5] = 0;
                        break;

                    case SDLK_e: // 6
                        keypad[0x6] = 0;
                        break;

                    case SDLK_r: // D
                        keypad[0xD] = 0;
                        break;

                    case SDLK_a: // 7
                        keypad[0x7] = 0;
                        break;

                    case SDLK_s: // 8
                        keypad[0x8] = 0;
                        break;

                    case SDLK_d: // 9
                        keypad[0x9] = 0;
                        break;

                    case SDLK_f: // E
                        keypad[0xE] = 0;
                        break;

                    case SDLK_z: // A
                        keypad[0xA] = 0;
                        break;

                    case SDLK_x: // 0
                        keypad[0x0] = 0;
                        break;

                    case SDLK_c: // B
                        keypad[0xB] = 0;
                        break;

                    case SDLK_v: // F
                        keypad[0xF] = 0;
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}