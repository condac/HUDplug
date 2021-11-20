#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

void DrawTest() {

    // Draw Text
    SetGLText(); // turn on blending
    float color[] = {0.0, 1.0, 0.0, 1.0};
    DrawHUDNumber((int)getHeading(), &fontMain, -3, 0, 300, 0, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());
}

#define MOVEMENT_ARROW_RADIUS_PX 20
#define MOVEMENT_ARROW_LEG_SIZE 10

void DrawVector() {
    float airspeed = getIAS();
    float y_pos = CalcFOVAngle(getAlphaA());
    float x_pos = CalcFOVAngle(getBetaA());
    float tail_pos = airspeed - getLandingSpeed();
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40);
    tail_pos = fmax(tail_pos, -40);
    //y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    glTranslatef(-x_pos, -y_pos, 0);

    DrawCircle(10 * HUD_SCALE);
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(30 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(-10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(-30 * HUD_SCALE, 0 * HUD_SCALE);

    if (gear) {
        glVertex2f(0 * HUD_SCALE, tail_pos - 10 * HUD_SCALE);
        glVertex2f(0 * HUD_SCALE, tail_pos + 10 * HUD_SCALE);
    }

    glEnd();

    glTranslatef(x_pos, y_pos, 0); // set position back

     char buffer[255];
     sprintf(buffer, "Alpha: %f, Beta %f, Pitch %f, FOV %f, FOVPixel %f, y_pos %f", getAlphaA(),getBetaA(), getPitch(), getFOV(), fov_pixels, y_pos);
      XPLMDrawString(color, -200, 300, buffer, NULL, xplmFont_Basic);
}

void DrawHorizionLines() {
    float y_pos = CalcFOVAngle(getPitch());
    float angle = getRoll();
    char tempText[10];
    float smallTextScale = 0.75;
    int gear = getGear();

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten

    glLineWidth(line_width);
    glBegin(GL_LINES);

    for (float i = -160; i < 160; i += 40) {
        glVertex2f((i)*HUD_SCALE, 0 * HUD_SCALE);
        glVertex2f((i + (line_width * 2)) * HUD_SCALE, 0 * HUD_SCALE);
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
    if (gear) {
        float yy = CalcFOVAngle(-2.86);

        // Heldragen linje vid hjorizonten när landställ är nere
        glLineWidth(line_width);
        glBegin(GL_LINES);
        glVertex2f(40 * HUD_SCALE, yy * HUD_SCALE);
        glVertex2f(200 * HUD_SCALE, yy * HUD_SCALE);
        
        glVertex2f(-40 * HUD_SCALE, yy * HUD_SCALE);
        glVertex2f(-200 * HUD_SCALE, yy * HUD_SCALE);
        glEnd();
    }

    for (int i = 10; i < 90; i += 10) {
        if ((i > getPitch() - 15) && (i < getPitch() + 10)) {
            glLineWidth(line_width);
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
        if ((i > getPitch() - 15) && (i < getPitch() + 10)) {
            glLineWidth(line_width);
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
        if ((i > getPitch() - 15) && (i < getPitch() + 10)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, -200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);

            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    for (int i = -10; i > -90; i -= 10) {
        if ((i > getPitch() - 15) && (i < getPitch() + 10)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, -200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);

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
    float tail_pos = airspeed - LANDING_SPEED;

    glColor4fv(color);

    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(SPEED_POS_X * HUD_SCALE, -100 * HUD_SCALE);
    glVertex2f(SPEED_POS_X * HUD_SCALE, 100 * HUD_SCALE);
    
    // Markör för landningshastighet
    if (airspeed < LANDING_SPEED+50 && airspeed > LANDING_SPEED-50) {
        tail_pos = -tail_pos*2;
        glVertex2f(SPEED_POS_X * HUD_SCALE, tail_pos * HUD_SCALE);
        glVertex2f(SPEED_POS_X +10 * HUD_SCALE, tail_pos * HUD_SCALE);
    }
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
    DrawHUDText(temp, &fontMain, (SPEED_POS_X - 20) * HUD_SCALE, (SPEED_POS_Y * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 1, color);

    sprintf(temp, "M %.2f", mach);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale)), 1, color);

    sprintf(temp, "GS%.0f", groundspeed);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);
    
    if (airspeed < LANDING_SPEED+50 && airspeed > LANDING_SPEED-50) {
        
        
        DrawHUDText("L", &fontMain, SPEED_POS_X +10 * HUD_SCALE, tail_pos * HUD_SCALE - ((fontMain.charHeight/2 * text_scale) ), 0, color);
        
    }
    
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha() {
    char temp[20];
    float alpha = getAlphaA();
    float gforce = getGForce();

    glColor4fv(color);

    SetGLText(); // turn on blending

    sprintf(temp, "G %.1f", gforce);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale)), 1, color);

    sprintf(temp, "$ %.0f", alpha);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale) * 2), 1, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

#define ALT_POS_X 315
#define ALT_POS_Y 0

