#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

void DrawTest() {

    // Draw Text
    SetGLText(); // turn on blending
    //float color[] = {0.0, 1.0, 0.0, 1.0};
    DrawHUDNumber((int)getHeading(), &fontMain, -3, 0, 200, 0, color);
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());
}

#define MOVEMENT_ARROW_RADIUS_PX 20
#define MOVEMENT_ARROW_LEG_SIZE 10

void drawCube(float x, float y, float size) {
    glBegin(GL_POLYGON);

    glVertex2f(x, y);
    glVertex2f(x, y + size);
    glVertex2f(x + size, y + size);
    glVertex2f(x + size, y);

    glEnd();
}

void drawCubes(int line, float size) {
    float colorgreen[] = {0.0, 1.0, 0.0, 1.0};
    float colornogreen[] = {1.0, 0.0, 1.0, 1.0};

    float colorgreen2[] = {0.0, 1.0, 0.0, 0.1};
    float colornogreen2[] = {1.0, 0.0, 1.0, 0.1};

    float colorgreen3[] = {0.0, 1.0, 0.0, 0.0};
    float colornogreen3[] = {1.0, 0.0, 1.0, 0.0};

    float colorwhite1[] = {1.0, 1.0, 1.0, 1.0};
    float colorwhite2[] = {0.2, 0.2, 0.2, 1.0};
    float colorwhite3[] = {0.2, 0.2, 0.2, 0.2};

    float colorgreen4[] = {0.0, 0.2, 0.0, 0.2};
    float colornogreen4[] = {0.2, 0.0, 0.2, 0.2};

    float colorgreen5[] = {0.0, 0.8, 0.0, 0.2};
    float colornogreen5[] = {0.8, 0.0, 0.8, 0.2};

    glColor4fv(colorgreen);
    drawCube(size * 0, line, size);
    glColor4fv(colornogreen);
    drawCube(size * 1, line, size);

    glColor4fv(colorgreen2);
    drawCube(size * 2, line, size);
    glColor4fv(colornogreen2);
    drawCube(size * 3, line, size);

    glColor4fv(colorgreen3);
    drawCube(size * 4, line, size);
    glColor4fv(colornogreen3);
    drawCube(size * 5, line, size);

    glColor4fv(colorwhite1);
    drawCube(size * 6, line, size);
    glColor4fv(colorwhite2);
    drawCube(size * 7, line, size);
    glColor4fv(colorwhite3);
    drawCube(size * 8, line, size);

    glColor4fv(colorgreen4);
    drawCube(size * 9, line, size);
    glColor4fv(colornogreen4);
    drawCube(size * 10, line, size);

    glColor4fv(colorgreen5);
    drawCube(size * 11, line, size);
    glColor4fv(colornogreen5);
    drawCube(size * 12, line, size);
}

void DrawGlass() {
    //XPLMSetGraphicsState(0 /*Fog*/, 0 /*TexUnits*/, 0 /*Lighting*/, 0 /*AlphaTesting*/, 1 /*AlphaBlending*/, 0 /*DepthTesting*/, 0 /*DepthWriting*/);

    float colorglass[] = {0.2, 0.0, 0.2, 0.5};
    float colorglassB[] = {0.25, 0.25, 0.25, 1.0};
    int modes[] = {GL_ZERO,
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

    //glClearColor(1.0, 1.0, 0.0, 0.5);

    //SetGLText();
    //int mode1 = glGet(GL_BLEND_EQUATION_RGB);

    //char buffer[255];
    // sprintf(buffer, "blend mode: %d", mode1);
    // XPLMDrawString(color, -200, 300, buffer, NULL, xplmFont_Basic);
    SetGLTransparentLines();
    glPushMatrix();

    glTranslatef(-500, -500, 0);

    glEnable(GL_BLEND);

    // for (int i = 0; i < 15; i++) {
    //     glBlendFunc(modes[i], GL_DST_COLOR);
    //     drawCubes(i * 50, 50);
    // }
    glTranslatef(500, 500, 0);
    //glBlendEquation(GL_MIN);
    //glBlendEquationSeparate(GL_MIN);
    glColor4fv(colorglass);
    glColor4fv(color);

    // for (int i = 0; i < 15; i++) {
    //     glBegin(GL_LINES);
    //     glColor4fv(color);
    //     glVertex2f(i * 50 + 25, 1000);
    //     glVertex2f(i * 50 + 25, 0);
    //     glEnd();
    // }

    glBlendFunc(modes[glass_type], modes[glass_type2]);
    glTranslatef(-glass_width / 2, -glass_height / 2, 0);
    if (draw_glass == 1) {
        //glColor4fv(colorglass);
        glBegin(GL_POLYGON);
        glColor4fv(colorglassB);
        glVertex2f(0, -glass_height);

        glColor4fv(colorglass);
        glVertex2f(0, glass_height - 50);
        glVertex2f(100, glass_height);

        glColor4f(0.25, 0.25, 0.25, 0.1);
        glVertex2f(glass_width - 100, glass_height);
        glVertex2f(glass_width, glass_height - 50);

        glColor4fv(colorglassB);
        glVertex2f(glass_width, -glass_height);

        glEnd();
    }

    glPopMatrix();
    SetGLText();
    SetGLTransparentLines();

    //glBlendEquation(mode1);
}

void DrawVector() {
    SetGLTransparentLines();
    float airspeed = getIAS();
    float y_pos = CalcFOVAngle(myGetAlpha());
    float x_pos = CalcFOVAngle(myGetBeta());
    float tail_pos = airspeed - getLandingSpeed();
    float angle = getRoll();
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40);
    tail_pos = fmax(tail_pos, -40);
    //y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(-x_pos, -y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

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
    glRotatef(angle, 0, 0, 1);
    glTranslatef(x_pos, y_pos, 0); // set position back
    glRotatef(-angle, 0, 0, 1);

    // char buffer[255];
    // sprintf(buffer, "Alpha: %f, Beta %f, Pitch %f, FOV %f, FOVPixel %f, y_pos %f", getAlphaA(), getBetaA(), getPitch(), getFOV(), fov_pixels, y_pos);
    // XPLMDrawString(color, -200, 300, buffer, NULL, xplmFont_Basic);
    // sprintf(buffer, "Vx: %f, Vy %f, Vz %f, newalpha %f, newbeta %f, heading %f, truehead %f", getVX(), getVY(), getVZ(), myGetAlpha(), myGetBeta(), newhead, 0.0);
    // XPLMDrawString(color, -200, 350, buffer, NULL, xplmFont_Basic);
    // sprintf(buffer, "LandingSpeed: %f, Mass %f, speed1 %f, speed2 %f, mass1 %f, mass2 %f", getLandingSpeed(), getTotalWeight(), landing_speed1, landing_speed2, landing_weight1, landing_weight2);
    // XPLMDrawString(color, -200, 250, buffer, NULL, xplmFont_Basic);
}

