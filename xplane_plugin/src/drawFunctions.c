#include "HUDplug.h"


#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "drawFunctions.h"
#include "fonts.h"

#define CIRCLE_LINES_COUNT 50


int screen_width;
int screen_height;

void CalculateCenter(void) {
    int screen_width;
    int screen_height;
    
    XPLMGetScreenSize(&screen_width, &screen_height);
    // HudConfig* lConfig = getHudConfig();

    glTranslated(screen_width / 2, screen_height / 2, 0);
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