#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

void DrawTest() {
    glLineWidth(3);
    int line_length = 20;
    int box_size = 200;
    float lSize = 1;
    glBegin(GL_LINES);
    // left bottom
    glVertex2i(0, 0);
    glVertex2f(line_length * lSize, 0);
    glVertex2i(0, 0);
    glVertex2f(0, line_length * lSize);
    // left top
    glVertex2f(0, box_size * lSize);
    glVertex2f(line_length * lSize, box_size * lSize);
    glVertex2f(0, box_size * lSize);
    glVertex2f(0, box_size * lSize - line_length * lSize);
    // right bottom
    glVertex2f(box_size * lSize, 0);
    glVertex2f(box_size * lSize - line_length * lSize, 0);
    glVertex2f(box_size * lSize, 0);
    glVertex2f(box_size * lSize, line_length * lSize);
    // right top
    glVertex2f(box_size * lSize, box_size * lSize);
    glVertex2f(box_size * lSize - line_length * lSize, box_size * lSize);
    glVertex2f(box_size * lSize, box_size * lSize);
    glVertex2f(box_size * lSize, box_size * lSize - line_length * lSize);
    glEnd();

    // Draw Text
    SetGLText(); // turn on blending
    float color[] = {0.0, 1.0, 0.0, 1.0};
    DrawHUDNumber((int)getHeading(), &fontBig, -3, 0, lSize / 2 - fontBig.charHeight, 0, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());
}

#define MOVEMENT_ARROW_RADIUS_PX 20
#define MOVEMENT_ARROW_LEG_SIZE 10

void DrawVector() {
    float color[] = {0.0, 1.0, 1.0};
    glColor3fv(color);

    DrawCircle(20);
}

#define SPEED_POS_X -315
#define SPEED_POS_Y 0

void DrawSpeed() {
    char temp[20];
    float airspeed = getIAS();
    float groundspeed = 135.0;
    float mach = 0.21;
    float color[] = {0.0, 1.0, 0.0, 1.0};

    // lodrät linje för hastighetsmätaren
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);

    glVertex2f(SPEED_POS_X * HUD_SCALE, -100 * HUD_SCALE);
    glVertex2f(SPEED_POS_X * HUD_SCALE, 100 * HUD_SCALE);

    // pil vid hastighetstexten
    glVertex2f(SPEED_POS_X * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f((SPEED_POS_X - 20) * HUD_SCALE, 10 * HUD_SCALE);

    glVertex2f(SPEED_POS_X * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f((SPEED_POS_X - 20) * HUD_SCALE, -10 * HUD_SCALE);

    glEnd();

    SetGLText(); // turn on blending
    sprintf(temp, "%.0f", airspeed);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X-20) * HUD_SCALE, (SPEED_POS_Y * HUD_SCALE) - (fontBig.charHeight/2), 1, color);
    
    sprintf(temp, "M %.2f", mach);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X) * HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - (fontBig.charHeight), 1, color);
    
    sprintf(temp, "GS%.0f", groundspeed);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X) * HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - (fontBig.charHeight*2), 1, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawMovementArrow(float trueHeading, float vx, float vy, float vz) {
    float speed; // in kias (or m/s)
    float angle;
    float draw_speed; // in pixels
    float lSize = 1.0;
    // todo: add wind to vx and vy
    vz = -vz;
    speed = sqrt(vx * vx + vz * vz);

    if (speed < 0.5) {
        speed = 0;
        angle = 0;
        return;
    }
    angle = asin(vz / speed) * 180 / M_PI;
    // convert angle from x-axis to angle from Z axis (heading 0)
    if (vx >= 0)
        // 1st & 2nd quandrant
        angle = 90 - angle;
    else
        //3rd & 4th
        angle = 270 + angle;
    // take heading into account
    angle -= (trueHeading);
    // translation already done, we have center in screen center
    draw_speed = MOVEMENT_ARROW_RADIUS_PX * lSize * speed / 1;
    if (draw_speed > MOVEMENT_ARROW_RADIUS_PX * lSize)
        draw_speed = MOVEMENT_ARROW_RADIUS_PX * lSize;
    glRotatef(-angle, 0, 0, 1);
    glBegin(GL_LINES);
    if (speed <= 0) {
        glVertex2f(0, 0);
        glVertex2f(0, draw_speed);
    }
    // arrow
    glVertex2f(0, draw_speed);
    glVertex2f(-MOVEMENT_ARROW_LEG_SIZE * lSize, draw_speed - 2 * MOVEMENT_ARROW_LEG_SIZE * lSize);
    glVertex2f(0, draw_speed);
    glVertex2f(MOVEMENT_ARROW_LEG_SIZE * lSize, draw_speed - 2 * MOVEMENT_ARROW_LEG_SIZE * lSize);
    glEnd();
    glRotatef(angle, 0, 0, 1);

    float color[] = {1.0, 1.0, 1.0}; /* RGB White */
    char buffer[255];
    sprintf(buffer, "True heading: %f", trueHeading);
    XPLMDrawString(color, -200, 200, buffer, NULL, xplmFont_Basic);
    sprintf(buffer, "Angle: %f", angle);
    XPLMDrawString(color, -200, 180, buffer, NULL, xplmFont_Basic);
    sprintf(buffer, "Vx: %f", vx);
    XPLMDrawString(color, -200, 160, buffer, NULL, xplmFont_Basic);
    sprintf(buffer, "Vz: %f", vz);
    XPLMDrawString(color, -200, 140, buffer, NULL, xplmFont_Basic);
}