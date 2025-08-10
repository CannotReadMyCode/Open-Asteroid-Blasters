#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
typedef struct Player {
    SDL_Rect body;
} Player;

typedef struct Bullet {
    SDL_Rect body;
    bool alive;
} Bullet;

typedef struct Asteroid {
    SDL_Rect body;
    bool alive;
} Asteroid;
void InitBody(SDL_Rect *body, int x, int y, int w, int h){
    body->h = h;
    body->w = w;
    body->x = x;
    body->y = y;
}

void DrawBody(SDL_Rect body, SDL_Renderer* r){
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderFillRect(r, &body);
}


int main(int argc, char* argv[]){
    if (SDL_Init(SDL_INIT_VIDEO) > 0){
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Blasters", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
    if (!window){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", SDL_GetError(), window);
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", SDL_GetError(), window);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    srand(time(NULL));
    Player player;
    InitBody(&player.body, 0, 0, 20, 20);

    Bullet bullets[20];

    for (int i = 0; i < 20; i++){
        bullets[i].alive = false;
    }

    Asteroid asteroids[10];
    for (int i = 0; i < 10; i++){
        asteroids[i].alive = false;
    }
    SDL_Event event;
    bool running = true;
    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_UP && player.body.y > 0){
                    player.body.y -= 5;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && player.body.y + player.body.h < 400){
                    player.body.y += 5;
                } 
                if (event.key.keysym.sym == SDLK_SPACE){
                    for (int i = 0; i < 20; i++){
                        if (!bullets[i].alive){
                            InitBody(&bullets[i].body, player.body.x + 10, player.body.y + 10, 10, 5);
                            bullets[i].alive = true;
                            break;
                        }
                    }
                }
                
            }
        }
        for (int i = 0; i < 10; i++){
            if (!asteroids[i].alive){
                InitBody(&asteroids[i].body, 600 + i * 40, rand() % 370, 30, 30);
                asteroids[i].alive = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        DrawBody(player.body, renderer);

        for (int i = 0; i < 20; i++){
            if (bullets[i].alive){
                DrawBody(bullets[i].body, renderer);
                bullets[i].body.x += 5;
                if (bullets[i].body.x > 600){
                    bullets[i].alive = false;
                }
            }
        }
        for (int i = 0; i < 10; i++){
            if (asteroids[i].alive){
                DrawBody(asteroids[i].body, renderer);
                asteroids[i].body.x -= 2;
                if (SDL_HasIntersection(&player.body, &asteroids[i].body)){
                    running = false;
                }
                if (asteroids[i].body.x < 0){
                    asteroids[i].alive = false;
                }
                for (int j = 0; j < 20; j++){
                    if (SDL_HasIntersection(&bullets[j].body, &asteroids[i].body) && bullets[j].alive){
                        bullets[j].alive = false;
                        asteroids[i].alive = false;
                        break;
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}