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

int screen_width = 0;
int screen_height = 0;
float line_width = 2.0;
float fov;
float fov_pixels;

float color[] = {-0.5, 1.0, -0.5, 1.0};
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

int CalculateCenter(void) {
    int screen_width;
    int screen_height;
    
    
    XPLMGetScreenSize(&screen_width, &screen_height);
    
    //debugLog("window geo %d %d %d %d", b[0], b[1], b[2], b[3]);
    //debugLog("ScreenSize %d %d", screen_width,screen_height);
    screen_width = screen_width;
    screen_height = screen_height;
    // HudConfig* lConfig = getHudConfig();
    float FOV_off_y = getFOVoff_y();
    float FOV_off_x = getFOVoff_x();
    float ratio = (float)screen_height / (float)screen_width;
    //float scalex = screen_width/(1024 +getPanelL() );
    fov = getFOV();
    float fovscale = 30/getFOV_x();
    float scissor_x = fovscale*screen_width;
    float scissor_y = 20/getFOV_x()*screen_width*0.8671875;
    
    if (getViewType() == 1000) {
        fov_pixels = (0.8671875*1024/getFOV_x()/fovscale); //*24/getFOV_x()
        //fov_pixels = 11.08/fovscale;
    }else {
        fov_pixels = screen_height / fov / hud_scale;
    }
    
    line_width = line_scale * hud_scale;
    CalculateColors();
    FOV_off_y = CalcFOVAngle(20)* hud_scale;
    FOV_off_x = CalcFOVAngle(30) * hud_scale;

    // if (FOV_off_x > 1000 || FOV_off_x < -1000) {
    //     return -1;
    // }

    //glTranslated((screen_width / 2) - FOV_off_x, (screen_height / 2) - FOV_off_y, 0);
    //glTranslated(512, 512*ratio , 0);
    
    if (getViewType() == 1000) {
        glScissor(getPanelL()+screen_width / 2 -scissor_x/2* hud_scale,
                screen_height/2 -scissor_y,
                scissor_x* hud_scale,
                scissor_x*0.8671875* hud_scale);
    }else {
        glScissor((screen_width / 2) - (glass_width * hud_scale / 2) + offset_x - FOV_off_x,
                  -FOV_off_y,
                  offset_x + glass_width * hud_scale,
                  offset_y + screen_height / 2 + glass_height * hud_scale / 2);
    }
    
    
    //glEnable(GL_SCISSOR_TEST);
    return 1;
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
void DrawBanaXY(float radius, float x, float y) {
    
    glBegin(GL_LINES);
    glVertex2f(x+radius, y);
    glVertex2f(x-radius, y);

    glVertex2f(x+radius*0.8, y+radius*1.2);
    glVertex2f(x+radius, y);

    glVertex2f(x-radius*0.8, y+radius*1.2);
    glVertex2f(x-radius, y);
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
    if (value1 > value2) {
        return value1;
    }
    return value2;
}

void DrawGlassObject(float width) {
    
    float scale = width/280;
    
    glPushMatrix();
    glTranslatef(width, width*0.857142857143f, 0);
    glBegin(GL_POLYGON);
    //glColor4fv(colorglass);
    glTexCoord2f(0.142857142857f, 0.0f); 
    glVertex2f(-100*scale, -240*scale);
    glTexCoord2f(0.0f, 0.321428571429f); 
    glVertex2f(-140*scale, -150*scale);

    glTexCoord2f(0.0f, 0.660714285714f);
    glVertex2f(-140*scale, -55*scale);
    glTexCoord2f(0.121428571429f, 0.798214285714f); 
    glVertex2f(-106*scale, -16.5*scale);

    // böjen
    glTexCoord2f(0.257142857143f, 0.835428571429f); 
    glVertex2f(-68*scale, -6.08*scale);
    glTexCoord2f(0.364285714286f, 0.849714285714f); 
    glVertex2f(-38*scale, -2.08*scale);
    glTexCoord2f(0.5f, 0.857142857143f); 
    glVertex2f(-0*scale, -0.00*scale);

    // andra sidan
    glTexCoord2f(1.0f-0.364285714286f, 0.849714285714f); 
    glVertex2f(38*scale, -2.08*scale);
    glTexCoord2f(1.0f-0.257142857143f, 0.835428571429f); 
    glVertex2f(68*scale, -6.08*scale);
    
    glTexCoord2f(1.0f-0.121428571429f, 0.798214285714f); 
    glVertex2f(106*scale, -16.5*scale);
    glTexCoord2f(1.0f, 0.660714285714f); 
    glVertex2f(140*scale, -55*scale);

    glTexCoord2f(1.0f, 0.321428571429f); 
    glVertex2f(140*scale, -150*scale);
    glTexCoord2f(1.0f-0.142857142857f, 0.0f);
    glVertex2f(100*scale, -240*scale);

    glEnd();
    glPopMatrix();
}
