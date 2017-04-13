#ifndef MAINH
#define MAINH


#define STAR_NUM 4000

#include "GLwrapper.h"

using namespace GLWrapper;

void printDebug(const char *format, ...);

void protoeffekt(int);
void modelzoom(int duration, Model mdl, Texture tex, int pattern, float offset);
void drumincrease(int duration, Model mdl, Texture tex, bool noice);
void noice();
void explode(int duration, Model mdl, Texture tex, float distance);
// void vectorslime(int duration, Model mdl, Model mdl_original, Texture tex, float amount);
void bumpmapmodel(int duration, Model mdl, Texture tex, Texture bump);
//void bumpyball(int starttime, int curtime, int progress, Model mdl, Model mdl_original, Texture tex, int style);
//void bumpyball(int progress, Texture tex, int style);

void cubegraphics(int duration, Model mdl, Texture tex, float speed);
void displaytext(double, double, double, char *);
void displayprogress(double, double, int pct);
void overlay(double x1, double y1, double x2, double y2, Texture tex);
void bloom(int duration, Model &mod, Texture tex, RenderTarget target1, RenderTarget target2, Texture vu, bool);

void vectorslime(int progress, Model &mdl, Model &mdl_original, float *distance_buffer, vertex *rotation_buffer, Texture tex, float amount);

void newbloom(int progress, Model &mod, Texture &tex, RenderTarget &target1, RenderTarget &target2, float bloomfactor);

void errorCallback();

#endif