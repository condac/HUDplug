#include "HUDplug.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "drawFunctions.h"
#include "fonts.h"
#include "datarefs.h"

#define CIRCLE_LINES_COUNT 50

int screen_width;
int screen_height;
float line_width = 2.0;
float fov;
float fov_pixels;

float color[] = {0.0, 1.0, 0.0, 1.0};
int viggen_mode = 1;

int blendmodes[] = {GL_ZERO,
                    GL_ONE,
                    GL_SRC_COLOR,
                    GL_ONE_MINUS_SRC_COLOR,
                    GL_DST_COLOR,
                    GL_ONE_MINUS_DST_COLOR,
                    GL_SRC_ALPHA,
                    GL_ONE_MINUS_SRC_ALPHA,
                    GL_DST_ALPHA,
                    GL_ONE_MINUS_DST_ALPHA,
                    GL_CONSTANT_COLOR,
                    GL_ONE_MINUS_CONSTANT_COLOR,
                    GL_CONSTANT_ALPHA,
                    GL_ONE_MINUS_CONSTANT_ALPHA,
                    GL_SRC_ALPHA_SATURATE};

void CalculateCenter(void) {
    int screen_width;
    int screen_height;

    XPLMGetScreenSize(&screen_width, &screen_height);
    // HudConfig* lConfig = getHudConfig();

    glTranslated(screen_width / 2, screen_height / 2, 0);
    fov = getFOV();
    fov_pixels = screen_height / fov;
    line_width = line_scale * hud_scale;
    CalculateColors();

    glScissor((screen_width / 2) - (glass_width * hud_scale / 2) + offset_x, 0, offset_x + glass_width * hud_scale, offset_y + screen_height / 2 + glass_height * hud_scale / 2);
    glEnable(GL_SCISSOR_TEST);
}

void CalculateColors() {
    if (th1) {
        float var1 = getThrottlePos();

        if (var1 < 0.1) {
            color[0] = 1.0;
            color[1] = 0.0;
            color[2] = 0.0;
            color[3] = 1.0;

        } else if (var1 < 0.9) {
            color[0] = 0.0;
            color[1] = var1;
            color[2] = 0.0;
            color[3] = 1.0;
        } else if (var1 <= 1.0) {
            color[0] = (var1 - 0.9) * 2;
            color[1] = var1;
            color[2] = 0.0;
            color[3] = 1.0;
        }
    }
}

float knotsTokmh(float in) {
    return in * 1.85200;
}
float kmhToknots(float in) {
    return in / 1.85200;
}

float feetTom(float in) {
    return in * 0.3048;
}
float m2feet(float in) {
    return in / 0.3048;
}
void TranslateToCenter(void) {

    glTranslated(screen_width / 2, screen_height / 2, 0);
}

void initGlResources() {
    InitFonts();
}

void DrawCircle(float radius) {
    double angle;
    int i;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < CIRCLE_LINES_COUNT; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f((cos(angle) * radius), (sin(angle) * radius));
    }
    glEnd();
}
void DrawCircleXY(float radius, float x, float y) {
    double angle;
    int i;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < CIRCLE_LINES_COUNT; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}
void DrawFillCircleXY(float radius, float x, float y) {
    double angle;
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < CIRCLE_LINES_COUNT - 1; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }

    glEnd();
}
void DrawCircleDown(float radius, float x, float y, float heading) {
    // Cirklarna som ska ha en avbrytning med pil som visas när man åker mot marken
    double angle;
    int i;
    float a = 15; // bredden på pilen i pixlar
    int segments = 10;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < (360 / segments); i++) {
        float aoff = asin(a / radius);
        angle = aoff * 2 + ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));

        angle = aoff + ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * (radius - 40)), y + (sin(angle) * (radius - 40)));
        angle = ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}

void DrawCircleUp(float radius, float x, float y, float heading) {
    double angle;
    int i;
    int segments = 2;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < (360 / segments); i++) {
        angle = (-i - heading / segments) * 2 * M_PI / (360 / segments);
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
    segments = 10;
    glBegin(GL_LINES);
    for (i = 0; i < (360 / segments); i++) {
        angle = (i - heading / segments) * 2 * M_PI / (360 / segments);
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
        glVertex2f(x + (cos(angle) * (radius - 10)), y + (sin(angle) * (radius - 10)));
    }
    glEnd();
}

float CalcFOVAngle(float input) {
    float out = (input * fov_pixels) / hud_scale;

    return out;
}

float getLandingSpeed() {
    float mass = getTotalWeight();
    float delta = (landing_speed2 - landing_speed1) / (landing_weight2 - landing_weight1);
    float speed = ((mass - landing_weight1) * delta) + landing_speed1;
    return speed;
}

float to_degrees(float radians) {
    return radians * (180.0 / M_PI);
}

float to_radians(float degrees) {
    return degrees / (180.0 / M_PI);
}

float nm2km(float nm) {
    return nm * 1.852;
}

float myGetAlpha() {
    float vx = getVX();
    float vy = getVY();
    float vz = getVZ();
    float pitch = getPitch();

    float length = sqrt(vy * vy + vx * vx + vz * vz);
    if (length > 1.0) {
        float alpha = asin(vy / length);
        alpha = pitch - to_degrees(alpha);
        return alpha;
    } else {
        return 0.0;
    }
}

float myGetBeta() {
    float beta = getTrueHeading() - myGetHead();
    if (beta < -180) {
        beta += 360;
    }
    if (beta > 180) {
        beta -= 360;
    }
    return beta;
}

float myGetHead() {
    float out = to_degrees(atan2(-getVX(), getVZ())) + 180;

    return out;
}

float lim(float value, float lower, float upper) {
    if (lower > upper) {
        float tmp = lower;
        lower = upper;
        upper = tmp;
    }
    if (value < lower) {
        return lower;
    }
    if (value > upper) {
        return upper;
    }
    return value;
}

float max2(float value1, float value2) {
    if (value1>value2) {
        return value1;
    }
    return value2;
}
