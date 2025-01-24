#include "../dependencies/include/SDL.h"
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 800
#define BLOCK_SIZE 40
#define SHAPE_SIZE 4
#define SPEED 300

#define RED 1
#define BLUE 2
#define GREEN 3
#define ORANGE 4
#define YELLOW 5
#define PURPLE 6
#define CYAN 7
#define WHITE 8

#define ROWS (SCREEN_HEIGHT / BLOCK_SIZE)
#define COLUMNS (SCREEN_WIDTH / BLOCK_SIZE)

int L_shape_left[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
};

int L_shape_right[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
};
int Z_shape_left[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
};

int Z_shape_right[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
};
int T_shape[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 0, 0, 0},
};
int Box_shape[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
};
int I_shape[SHAPE_SIZE][SHAPE_SIZE] = {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
};

int global_matrix[ROWS][COLUMNS] = {0};

struct Tetris_block {
    int (*shape)[4];
    int color;
    int y_pos;
    int x_pos;
};



void drawBlock(SDL_Renderer *ren, int x, int y, int color);
void renderAll(SDL_Renderer *ren, struct Tetris_block *block);
void createBlock(struct Tetris_block *block);
void clearRows();
void updateGlobalMatrix(struct Tetris_block *block);
void handleInput(SDL_Event *event, struct Tetris_block *block);
bool checkCollision(struct Tetris_block *block);
void rotateBlock(struct Tetris_block *block);
void setColor(SDL_Renderer *ren, int color);

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // First block.
    struct Tetris_block block;
    createBlock(&block);

    // Time delay.
    Uint32 last_tick = SDL_GetTicks();
    const Uint32 fall_delay = SPEED;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        // Input handling.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            handleInput(&event, &block);
        }

        Uint32 current_tick = SDL_GetTicks();
        if (current_tick - last_tick > fall_delay) {
            if(checkCollision(&block)) {
                updateGlobalMatrix(&block);
                createBlock(&block);
                // Delete current block and create new.
                // Check for clearing.
                // Add points etc.
            } else {
                block.y_pos++;
            }
            last_tick = current_tick;
        }

        renderAll(renderer, &block);
        SDL_Delay(16); // ca 60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void updateGlobalMatrix(struct Tetris_block *block) {
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            if (block->shape[i][j] == 1) {
                if (block->color == RED) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = RED;
                }
                else if (block->color == GREEN) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = GREEN;
                }
                else if (block->color == BLUE) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = BLUE;
                }
                else if (block->color == YELLOW) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = YELLOW;
                }
                else if (block->color == ORANGE) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = ORANGE;
                }
                else if (block->color == PURPLE) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = PURPLE;
                }
                else if (block->color == CYAN) {
                    global_matrix[block->y_pos + i][block->x_pos + j] = CYAN;
                }
                else {
                    global_matrix[block->y_pos + i][block->x_pos + j] = WHITE;
                }
            }
        }
    }
}

void setColor(SDL_Renderer *ren, int color) {
    if (color == GREEN) {
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
    }
    else if (color == RED) {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    }
    else if (color == BLUE) {
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    }
    else if (color == CYAN) {
        SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    }
    else if (color == YELLOW) {
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
    }
    else if (color == ORANGE) {
        SDL_SetRenderDrawColor(ren, 255, 128, 0, 255);
    }
    else if (color == PURPLE) {
        SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
    } else {
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Else White
    }
}



bool checkCollision(struct Tetris_block *block) {
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            if (block->shape[i][j] == 1) {
                int global_y = block->y_pos + i;
                int global_x = block->x_pos + j;
                // new_y is the global y-location for the square inside the tetris-block. If there is a block underneeth(+1 row) it will return true.
                if (global_y == ROWS-1 || global_matrix[global_y + 1][global_x] > 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void rotateBlock(struct Tetris_block *block) {
    int rotatedBlock[SHAPE_SIZE][SHAPE_SIZE] = {0};
    // First, switch rows and columns.
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            rotatedBlock[j][i] = block->shape[i][j];
        }
    }
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            block->shape[i][j] = rotatedBlock[i][SHAPE_SIZE - 1 - j];
        }
    }
    
}

void createBlock(struct Tetris_block *block) {
    int (*shapes[7])[SHAPE_SIZE] = {
    I_shape, L_shape_left, L_shape_right, 
    Z_shape_left, Z_shape_right, T_shape, Box_shape
    };

    int shapeColors[7] = {
        CYAN,      // Färg för I_shape
        ORANGE,    // Färg för L_shape_left
        BLUE,      // Färg för L_shape_right
        RED,       // Färg för Z_shape_left
        GREEN,     // Färg för Z_shape_right
        PURPLE,    // Färg för T_shape
        YELLOW     // Färg för Box_shape
    };
    
    int randomIndex = rand() % 7;

    block->shape = shapes[randomIndex];
    block->x_pos = (COLUMNS / 2) - 2;
    block->y_pos = 0;                
    block->color = shapeColors[randomIndex];
}

void clearRows() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            
        }
    }
}

void renderAll(SDL_Renderer *ren, struct Tetris_block *block){
    // Clear screen
    SDL_SetRenderDrawColor(ren, 41, 41, 41, 255);
    SDL_RenderClear(ren);

    // Draw falling block
    for (int i = 0; i < SHAPE_SIZE; i++) {
        for (int j = 0; j < SHAPE_SIZE; j++) {
            if (block->shape[i][j] == 1) {
                SDL_Rect rect = {
                    (block->x_pos + j) * BLOCK_SIZE,
                    (block->y_pos + i) * BLOCK_SIZE, 
                    BLOCK_SIZE, 
                    BLOCK_SIZE
                };
                setColor(ren, block->color);
                SDL_RenderFillRect(ren, &rect);
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Black.
                SDL_RenderDrawRect(ren, &rect);
            }
        }
    }

    // Draw matrix
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (global_matrix[i][j] > 0) {
                SDL_Rect rect = {
                    j * BLOCK_SIZE,
                    i * BLOCK_SIZE, 
                    BLOCK_SIZE, 
                    BLOCK_SIZE
                };

                setColor(ren, global_matrix[i][j]);
                SDL_RenderFillRect(ren, &rect);
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Black.
                SDL_RenderDrawRect(ren, &rect);
            }
        }
    }
    SDL_RenderPresent(ren);
}

void handleInput(SDL_Event *event, struct Tetris_block *block) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_LEFT) {
            block->x_pos--;
        }
        if (event->key.keysym.sym == SDLK_RIGHT) {
            block->x_pos++;
        }
        if (event->key.keysym.sym == SDLK_DOWN) {
            // Hard hit function. (iterate ypos++ until collision.)
        }
        if (event->key.keysym.sym == SDLK_UP) {
            rotateBlock(block);
        }
    }
}