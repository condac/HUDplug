
#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

extern int screen_width;
extern int screen_height;
extern float fov;
extern float fov_pixels;
extern float hud_scale;
extern float text_scale;
extern int line_width;
extern float color[4];
extern int viggen_mode;

void CalculateCenter(void);
void TranslateToCenter(void);
void initGlResources();
void DrawCircle(int radius);

float CalcFOVAngle(float input);
float getLandingSpeed();

#endif
