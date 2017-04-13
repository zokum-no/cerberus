#ifndef STARH
#define STARH

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLWrapper.h"

#include <math.h>

#include <cstdlib>
#include <ctime>

#include "bass.h"
#include "basswrap.h"

#include "main.h"

void initstarfield(float starfield[4000][6]);
void starfield(DWORD progress, float starfield[4000][6],  Texture &star, DWORD rotation);

extern float sync[512];
extern float oldsync[512];
extern float ancientsync[512];

#endif