#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

const int WIDTH = 64, HEIGHT = 32, SCALE = 10;

int main(int argc, char *argv[])
{

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

    SDL_Event *windowevent;
    // draw display

    while (1)
    {
        SDL_RenderClear(renderer);
        for (int i = 0; i < WIDTH; i++)
        {
            for (int j = 0; j < HEIGHT; j++)
            {
                if (display[i][j])
                    SDL_RenderDrawPoint(renderer, i, j);
            }
        }

        if (SDL_PollEvent(windowevent))
        {
            if (windowevent->type == SDL_QUIT)
            {
                break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}