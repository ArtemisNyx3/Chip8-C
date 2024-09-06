#include <stdio.h>
#include <SDL2/SDL.h>

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
    SDL_RenderDrawPoint(renderer, WIDTH / 2, HEIGHT / 2);

    SDL_RenderPresent(renderer);

    SDL_Event windowevent;

    while (1)
    {
        if (SDL_PollEvent(&windowevent))
        {
            if (SDL_QUIT == windowevent.type)
                break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}