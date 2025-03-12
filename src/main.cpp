#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "input.h" // Include the InputHandler class

int main(int argc, char* argv[]) {
    InputHandler inputHandler; // Create an InputHandler instance

    if (!inputHandler.initialize()) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            inputHandler.processEvent(event); // Use the InputHandler class
        }
        SDL_Delay(16); // Add a small delay to prevent CPU overuse
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}