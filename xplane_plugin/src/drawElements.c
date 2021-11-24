#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>

int stab_error;

void DrawTest() {
    float compas_y = 220;
    char tempText[100];
    float heading = getHeading();
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

    glVertex2f(0 * HUD_SCALE, compas_y + 0 * HUD_SCALE);
    glVertex2f(0 + 10 * HUD_SCALE, compas_y - 20 * HUD_SCALE);
    glVertex2f(0 * HUD_SCALE, compas_y + 0 * HUD_SCALE);
    glVertex2f(0 - 10 * HUD_SCALE, compas_y - 20 * HUD_SCALE);

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

            glVertex2f(offset * HUD_SCALE, compas_y * HUD_SCALE);
            glVertex2f(offset * HUD_SCALE, compas_y + 20 * HUD_SCALE);

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

            glVertex2f(offset * HUD_SCALE, compas_y * HUD_SCALE);
            glVertex2f(offset * HUD_SCALE, compas_y + 5 * HUD_SCALE);

            glEnd();
        }
    }
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
            DrawHUDText(tempText, &fontMain, offset * HUD_SCALE, compas_y + 25 * HUD_SCALE, 1, color);
        }
    }
    // sprintf(tempText, "test%02d", 1);
    // DrawHUDText(tempText, &fontMain, 0 * HUD_SCALE, compas_y * HUD_SCALE, 1, color);
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
    colorglass[3] = GetGlassDarkness();
    colorglassB[3] = GetGlassDarkness() - 0.3;

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
    // DrawHUDText(tempText, &fontMain, 10 * HUD_SCALE, 10, 1, color);

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
    int gear = getGear();
    tail_pos = fmin(tail_pos, 40); // Fenans längd ska motsvara 20km/h
    tail_pos = fmax(tail_pos, -40);
    //y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(-x_pos, -y_pos, 0);
    glRotatef(-angle, 0, 0, 1);

    if (!markKontakt()) {
        DrawCircle(10 * HUD_SCALE);
    }
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
    float heading = getHeading();
    char tempText[10];
    float smallTextScale = 0.75;
    int gear = getGear();

    glColor4fv(color);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten

    glLineWidth(line_width + 1.0);
    glBegin(GL_LINES);

    // prickar
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
            glVertex2f(offset, compas_y * HUD_SCALE);
            glVertex2f(offset, (compas_y + 10) * HUD_SCALE);
        }
    }

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
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {
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
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(15 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(400 - 15 * HUD_SCALE, CalcFOVAngle(i));

            // en knöl uppåt
            //mitten
            glVertex2f(0 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(15 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(0 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(-15 * HUD_SCALE, CalcFOVAngle(i));

            // höger
            glVertex2f(400 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(400 + 15 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(400 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(400 - 15 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(400 + 15 + 80 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(400 + 15 * HUD_SCALE, CalcFOVAngle(i));

            // andra sidan
            glVertex2f(-15 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-400 + 15 * HUD_SCALE, CalcFOVAngle(i));

            // en knöl uppåt
            // vänster
            glVertex2f(-400 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(-400 - 15 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-400 * HUD_SCALE, CalcFOVAngle(i) + (40 * HUD_SCALE));
            glVertex2f(-400 + 15 * HUD_SCALE, CalcFOVAngle(i));

            glVertex2f(-400 - 15 - 80 * HUD_SCALE, CalcFOVAngle(i));
            glVertex2f(-400 - 15 * HUD_SCALE, CalcFOVAngle(i));

            glEnd();
            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }

    // Cirkel vid 90 grader
    DrawCircleXY(200 * HUD_SCALE, 0, CalcFOVAngle(90));
    DrawCircleXY(200 * HUD_SCALE, 0, CalcFOVAngle(-90));
    float textradius_pos = 100;

    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(0 * HUD_SCALE, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading)) * textradius_pos * HUD_SCALE, CalcFOVAngle(-90) - (textradius_pos * HUD_SCALE * cos(to_radians(heading))));
    glVertex2f(0 * HUD_SCALE, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90)) * textradius_pos * HUD_SCALE, CalcFOVAngle(-90) - (textradius_pos * HUD_SCALE * cos(to_radians(heading + 90))));
    glVertex2f(0 * HUD_SCALE, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90 + 90)) * textradius_pos * HUD_SCALE, CalcFOVAngle(-90) - (textradius_pos * HUD_SCALE * cos(to_radians(heading + 90 + 90))));
    glVertex2f(0 * HUD_SCALE, CalcFOVAngle(-90));
    glVertex2f(sin(to_radians(heading + 90 + 90 + 90)) * textradius_pos * HUD_SCALE, CalcFOVAngle(-90) - (textradius_pos * HUD_SCALE * cos(to_radians(heading + 90 + 90 + 90))));

    glEnd();

    SetGLText();

    glPushMatrix();
    //glRotatef(heading, 0, CalcFOVAngle(90), 1);
    sprintf(tempText, "N");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading)) * textradius_pos * HUD_SCALE, CalcFOVAngle(90) - (cos(to_radians(-heading)) * textradius_pos), 1, color);
    glRotatef(90, sin(to_radians(-heading + 180)) * textradius_pos * HUD_SCALE, CalcFOVAngle(90) - (cos(to_radians(-heading + 180)) * textradius_pos), 1);
    sprintf(tempText, "S");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 180)) * textradius_pos * HUD_SCALE, CalcFOVAngle(90) - (cos(to_radians(-heading + 180)) * textradius_pos), 1, color);
    glRotatef(90, 0, 0, 1);
    sprintf(tempText, "E");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 90)) * textradius_pos * HUD_SCALE, CalcFOVAngle(90) - (cos(to_radians(-heading + 90)) * textradius_pos), 1, color);
    glRotatef(90, 0, 0, 1);
    sprintf(tempText, "W");
    DrawHUDText(tempText, &fontMain, sin(to_radians(-heading + 270)) * textradius_pos * HUD_SCALE, CalcFOVAngle(90) - (cos(to_radians(-heading + 270)) * textradius_pos), 1, color);

    glPopMatrix();

    SetGLText(); // turn on blending
    glPushMatrix();
    glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 10; i < 90; i += 10) {
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, -200 * HUD_SCALE / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);

            // char buffer2[10];
            // sprintf(buffer2, "%d", i);
            // XPLMDrawString(color, -200 * HUD_SCALE, CalcFOVAngle(i), buffer2, NULL, xplmFont_Basic);
        }
    }
    for (int i = -10; i > -90; i -= 10) {
        if ((i > getPitch() - 35) && (i < getPitch() + 30)) {

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
    char temp[100];
    float airspeed = getIAS();
    float groundspeed = getGroundSpeed() * 1.944;
    float mach = getMachSpeed();
    float landingspeed = getLandingSpeed();
    float landingspeed2 = landingspeed + 30;
    int gear = getGear();
    float tail_pos = airspeed - landingspeed;
    float maxSpeed = kmhToknots(2000);
    float climbSpeed = 500; // 500-550knop 0.85 mach

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

    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y - 100 * HUD_SCALE);
    glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + 100 * HUD_SCALE);

    // Markör för snabbast stigningshastighet
    if (airspeed < climbSpeed + 50 && airspeed > climbSpeed - 50) {
        glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + (airspeed - climbSpeed) * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 10 * HUD_SCALE, SPEED_POS_Y + (airspeed - climbSpeed) * HUD_SCALE);
    }

    // Markör för landningshastighet
    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {
        tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 10 * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE);
    }
    // Markör för landningshastighet inflygning
    if (airspeed < landingspeed2 + 50 && airspeed > landingspeed2 - 50) {
        //tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y - (airspeed - landingspeed2) * 2 * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 20 * HUD_SCALE, SPEED_POS_Y - (airspeed - landingspeed2) * 2 * HUD_SCALE);
    }
    // Markör för maxhastighet
    if (airspeed < maxSpeed + 50 && airspeed > maxSpeed - 50) {
        //tail_pos = -tail_pos * 2;
        glVertex2f(SPEED_POS_X * HUD_SCALE, SPEED_POS_Y - (airspeed - maxSpeed) * 2 * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 20 * HUD_SCALE, SPEED_POS_Y - (airspeed - maxSpeed) * 2 * HUD_SCALE);

        glVertex2f(SPEED_POS_X + 20 * HUD_SCALE, SPEED_POS_Y - (airspeed - maxSpeed) * 2 * HUD_SCALE);
        glVertex2f(SPEED_POS_X + 20 * HUD_SCALE, SPEED_POS_Y - 15 - (airspeed - maxSpeed) * 2 * HUD_SCALE);
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
    sprintf(temp, "%.0f", knotsTokmh(airspeed));
    DrawHUDText(temp, &fontMain, (SPEED_POS_X - 30) * HUD_SCALE, (SPEED_POS_Y * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 2, color);

    sprintf(temp, "M %.2f", mach);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale)), 2, color);

    sprintf(temp, "GS%.0f", groundspeed);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 2, color);

    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {

        DrawHUDText("L", &fontMain, SPEED_POS_X + 10 * HUD_SCALE, SPEED_POS_Y + tail_pos * HUD_SCALE - ((fontMain.charHeight / 2 * text_scale)), 0, color);
    }
    if (getSpeedBrake()) {
        sprintf(temp, "LUFTBROMS UTE");
        DrawHUDText(temp, &fontMain, (0) * HUD_SCALE, ((200) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);
    }
    if (getParkBrake()) {
        sprintf(temp, "PARKERINGSBROMS");
        DrawHUDText(temp, &fontMain, (0) * HUD_SCALE, ((150) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);
    }
    if (stab_error > 100) {
        sprintf(temp, "FEL I STABILISERINGS PLUGIN");
        DrawHUDText(temp, &fontMain, (0) * HUD_SCALE, ((100) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);
    }
    if (getStabStatus()) {
        stab_error = 0;
        setStabStatus(0);

    } else {
        stab_error++;
    }

    if (airspeed > maxSpeed) { // Varning när hög fart och landställ ute
        sprintf(temp, "MINSKA FART");
        DrawHUDText(temp, &fontMain, (0) * HUD_SCALE, ((175) * HUD_SCALE) - ((fontMain.charHeight * text_scale) * 2), 1, color);
    }

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha() {
    char temp[20];
    float alpha = getAlphaA();
    float gforce = getGForce();

    glColor4fv(color);

    SetGLText(); // turn on blending

    sprintf(temp, "G %.1f", gforce);
    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale)), 2, color);

    if (getIAS() > 50) {
        sprintf(temp, "$ %.0f", alpha);
    } else {
        sprintf(temp, "$ X");
    }

    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y + 120) * HUD_SCALE) + ((fontMain.charHeight * text_scale) * 2), 2, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

