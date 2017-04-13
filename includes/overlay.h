#ifndef OVERLAYH
#define OVERLAYH

#define OVERLAY_STYLE_BOTTOM 1

void overlay(double x1, double y1, double x2, double y2, Texture tex);
void overlay_advanced(double x1, double y1, double z1, double x2, double y2, double z2, Texture tex, double xtex1, double ytex1, double xtex2, double ytex2);
void overlay_advanced_stretchfade(DWORD progress, int style, double x1, double y1, double z1, double x2, double y2, double z2, Texture tex, double xtex1, double ytex1, double xtex2, double ytex2);
//void overlay_clockstyle(DWORD progress, float x1, float y1, float x2, float y2, Texture &tex, float start);
#endif