void DrawHorizionLines() {

    SetGLTransparentLines();

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
#define SPEED_POS_Y -50

void DrawSpeed() {
    char temp[20];
    float airspeed = getIAS();
    float groundspeed = getGroundSpeed() * 1.944;
    float mach = getMachSpeed();
    float landingspeed = getLandingSpeed();
    float tail_pos = airspeed - landingspeed;

    SetGLTransparentLines();
    glColor4fv(color);

    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y - 100 * HUD_SCALE);
    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + 100 * HUD_SCALE);

    // Markör för landningshastighet
    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {
        tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 10 * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE);
    }
    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y - 100 * HUD_SCALE);
    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + 100 * HUD_SCALE);

    // pil vid hastighetstexten
    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + 0 * HUD_SCALE);
    glVertex2f((SPEED_POS_X - 20) * HUD_SCALE, SPEED_POS_Y + 10 * HUD_SCALE);

    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + 0 * HUD_SCALE);
    glVertex2f((SPEED_POS_X - 20) * HUD_SCALE, SPEED_POS_Y - 10 * HUD_SCALE);

    glEnd();

    SetGLText(); // turn on blending
    sprintf(temp, "%.0f", airspeed);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X - 20) * HUD_SCALE, (SPEED_POS_Y * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 1, color);

    sprintf(temp, "M %.2f", mach);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale)), 1, color);

    sprintf(temp, "GS%.0f", groundspeed);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);

    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {

        DrawHUDText("L", &fontMain, SPEED_POS_X + 10 * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE - ((fontMain.charHeight / 2 * text_scale)), 0, color);
    }

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha() {
    char temp[20];
    float alpha = myGetAlpha();
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
#define ALT_POS_Y -50

void DrawAltitude() {
    char temp[20];
    float altitude = getAltitude();

    SetGLTransparentLines();

    glColor4fv(color);
    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y - 100 * HUD_SCALE);
    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 100 * HUD_SCALE);

    // pil vid hastighetstexten
    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, ALT_POS_Y + 10 * HUD_SCALE);

    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, ALT_POS_Y - 10 * HUD_SCALE);

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
    float alpha = myGetAlpha();
    float beta = myGetBeta();
    //float gforce = getGForce();
    float mach = getMachSpeed();
    float y_pos = CalcFOVAngle(alpha);
    float x_pos = CalcFOVAngle(beta);
    float heading = getHeading();
    float angle = getRoll();
    float pitch = getPitch();
    char tempText[32];
    float smallTextScale = 0.85;
    float tail_pos = airspeed - LANDING_SPEED;
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40);
    tail_pos = fmax(tail_pos, -40);
    y_pos = fov_pixels * alpha;

    SetGLTransparentLines();
    glColor4fv(color);

    glPushMatrix();
    glRotatef(angle, 0, 0, 1);
    glTranslatef(-x_pos, -y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

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

    //glTranslatef(x_pos, y_pos, 0); // set position back
    glPopMatrix(); // set position back
    // Horizontal lines

    y_pos = CalcFOVAngle(pitch);

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

    glPushMatrix();
    SetGLText(); // turn on blending
    //glScalef(smallTextScale, smallTextScale, 0);

    int alt = altitude * 0.3048;
    alt = alt / 10;
    alt = alt * 10;
    snprintf(tempText, 13, "%03d", alt);
    DrawHUDText(tempText, &fontMain, -200 * HUD_SCALE, yy, 1, color);

    glPopMatrix();
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    SetGLTransparentLines();
    // linjer
    glColor4fv(color);
    for (int i = 5; i < 90; i += 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {
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
        if ((i > pitch - 8) && (i < pitch + 8)) {
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
        if ((i > pitch - 8) && (i < pitch + 8)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }
    start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > pitch - 8) && (i < pitch + 8)) {

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
            sprintf(tempText, "%02d", i);
            DrawHUDText(tempText, &fontMain, offset / smallTextScale, compas_y / smallTextScale * HUD_SCALE, 1, color);
        }
    }
    glPopMatrix();

    glTranslatef(0, y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    sprintf(tempText, "$ %.0f", alpha); // $ is replaced with alpha sign in bitmap
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale) * 2), 1, color);

    sprintf(tempText, "%.0f", airspeed);
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE), 1, color);

    sprintf(tempText, "M %.2f", mach);
    DrawHUDText(tempText, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale)), 1, color);

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}
