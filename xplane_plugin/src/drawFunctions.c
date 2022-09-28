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

int screen_width = 0;
int screen_height = 0;
float line_width = 2.0;
float fov;
float fov_pixels;

float color[] = {-0.5, 1.0, -0.5, 1.0};
int viggen_mode = 1;

int blendmodes[] = {GL_ZERO,
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

int CalculateCenter(void) {
    int screen_width;
    int screen_height;

    XPLMGetScreenSize(&screen_width, &screen_height);

    //debugLog("window geo %d %d %d %d", b[0], b[1], b[2], b[3]);
    //debugLog("ScreenSize %d %d", screen_width,screen_height);
    screen_width = screen_width;
    screen_height = screen_height;
    // HudConfig* lConfig = getHudConfig();
    // float FOV_off_y = getFOVoff_y();
    // float FOV_off_x = getFOVoff_x();
    //float ratio = (float)screen_height / (float)screen_width;
    //float scalex = screen_width/(1024 +getPanelL() );
    fov = getFOV();
    float fovscale = GLASS_FOV / getFOV_x();
    //float scissor_x = fovscale * screen_width;
    //float scissor_y = 20 / getFOV_x() * screen_width * 0.8671875;

    if (getViewType() == 1000) {
        fov_pixels = (0.9571875 * TEXTURE_WIDTH / getFOV_x() / fovscale); //*24/getFOV_x()  kalibreringsfaktor 0.8671875
        //fov_pixels = 11.08/fovscale;
    } else {
        fov_pixels = screen_height / fov / hud_scale;
        fov_pixels = (0.9571875 * TEXTURE_WIDTH / getFOV_x() / fovscale);
        //fov_pixels = (0.9571875 * 1024 / getFOV_x() / fovscale);
        //glTranslated((screen_width / 2) - FOV_off_x, (screen_height / 2) - FOV_off_y, 0);
    }
    fov_pixels = TEXTURE_WIDTH / GLASS_FOV;

    line_width = line_scale * hud_scale;
    CalculateColors();
    // FOV_off_y = CalcFOVAngle(20) * hud_scale;
    // FOV_off_x = CalcFOVAngle(30) * hud_scale;

    // if (FOV_off_x > 1000 || FOV_off_x < -1000) {
    //     return -1;
    // }

    //glTranslatef((screen_width / 2) - FOV_off_x, (screen_height / 2) - FOV_off_y, 0);
    glTranslatef(TEXTURE_WIDTH / 2, TEXTURE_WIDTH * (2.0f / 3.0f), 0);

    // Slumpbrus genom förflyttning i sidled
    // float randomfloat = (float)rand()/(float)(RAND_MAX/line_width);
    // float randomfloat2 = (float)rand()/(float)(RAND_MAX/line_width);
    // glTranslatef(randomfloat/2.0f, randomfloat2/2.0f, 0);
    
    glScalef(TEXTURE_WIDTH/1024.0f, TEXTURE_WIDTH/1024.0f, 0);
    // #ifdef NO_FRAMEBUFFER
    //     glScissor(0, 0, TEXTURE_WIDTH, TEXTURE_WIDTH);
    //     // if (getViewType() == 1000) {
    //     //     glScissor(getPanelL() + screen_width / 2 - scissor_x / 2 * hud_scale, screen_height / 2 - scissor_y, scissor_x * hud_scale, scissor_x * 0.8671875 * hud_scale);
    //     // } else {
    //     //     glScissor((screen_width / 2) - (glass_width * hud_scale / 2) + offset_x - FOV_off_x,
    //     //               -FOV_off_y,
    //     //               offset_x + glass_width * hud_scale,
    //     //               offset_y + screen_height / 2 + glass_height * hud_scale / 2);
    //     // }
    //
    //     glEnable(GL_SCISSOR_TEST);
    // #endif
    return 1;
}

void CalculateColors() {
    if (th1) {
        float var1 = getThrottlePos();

        if (var1 < 0.1) {
            color[0] = 1.0;
            color[1] = 0.0;
            color[2] = 0.0;
            color[3] = 1.0;

        } else if (var1 < 0.9) {
            color[0] = 0.0;
            color[1] = var1;
            color[2] = 0.0;
            color[3] = 1.0;
        } else if (var1 <= 1.0) {
            color[0] = (var1 - 0.9) * 2;
            color[1] = var1;
            color[2] = 0.0;
            color[3] = 1.0;
        }
    }
}

float knotsTokmh(float in) {
    return in * 1.85200;
}
float kmhToknots(float in) {
    return in / 1.85200;
}

float feetTom(float in) {
    return in * 0.3048;
}
float m2feet(float in) {
    return in / 0.3048;
}
void TranslateToCenter(void) {
    // Har denna funkat?
    //glTranslated(screen_width / 2, screen_height / 2, 0);
}

void initGlResources() {
    InitFonts();
}

void DrawCircle(float radius) {
    double angle;
    int i;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < CIRCLE_LINES_COUNT; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f((cos(angle) * radius), (sin(angle) * radius));
    }
    glEnd();
}
void DrawCircleXY(float radius, float x, float y) {
    double angle;
    int i;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < CIRCLE_LINES_COUNT; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}
