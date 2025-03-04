#pragma once
#include <C:/SDL2/include/SDL.h>
#include <C:/SDL2/include/SDL_mixer.h>

class InputHandler {
public:
    bool initialize();
    void processEvent(const SDL_Event& event);
    ~InputHandler();

private:
    SDL_GameController* controller = nullptr;
    Mix_Chunk* soundEffect = nullptr;
    static const char* buttonNames[];
};