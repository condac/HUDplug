#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

int stab_error;

void DrawTest() {

    SetGLTransparentLines();
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Standard blend, baserat på alpha
    // detta motsvarar mode 6 7
    //glBlendFunc(modes[glass_type], modes[glass_type2]);
    float testcolor[] = {0.0, 1.0, 0.0, 1.0};
    float testcolor2[] = {0.0, 1.0, 0.0, 0.5};
    float testcolor3[] = {0.0, 0.5, 0.0, 0.5};
    for (int i = 0; i < 15; i++) {
        for (int y = 0; y < 15; y++) {
            SetGLTransparentLines();
            glEnable(GL_BLEND);
            glBlendFunc(blendmodes[i], blendmodes[y]);

            glColor4fv(testcolor);
            glLineWidth(2);
            glBegin(GL_LINES);

            glVertex2f(-500 + i * 30, -500 + y * 30);
            glVertex2f(-500 + i * 30 + 25, -500 + y * 30 + 25);

            glEnd();
            glColor4fv(testcolor2);
            glLineWidth(2);
            glBegin(GL_LINES);

            glVertex2f(-500 + i * 30 + 25, -500 + y * 30);
            glVertex2f(-500 + i * 30, -500 + y * 30 + 25);

            glEnd();
            glEnd();
            glColor4fv(testcolor3);
            glLineWidth(2);
            glBegin(GL_LINES);

            glVertex2f(-500 + i * 30 + 25, -500 + y * 30);
            glVertex2f(-500 + i * 30, -500 + y * 30);

            glEnd();
            SetGLText();
            glBlendFunc(blendmodes[i], blendmodes[y]);
            DrawHUDText("A", &fontMain, 0 + i * 30 + 25, -500 + y * 30, 1, color);
        }
    }
}

void DrawCompass(float x, float y) {
    float compas_y = y;
    char tempText[100];
    float heading = getHeading();
    float nav1_heading = getNAV1Heading();
    // Draw Text
    //SetGLText(); // turn on blending
    //float color[] = {0.0, 1.0, 0.0, 1.0};
    //DrawHUDNumber((int)getHeading(), &fontMain, -3, 0, compas_y, 1, color);
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());

    SetGLTransparentLines();
    glColor4fv(color);
    // pilen som pekar på kursen
    glBegin(GL_LINES);

    glVertex2f(0, compas_y + 0);
    glVertex2f(0 + 10, compas_y - 20);
    glVertex2f(0, compas_y + 0);
    glVertex2f(0 - 10, compas_y - 20);

    glEnd();
    // Compas lines long
    for (int i = 0; i < 37; i++) {

        float offset = (i * 10) - heading;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 15) && (offset > -15)) {
            // 80 pixlar mellan 10 grader
            float rate = 80 / 10;
            offset = offset * rate;
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(offset, compas_y);
            glVertex2f(offset, compas_y + 20);

            glEnd();
        }
    }
    // Compas lines short 5grader
    for (int i = 0; i < 37; i++) {

        float offset = (i * 10) - heading + 5;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 15) && (offset > -15)) {
            // 80 pixlar mellan 10 grader
            float rate = 80 / 10;
            offset = offset * rate;
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(offset, compas_y);
            glVertex2f(offset, compas_y + 5);

            glEnd();
        }
    }

    // NAV 1 riktning
    float offset = (nav1_heading)-heading;
    float rate = 80 / 10;
    offset = lim(offset * rate, -150, 150);

    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(offset, compas_y);
    glVertex2f(offset, compas_y - 25);
    glVertex2f(offset - 5, compas_y - 20);
    glVertex2f(offset - 5, compas_y - 25);
    glVertex2f(offset + 5, compas_y - 20);
    glVertex2f(offset + 5, compas_y - 25);

    glEnd();

    SetGLText();
    // Compas lines text
    for (int i = 0; i < 37; i++) {

        float offset = (i * 10) - heading;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 15) && (offset > -15)) {
            // 80 pixlar mellan 10 grader
            float rate = 80 / 10;
            offset = offset * rate;
            sprintf(tempText, "%02d", i);
            DrawHUDText(tempText, &fontMain, offset, compas_y + 25, 1, color);
        }
    }
    // sprintf(tempText, "test%02d", 1);
    // DrawHUDText(tempText, &fontMain, 0  , compas_y  , 1, color);
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
    float colorglassB[] = {0.2, 0., 0.2, 0.2};
    colorglass[3] = GetGlassDarkness() * glass_darkness;
    colorglassB[3] = GetGlassDarkness() * glass_darkness - 0.3;

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

    glEnable(GL_BLEND);

    //glBlendEquation(GL_MIN);
    //glBlendEquationSeparate(GL_MIN);
    //glColor4fv(colorglass);
    //glColor4fv(color);

    // for (int i = 0; i < 15; i++) {
    //     glBegin(GL_LINES);
    //     glColor4fv(color);
    //     glVertex2f(i * 50 + 25, 1000);
    //     glVertex2f(i * 50 + 25, 0);
    //     glEnd();
    // }

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Standard blend, baserat på alpha
    // detta motsvarar mode 6 7
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

        //glColor4f(0.2, 0.0, 0.2, 0.2);
        glVertex2f(glass_width - 100, glass_height);
        glVertex2f(glass_width, glass_height - 50);

        glColor4fv(colorglassB);
        glVertex2f(glass_width, -glass_height);

        glEnd();
    }

    glPopMatrix();
    SetGLText(); // för att nollställa blendFunc
    // char tempText[100];
    // sprintf(tempText, "glass %f", glass_darkness);
    // DrawHUDText(tempText, &fontMain, 10  , 10, 1, color);

    SetGLTransparentLines();

    //glBlendEquation(mode1);
}

