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
    SDL_Delay(5000);
}

void APU::audioCallback(void* userdata, Uint8* stream, int len)
{
    float* buffer = reinterpret_cast<float*>(stream);
    static double phase = 0.0;
    static int currentDutyCycle = 0;  // Track which duty cycle we are on
    static int samplesElapsed = 0;    // Track samples elapsed to switch every second
    const double frequency = 440.0;   // A4
    const double sampleRate = 44100.0;
    const double phaseIncrementSquare = (frequency / sampleRate) * 8.0; // 8-step duty cycle
    const double phaseIncrementTriangle = (frequency / sampleRate) * 32.0; // 32-step triangle wave
    const int samplesPerSecond = static_cast<int>(sampleRate);  // 1 second of samples
    static bool useTriangleWave = false; // Switch to triangle wave after all square patterns

    // NES duty cycles (each is an 8-step pattern)
    const uint8_t dutyPatternsSquare[4] = {
        0b01000000,  // 12.5%
        0b01100000,  // 25%
        0b01111000,  // 50%
        0b01111110   // 75%
    };

    const uint8_t trianglePattern[32] = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    const float volume = 0.08f;

    for (int i = 0; i < len / sizeof(float); ++i)
    {
        if (!useTriangleWave)
        {
            int step = static_cast<int>(phase) & 7;  // Keep phase within 8 steps
            buffer[i] = (dutyPatternsSquare[currentDutyCycle] & (1 << step)) ? volume : -volume;

            phase += phaseIncrementSquare;
            if (phase >= 8.0)
            {
                phase -= 8.0;
            }

            // Track elapsed samples and switch duty cycle every second
            if (++samplesElapsed >= samplesPerSecond)
            {
                samplesElapsed = 0;
                currentDutyCycle = (currentDutyCycle + 1) % 4;  // Cycle through the 4 duty patterns
                if (currentDutyCycle == 0) // After completing one cycle of duty patterns, switch to triangle wave
                {
                    useTriangleWave = true;
                    phase = 0.0; // Reset phase for triangle wave
                }
            }
        }
        else
        {
            int step = static_cast<int>(phase) & 31;  // Keep phase within 32 steps
            buffer[i] = (trianglePattern[step] / 15.0f - 0.5f) * volume; // Normalize and scale

            phase += phaseIncrementTriangle;
            if (phase >= 32.0)
            {
                phase -= 32.0;
            }
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