void DrawFillCircleXY(float radius, float x, float y) {
    double angle;
    int i;
    glBegin(GL_POLYGON);
    for (i = 0; i < CIRCLE_LINES_COUNT - 1; i++) {
        angle = -i * 2 * M_PI / CIRCLE_LINES_COUNT;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }

    glEnd();
}
void DrawCircleDown(float radius, float x, float y, float heading) {
    // Cirklarna som ska ha en avbrytning med pil som visas när man åker mot marken
    double angle;
    int i;
    float a = 15; // bredden på pilen i pixlar
    int segments = 10;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < (360 / segments); i++) {
        float aoff = asin(a / radius);
        angle = aoff * 2 + ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));

        angle = aoff + ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * (radius - 40)), y + (sin(angle) * (radius - 40)));
        angle = ((i + heading / segments) * 2 * M_PI / (360 / segments));
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}

void DrawCircleUp(float radius, float x, float y, float heading) {
    double angle;
    int i;
    int segments = 2;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < (360 / segments); i++) {
        angle = (-i - heading / segments) * 2 * M_PI / (360 / segments);
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
    segments = 10;
    glBegin(GL_LINES);
    for (i = 0; i < (360 / segments); i++) {
        angle = (i - heading / segments) * 2 * M_PI / (360 / segments);
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
        glVertex2f(x + (cos(angle) * (radius - 10)), y + (sin(angle) * (radius - 10)));
    }
    glEnd();
}
void DrawBanaXY(float radius, float x, float y) {

    glBegin(GL_LINES);
    glVertex2f(x + radius, y);
    glVertex2f(x - radius, y);

    glVertex2f(x + radius * 0.8, y + radius * 1.2);
    glVertex2f(x + radius, y);

    glVertex2f(x - radius * 0.8, y + radius * 1.2);
    glVertex2f(x - radius, y);
    glEnd();
}
float CalcFOVAngle(float input) {
    float out = (input * fov_pixels * fov_trim) / hud_scale;

    return out;
}

float getLandingSpeed() {
    float mass = getTotalWeight();
    float delta = (landing_speed2 - landing_speed1) / (landing_weight2 - landing_weight1);
    float speed = ((mass - landing_weight1) * delta) + landing_speed1;
    return speed;
}

float to_degrees(float radians) {
    return radians * (180.0 / M_PI);
}

float to_radians(float degrees) {
    return degrees / (180.0 / M_PI);
}

float nm2km(float nm) {
    return nm * 1.852;
}

