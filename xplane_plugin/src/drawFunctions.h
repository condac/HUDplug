
#ifndef DRAWFUNC_H_INCLUDED
#define DRAWFUNC_H_INCLUDED

extern int screen_width;
extern int screen_height;
extern float fov;
extern float fov_pixels;

extern float line_width;
extern float color[4];
extern int viggen_mode;
#define SIZE_OF_BLENDMODES 15
extern int blendmodes[SIZE_OF_BLENDMODES];

int CalculateCenter(void);
void TranslateToCenter(void);
void initGlResources();
void DrawCircle(float radius);
void DrawCircleXY(float radius, float x, float y);
void DrawFillCircleXY(float radius, float x, float y);
void DrawCircleDown(float radius, float x, float y, float heading);
void DrawCircleUp(float radius, float x, float y, float heading);

float CalcFOVAngle(float input);
float getLandingSpeed();
float myGetAlpha();

float myGetBeta();
float myGetHead();

float knotsTokmh(float in);
float kmhToknots(float in);
float feetTom(float in);
float m2feet(float in);
float nm2km(float nm);

void CalculateColors();
float to_radians(float degrees);
float to_degrees(float radians);
float lim(float value, float lower, float upper);

float max2(float value1, float value2);

#endif
