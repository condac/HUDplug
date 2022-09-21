#define SPEED_POS_X -250
#define SPEED_POS_Y -50
#define SPEED_SCALE_PIXEL 150.0f

#define ALT_POS_X 255
#define ALT_POS_Y -50
#define ALT_SCALE_METER 450.0f
#define ALT_SCALE_PIXEL 150.0f

void DrawTest();
void DrawVector();
void DrawMovementArrow(float trueHeading, float vx, float vy, float vz);
void DrawHorizionLines();
void DrawAltitude(float x, float y);
void DrawSpeed(float x, float y);
void DrawAlpha(float x, float y);

void DrawGlass();
void DrawGroundCollision();
void DrawFuelTime(float x, float y);
void DrawCompass(float x, float y);
void DrawNAVText(float x, float y);
void DrawHorizionLinesViggen();
void DrawCompassViggen(float x, float y);
void drawSpeedAlphaViggen(float x, float y);
void drawADHelp();

float interpolate(float x1, float y1, float x2, float y2, float value);
float constrain(float val, float lower, float upper);