float myGetAlpha() {
    float vx = getVX();
    float vy = getVY();
    float vz = getVZ();
    float pitch = getPitch();

    float length = sqrt(vy * vy + vx * vx + vz * vz);
    if (length > 1.0) {
        float alpha = asin(vy / length);
        alpha = pitch - to_degrees(alpha);
        return alpha;
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
    float out = to_degrees(atan2(-getVX(), getVZ())) + 180;

    return out;
}

float lim(float value, float lower, float upper) {
    if (lower > upper) {
        float tmp = lower;
        lower = upper;
        upper = tmp;
    }
    if (value < lower) {
        return lower;
    }
    if (value > upper) {
        return upper;
    }
    return value;
}

float max2(float value1, float value2) {
    if (value1 > value2) {
        return value1;
    }
    return value2;
}

float fix180(float input) {
    
    if (input <-180.0f) {
        input = input +360;
    }
    if (input >180) {
        input = input -360;
    }	
    return input;
}

void DrawGlassObject(float width) {

    float scale = width / 280.0f;
    float center = 20.0f;
    float centerUV = center / scale / 280.0f;
    //centerUV = 0.0f;

    glPushMatrix();
    glTranslatef(0, width * 0.857142857143f, 0);
    glBegin(GL_POLYGON);
    //glColor4fv(colorglass);
    glTexCoord2f(0.142857142857f, centerUV + 0.0f);
    glVertex2f(-100 * scale, center - 240 * scale);
    glTexCoord2f(0.0f, centerUV + 0.321428571429f);
    glVertex2f(-140 * scale, center - 150 * scale);

    glTexCoord2f(0.0f, centerUV + 0.660714285714f);
    glVertex2f(-140 * scale, center - 55 * scale);
    glTexCoord2f(0.121428571429f, centerUV + 0.798214285714f);
    glVertex2f(-106 * scale, center - 16.5 * scale);

    // böjen
    glTexCoord2f(0.257142857143f, centerUV + 0.835428571429f);
    glVertex2f(-68 * scale, center - 6.08 * scale);
    glTexCoord2f(0.364285714286f, centerUV + 0.849714285714f);
    glVertex2f(-38 * scale, center - 2.08 * scale);
    glTexCoord2f(0.5f, centerUV + 0.857142857143f);
    glVertex2f(-0 * scale, center - 0.00 * scale);

    // andra sidan
    glTexCoord2f(1.0f - 0.364285714286f, centerUV + 0.849714285714f);
    glVertex2f(38 * scale, center - 2.08 * scale);
    glTexCoord2f(1.0f - 0.257142857143f, centerUV + 0.835428571429f);
    glVertex2f(68 * scale, center - 6.08 * scale);

    glTexCoord2f(1.0f - 0.121428571429f, centerUV + 0.798214285714f);
    glVertex2f(106 * scale, center - 16.5 * scale);
    glTexCoord2f(1.0f, centerUV + 0.660714285714f);
    glVertex2f(140 * scale, center - 55 * scale);

    glTexCoord2f(1.0f, centerUV + 0.321428571429f);
    glVertex2f(140 * scale, center - 150 * scale);
    glTexCoord2f(1.0f - 0.142857142857f, centerUV + 0.0f);
    glVertex2f(100 * scale, center - 240 * scale);

    glEnd();
    glPopMatrix();
}

void DrawGlassObjectViggen(float width) {

    // viggen 136mm bred 68 halva
    // 180mm hög, ser ut att va ca 12cm hög som man ser den eftersom den är vinklad
    float scale = width / 280.0f;
    float center = 20.0f;
    float centerUV = center / scale / 280.0f;
    //centerUV = 0.0f;

    glPushMatrix();
    glTranslatef(0, width * 0.857142857143f, 0);
    glBegin(GL_POLYGON);
    //glColor4fv(colorglass);
    glTexCoord2f(0.242857142857f, centerUV + 0.364285714286f);
    glVertex2f(-68 * scale, center - 138 * scale);
    glTexCoord2f(0.242857142857f, centerUV + 0.364285714286f);
    glVertex2f(-68 * scale, center - 138 * scale);

    glTexCoord2f(0.242857142857f, centerUV + 0.660714285714f);
    glVertex2f(-68 * scale, center - 55 * scale);
    glTexCoord2f(0.242857142857f, centerUV + 0.798214285714f);
    glVertex2f(-68 * scale, center - 16.5 * scale);

    // böjen
    glTexCoord2f(0.242857142857f, centerUV + 0.835428571429f);
    glVertex2f(-68 * scale, center - 6.08 * scale);
    glTexCoord2f(0.364285714286f, centerUV + 0.849714285714f);
    glVertex2f(-38 * scale, center - 2.08 * scale);
    glTexCoord2f(0.5f, centerUV + 0.857142857143f);
    glVertex2f(-0 * scale, center - 0.00 * scale);

    // andra sidan
    glTexCoord2f(1.0f - 0.364285714286f, centerUV + 0.849714285714f);
    glVertex2f(38 * scale, center - 2.08 * scale);
    glTexCoord2f(1.0f - 0.242857142857f, centerUV + 0.835428571429f);
    glVertex2f(68 * scale, center - 6.08 * scale);

    glTexCoord2f(1.0f - 0.242857142857f, centerUV + 0.798214285714f);
    glVertex2f(68 * scale, center - 16.5 * scale);
    glTexCoord2f(1.0f - 0.242857142857f, centerUV + 0.660714285714f);
    glVertex2f(68 * scale, center - 55 * scale);

    glTexCoord2f(1.0f - 0.242857142857f, centerUV + 0.364285714286f);
    glVertex2f(68 * scale, center - 138 * scale);
    glTexCoord2f(1.0f - 0.242857142857f, centerUV + 0.364285714286f);
    glVertex2f(68 * scale, center - 138 * scale);

    glEnd();
    glPopMatrix();
}
int LoadTexture(const char* filename) {
    int texture;
    int width, height;
    unsigned char* data;
    char lDirName[512];
    char loadfile[1512];
    // find out plugin path
    XPLMPluginID lMyID = XPLMGetMyID();
    XPLMGetPluginInfo(lMyID, NULL, lDirName, NULL, NULL);
    char* lFilePart = XPLMExtractFileAndPath(lDirName);
    const char* lSep = XPLMGetDirectorySeparator();
    if (lFilePart != NULL) {
        *lFilePart = 0;
        strcat(lDirName, lSep);
    }
    strcat(lDirName, "..");
    strcat(lDirName, lSep);
    sprintf(loadfile, "%s/%s", lDirName, filename);

    debugLog("read file %s \n", loadfile);

    FILE* file;
    file = fopen(loadfile, "rb");

    if (file == NULL) {
        debugLog("error loading texture %s", loadfile);

        return 0;
    }

    width = 1024;
    height = 1024;
    data = (unsigned char*)malloc(width * height * 3);
    //int size = fseek(file,);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    // for (int i = 0; i < width * height; ++i) {
    //     int index = i * 3;
    //     unsigned char B, R;
    //     B = data[index];
    //     R = data[index + 2];
    //
    //     data[index] = R;
    //     data[index + 2] = B;
    // }

    XPLMGenerateTextureNumbers((int*)&texture, 1); //glGenTextures(1, &texture);
    XPLMBindTexture2d(texture, 0);                 //glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    debugLog("loaded texture %s %d", loadfile, texture);
    return texture;
}