#define ALT_POS_X 315
#define ALT_POS_Y -50
#define ALT_SCALE_METER 400.0f
#define ALT_SCALE_PIXEL 220.0f

float altToPixelY(float value) {
    return value * (ALT_SCALE_PIXEL / ALT_SCALE_METER);
}

void DrawAltitude() {
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

    //glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y - 100 * HUD_SCALE);
    //glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 100 * HUD_SCALE);

    // pil vid texten
    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, ALT_POS_Y + 10 * HUD_SCALE);

    glVertex2f(ALT_POS_X * HUD_SCALE, ALT_POS_Y + 0 * HUD_SCALE);
    glVertex2f((ALT_POS_X - 20) * HUD_SCALE, ALT_POS_Y - 10 * HUD_SCALE);

    glEnd();

    int start = (altitude - ALT_SCALE_METER) / 100;
    int maxA = altitude + ALT_SCALE_METER / 2;
    int minA = altitude - ALT_SCALE_METER / 2;

    for (int i = start * 100; i < altitude + ALT_SCALE_METER; i += 100) {
        if (i < maxA && i > minA && i >= 0) {

            if (((i / 100) % 2) == 0) {
                glLineWidth(line_width);
                glBegin(GL_LINES);
                glVertex2f(ALT_POS_X * HUD_SCALE, (ALT_POS_Y + altToPixelY(-altitude) + altToPixelY(i)) * HUD_SCALE);
                glVertex2f(ALT_POS_X + 20 * HUD_SCALE, (ALT_POS_Y + altToPixelY(-altitude) + altToPixelY(i)) * HUD_SCALE);
                glEnd();
            } else {
                glLineWidth(max(line_width / 2, 1.0));
                glBegin(GL_LINES);
                glVertex2f(ALT_POS_X * HUD_SCALE, (ALT_POS_Y + altToPixelY(-altitude) + altToPixelY(i)) * HUD_SCALE);
                glVertex2f(ALT_POS_X + 10 * HUD_SCALE, (ALT_POS_Y + altToPixelY(-altitude) + altToPixelY(i)) * HUD_SCALE);
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
        glVertex2f(ALT_POS_X * HUD_SCALE, (ALT_POS_Y + rhY) * HUD_SCALE);
        glVertex2f(ALT_POS_X - 30 * HUD_SCALE, (ALT_POS_Y + rhY) * HUD_SCALE);

        glVertex2f(ALT_POS_X * HUD_SCALE, (ALT_POS_Y + rhY) * HUD_SCALE);
        glVertex2f(ALT_POS_X - 10 * HUD_SCALE, (ALT_POS_Y - 20 + rhY) * HUD_SCALE);

        glVertex2f(ALT_POS_X - 15 * HUD_SCALE, (ALT_POS_Y + rhY) * HUD_SCALE);
        glVertex2f(ALT_POS_X - 15 - 10 * HUD_SCALE, (ALT_POS_Y - 20 + rhY) * HUD_SCALE);

        glEnd();
    }

    SetGLText(); // turn on blending

    if (drawRH && radaralt < 1500) {
        sprintf(temp, "RH %.0f", radaralt);
        DrawHUDText(temp, &fontMain, (ALT_POS_X - 20) * HUD_SCALE, (ALT_POS_Y - 20 + rhY) - ((fontMain.charHeight * text_scale)), 0, color);
    }

    for (int i = start * 100; i < altitude + ALT_SCALE_METER; i += 100) {
        if (i < maxA && i > minA && i >= 0) {

            if (((i / 100) % 2) == 0) {
                sprintf(temp, "%d", i);
                DrawHUDText(temp,
                            &fontMain,
                            (ALT_POS_X + 20) * HUD_SCALE,
                            ((ALT_POS_Y + altToPixelY(-altitude) + altToPixelY(i)) * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2),
                            0,
                            color);
            }
        }
    }
    int altdraw = altitude / 10;
    sprintf(temp, "%d", altdraw * 10);
    DrawHUDText(temp, &fontMain, (ALT_POS_X - 30) * HUD_SCALE, (ALT_POS_Y * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 2, color);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
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
                SetGLText();
                sprintf(temp, "MARKKOLLISION %.1f", timeLeft);
                DrawHUDText(temp, &fontMain, 0 * HUD_SCALE, (50 * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 1, color);
            }
        }
    }
}

void DrawFuelTime() {
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

    SetGLText();
    sprintf(temp, "%.0f %.2f - %02d:%02d:%02d", totalFuel, fuelFlow, h, m, s);
    DrawHUDText(temp, &fontMain, 0 * HUD_SCALE, (-600 * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 0, color);
    sprintf(temp, "Range %.0f km", range / 1000);
    DrawHUDText(temp, &fontMain, 0 * HUD_SCALE, (-650 * HUD_SCALE) - ((fontMain.charHeight * text_scale) / 2), 0, color);
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

    int alt = altitude; // * 0.3048;
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
