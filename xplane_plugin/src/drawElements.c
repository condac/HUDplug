#include "HUDplug.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "fonts.h"
#include "datarefs.h"
#include <math.h>
#include <complex.h>
int stab_error;

void DrawTest() {

    SetGLTransparentLines();
    glEnable(GL_BLEND);

    glLineWidth(2);
    glBegin(GL_LINES);

    glVertex2f(-3000, 500);
    glVertex2f(3000, -500);

    glVertex2f(-3000, -500);
    glVertex2f(3000, 500);

    glEnd();
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Standard blend, baserat på alpha
    // detta motsvarar mode 6 7
    //glBlendFunc(modes[glass_type], modes[glass_type2]);
    float testcolor[] = {-0.5, 1.0, -0.5, 1.0};
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
    float heading = getTrueHeading();
    float nav1_heading = getNAVxHeading();
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
    for (int i = 0; i < 36; i++) {

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
    colorglassB[3] = GetGlassDarkness() * glass_darkness;

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
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(-glass_width / 2, CalcFOVAngle(-18));
    glVertex2f(-glass_width / 2 + 50, CalcFOVAngle(-18));

    glEnd();
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
        float scale = glass_width / 280;
        glTranslatef(glass_width / 2, glass_height / 2 + glass_width / 3, 0);
        glBegin(GL_POLYGON);

        // glColor4fv(colorglassB);
        // glVertex2f(0, -glass_height);
        //
        // glColor4fv(colorglass);
        // glVertex2f(0, glass_height - 50);
        // glVertex2f(100, glass_height);
        //
        // glVertex2f(glass_width - 100, glass_height);
        // glVertex2f(glass_width, glass_height - 50);
        //
        // glColor4fv(colorglassB);
        // glVertex2f(glass_width, -glass_height);

        glColor4fv(colorglassB);
        glVertex2f(-100 * scale, -240 * scale);
        glVertex2f(-140 * scale, -150 * scale);

        glColor4fv(colorglass);
        glVertex2f(-140 * scale, -55 * scale);
        glVertex2f(-106 * scale, -16.5 * scale);

        // böjen
        glVertex2f(-68 * scale, -6.08 * scale);
        glVertex2f(-38 * scale, -2.08 * scale);
        glVertex2f(-0 * scale, -0.00 * scale);

        // andra sidan

        glVertex2f(38 * scale, -2.08 * scale);
        glVertex2f(68 * scale, -6.08 * scale);

        glVertex2f(106 * scale, -16.5 * scale);
        glVertex2f(140 * scale, -55 * scale);

        glVertex2f(140 * scale, -150 * scale);
        glVertex2f(100 * scale, -240 * scale);

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

void DrawMKV() {
}

void DrawVector() {
    SetGLTransparentLines();
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    float airspeed = getIAS();
    float y_pos = 0.0;
    float x_pos = 0.0;
    //float alpha = CalcFOVAngle(myGetAlpha());
    float beta = CalcFOVAngle(myGetBeta());
    float tail_length = 15;
    float wing_length = 34;
    float body_radius = 8;
    float tail_pos = airspeed - getLandingSpeed() + 10;
    float angle = getRoll();
    float alphaA = getAlphaA();
    float radaralt = getRadarAltitude();
    float local_vy = getVY();
    // float prickx = getPrickX();
    // float pricky = getPrickY();
    int gear = getGear();
    int screen_width;
    int screen_height;
    XPLMGetScreenSize(&screen_width, &screen_height);

    if (viggen_mode >= 1) {
        // Fenan ska ligga diktan vid alfa 12, (eller alfa 15 om valt)
        // Halva fenan fel motsvarar ca 2 alfa enheter
        // Fenan är 20 pixlar lång
        float landningsAlfa = viggen_landning_alfa;
        tail_pos = body_radius + (landningsAlfa - alphaA) * 5.0f;
    } else {
        // JAS
        tail_pos = airspeed - getLandingSpeed() + 0;
    }
    if (getGroundSpeed() < 5) {
        //alpha = 0;
        beta = 0;
    }

    tail_pos = fmin(tail_pos, 30); // Fenans längd ska motsvara 20km/h
    tail_pos = fmax(tail_pos, -40);
    //y_pos = fov_pixels * getAlphaA();
    glColor4fv(color);

    // x_pos = sin(to_radians(-angle)) * alpha;
    // y_pos = cos(to_radians(-angle)) * alpha;
    // x_pos = x_pos + cos(to_radians(angle)) * beta;
    // y_pos = y_pos + sin(to_radians(angle)) * beta;
    
    x_pos = CalcFOVAngle(-dr_vectorBeta);
    y_pos = CalcFOVAngle(-dr_vectorAlpha);
    

    if (viggen_mode >= 1) {
        // Med Viggen när man landar så ska vektorns position övergå till att visa sjunkhastighet relaterat till 2.96 meter per sekund
        // Detta sker vid ca 20 meter RHM
        if (radaralt < 20 && local_vy < 0.0f && gear) {
            float y_pos2 = CalcFOVAngle(getPitch());
            //y_pos = CalcFOVAngle(local_vy);
            x_pos = sin(to_radians(-angle)) * CalcFOVAngle(-local_vy);
            y_pos = cos(to_radians(-angle)) * CalcFOVAngle(-local_vy);
            x_pos = x_pos + cos(to_radians(angle)) * beta;
            y_pos = y_pos2 + CalcFOVAngle(-local_vy);
        }
        if (markKontakt() >= 1) {
            float y_pos2 = CalcFOVAngle(getPitch());
            y_pos = y_pos2 + CalcFOVAngle(8);
        }
    }
    int utanfor = 0;
    if (x_pos > 400) {
        x_pos = 400 - 60;
        utanfor = 1;
    }
    if (x_pos < -400) {
        x_pos = -400 + 60;
        utanfor = 1;
    }
    if (y_pos < -240) {
        y_pos = -240 + 30;
        utanfor = 1;
    }
    if (y_pos > 612.0f) { // 682 är TEXTURE_WIDTH*0.666 som är 2/3 delar av glaset i nederkan
        y_pos = 612.0f - 30;
        utanfor = 1;
    }
    // if (alphaA > 20) {
    //     utanfor = 1;
    // }
    // if (alphaA < -10) {
    //     utanfor = 1;
    // }

    glPushMatrix();
    //glRotatef(angle, 0, 0, 1);
    glTranslatef(-x_pos, -y_pos, 0);
    //glRotatef(-angle, 0, 0, 1);

    if (!markKontakt() && utanfor == 0) {
        DrawCircle(body_radius);
    } else if (viggen_mode >= 1) {
        DrawCircle(body_radius); // viggen ritar alltid en cirkel
    }
    if (utanfor == 1) {

        char tempText[10];
        sprintf(tempText, "%.0f", alphaA);
        //DrawHUDText(tempText, &fontMain, 0, -textHeight(1.0), 1, color);
        drawLineText(tempText, 0, -textHeight(1.0), 1.0, 1);
    }
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(body_radius, 0);
    glVertex2f(wing_length, 0);

    glVertex2f(-body_radius, 0);
    glVertex2f(-wing_length, 0);

    if (gear) {
        if (viggen_mode >= 1) {
            // Viggen ritarn ingen fena på marken i vissa lägen
            //if (!markKontakt()) {
            glVertex2f(0, tail_pos - 0);
            glVertex2f(0, tail_pos + tail_length);
            //}
        } else {
            glVertex2f(0, tail_pos - 0);
            glVertex2f(0, tail_pos + tail_length);
        }

    } else {
        glVertex2f(0, body_radius);
        glVertex2f(0, body_radius + tail_length);
    }

    glEnd();

    // // ILS indikator
    //
    // if (getPrickActive() == 1 && markKontakt() == 0) {
    //     DrawFillCircleXY(5, CalcFOVAngle(prickx), pricky);
    // }

    glRotatef(angle, 0, 0, 1);
    glTranslatef(x_pos, y_pos, 0); // set position back
    glRotatef(-angle, 0, 0, 1);
    glPopMatrix();
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

    int screen_width;
    int screen_height;
    XPLMGetScreenSize(&screen_width, &screen_height);
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

    // Solida 0 linjen med uppehåll för hastighet och höjd
    glVertex2f(124, 0);
    glVertex2f(230, 0);

    glVertex2f(330, 0);
    glVertex2f(590, 0);

    glVertex2f(-124, 0);
    glVertex2f(-230, 0);

    glVertex2f(-330, 0);
    glVertex2f(-590, 0);

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

    // //y_prick
    // sprintf(tempText, "%.2f", banax);
    // DrawHUDText(tempText, &fontMain, 0, 0, 1, color);
    // sprintf(tempText, "%.2f", banay);
    // DrawHUDText(tempText, &fontMain, 0, 100, 1, color);

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

// #define SPEED_POS_X -250 // flyttade till .h filen
// #define SPEED_POS_Y -50
// #define SPEED_SCALE_PIXEL 150.0f

void DrawSpeed(float x, float y) {
    char temp[100];
    char temp2[50];
    float frp = getFRP();
    float airspeed = getIAS();
    float groundspeed = getGroundSpeed() * 1.944;
    float mach = getMachSpeed();
    float landingspeed = getLandingSpeed();
    float landingspeed2 = landingspeed + 30;
    int gear = getGear();
    float tail_pos = airspeed - landingspeed;
    float maxSpeed = kmhToknots(2000);
    float eco = 300; //300 knots
    static float avgspeed = 0.0;
    static float prev_speed = 0;
    static float avg_frp = 0.0;

    float machKnotsRatio = airspeed / mach;
    float climbSpeed = machKnotsRatio * 0.85; // 500-550knop 0.85 mach baserat på viggen innan vi hittat data för jas

    avgspeed = (avgspeed * 9 + airspeed) / 10;
    avg_frp = (avg_frp * 9 + frp) / 10;

    if (gear) {
        maxSpeed = kmhToknots(600);
    }
    if (markKontakt()) {
        maxSpeed = kmhToknots(350);
    }

    SetGLTransparentLines();
    glColor4fv(color);

    glPushMatrix();
    glTranslatef(x, 0, 0);
    // lodrät linje för hastighetsmätaren
    glLineWidth(line_width);
    glBegin(GL_LINES);

    glVertex2f(SPEED_POS_X, y - SPEED_SCALE_PIXEL / 2);
    glVertex2f(SPEED_POS_X, y + SPEED_SCALE_PIXEL / 2);

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

    // pil vid hastighetstexten
    glVertex2f(SPEED_POS_X, y + 0);
    glVertex2f((SPEED_POS_X - 13), y + 6);

    glVertex2f(SPEED_POS_X, y + 0);
    glVertex2f((SPEED_POS_X - 13), y - 6);

    // Linjer upp och ner beroende på om hatigheten ökar eller minskar

    float newspeed = -(prev_speed * 10 - avgspeed * 10) * avg_frp * 2500;
    prev_speed = avgspeed;
    newspeed = fmax(newspeed, -120);
    newspeed = fmin(newspeed, 120);
    if (newspeed > 0) {
        glVertex2f((SPEED_POS_X - 13), y + 7);
        glVertex2f((SPEED_POS_X - 13), y + 7 + newspeed);
    }
    if (newspeed < 0) {
        glVertex2f((SPEED_POS_X - 13), y - 7);
        glVertex2f((SPEED_POS_X - 13), y - 7 + newspeed);
    }

    glEnd();

    SetGLText(); // turn on blending
    if (metric) {
        sprintf(temp, "%.0f", knotsTokmh(airspeed));
    } else {
        sprintf(temp, "%.0f", airspeed);
    }
    //DrawHUDText(temp, &fontMain, (SPEED_POS_X - 30)  , (SPEED_POS_Y  ) - (textHeight(1.0) / 2), 2, color);
    drawLineText(temp, (SPEED_POS_X - 18), (y) - (textHeight(1.0) / 2), 1.0, 2);

    sprintf(temp2, "%.2f", mach);
    if (temp2[0] == '0') {
        temp2[0] = ' ';
    }
    if (temp2[1] == '.') {
        temp2[1] = ',';
    }

    sprintf(temp, "M%s", temp2);
    //DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120)  ) - ((textHeight(1.0) * text_scale)), 2, color);
    drawLineText(temp, (SPEED_POS_X)*HUD_SCALE, ((y - 85)) - (textHeight(1.4) * 1), 1.0, 2);

    if (metric) {
        sprintf(temp, "GS%.0f", knotsTokmh(groundspeed));
    } else {
        sprintf(temp, "GS%.0f", groundspeed);
    }
    //sprintf(temp, "GS%.0f", groundspeed);
    //    DrawHUDText(temp, &fontMain, (SPEED_POS_X)*HUD_SCALE, ((SPEED_POS_Y - 120)  ) - ((textHeight(1.0) * text_scale) * 2), 2, color);
    drawLineText(temp, (SPEED_POS_X)*HUD_SCALE, ((y - 85)) - (textHeight(1.4) * 2.0), 1.0, 2);
    // Markör för snabbast stigningshastighet
    if (airspeed < climbSpeed + 50 && airspeed > climbSpeed - 50) {
        DrawHUDText("O", &fontMain, SPEED_POS_X + 10, y - (airspeed - climbSpeed) * 2 - ((textHeight(1.0) / 2 * text_scale)), 0, color);
    }
    if (airspeed < landingspeed + 50 && airspeed > landingspeed - 50) {

        DrawHUDText("L", &fontMain, SPEED_POS_X + 10, y + tail_pos - ((textHeight(1.0) / 2 * text_scale)), 0, color);
    }

    float trim = getPitchTrim();
    static float trim_prev;
    if (trim_prev != trim) {
        trim_prev = trim;
        sprintf(temp, "TRIM %.0f", trim * 100);
        DrawHUDText(temp, &fontMain, (0), ((180)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }
    // if (getSpeedBrake()) {
    //     sprintf(temp, "LUFTBROMS UTE");
    //     DrawHUDText(temp, &fontMain, (0), ((200)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    // }
    if (getParkBrake()) {
        sprintf(temp, "PARKERINGSBROMS");
        DrawHUDText(temp, &fontMain, (0), ((225 - 80)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    }
    // if (stab_error > 100 && !getPause() && viggen_mode == 0) {
    //     sprintf(temp, "FEL I STABILISERINGS PLUGIN");
    //     DrawHUDText(temp, &fontMain, (0), ((100)) - ((textHeight(1.0) * text_scale) * 2), 1, color);
    // }
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
    glPopMatrix();
    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

void DrawAlpha(float x, float y) {
    char temp[20];
    float alpha = getAlphaA();
    float gforce = getGForce();

    glColor4fv(color);

    SetGLText(); // turn on blending

    sprintf(temp, "G %.1f", gforce);
    DrawHUDText(temp, &fontMain, (x - 60), ((y + 65)) + ((textHeight(1.0) * text_scale)), 0, color);

    if (getIAS() > 50) {
        sprintf(temp, "& %.0f", alpha);
    } else {
        sprintf(temp, "& X");
    }

    DrawHUDText(temp, &fontMain, (x - 60), ((y + 65)) + ((textHeight(1.4) * 2 * text_scale)), 0, color);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

// #define ALT_POS_X 255
// #define ALT_POS_Y -50
// #define ALT_SCALE_METER 450.0f
// #define ALT_SCALE_PIXEL 150.0f

float altToPixelY(float value) {
    return value * (ALT_SCALE_PIXEL / ALT_SCALE_METER);
}

void DrawAltitude(float x, float y) {
    char temp[20];
    float altitude = getAltitude();
    float radaralt = getRadarAltitude();

    if (metric) {
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
                glLineWidth(max2(line_width / 2, 1.0));
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
                // if (metric) {
                //     sprintf(temp, "%d", i);
                // } else {
                //     int aaa = m2feet(i);
                //     sprintf(temp, "%d", aaa);
                // }
                if (i < 1000) {
                    sprintf(temp, "%d", i);
                    DrawHUDText(temp, &fontMain, (ALT_POS_X + 20), ((y + altToPixelY(-altitude) + altToPixelY(i))) - ((textHeight(1.0) * text_scale) / 2), 0, color);
                } else {
                    int tusental = i / 1000;
                    sprintf(temp, "%d", tusental);
                    //DrawHUDText(temp, &fontMain, (ALT_POS_X + 20), ((y + altToPixelY(-altitude) + altToPixelY(i))) - ((textHeight(1.0) * text_scale) / 2), 0, color);
                    drawLineText(temp, (ALT_POS_X + 48), ((y + altToPixelY(-altitude) + altToPixelY(i))) - ((textHeight(1.0) * text_scale) / 2), 1.0, 2);
                    tusental = tusental * 1000;
                    sprintf(temp, "%03d", i - tusental);
                    drawLineText(temp, (ALT_POS_X + 48), ((y + altToPixelY(-altitude) + altToPixelY(i))) - ((textHeight(1.0) * text_scale) / 2), 0.7, 0);
                }
            }
        }
    }
    int altdraw = 0; // altitude / 10;

    if (metric) {
        altdraw = feetTom(altitude) / 10;
    } else {
        altdraw = altitude / 10;
    }

    if (altitude < 1050) {
        altdraw = altitude / 10;
        sprintf(temp, "%d", altdraw * 10);
        DrawHUDText(temp, &fontMain, (ALT_POS_X - 30), (y) - ((textHeight(1.0) * text_scale) / 2), 2, color);
    }

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
}

float constrain(float val, float lower, float upper) {
    if (lower > upper) {
        float temp = lower;
        lower = upper;
        upper = temp;
    }
    return fmax(lower, fmin(upper, val));
}

float interpolate(float x1, float y1, float x2, float y2, float value) {
    float y = y1 + (y2 - y1) / (x2 - x1) * (value - x1);
    return y;
}

void DrawGroundCollision() {
    char temp[100];
    float y_pos = CalcFOVAngle(getPitch());
    float angle = getRoll();
    float airspeed = getIAS();

    int gear = getGear();

    // Beräkna lastfaktorn
    float lastfaktor = interpolate(200, 1.0, 550, 5.0, knotsTokmh(airspeed));
    lastfaktor = constrain(lastfaktor, 1.0, 5.0);

    float radaralt = getRadarAltitude() * 0.9 - 30;
    radaralt = fmax(0.0, radaralt);
    float speed = getTrueSpeed();

    float radie = (speed * speed) / (lastfaktor * 9.82);
    float bb = radie - radaralt;
    float vinkel = acos(bb / radie);
    float markvinkeln = CalcFOVAngle(to_degrees(vinkel));

    if (getMkvLarm() > 0) {
        float gneed = getMkvGneed();
        int needMore = getMkvNeedMore();

        SetGLTransparentLines();
        SetGLText();
        sprintf(temp, "MARKKOLLISION");
        DrawHUDText(temp, &fontMain, 0, (50) - ((textHeight(1.0) * text_scale) / 2), 1, color);

        glPushMatrix();
        SetGLTransparentLines();
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        glColor4fv(color);
        glLineWidth(line_width);

        glRotatef(angle, 0, 0, 1);
        glTranslatef(0, fmin(fmax(-y_pos - markvinkeln, -300.0), 100), 0);
        //glTranslatef(0, -50, 0);

        if (gneed > 9) {
            sprintf(temp, "9.0+");
            DrawHUDText(temp, &fontMain, 0, -30, 1, color);
        } else if (gneed > lastfaktor) {
            sprintf(temp, "%.1f", gneed);
            DrawHUDText(temp, &fontMain, 0, -30, 1, color);
        }

        if (needMore == 0) {
            // basen

            glBegin(GL_LINE_STRIP);

            glVertex2f(80, 0);
            glVertex2f(80, -45);
            glVertex2f(-80, -45);
            glVertex2f(-80, 0);

            glVertex2f(-80 - 12, 0);
            glVertex2f(-80 - 12, -45 - 12);
            glVertex2f(80 + 12, -45 - 12);
            glVertex2f(80 + 12, 0);

            glEnd();

            // Pil
            glBegin(GL_LINE_STRIP);

            glVertex2f(86 + 13, 0);
            glVertex2f(86 - 13, 0);
            glVertex2f(86, 22);
            glVertex2f(86 + 13, 0);

            glEnd();
            // Pil
            glBegin(GL_LINE_STRIP);

            glVertex2f(-86 + 13, 0);
            glVertex2f(-86 - 13, 0);
            glVertex2f(-86, 22);
            glVertex2f(-86 + 13, 0);

            glEnd();
        }

        glPopMatrix();
    } else {
        if (!gear && radaralt < 575 && viggen_mode < 1) {
            glPushMatrix();
            SetGLTransparentLines();
            glColor4fv(color);

            glRotatef(angle, 0, 0, 1);
            glTranslatef(0, -y_pos - markvinkeln, 0);

            glBegin(GL_LINE_STRIP);

            glVertex2f(80, 10);
            glVertex2f(80, 0);
            glVertex2f(-80, 0);
            glVertex2f(-80, 10);

            glEnd();
            glPopMatrix();
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
    if (strlen(textId) > 1) {
        sprintf(temp, "%s  %.1fkm - %02d:%02d:%02d", textId, nm2km(nav1_distance), h, m, s);
        DrawHUDText(temp, &fontMain, x, y, 2, color);
    }
}

void DrawHorizionLinesViggen() {
    // Viggen mode
    float airspeed = getIAS();
    float altitude = getAltitude();
    float alpha = myGetAlpha();
    float y_pos = CalcFOVAngle(alpha);
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
    y_pos = CalcFOVAngle(pitch);

    glColor4fv(color);
    glPushMatrix();
    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    // 0 horizonten
    float yy = 0;
    if (gear) {
        yy = CalcFOVAngle(-3);

        // Heldragen linje vid hjorizonten när landställ är nere
        glLineWidth(line_width * 2);
        glBegin(GL_LINES);
        glVertex2f(-480, CalcFOVAngle(0));
        glVertex2f(480, CalcFOVAngle(0));
        glEnd();
        glLineWidth(line_width);
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
    } else {
        glLineWidth(line_width * 2);
    }

    glBegin(GL_LINES);

    // Prick i mitten
    glVertex2f(-line_width, yy);
    glVertex2f(line_width, yy);

    glVertex2f(45, yy);
    glVertex2f(480, yy);

    glVertex2f(-45, yy);
    glVertex2f(-480, yy);

    glEnd();
    glLineWidth(line_width);
    // Altitude text på 0 horizonten

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
    DrawHUDText(tempText, &fontMain, 135, yy + line_width * 3, 1, color);

    //XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    SetGLTransparentLines();
    // linjer
    glColor4fv(color);
    for (int i = 5; i < 90; i += 5) {
        if ((i > pitch - 28) && (i < pitch + 28)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400, CalcFOVAngle(i));
            glVertex2f(140, CalcFOVAngle(i));

            glVertex2f(-400, CalcFOVAngle(i));
            glVertex2f(-140, CalcFOVAngle(i));
            glEnd();
        }
    }
    int start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > pitch - 28) && (i < pitch + 28)) {
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(400, CalcFOVAngle(i));
            glVertex2f(140, CalcFOVAngle(i));

            glVertex2f(-400, CalcFOVAngle(i));
            glVertex2f(-140, CalcFOVAngle(i));
            glEnd();
        }
    }
    SetGLText(); // turn on blending

    glScalef(smallTextScale, smallTextScale, 0);
    for (int i = 5; i < 90; i += 5) {
        if ((i > pitch - 28) && (i < pitch + 28)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }
    start = -5;
    if (gear) {
        start = -10;
    }
    for (int i = start; i > -90; i -= 5) {
        if ((i > pitch - 28) && (i < pitch + 28)) {

            sprintf(tempText, "%d", i);
            DrawHUDText(tempText, &fontMain, 200 / smallTextScale, CalcFOVAngle(i) / smallTextScale, 1, color);
        }
    }
    glPopMatrix();
}

void DrawCompassViggen(float x, float y) {
    // Compas lines
    int gear = getGear();
    float angle = getRoll();

    float heading = getHeading();
    float pitch = getPitch();
    char tempText[32];

    float smallTextScale = 0.85;

    float compas_y = CalcFOVAngle(-4);
    if (gear) {

        compas_y = CalcFOVAngle(0.5);
    }
    glPushMatrix();

    float y_pos = CalcFOVAngle(pitch);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(0, -y_pos, 0);

    SetGLTransparentLines();
    glColor4fv(color);

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

    glScalef(smallTextScale, smallTextScale, 0);

    // Compas lines text
    for (int i = 0; i < 36; i++) {

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
    glRotatef(-angle, 0, 0, 1);
    glPopMatrix();
}

void drawSpeedAlphaViggen(float x, float y) {
    float airspeed = getIAS();
    float alpha = myGetAlpha();
    float mach = getMachSpeed();

    char tempText[32];

    sprintf(tempText, "& %.0f", alpha); // $ is replaced with alpha sign in bitmap
    DrawHUDText(tempText, &fontMain, (x)*HUD_SCALE, ((y + 120)) + ((textHeight(1.0) * text_scale) * 2), 1, color);

    //sprintf(tempText, "%.0f", airspeed);
    if (metric) {
        sprintf(tempText, "%.0f", knotsTokmh(airspeed));
    } else {
        sprintf(tempText, "%.0f", airspeed);
    }
    DrawHUDText(tempText, &fontMain, (x)*HUD_SCALE, ((y + 120)), 1, color);

    if (!dr_gear) {
        sprintf(tempText, "M %.2f", mach);
        DrawHUDText(tempText, &fontMain, (x)*HUD_SCALE, ((y - 120)) - ((textHeight(1.0) * text_scale)), 1, color);
    }

    if (dr_pitch > 9 || dr_pitch < -9) {
        int alt = dr_altitude; // * 0.3048;
        if (metric) {
            alt = feetTom(dr_altitude);
        } else {
            alt = dr_altitude;
        }
        alt = alt / 10;
        alt = alt * 10;
        snprintf(tempText, 13, "%03d", alt);
        DrawHUDText(tempText, &fontMain, 135, -10, 1, color);
    }
}

void drawADHelp() {
    // liten linje och indikerare för att få 16 grader AD broms
    if (markKontakt()) {

        float angle = getRoll();

        float pitch = getPitch();
        char tempText[32];

        float smallTextScale = 0.85;

        glPushMatrix();
        SetGLTransparentLines();
        glColor4fv(color);

        glLineWidth(line_width);
        glBegin(GL_LINES);

        glVertex2f(0, 0);
        glVertex2f(0, -20);

        glEnd();

        float y_pos = CalcFOVAngle(pitch);
        float ad_angle = CalcFOVAngle(16);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(0, -y_pos, 0);

        glBegin(GL_LINES);

        glVertex2f(-30, ad_angle);
        glVertex2f(30, ad_angle);

        glEnd();

        SetGLText(); // turn on blending

        glScalef(smallTextScale, smallTextScale, 0);

        sprintf(tempText, "AD");
        DrawHUDText(tempText, &fontMain, 0, ad_angle / smallTextScale, 1, color);

        glRotatef(-angle, 0, 0, 1);
        glPopMatrix();
    }
}

void drawPrick() {

    float y_pos = CalcFOVAngle(getPitch());
    float angle = getRoll();
    //float heading = getHeading();
    //char tempText[10];
    //float smallTextScale = 0.75;
    //int gear = getGear();
    float prickx = getPrickX();
    float pricky = getPrickY();
    float x_pos = CalcFOVAngle(prickx);
    float y_prick = CalcFOVAngle(pricky);
    float banax = CalcFOVAngle(getBanaX());
    float banay = CalcFOVAngle(getBanaY());
    // ILS indikator

    glPushMatrix();

    if (getPrickActive() == 1 && markKontakt() == 0) {
        SetGLTransparentLines();
        //DrawFillCircleXY(5, 0, -100);
        glColor4fv(color);
        glPushMatrix();
        glRotatef(angle, 0, 0, 1);
        glTranslatef(0, -y_pos, 0);

        // Landingsbanan inringad
        glLineWidth(line_width);
        float storlek = interpolate(20.0f, 70.0f, 3500.0f, 20.0f, getBanaDist());
        if (storlek < 20) {
            storlek = 20;
        }
        DrawBanaXY(storlek, banax, banay);

        int utanfor = 0;
        if (x_pos > CalcFOVAngle(13)) {
            x_pos = CalcFOVAngle(13);
            utanfor = 1;
            y_pos = y_pos / 10;
        }
        if (x_pos < CalcFOVAngle(-13)) {
            x_pos = CalcFOVAngle(-13);
            utanfor = 1;
            y_pos = y_pos / 10;
        }
        if (y_pos - y_prick < CalcFOVAngle(-10)) {
            y_prick = CalcFOVAngle(10);
            utanfor = 1;
            //y_prick = y_pos;
        }
        if (y_pos - y_prick > -CalcFOVAngle(-19)) {

            // den här är nedåt i bilden
            y_prick = CalcFOVAngle(-19);
            utanfor = 1;
            //     y_prick = y_pos-glass_height / 2;
            //
        }
        if (utanfor == 1) {
            glPopMatrix();
            glRotatef(angle, 0, 0, 1);
            glTranslatef(0, -100, 0);
            glLineWidth(line_width);
            glBegin(GL_LINES);

            glVertex2f(x_pos / 2, y_prick / 2);

            glVertex2f(x_pos, y_prick);

            glEnd();
            DrawFillCircleXY(6, x_pos, y_prick);

        } else {
            DrawFillCircleXY(5, x_pos, y_prick);
            glPopMatrix();
        }
    }
    glPopMatrix();
}


float norm(float v[3]) {

    int i;
    float square_sum = 0.0;

    for (i = 0; i < 3; i++)
        square_sum += v[i] * v[i];

    if (square_sum > 0.0) {
        square_sum = sqrt(square_sum);
        for (i = 0; i < 3; i++)
            v[i] /= square_sum;
        return square_sum;
    } else {
        return 0;
    }
}

void HUD_HeadingVector(float vx, float vy, float vz,      //
                       float theta, float psi, float phi, //
                       float* hx, float* hy) {
    int i;

    float v[3] = {vx, vy, vz};
    norm(v);

    theta *= M_PI / 180.0;
    psi *= M_PI / 180.0;
    phi *= M_PI / 180.0;

    float s_theta = sin(theta);
    float c_theta = cos(theta);

    float s_phi = sin(-phi);
    float c_phi = cos(-phi);

    float s_psi = sin(psi);
    float c_psi = cos(psi);

    float n[3] = {
        c_theta * s_psi,
        s_theta,
        -c_theta * c_psi,
    };

    norm(n);

    float xh[3] = {
        c_theta * c_psi,
        0.0,
        c_theta * s_psi,
    };
    float yh[3] = {-s_theta * s_psi, c_theta, s_theta * c_psi};

    norm(xh);
    norm(yh);

    float hx_s = 0.0, hy_s = 0.0;

    float vn_diff[3], n_dot_v = 0.0;

    for (i = 0; i < 3; i++) {

        vn_diff[i] = v[i] - n[i];
        n_dot_v += n[i] * v[i];

        hx_s += vn_diff[i] * xh[i];
        hy_s += vn_diff[i] * yh[i];
    }

    if (hx_s * hx_s + hy_s * hy_s < 1.0e-6) {
        *hx = 0.0;
        *hy = 0.0;
        return;
    }

    float alpha = acos(n_dot_v) * 180 / M_PI;

    *hx = hx_s * c_phi + hy_s * s_phi;
    *hy = hy_s * c_phi - hx_s * s_phi;

    float h[3] = {*hx, *hy, 0.0};
    float norm_h = norm(h);

    *hx *= alpha / norm_h;
    *hy *= alpha / norm_h;
}
