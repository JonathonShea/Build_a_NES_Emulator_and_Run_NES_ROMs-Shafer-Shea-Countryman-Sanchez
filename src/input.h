#pragma once
//#include <C:/SDL2/include/SDL.h>
#include <SDL2/SDL.h>
//#include <C:/SDL2/include/SDL_mixer.h>
//#include <SDL_Mixer.h>

class InputHandler {
public:
    bool initialize();
    void processEvent(const SDL_Event& event);
    ~InputHandler();
    uint8_t getControllerState() const;

private:
    SDL_GameController* controller = nullptr;
    uint8_t controllerState = 0;
    static const char* buttonNames[];

    void setButtonState(int button, bool pressed);
};