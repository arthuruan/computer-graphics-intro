#ifndef AUDIO_H
#define AUDIO_H
#define MINIAUDIO_IMPLEMENTATION
#include <string>
#include <stdio.h>
#include "miniaudio.h"

using namespace std;

class Audio{
    private:
        char goalPath[16] = "assets/goal.wav";
        char hitPath[15] = "assets/hit.wav";
        char* files[2] = {goalPath, hitPath};
        ma_sound sounds[2];
        ma_engine engine;
        ma_result result;

    public:
        Audio();
        ~Audio();

        void goal();
        void hit();
};

#endif