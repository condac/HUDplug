
#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

extern int screen_width;
extern int screen_height;
extern float fov;
extern float fov_pixels;
extern float hud_scale;
extern float text_scale;
extern float line_width;
extern float color[4];
extern int viggen_mode;

void CalculateCenter(void);
void TranslateToCenter(void);
void initGlResources();
void DrawCircle(float radius);
void DrawCircleXY(float radius, float x, float y);

float CalcFOVAngle(float input);
float getLandingSpeed();
float myGetAlpha();

float myGetBeta();
float myGetHead();
float knotsTokmh(float in);

float feetTom(float in);

void CalculateColors();
float to_radians(float degrees);
float to_degrees(float radians);

#define max(a, b)                                                                                                                                                                  \
    ({                                                                                                                                                                             \
        __typeof__(a) _a = (a);                                                                                                                                                    \
        __typeof__(b) _b = (b);                                                                                                                                                    \
        _a > _b ? _a : _b;                                                                                                                                                         \
    })

#endif
