
#pragma once

#include "bass.h"

void basswrap_init(HWND wnd);
void basswrap_load(char *filename);
void basswrap_unload();
void basswrap_play(double);
void basswrap_playsample();
void basswrap_loadsample();
void basswrap_uninit();
float basswrap_gettime();
void basswrap_volume(int volume);

void basswrap_getsync(float* data);


// zokum
void basswrap_play_seconds(double in);