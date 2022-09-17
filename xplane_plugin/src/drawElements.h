
void DrawTest();
void DrawVector();
void DrawMovementArrow(float trueHeading, float vx, float vy, float vz);
void DrawHorizionLines();
void DrawAltitude(float x, float y);
void DrawSpeed(float x, float y);
void DrawAlpha(float x, float y);
void DrawViggen();
void DrawViggenMode2();
void DrawModesJAS();
void DrawGlass();
void DrawGroundCollision();
void DrawFuelTime(float x, float y);
void DrawCompass(float x, float y);

float interpolate(float x1, float y1, float x2, float y2, float value);
float constrain(float val, float lower, float upper);