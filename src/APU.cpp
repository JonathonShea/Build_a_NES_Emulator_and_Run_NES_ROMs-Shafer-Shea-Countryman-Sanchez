/*
    Created by Gwyn Shafer on 11/28/2024
    Last Updated: 11/28/2024
*/
#include "APU.h"
#include <iostream>
#include <cmath>


APU::APU()
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 44100; // Sample rate
    want.format = AUDIO_F32SYS; // 32-bit floating-point audio
    want.channels = 1; // Mono audio
    want.samples = 4096;
    want.callback = audioCallback;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (dev == 0)
    {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_PauseAudioDevice(dev, 0); // Start audio playback
    }
}

APU::~APU()
{
    if (dev != 0)
    {
        SDL_CloseAudioDevice(dev);
    }
    SDL_Quit();
}

void APU::test()
{
    std::cout << "APU Test!" << std::endl;
}

void APU::sdlAudioTest()
{
    // Play a 440 Hz tone for 1 second
    SDL_Delay(1000);
}

void APU::audioCallback(void* userdata, Uint8* stream, int len)
{
    float* buffer = reinterpret_cast<float*>(stream);
    static double phase = 0.0;
    const double frequency = 440.0; // A4
    const double sampleRate = 44100.0;
    const double phaseIncrement = (2.0 * M_PI * frequency) / sampleRate;

    for (int i = 0; i < len / sizeof(float); ++i)
    {
        buffer[i] = 0.5f * sin(phase); // Generate a sine wave
        phase += phaseIncrement;
        if (phase >= 2.0 * M_PI)
        {
            phase -= 2.0 * M_PI;
        }
    }
}

int main()
{
    APU testAPU;
    testAPU.test();
    testAPU.sdlAudioTest();
    return 0;
}