void DrawAltitude() {
    char temp[20];
    float altitude = getAltitude();

    glColor4fv(color);
    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
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
    DrawHUDText(temp, &fontMain, (ALT_POS_X - 20) * HUD_SCALE, (ALT_POS_Y * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 1, color);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawViggen() {
    // Viggen mode
    float airspeed = getIAS();
    float altitude = getAltitude();
    float alpha = getAlphaA();
    //float gforce = getGForce();
    float mach = getMachSpeed();
    float y_pos = CalcFOVAngle(alpha);
    float x_pos = CalcFOVAngle(getBetaA());
    float heading = getHeading();
    float angle = getRoll();
    char tempText[32];
    float smallTextScale = 0.85;
    float tail_pos = airspeed - LANDING_SPEED;
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40);
    tail_pos = fmax(tail_pos, -40);
    y_pos = fov_pixels * alpha;
    
    glColor4fv(color);

    glTranslatef(-x_pos, -y_pos, 0);

    DrawCircle(10 * HUD_SCALE);
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(40 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(-10 * HUD_SCALE, 0 * HUD_SCALE);
    glVertex2f(-40 * HUD_SCALE, 0 * HUD_SCALE);

    glVertex2f(0 * HUD_SCALE, tail_pos - 10 * HUD_SCALE);
    glVertex2f(0 * HUD_SCALE, tail_pos + 10 * HUD_SCALE);

    glEnd();

    glTranslatef(x_pos, y_pos, 0); // set position back

    // Horizontal lines

    y_pos = CalcFOVAngle(getPitch());

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten

    // Prickar
    // for (float i = -160; i < 160; i += 40) {
    //     glVertex2f((i)*HUD_SCALE, 0 * HUD_SCALE);
    //     glVertex2f((i + (line_width * 2)) * HUD_SCALE, 0 * HUD_SCALE);
    // }

    // 0 horizonten
    float yy = 0;
    float compas_y = CalcFOVAngle(-4);
    if (gear) {
        yy = CalcFOVAngle(-3);
        compas_y = CalcFOVAngle(-0.5);

        // Heldragen linje vid hjorizonten när landställ är nere
        glLineWidth(line_width);
        glBegin(GL_LINES);
        glVertex2f(-480 * HUD_SCALE, CalcFOVAngle(0) * HUD_SCALE);
        glVertex2f(480 * HUD_SCALE, CalcFOVAngle(0) * HUD_SCALE);
        glEnd();
    }
    glLineWidth(line_width);
    glBegin(GL_LINES);

    // Prick i mitten
    glVertex2f(-line_width * HUD_SCALE, yy * HUD_SCALE);
    glVertex2f(line_width * HUD_SCALE, yy * HUD_SCALE);

    glVertex2f(45 * HUD_SCALE, yy * HUD_SCALE);
    glVertex2f(480 * HUD_SCALE, yy * HUD_SCALE);

    glVertex2f(-45 * HUD_SCALE, yy * HUD_SCALE);
    glVertex2f(-480 * HUD_SCALE, yy * HUD_SCALE);

    glEnd();

    // Altitude text på 0 horizonten
    SetGLText(); // turn on blending
    glPushMatrix();
    //glScalef(smallTextScale, smallTextScale, 0);

    int alt = altitude * 0.3048;
    alt = alt / 10;
    alt = alt * 10;
    snprintf(tempText, 13, "%03d", alt);
    DrawHUDText(tempText, &fontMain, 200 * HUD_SCALE , yy , 1, color);

    glPopMatrix();
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    // linjer
    glColor4fv(color);
    for (int i = 5; i < 90; i += 5) {
        if ((i > getPitch() - 8) && (i < getPitch() + 8)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-400 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            glEnd();
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    int start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > getPitch() - 8) && (i < getPitch() + 8)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(200 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-400 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-200 * HUD_SCALE, CalcFOVAngle(i));
            glEnd();
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }

    // Compas lines
    glLineWidth(line_width);
    glBegin(GL_LINES);
    for (int i = 0; i < 37; i++) {

        float offset = (i * 10) - heading + 5;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 90) && (offset > -90)) {
            offset = CalcFOVAngle(offset);
            glVertex2f(offset, compas_y * HUD_SCALE);
            glVertex2f(offset, (compas_y + 20) * HUD_SCALE);
        }
    }
    glEnd();

    SetGLText(); // turn on blending
    glPushMatrix();
    glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 5; i < 90; i += 5) {
        if ((i > getPitch() - 8) && (i < getPitch() + 8)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }
    start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > getPitch() - 8) && (i < getPitch() + 8)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }

    // Compas lines text
    for (int i = 0; i < 37; i++) {

        float offset = (i * 10) - heading;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 90) && (offset > -90)) {
            offset = CalcFOVAngle(offset);
            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, offset / smallTextScale, compas_y / smallTextScale * HUD_SCALE, 1, color);
        }
    }
    glPopMatrix();


    

    sprintf(tempText, "$ %.0f", alpha); // $ is replaced with alpha sign in bitmap
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale) * 2), 1, color);

    sprintf(tempText, "%.0f", airspeed);
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) , 1, color);
    
    sprintf(tempText, "M %.2f", mach);
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale)), 1, color);

    
    
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    glTranslatef(0, y_pos, 0);
    glRotatef(-angle, 0, 0, 1);
}
