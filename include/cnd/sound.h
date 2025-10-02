#pragma once

#include <cnd/types.h>

extern const music_t g_musicOff;
extern const sfx_t g_sfxOff;

void sound_clear();
void sound_push_sfx(sfx_t const* sfx);
void sound_push_music(music_t const* music);
void sound_play();
void sound_stop_music();
