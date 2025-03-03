/*
    Created by Gwyn Shafer on 11/28/2024
    Last Updated: 11/28/2024
*/
#ifndef APU_H
#define APU_H

#include <cstdint>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class APU
{
public:
    APU();
    ~APU();
    void test();
    void sdlAudioTest();

private:
    static void audioCallback(void* userdata, Uint8* stream, int len);
    SDL_AudioSpec want, have;
    SDL_AudioDeviceID dev;
};

#endif