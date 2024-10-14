#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool collisionDetect(SDL_Rect& rect1, SDL_Rect& rect2) {
    return (rect1.x < rect2.x + rect2.w &&
            rect1.x + rect1.w > rect2.x &&
            rect1.y < rect2.y + rect2.h &&
            rect1.y + rect1.h > rect2.y);
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("Bouncing Square", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int squareSize = 20;
    float x = SCREEN_WIDTH / 2 - squareSize / 2;
    float y = SCREEN_HEIGHT / 2 - squareSize / 2;
    float dx = 250, dy = 250;  // Speed in pixels per second

    float paddleY = 40;
    int paddleX = SCREEN_WIDTH - 50;
    float paddleSpeed = 300;  // Pixels per second

    SDL_Rect paddle = {paddleX, (int)paddleY, 20, 80};
    SDL_Rect ai_paddle = {40,40, 20, 80};
    SDL_Rect squareRect = {(int)x, (int)y, squareSize, squareSize};

    bool quit = false;
    SDL_Event event;

    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        // Move the paddle
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_UP] && paddle.y > 0) {
            paddleY -= paddleSpeed * deltaTime;
        }
        if (keyState[SDL_SCANCODE_DOWN] && paddle.y < SCREEN_HEIGHT - paddle.h) {
            paddleY += paddleSpeed * deltaTime;
        }
        paddle.y = (int)paddleY;

        //Move the rival paddle
    
        if(squareRect.y > ai_paddle.y + 40){
           ai_paddle.y += (250*deltaTime);
        }
        else if(squareRect.y < ai_paddle.y + 40){
           ai_paddle.y -= (250*deltaTime);
          }
          else{
           ai_paddle.y += 0;
      }
    
        // Move the square
        x += dx * deltaTime;
        y += dy * deltaTime;

        // Update square position
        squareRect.x = (int)x;
        squareRect.y = (int)y;

        // Bounce off the edges
        if (x <= 0 || x + squareSize >= SCREEN_WIDTH || collisionDetect(squareRect, paddle) || collisionDetect(squareRect, ai_paddle)) {
            dx = -dx;
            x = (x <= 0) ? 0 : (x + squareSize >= SCREEN_WIDTH) ? SCREEN_WIDTH - squareSize : x;
        }
        if (y <= 0 || y + squareSize >= SCREEN_HEIGHT) {
            dy = -dy;
            y = (y <= 0) ? 0 : (y + squareSize >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - squareSize : y;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &squareRect);

        // Draw the paddle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle);

        //Draw the AI paddle
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&ai_paddle);

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
