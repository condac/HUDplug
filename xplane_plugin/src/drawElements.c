#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

float color[] = {0.0, 1.0, 0.0, 1.0};

void DrawTest() {

    // Draw Text
    SetGLText(); // turn on blending
    float color[] = {0.0, 1.0, 0.0, 1.0};
    DrawHUDNumber((int)getHeading(), &fontBig, -3, 0, 300, 0, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());
}

#define MOVEMENT_ARROW_RADIUS_PX 20
#define MOVEMENT_ARROW_LEG_SIZE 10

void DrawVector() {
    float y_pos = CalcFOVAngle(getAlphaA());
    float x_pos = CalcFOVAngle(getBetaA());
    float tail_pos = 0.0;
    y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    glTranslatef(-x_pos, -y_pos, 0);

    DrawCircle(10 * HUD_SCALE);
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);

    glVertex2f(10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(30 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(-10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(-30 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(0 * HUD_SCALE, tail_pos + 10 * HUD_SCALE);
    glVertex2f(0 * HUD_SCALE, tail_pos + 30 * HUD_SCALE);

    glEnd();

    glTranslatef(x_pos, y_pos, 0); // set position back

    // char buffer[255];
    // sprintf(buffer, "Alpha: %f, Pitch %f, FOV %f, FOVPixel %f, y_pos %f", getAlphaA(), getPitch(), getFOV(), fov_pixels, y_pos);
    //  XPLMDrawString(color, -200, 300, buffer, NULL, xplmFont_Basic);
}

void DrawHorizionLines() {
    float y_pos = CalcFOVAngle(getPitch());
    float angle = getRoll();
    char tempText[10];
    float smallTextScale = 0.75;

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten

    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);

    for (float i = -160; i < 160; i += 40) {
        glVertex2f((i)*HUD_SCALE, 0 * HUD_SCALE);
        glVertex2f((i + (LINE_WIDTH * 2)) * HUD_SCALE, 0 * HUD_SCALE);
    }
    glVertex2f(160 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(295 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(410 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(480 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(-160 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(-295 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(-410 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(-480 * HUD_SCALE, 0 * HUD_SCALE);

    glEnd();

    // linjer

    for (int i = 10; i < 90; i += 10) {
        if  ( (i > getPitch() - 10) && (i < getPitch() + 10) ){
            glLineWidth(LINE_WIDTH);
            glBegin(GL_LINES);

            glVertex2f(40 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-40 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            glEnd();
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }

    for (int i = -10; i > -90; i -= 10) {
        if  ( (i > getPitch() - 10) && (i < getPitch() + 10) ){
            glLineWidth(LINE_WIDTH);
            glBegin(GL_LINES);

            glVertex2f(40 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-40 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            glEnd();
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }

    SetGLText(); // turn on blending
    glPushMatrix();
    glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 10; i < 90; i += 10) {
        if  ( (i > getPitch() - 10) && (i < getPitch() + 10) ){
            // glLineWidth(LINE_WIDTH);
            // glBegin(GL_LINES);
            //
            // glVertex2f(40 * HUD_SCALE, CalcFOVAngle(i));
            // glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));
            //
            // glVertex2f(-40 * HUD_SCALE, CalcFOVAngle(i));
            // glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            // glEnd();

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontBig, -200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);

            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    for (int i = -10; i > -90; i -= 10) {
        if  ( (i > getPitch() - 10) && (i < getPitch() + 10) ){
            // glLineWidth(LINE_WIDTH);
            // glBegin(GL_LINES);
            //
            // glVertex2f(40 * HUD_SCALE, CalcFOVAngle(i));
            // glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));
            //
            // glVertex2f(-40 * HUD_SCALE, CalcFOVAngle(i));
            // glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            // glEnd();

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontBig, -200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);

            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    glPopMatrix();
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    glTranslatef(0, y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    // char buffer[255];
    // sprintf(buffer, "Roll: %f, Pitch %f, FOV %f, FOVPixel %f, y_pos %f", getRoll(), getPitch(), getFOV(), fov_pixels, y_pos);
    // XPLMDrawString(color, -250, 350, buffer, NULL, xplmFont_Basic);
}

#define SPEED_POS_X -315
#define SPEED_POS_Y 0

void DrawSpeed() {
    char temp[20];
    float airspeed = getIAS();
    float groundspeed = getGroundSpeed() * 1.944;
    float mach = getMachSpeed();
    
    glColor4fv(color);

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
    DrawHUDText(temp, &fontBig, (SPEED_POS_X - 20) * HUD_SCALE, (SPEED_POS_Y * HUD_SCALE) - (fontBig.charHeight / 2), 1, color);

    sprintf(temp, "M %.2f", mach);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - (fontBig.charHeight), 1, color);

    sprintf(temp, "GS%.0f", groundspeed);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - (fontBig.charHeight * 2), 1, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha() {
    char temp[20];
    float alpha = getAlphaA();
    float gforce = getGForce();
    
    glColor4fv(color);

    

    SetGLText(); // turn on blending

    sprintf(temp, "G %.1f", gforce);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + (fontBig.charHeight), 1, color);

    sprintf(temp, "a %.0f", alpha);
    DrawHUDText(temp, &fontBig, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + (fontBig.charHeight * 2), 1, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

#define ALT_POS_X 315
#define ALT_POS_Y 0

void DrawAltitude() {
    char temp[20];
    float altitude = getAltitude();
    
    glColor4fv(color);
    // lodrät linje för hastighetsmätaren
    glLineWidth(LINE_WIDTH);
    glBegin(GL_LINES);

    glVertex2f(ALT_POS_X * HUD_SCALE, -100 * HUD_SCALE);
    glVertex2f(ALT_POS_X * HUD_SCALE, 100 * HUD_SCALE);

    // pil vid hastighetstexten
    glVertex2f(ALT_POS_X * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, 10 * HUD_SCALE);

    glVertex2f(ALT_POS_X * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, -10 * HUD_SCALE);

    glEnd();

    SetGLText(); // turn on blending
    sprintf(temp, "%.0f", altitude);
    DrawHUDText(temp, &fontBig, (ALT_POS_X - 10) * HUD_SCALE, (ALT_POS_Y * HUD_SCALE) - (fontBig.charHeight / 2), 1, color);

    
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

    // float color[] = {1.0, 1.0, 1.0}; /* RGB White */
    // char buffer[255];
    // sprintf(buffer, "True heading: %f", trueHeading);
    // XPLMDrawString(color, -200, 200, buffer, NULL, xplmFont_Basic);
    // sprintf(buffer, "Angle: %f", angle);
    // XPLMDrawString(color, -200, 180, buffer, NULL, xplmFont_Basic);
    // sprintf(buffer, "Vx: %f", vx);
    // XPLMDrawString(color, -200, 160, buffer, NULL, xplmFont_Basic);
    // sprintf(buffer, "Vz: %f", vz);
    // XPLMDrawString(color, -200, 140, buffer, NULL, xplmFont_Basic);
}