void DrawVector() {
    SetGLTransparentLines();
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    float airspeed = getIAS();
    float y_pos = CalcFOVAngle(myGetAlpha());
    float x_pos = CalcFOVAngle(myGetBeta());
    float tail_pos = airspeed - getLandingSpeed() + 20;
    float angle = getRoll();
    float vdef = getILSv();
    float hdef = getILSh();
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40); // Fenans längd ska motsvara 20km/h
    tail_pos = fmax(tail_pos, -40);
    //y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(-x_pos, -y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    if (!markKontakt()) {
        DrawCircle(10);
    }
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(10, 0);
    glVertex2f(30, 0);

    glVertex2f(-10, 0);
    glVertex2f(-30, 0);

    if (gear) {
        glVertex2f(0, tail_pos - 10);
        glVertex2f(0, tail_pos + 10);
    }

    glEnd();

    // ILS indikator
    if (gear) {
        if (ifILSEnabled() == 1 && markKontakt() == 0) {
            DrawFillCircleXY(5, hdef * 50, -vdef * 50);
        }
    }

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
    float heading = getHeading();
    char tempText[10];
    float smallTextScale = 0.75;
    int gear = getGear();
    float vdef = getILSv();
    //float hdef = getILSh();

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten

    glLineWidth(line_width + 1.0);
    glBegin(GL_LINES);

    // prickar
    for (float i = -160; i < 160; i += 40) {
        glVertex2f((i)*HUD_SCALE, 0);
        glVertex2f((i + (line_width * 2)), 0);
    }
    glVertex2f(160, 0);
    glVertex2f(295, 0);

    glVertex2f(410, 0);
    glVertex2f(480, 0);

    glVertex2f(-160, 0);
    glVertex2f(-295, 0);

    glVertex2f(-410, 0);
    glVertex2f(-480, 0);

    glEnd();

    glLineWidth(line_width);
    // Compas lines
    // Var 5 grad är det ett litet streck på 0 horizonten
    float compas_y = 0;
    glLineWidth(line_width);
    glBegin(GL_LINES);
    for (int i = 0; i < 370; i = i + 5) {

        float offset = (i)-heading + 5;
        if (offset > 180) {

            offset -= 360;
        }
        if (offset < -180) {

            offset += 360;
        }
        if ((offset < 90) && (offset > -90)) {
            offset = CalcFOVAngle(offset);
            glVertex2f(offset, compas_y);
            glVertex2f(offset, (compas_y + 10));
        }
    }

    // linjer
    if (gear) {
        float yy = CalcFOVAngle(-2.86);

        // Heldragen linje vid hjorizonten när landställ är nere
        glLineWidth(line_width);
        glBegin(GL_LINES);
        glVertex2f(40, yy);
        glVertex2f(200, yy);

        glVertex2f(-40, yy);
        glVertex2f(-200, yy);
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

    for (int i = 10; i < 90; i += 10) {
        // if ((i > getPitch() - 35) && (i < getPitch() + 30)) {
        //     glLineWidth(line_width);
        //     glBegin(GL_LINES);
        //
        //     glVertex2f(40  , CalcFOVAngle(i));
        //     glVertex2f(200  , CalcFOVAngle(i));
        //
        //     glVertex2f(-40  , CalcFOVAngle(i));
        //     glVertex2f(-200  , CalcFOVAngle(i));
        //     glEnd();
        //
        // }
        DrawCircleUp(CalcFOVAngle(i), 0, CalcFOVAngle(90), heading);
    }

    for (int i = -10; i > -90; i -= 10) {
        // if ((i > getPitch() - 35) && (i < getPitch() + 30)) {
        //     glLineWidth(line_width);
        //     glBegin(GL_LINES);
        //
        //     glVertex2f(15  , CalcFOVAngle(i));
        //     glVertex2f(400 - 15  , CalcFOVAngle(i));
        //
        //     // en knöl uppåt
        //     //mitten
        //     glVertex2f(0  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(15  , CalcFOVAngle(i));
        //
        //     glVertex2f(0  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(-15  , CalcFOVAngle(i));
        //
        //     // höger
        //     glVertex2f(400  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(400 + 15  , CalcFOVAngle(i));
        //
        //     glVertex2f(400  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(400 - 15  , CalcFOVAngle(i));
        //
        //     glVertex2f(400 + 15 + 80  , CalcFOVAngle(i));
        //     glVertex2f(400 + 15  , CalcFOVAngle(i));
        //
        //     // andra sidan
        //     glVertex2f(-15  , CalcFOVAngle(i));
        //     glVertex2f(-400 + 15  , CalcFOVAngle(i));
        //
        //     // en knöl uppåt
        //     // vänster
        //     glVertex2f(-400  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(-400 - 15  , CalcFOVAngle(i));
        //
        //     glVertex2f(-400  , CalcFOVAngle(i) + (40  ));
        //     glVertex2f(-400 + 15  , CalcFOVAngle(i));
        //
        //     glVertex2f(-400 - 15 - 80  , CalcFOVAngle(i));
        //     glVertex2f(-400 - 15  , CalcFOVAngle(i));
        //
        //     glEnd();
        //
        // }
        DrawCircleDown(CalcFOVAngle(i), 0, CalcFOVAngle(-90), heading);
    }

    // Cirkel vid 90 grader
    DrawCircleXY(200, 0, CalcFOVAngle(90));
    //DrawCircleXY(CalcFOVAngle(70)  , 0, CalcFOVAngle(90));

    DrawCircleXY(200, 0, CalcFOVAngle(-90));
    float textradius_pos = 180;

    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(0, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading)) * textradius_pos, CalcFOVAngle(-90) - (textradius_pos * cos(to_radians(heading))));
    glVertex2f(0, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90)) * textradius_pos, CalcFOVAngle(-90) - (textradius_pos * cos(to_radians(heading + 90))));
    glVertex2f(0, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90 + 90)) * textradius_pos, CalcFOVAngle(-90) - (textradius_pos * cos(to_radians(heading + 90 + 90))));
    glVertex2f(0, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90 + 90 + 90)) * textradius_pos, CalcFOVAngle(-90) - (textradius_pos * cos(to_radians(heading + 90 + 90 + 90))));

    glEnd();

    SetGLText();

    glPushMatrix();
    //glRotatef(heading, 0, CalcFOVAngle(90), 1);
    sprintf(tempText, "N");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading)) * textradius_pos, CalcFOVAngle(90) - (cos(to_radians(-heading)) * textradius_pos), 1, color);
    //glRotatef(90, sin(to_radians(-heading + 180)) * textradius_pos  , CalcFOVAngle(90) - (cos(to_radians(-heading + 180)) * textradius_pos), 1);
    sprintf(tempText, "S");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 180)) * textradius_pos, CalcFOVAngle(90) - (cos(to_radians(-heading + 180)) * textradius_pos), 1, color);
    //glRotatef(90, 0, 0, 1);
    sprintf(tempText, "E");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 90)) * textradius_pos, CalcFOVAngle(90) - (cos(to_radians(-heading + 90)) * textradius_pos), 1, color);
    //glRotatef(90, 0, 0, 1);
    sprintf(tempText, "W");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 270)) * textradius_pos, CalcFOVAngle(90) - (cos(to_radians(-heading + 270)) * textradius_pos), 1, color);

    glPopMatrix();

    SetGLText(); // turn on blending
    // glPushMatrix();
    // glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 10; i < 90; i += 10) {
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {

            sprintf(tempText, "+%d", i);
            //DrawHUDText(tempText, &fontMain, -200   / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
            drawLineText(tempText, -200, CalcFOVAngle(i) + textHeight(smallTextScale) * 2.2, smallTextScale, 1);
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200  , CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    for (int i = -10; i > -90; i -= 10) {
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {

            sprintf(tempText, "%d", i);
            //DrawHUDText(tempText, &fontMain, -200   / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
            drawLineText(tempText, -200, CalcFOVAngle(i) + textHeight(smallTextScale) * 0.2, smallTextScale, 1);
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200  , CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    // glPopMatrix();
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    glTranslatef(0, y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    // char buffer[255];
    // sprintf(buffer, "Roll: %f, Pitch %f, FOV %f, FOVPixel %f, y_pos %f", getRoll(), getPitch(), getFOV(), fov_pixels, y_pos);
    // XPLMDrawString(color, -250, 350, buffer, NULL, xplmFont_Basic);
}

#define SPEED_POS_X -315
#define SPEED_POS_Y -50

void DrawSpeed(float x, float y) {
    char temp[100];
    float airspeed = getIAS();
    float groundspeed = getGroundSpeed() * 1.944;
    float mach = getMachSpeed();
    float landingspeed = getLandingSpeed();
    float landingspeed2 = landingspeed + 30;
    int gear = getGear();
    float tail_pos = airspeed - landingspeed;
    float maxSpeed = kmhToknots(2000);
    float eco = 300; //300 knots

    float machKnotsRatio = airspeed / mach;
    float climbSpeed = machKnotsRatio * 0.85; // 500-550knop 0.85 mach baserat på viggen innan vi hittat data för jas

    if (gear) {
        maxSpeed = kmhToknots(600);
    }
    if (markKontakt()) {
        maxSpeed = kmhToknots(350);
    }

    SetGLTransparentLines();
    glColor4fv(color);

    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(SPEED_POS_X, y - 100);
    glVertex2f(SPEED_POS_X, y + 100);

    // Markör för snabbast stigningshastighet
    if (airspeed < climbSpeed + 50 && airspeed > climbSpeed - 50) {
        glVertex2f(SPEED_POS_X, y - (airspeed - climbSpeed) * 2);
        glVertex2f(SPEED_POS_X + 10, y - (airspeed - climbSpeed) * 2);
    }

    // Markör för landningshastighet
    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {
        tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X, y + tail_pos);
        glVertex2f(SPEED_POS_X + 10, y + tail_pos);
    }
    // Markör för landningshastighet inflygning
    if (airspeed < landingspeed2 + 50 && airspeed > landingspeed2 - 50) {
        //tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X, y - (airspeed - landingspeed2) * 2);
        glVertex2f(SPEED_POS_X + 20, y - (airspeed - landingspeed2) * 2);
    }
    // Markör för maxhastighet
    if (airspeed < maxSpeed + 50 && airspeed > maxSpeed - 50) {
        //tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X, y - (airspeed - maxSpeed) * 2);
        glVertex2f(SPEED_POS_X + 20, y - (airspeed - maxSpeed) * 2);

        glVertex2f(SPEED_POS_X + 20, y - (airspeed - maxSpeed) * 2);
        glVertex2f(SPEED_POS_X + 20, y - 15 - (airspeed - maxSpeed) * 2);
    }
    // Markör för distansekonomisk hastighet
    if (airspeed < eco + 50 && airspeed > eco - 50) {
        //tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X, y - (airspeed - eco) * 2);
        glVertex2f(SPEED_POS_X + 20, y - (airspeed - eco) * 2);

        glVertex2f(SPEED_POS_X + 20, y + 10 - (airspeed - eco) * 2);
        glVertex2f(SPEED_POS_X + 20, y - 10 - (airspeed - eco) * 2);

        glVertex2f(SPEED_POS_X + 20, y + 10 - (airspeed - eco) * 2);
        glVertex2f(SPEED_POS_X + 10, y + 10 - (airspeed - eco) * 2);

        glVertex2f(SPEED_POS_X + 20, y - 10 - (airspeed - eco) * 2);
        glVertex2f(SPEED_POS_X + 10, y - 10 - (airspeed - eco) * 2);
    }
    glVertex2f(SPEED_POS_X, y - 100);
    glVertex2f(SPEED_POS_X, y + 100);

    // pil vid hastighetstexten
    glVertex2f(SPEED_POS_X, y + 0);
    glVertex2f((SPEED_POS_X - 20), y + 10);

    glVertex2f(SPEED_POS_X, y + 0);
    glVertex2f((SPEED_POS_X - 20), y - 10);

    glEnd();

    SetGLText(); // turn on blending
    if (metric) {
        sprintf(temp, "%.0f", knotsTokmh(airspeed));
    } else {
        sprintf(temp, "%.0f", airspeed);
    }
    //DrawHUDText(temp, &fontMain, (SPEED_POS_X - 30)  , (SPEED_POS_Y  ) - (textHeight(1.0) / 2), 2, color);
    drawLineText(temp, (SPEED_POS_X - 30), (y) - (textHeight(1.0) / 2), 1.0, 2);

    sprintf(temp, "M %.2f", mach);
    //DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120)  ) - ((textHeight(1.0) * text_scale)), 2, color);
    drawLineText(temp, (SPEED_POS_X)*HUD_SCALE, ((y - 120)) - (textHeight(1.2) * 1), 1.0, 2);

    if (metric) {
        sprintf(temp, "GS%.0f", knotsTokmh(groundspeed));
    } else {
        sprintf(temp, "GS%.0f", groundspeed);
    }
    //sprintf(temp, "GS%.0f", groundspeed);
    //    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120)  ) - ((textHeight(1.0) * text_scale) * 2), 2, color);
    drawLineText(temp, (SPEED_POS_X)*HUD_SCALE, ((y - 120)) - (textHeight(1.2) * 2), 1.0, 2);
    // Markör för snabbast stigningshastighet
    if (airspeed < climbSpeed + 50 && airspeed > climbSpeed - 50) {
        DrawHUDText("O", &fontMain, SPEED_POS_X + 10, y - (airspeed - climbSpeed) * 2 - ((textHeight(1.0) / 2 * text_scale)), 0, color);
    }
    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {

        DrawHUDText("L", &fontMain, SPEED_POS_X + 10, y + tail_pos - ((textHeight(1.0) / 2 * text_scale)), 0, color);
    }
    if (getSpeedBrake()) {
        sprintf(temp, "LUFTBROMS UTE");
        DrawHUDText(temp, &fontMain, (0), ((200)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }
    if (getParkBrake()) {
        sprintf(temp, "PARKERINGSBROMS");
        DrawHUDText(temp, &fontMain, (0), ((225)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }
    if (stab_error > 100 && !getPause()) {
        sprintf(temp, "FEL I STABILISERINGS PLUGIN");
        DrawHUDText(temp, &fontMain, (0), ((100)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }
    if (getStabStatus()) {
        stab_error = 0;
        setStabStatus(0);

    } else {
        stab_error++;
    }

    if (airspeed > maxSpeed) { // Varning när hög fart och landställ ute
        sprintf(temp, "MINSKA FART");
        DrawHUDText(temp, &fontMain, (0), ((175)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha(float x, float y) {
    char temp[20];
    float alpha = getAlphaA();
    float gforce = getGForce();

    glColor4fv(color);

    SetGLText(); // turn on blending

    sprintf(temp, "G %.1f", gforce);
    DrawHUDText(temp, &fontMain, (x), ((y + 120)) + ((textHeight(1.0) * text_scale)), 2, color);

    if (getIAS() > 50) {
        sprintf(temp, "& %.0f", alpha);
    } else {
        sprintf(temp, "& X");
    }

    DrawHUDText(temp, &fontMain, (x), ((y + 120)) + ((textHeight(1.0) * text_scale) * 2.5), 2, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

#define ALT_POS_X 315
#define ALT_POS_Y -50
#define ALT_SCALE_METER 400.0f
#define ALT_SCALE_PIXEL 220.0f

float altToPixelY(float value) {
    return value * (ALT_SCALE_PIXEL / ALT_SCALE_METER);
}

void DrawAltitude(float x, float y) {
    char temp[20];
    float altitude = getAltitude();
    float radaralt = getRadarAltitude();

    if (1) {
        altitude = feetTom(altitude);
    }
    SetGLTransparentLines();

    glColor4fv(color);
    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    //glVertex2f(ALT_POS_X  , ALT_POS_Y - 100  );
    //glVertex2f(ALT_POS_X  , ALT_POS_Y + 100  );

    // pil vid texten
    glVertex2f(ALT_POS_X, y + 0);
    glVertex2f((ALT_POS_X - 20), y + 10);

    glVertex2f(ALT_POS_X, y + 0);
    glVertex2f((ALT_POS_X - 20), y - 10);

    glEnd();

    int start = (altitude - ALT_SCALE_METER) / 100;
    int maxA = altitude + ALT_SCALE_METER / 2;
    int minA = altitude - ALT_SCALE_METER / 2;

    for (int i = start * 100; i < altitude + ALT_SCALE_METER; i += 100) {
        if (i < maxA && i > minA && i >= 0) {

            if (((i / 100) % 2) == 0) {
                glLineWidth(line_width);
                glBegin(GL_LINES);
                glVertex2f(ALT_POS_X, (y + altToPixelY(-altitude) + altToPixelY(i)));
                glVertex2f(ALT_POS_X + 20, (y + altToPixelY(-altitude) + altToPixelY(i)));
                glEnd();
            } else {
                glLineWidth(max(line_width / 2, 1.0));
                glBegin(GL_LINES);
                glVertex2f(ALT_POS_X, (y + altToPixelY(-altitude) + altToPixelY(i)));
                glVertex2f(ALT_POS_X + 10, (y + altToPixelY(-altitude) + altToPixelY(i)));
                glEnd();
            }
        }
    }

    // Draw radar altitude lines
    int drawRH = 0;
    float rhY = altToPixelY(-radaralt);

    if (rhY < -ALT_SCALE_PIXEL / 2) {
        drawRH = 1;
        rhY = -ALT_SCALE_PIXEL / 2;
    } else {

        glLineWidth(line_width);
        glBegin(GL_LINES);
        glVertex2f(ALT_POS_X, (y + rhY));
        glVertex2f(ALT_POS_X - 30, (y + rhY));

        glVertex2f(ALT_POS_X, (y + rhY));
        glVertex2f(ALT_POS_X - 10, (y - 20 + rhY));

        glVertex2f(ALT_POS_X - 15, (y + rhY));
        glVertex2f(ALT_POS_X - 15 - 10, (y - 20 + rhY));

        glEnd();
    }

    SetGLText(); // turn on blending

    if (drawRH && radaralt < 1500) {
        if (metric) {
            sprintf(temp, "RH %.0f", radaralt);
        } else {
            sprintf(temp, "RH %.0f", m2feet(radaralt));
        }

        DrawHUDText(temp, &fontMain, (ALT_POS_X - 20), (y - 20 + rhY) - ((textHeight(1.0) * text_scale)), 0, color);
    }

    for (int i = start * 100; i < altitude + ALT_SCALE_METER; i += 100) {
        if (i < maxA && i > minA && i >= 0) {

            if (((i / 100) % 2) == 0) {
                if (metric) {
                    sprintf(temp, "%d", i);
                } else {
                    int aaa = m2feet(i);
                    sprintf(temp, "%d", aaa);
                }

                DrawHUDText(temp, &fontMain, (ALT_POS_X + 20), ((y + altToPixelY(-altitude) + altToPixelY(i))) - ((textHeight(1.0) * text_scale) / 2), 0, color);
            }
        }
    }
    int altdraw = 0; // altitude / 10;

    if (metric) {
        altdraw = altitude / 10;
    } else {
        altdraw = m2feet(altitude) / 10;
    }
    sprintf(temp, "%d", altdraw * 10);
    DrawHUDText(temp, &fontMain, (ALT_POS_X - 30), (y) - ((textHeight(1.0) * text_scale) / 2), 2, color);

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawGroundCollision() {
    char temp[100];
    float vy = getVY();
    float radaralt = getRadarAltitude();
    int gear = getGear();

    if (!gear) {
        if (vy < 0) {
            if (-vy * 7 > radaralt) {
                float timeLeft = radaralt / -vy;
                SetGLTransparentLines();
                SetGLText();
                sprintf(temp, "MARKKOLLISION %.1f", timeLeft);
                DrawHUDText(temp, &fontMain, 0, (50) - ((textHeight(1.0) * text_scale) / 2), 1, color);
            }
        }
    }
}

void DrawFuelTime(float x, float y) {
    char temp[100];
    float totalFuel = getTotalFuel();
    float fuelFlow = getFuelFlow();
    //float vx = getVX();
    float groundspeed = getGroundSpeed();

    float eta = totalFuel / fuelFlow;
    float range = groundspeed * eta;
    int sec, h, m, s;
    sec = eta;

    h = (sec / 3600);

    m = (sec - (3600 * h)) / 60;

    s = (sec - (3600 * h) - (m * 60));

    SetGLTransparentLines();
    SetGLText();
    glColor4fv(color);
    sprintf(temp, "Bransle %.0f %.2f - %.0f km - %02d:%02d:%02d", totalFuel, fuelFlow, range / 1000, h, m, s);
    DrawHUDText(temp, &fontMain, x, y, 0, color);
    // sprintf(temp, "Range %.0f km", range / 1000);
    // DrawHUDText(temp, &fontMain, x, y - (textHeight(1.2) * text_scale), 0, color);
}

void DrawNAVText(float x, float y) {
    char temp[260];
    char textId[160];
    float nav1_distance = getNAV1Distance();
    float nav1_eta = getNAV1ETA();
    //float vx = getVX();

    int sec, h, m, s;
    sec = nav1_eta;

    h = (sec / 3600);

    m = (sec - (3600 * h)) / 60;

    s = (sec - (3600 * h) - (m * 60));

    SetGLTransparentLines();
    SetGLText();
    glColor4fv(color);
    getNAV1Id(textId);
    sprintf(temp, "%s  %.1fkm - %02d:%02d:%02d", textId, nm2km(nav1_distance), h, m, s);
    DrawHUDText(temp, &fontMain, x, y, 0, color);
}

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
    DrawHUDText(tempText, &fontMain, -200, yy, 1, color);

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
        DrawCompass(0, y_pos + 220);
        TranslateToCenter();
        DrawSpeed(0, y_pos);
        DrawAlpha(SPEED_POS_X, y_pos);
        DrawAltitude(0, y_pos);
        DrawNAVText(-100, 280);
        DrawFuelTime(-450, 300);
    } else {
        TranslateToCenter();
        DrawCompass(0, 320);
        TranslateToCenter();
        DrawSpeed(0, 0);
        DrawAlpha(SPEED_POS_X, 0);
        DrawAltitude(0, 0);
        DrawNAVText(-100, -450);
        DrawFuelTime(-450, -750);
    }

    TranslateToCenter();
    DrawVector();

    TranslateToCenter();

    DrawGroundCollision();

    TranslateToCenter();
    DrawHorizionLines();
}