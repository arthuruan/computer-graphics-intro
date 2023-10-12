#include "audio.h"

Audio::Audio(){
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
        printf("Failed to initialize audio engine.");

    for (int i = 0; i < 2; i++) {
        result = ma_sound_init_from_file(&engine, files[i], MA_SOUND_FLAG_DECODE, NULL, NULL, &sounds[i]);

        if (result != MA_SUCCESS)
            printf("Failed to open %s", files[i]);
    }
}

Audio::~Audio(){
    ma_engine_uninit(&engine);
    for (int i = 0; i < 2; i++)
        ma_sound_uninit(&sounds[i]);
}

void Audio::goal(){
    ma_sound_start(&sounds[0]);
}

void Audio::hit(){
    ma_sound_start(&sounds[1]);
}