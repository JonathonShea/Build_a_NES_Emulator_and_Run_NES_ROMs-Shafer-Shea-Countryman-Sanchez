#include "input.h"
#include <iostream>

const char* InputHandler::buttonNames[] = {
    "A", "B", "X", "Y", "Back", "Guide", "Start", "Left Stick", "Right Stick",
    "Left Shoulder", "Right Shoulder", "D-Pad Up", "D-Pad Down", "D-Pad Left", "D-Pad Right"
};

bool InputHandler::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (SDL_NumJoysticks() < 1) {
        std::cerr << "Warning: No joysticks connected!" << std::endl;
    } else {
        // Load joystick
        controller = SDL_GameControllerOpen(0);
        if (controller == nullptr) {
            std::cerr << "Warning: Unable to open game controller! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Game controller connected!" << std::endl;
        }
    }
    return true;
}

void InputHandler::processEvent(const SDL_Event& event) {
    // keyboard input
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        SDL_Keycode key = event.key.keysym.sym;
        bool shiftPressed = (SDL_GetModState() & KMOD_SHIFT);

        // normal input
        // WASD for directional
        // JK for A and B
        // RETURN and SPACE for Start and Select
        if (!shiftPressed) {
            bool pressed = (event.type == SDL_KEYDOWN);
            switch (key) {
            case SDLK_j: setButtonState(0, pressed); break; // A
            case SDLK_k: setButtonState(1, pressed); break; // B
            case SDLK_SPACE: setButtonState(2, pressed); break; // Select
            case SDLK_RETURN: setButtonState(3, pressed); break; // Start
            case SDLK_w: setButtonState(4, pressed); break; // Up
            case SDLK_s: setButtonState(5, pressed); break; // Down
            case SDLK_a: setButtonState(6, pressed); break; // Left
            case SDLK_d: setButtonState(7, pressed); break; // Right
            }
        }
        if (!shiftPressed) {
            if (key == SDLK_w || key == SDLK_a || key == SDLK_s || key == SDLK_d || key == SDLK_j || key == SDLK_k || key == SDLK_SPACE || key == SDLK_RETURN) {
                if (event.type == SDL_KEYDOWN) {
                    std::cout << "Key Pressed: " << SDL_GetKeyName(key) << std::endl;
                } else if (event.type == SDL_KEYUP) {
                    std::cout << "Key Released: " << SDL_GetKeyName(key) << std::endl;
                }
            }
        } 
        // shift modified functions:
        // SHIFT + ESC to exit program
        else if (shiftPressed && key == SDLK_ESCAPE) { 
            std::cout << "SHIFT + ESCAPE pressed. Exiting program." << std::endl;
            exit(0);
        }
    } 
    // controller input
    else if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
        if (event.cbutton.button <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
            const char* buttonName = buttonNames[event.cbutton.button];
            if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                std::cout << "Controller Button Pressed: " << buttonName << std::endl;
            } else if (event.type == SDL_CONTROLLERBUTTONUP) {
                std::cout << "Controller Button Released: " << buttonName << std::endl;
            }
        }
    }
}

InputHandler::~InputHandler() {
    if (controller != nullptr) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
    SDL_Quit();
}

uint8_t InputHandler::getControllerState() const {
    return controllerState;
}

void InputHandler::setButtonState(int button, bool pressed) {
    if (pressed)
        controllerState |= (1 << button);
    else
        controllerState &= ~(1 << button);
}