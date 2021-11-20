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
int line_width = 2;
float fov;
float fov_pixels;
float hud_scale = 1.0;
float text_scale = 1.0;
float color[] = {0.0, 1.0, 0.0, 1.0};
int viggen_mode = 1;


void CalculateCenter(void) {
    int screen_width;
    int screen_height;
    
    XPLMGetScreenSize(&screen_width, &screen_height);
    // HudConfig* lConfig = getHudConfig();

    glTranslated(screen_width / 2, screen_height / 2, 0);
    fov = getFOV();
    fov_pixels = screen_height / fov;
    line_width = LINE_WIDTH*hud_scale;
}

void TranslateToCenter(void) {

    glTranslated(screen_width / 2, screen_height / 2, 0);
}

void initGlResources() {
    InitFonts();
}


void DrawCircle(int radius) {
  double angle;
  int i;
  glBegin(GL_LINE_LOOP);
  for(i = 0; i < CIRCLE_LINES_COUNT; i++)
  {
    angle = i*2*M_PI/CIRCLE_LINES_COUNT;
    glVertex2f((cos(angle) * radius), (sin(angle) * radius));
  }
  glEnd();
}

float CalcFOVAngle(float input) {
    float out = (input*fov_pixels) / hud_scale;
    
    return out;
}

float getLandingSpeed() {
    float mass = getTotalWeight();
    float delta = (landing_speed2 - landing_speed1) / (landing_weight2- landing_weight1);
    float speed = ( (mass - landing_weight1) * delta) + landing_speed1;
    return speed;
}

float to_degrees(float radians) {
    return radians * (180.0 / M_PI);
}

float myGetAlpha() {
    float vx = getVX();
    float vy = getVY();
    float vz = getVZ();
    float pitch = getPitch();
    
    float length = sqrt(vy *vy + vx*vx + vz*vz);
    if (length >1.0) {
        float alpha = asin(vy/length);
        alpha = pitch - to_degrees (alpha );
        return alpha ;
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
    float out = to_degrees ( atan2(-getVX(),getVZ()) ) + 180;
    
    return out ;
}

