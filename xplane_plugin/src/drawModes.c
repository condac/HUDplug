#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "drawModes.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

void DrawViggen() {
    // Viggen mode
    float airspeed = getIAS();
    float altitude = getAltitude();
    float alpha = myGetAlpha();
    //float beta = myGetBeta();
    //float gforce = getGForce();
    float mach = getMachSpeed();
    float y_pos = CalcFOVAngle(alpha);
    //float x_pos = CalcFOVAngle(beta);
    float heading = getHeading();
    float angle = getRoll();
    float pitch = getPitch();
    char tempText[32];
    float smallTextScale = 0.85;
    float tail_pos = airspeed - LANDING_SPEED;
    int gear = getGear();
    float vdef = getILSv();

    tail_pos = fmin(tail_pos, 40);
    tail_pos = fmax(tail_pos, -40);
    y_pos = fov_pixels * alpha;

    SetGLTransparentLines();
    glColor4fv(color);

    DrawVector();

    y_pos = CalcFOVAngle(pitch);

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten
    float yy = 0;
    float compas_y = CalcFOVAngle(-4);
    if (gear) {
        yy = CalcFOVAngle(-3);
        compas_y = CalcFOVAngle(0.5);

        // Heldragen linje vid hjorizonten när landställ är nere
        glLineWidth(line_width);
        glBegin(GL_LINES);
        glVertex2f(-480, CalcFOVAngle(0));
        glVertex2f(480, CalcFOVAngle(0));
        glEnd();
        // Landingssstolpar
        if (ifILSEnabled() == 1 && markKontakt() == 0) {
            float yy2 = yy - vdef * 20;
            glBegin(GL_LINES);
            glVertex2f(90, yy2);
            glVertex2f(90, yy2 - 80);

            glVertex2f(180, yy2);
            glVertex2f(180, yy2 - 120);

            glVertex2f(-90, yy2);
            glVertex2f(-90, yy2 - 80);

            glVertex2f(-180, yy2);
            glVertex2f(-180, yy2 - 120);
            glEnd();
        }
    }
    glLineWidth(line_width);
    glBegin(GL_LINES);

    // Prick i mitten
    glVertex2f(-line_width, yy);
    glVertex2f(line_width, yy);

    glVertex2f(45, yy);
    glVertex2f(480, yy);

    glVertex2f(-45, yy);
    glVertex2f(-480, yy);

    glEnd();

    // Altitude text på 0 horizonten

    glPushMatrix();
    SetGLText(); // turn on blending
    //glScalef(smallTextScale, smallTextScale, 0);

    int alt = altitude; // * 0.3048;
    if (metric) {
        alt = feetTom(altitude);
    } else {
        alt = altitude;
    }
    alt = alt / 10;
    alt = alt * 10;
    snprintf(tempText, 13, "%03d", alt);
    DrawHUDText(tempText, &fontMain, -200, yy + 5, 1, color);

    glPopMatrix();
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    SetGLTransparentLines();
    // linjer
    glColor4fv(color);
    for (int i = 5; i < 90; i += 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400, CalcFOVAngle(i));
            glVertex2f(200, CalcFOVAngle(i));

            glVertex2f(-400, CalcFOVAngle(i));
            glVertex2f(-200, CalcFOVAngle(i));
            glEnd();
        }
    }
    int start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400, CalcFOVAngle(i));
            glVertex2f(200, CalcFOVAngle(i));

            glVertex2f(-400, CalcFOVAngle(i));
            glVertex2f(-200, CalcFOVAngle(i));
            glEnd();
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
            glVertex2f(offset, compas_y);
            glVertex2f(offset, (compas_y + 20));
        }
    }
    glEnd();

    SetGLText(); // turn on blending
    glPushMatrix();

    glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 5; i < 90; i += 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }
    start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
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
            sprintf(tempText, "%02d", i);
            DrawHUDText(tempText, &fontMain, offset / smallTextScale, compas_y / smallTextScale, 1, color);
        }
    }
    glPopMatrix();

    glTranslatef(0, y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    sprintf(tempText, "& %.0f", alpha); // $ is replaced with alpha sign in bitmap
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120)) + ((textHeight(1.0) * text_scale) * 2), 1, color);

    //sprintf(tempText, "%.0f", airspeed);
    if (metric) {
        sprintf(tempText, "%.0f", knotsTokmh(airspeed));
    } else {
        sprintf(tempText, "%.0f", airspeed);
    }
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120)), 1, color);

    sprintf(tempText, "M %.2f", mach);
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120)) - ((textHeight(1.0) * text_scale)), 1, color);

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawModesJAS() {

    int gear = getGear();
    int mark = markKontakt();
    float y_pos = CalcFOVAngle(-myGetAlpha());

    if (mark) {
        y_pos = CalcFOVAngle(-3);
    }
    if (y_pos > 20) {
        y_pos = 20;
    }
    if (y_pos < -600) {
        y_pos = -600;
    }

    if (gear) {
        // Landings mod
        TranslateToCenter();
        DrawCompass(0, y_pos + 170);
        TranslateToCenter();
        DrawSpeed(0, y_pos);
        DrawAlpha(SPEED_POS_X, y_pos);
        DrawAltitude(0, y_pos);
        DrawNAVText(glass_width / 2 - 30, 280);
        // DrawFuelTime(-glass_width / 2 + 30, 310);
    } else {
        TranslateToCenter();
        DrawCompass(0, 170);
        TranslateToCenter();
        DrawSpeed(0, 0);
        DrawAlpha(SPEED_POS_X, 0);
        DrawAltitude(0, 0);
        DrawNAVText(glass_width / 2 - 30, -450);
        // DrawFuelTime(-glass_width / 2 + 30, -750);

        //TranslateToCenter();
        //DrawMarkindikator();
    }

    TranslateToCenter();
    DrawVector();

    TranslateToCenter();
    DrawGroundCollision();

    TranslateToCenter();
    DrawHorizionLines();
}
void DrawViggenMode1() {
    // Viggen mode 1
    int gear = getGear();
    int mark = markKontakt();
    float y_pos = CalcFOVAngle(-myGetAlpha());

    if (mark) {
        y_pos = CalcFOVAngle(-3);
    }
    if (y_pos > 20) {
        y_pos = 20;
    }
    if (y_pos < -600) {
        y_pos = -600;
    }
    if (gear) {
        // Landings mod
        DrawCompassViggen(0, y_pos + 220);
        drawSpeedAlphaViggen(SPEED_POS_X, -50);
        //DrawSpeed(-50, 0);
        //DrawAlpha(SPEED_POS_X-50, 0);
        //DrawAltitude(0, y_pos);
        drawADHelp();

    } else {

        DrawCompassViggen(0, 320);
        drawSpeedAlphaViggen(SPEED_POS_X, -50);
        //DrawSpeed(-50, 0);
        //DrawSpeed(0, 0);
        //DrawAlpha(SPEED_POS_X-50, 0);
        //DrawAltitude(0, 0);
        //DrawNAVText(-100, -450);
        //DrawFuelTime(-450, -750);
    }
    DrawVector();
    DrawGroundCollision();
    DrawHorizionLinesViggen();
}
void DrawViggenMode2() {

    // Viggen mode 2 är en overklig variant där vi lyft in det vi saknar i fysisk instrumentering

    int gear = getGear();
    int mark = markKontakt();
    float y_pos = CalcFOVAngle(-myGetAlpha());

    if (mark) {
        y_pos = CalcFOVAngle(-3);
    }
    if (y_pos > 20) {
        y_pos = 20;
    }
    if (y_pos < -600) {
        y_pos = -600;
    }
    if (gear) {
        // Landings mod
        DrawCompassViggen(0, y_pos + 220);
        //drawSpeedAlphaViggen(SPEED_POS_X, -50);
        DrawSpeed(-50, 0);
        DrawAlpha(SPEED_POS_X - 50, 0);
        //DrawAltitude(0, y_pos);
        //DrawNAVText(-100, 280);
        //DrawFuelTime(-450, 300);
    } else {
        DrawCompassViggen(0, 320);
        DrawSpeed(-50, 0);
        DrawAlpha(SPEED_POS_X - 50, 0);
        //DrawAltitude(0, 0);
        //DrawNAVText(-100, -450);
        //DrawFuelTime(-450, -750);
    }

    DrawVector();
    DrawGroundCollision();
    DrawHorizionLinesViggen();
}

void DrawViggenMode3() {

    // Viggen mode 2 är en overklig variant där vi lyft in det vi saknar i fysisk instrumentering

    int gear = getGear();
    int mark = markKontakt();
    float y_pos = CalcFOVAngle(-myGetAlpha());

    if (mark) {
        y_pos = CalcFOVAngle(-3);
    }
    if (y_pos > 20) {
        y_pos = 20;
    }
    if (y_pos < -600) {
        y_pos = -600;
    }
    if (gear) {
        // Landings mod
        DrawCompassViggen(0, y_pos + 220);
        drawSpeedAlphaViggen(SPEED_POS_X, -50);
        DrawSpeed(0, y_pos);
        DrawAlpha(SPEED_POS_X, y_pos);
        DrawAltitude(0, y_pos);
        //DrawNAVText(-100, 280);
        //DrawFuelTime(-450, 300);
    } else {
        DrawCompassViggen(0, 320);
        DrawSpeed(0, 0);
        DrawAlpha(SPEED_POS_X, 0);
        DrawAltitude(0, 0);
        //DrawNAVText(-100, -450);
        //DrawFuelTime(-450, -750);
    }

    DrawVector();
    DrawGroundCollision();
    DrawHorizionLinesViggen();
}
