

#include "HUDplug.h"
#include "datarefs.h"
#include "config.h"
#include "statusDisplay.h"
#include "drawFunctions.h"
#include "drawElements.h"
#include "drawModes.h"
// Downloaded from https://developer.x-plane.com/code-sample/hello-world-sdk-3/

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include <string.h>

//#ifndef XPLM300
//      #error This is made to be compiled against the XPLM300 SDK
//#endif

// An opaque handle to the window we will create

#ifdef XPLM301
static XPLMWindowID g_window;

void draw12(XPLMWindowID in_window_id, void* in_refcon);

// Callbacks we will register when we create our window
void draw_hello_world(XPLMWindowID in_window_id, void* in_refcon);
int dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void* in_refcon) {
    return 0;
}
XPLMCursorStatus dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void* in_refcon) {
    return xplm_CursorDefault;
}
int dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void* in_refcon) {
    return 0;
}
void dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void* in_refcon, int losing_focus) {
}
#endif
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
// static int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static int DrawPanelCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static int DrawScreenCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
static XPLMDataRef gDataRef = NULL;
static XPLMDataRef testDataRef = NULL;
XPLMDataRef drHUDheartbeat;

int TeensyControls_show = 0;
int statusDisplayShow = 0;

GLuint hud_texture;
GLuint rbo_;
GLuint fbo_;
GLuint fbo;
GLuint fboDepth;
GLuint fboBuff;
GLuint fboTexture;

GLuint fbo_blur;
GLuint fboDepth_blur;
GLuint fboBuff_blur;
GLuint fboTexture_blur;
int texturemask;

GLuint nullptr;
// This is our texture ID.  Texture IDs in OpenGL are just ints...but this is a global for the life of our plugin.
static int g_tex_num = 0;
int fboInit = 0;

static unsigned char buffer[WIDTH * HEIGHT * 4];

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

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
    strcpy(outName, "HUDplug");
    strcpy(outSig, "github.condac.HUDplug");
    strcpy(outDesc, "A plug-in for HUDplug.");
#ifndef NO_FRAMEBUFFER
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        debugLog("HUDplug: GLEW init failed: %s\n", (const char*)glewGetErrorString(err));

    } else {
        debugLog("HUDplug: GLEW init ok\n");
    }
#endif

#ifdef XPLM301

#endif

    // Menu
    XPLMMenuID myMenu;
    int mySubMenuItem;

    /* First we put a new menu item into the plugin menu.
     * This menu item will contain a submenu for us. */
    mySubMenuItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), /* Put in plugins menu */
                                       "HUDplug",             /* Item Title */
                                       0,                     /* Item Ref */
                                       1);                    /* Force English */

    /* Now create a submenu attached to our menu item. */
    myMenu = XPLMCreateMenu("HUDplug",
                            XPLMFindPluginsMenu(),
                            mySubMenuItem,         /* Menu Item to attach to. */
                            MyMenuHandlerCallback, /* The handler */
                            0);                    /* Handler Ref */

    /* Append a few menu items to our submenu.  We will use the refcon to
     * store the amount we want to change the radio by. */

    XPLMAppendMenuItem(myMenu, "Skala 1", (void*)1, 1);
    XPLMAppendMenuItem(myMenu, "Skala 2", (void*)2, 1);
    XPLMAppendMenuItem(myMenu, "Text 1", (void*)3, 1);
    XPLMAppendMenuItem(myMenu, "Text 2", (void*)4, 1);
    XPLMAppendMenuItem(myMenu, "Color Green", (void*)5, 1);
    XPLMAppendMenuItem(myMenu, "Color Green Dark", (void*)6, 1);
    XPLMAppendMenuItem(myMenu, "Color Red", (void*)7, 1);
    XPLMAppendMenuItem(myMenu, "Color Green Transparent", (void*)8, 1);

    XPLMAppendMenuItem(myMenu, "Read Config", (void*)19, 1);
    XPLMAppendMenuItem(myMenu, "Viggen Mode", (void*)20, 1);
    /* Look up our data ref.  You find the string name of the data ref
     * in the master list of data refs, including in HTML form in the
     * plugin SDK.  In this case, we want the nav1 frequency. */

    gDataRef = XPLMFindDataRef("sim/cockpit/switches/pitot_heat_on");
    testDataRef = XPLMFindDataRef("sim/joystick/yoke_roll_ratio");
    drHUDheartbeat = XPLMFindDataRef("JAS/system/hud/heartbeat");

    TeensyControls_show = 0;

    TeensyControls_display_init();
    display("started HUDplug");

    statusDisplayInit();

    XPLMDebugString("HUDplug:read config\n");
    readConfig();

    //asock = createUDPSocket("192.168.0.105", 34555);
    /* Register our callback for once a second.  Positive intervals
     * are in seconds, negative are the negative of sim frames.  Zero
     * registers but does not schedule a callback for time. */
    XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, /* Callback */
                                   1.0,                  /* Interval */
                                   NULL);                /* refcon not used. */

    initGlResources();
    initDataRefs();
    //xplm_Phase_FirstCockpit // funkar med opengl och skärm offsets

    //xplm_Phase_Window // den vi brukar köra
    XPLMRegisterDrawCallback(DrawScreenCallback,
                             xplm_Phase_Window, /* Draw when sim is doing windows */
                             0,                 /* Before plugin windows */
                             NULL);             /* No refcon needed */
                                                //    xplm_Phase_LastCockpit

    //xplm_Phase_Panel
    XPLMRegisterDrawCallback(DrawPanelCallback,
                             xplm_Phase_Panel, /* Draw when sim is doing windows */
                             0,                /* Before plugin windows */
                             NULL);            /* No refcon needed */
    XPLMRegisterDrawCallback(DrawPanelCallback,
                             xplm_Phase_Gauges, /* Draw when sim is doing windows */
                             0,                 /* Before plugin windows */
                             NULL);             /* No refcon needed */

    // Initialization: allocate a textiure number.
    XPLMGenerateTextureNumbers(&g_tex_num, 1);
    XPLMBindTexture2d(g_tex_num, 0);
    // Init to black for now.
    memset(buffer, 0, WIDTH * HEIGHT * 4);
    // The first time we must use glTexImage2D.
    glTexImage2D(GL_TEXTURE_2D,
                 0,       // mipmap level
                 GL_RGBA, // internal format for the GL to use.  (We could ask for a floating point tex or 16-bit tex if we were crazy!)
                 WIDTH,
                 HEIGHT,
                 0,                // border size
                 GL_RGBA,          // format of color we are giving to GL
                 GL_UNSIGNED_BYTE, // encoding of our data
                 buffer);

    // Note: we must set the filtering params to SOMETHING or OpenGL won't draw anything!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 1; //g_window != NULL;
}

PLUGIN_API void XPluginStop(void) {
    // Since we created the window, we'll be good citizens and clean it up
#ifdef XPLM301
    XPLMDestroyWindow(g_window);
    g_window = NULL;
#endif
    XPLMUnregisterDrawCallback(DrawScreenCallback, xplm_Phase_Window, 1, NULL);
    XPLMUnregisterDrawCallback(DrawPanelCallback, xplm_Phase_Panel, 1, NULL);
    XPLMUnregisterDrawCallback(DrawPanelCallback, xplm_Phase_Gauges, 1, NULL);
}

PLUGIN_API void XPluginDisable(void) {
}
PLUGIN_API int XPluginEnable(void) {
    return 1;
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {
}

void MyMenuHandlerCallback(void* inMenuRef, void* inItemRef) {
    //display("%d",(int)inItemRef);
    int incommand = (uintptr_t)inItemRef;
    if (incommand == 1) {
        // toggle console
        hud_scale = 1.0;
    }
    if (incommand == 2) {
        hud_scale = 2.0;
    }
    if (incommand == 3) {
        text_scale = 1.0;
    }
    if (incommand == 4) {
        text_scale = 2.0;
    }
    if (incommand == 5) {
        // green
        color[0] = 0.0;
        color[1] = 1.0;
        color[2] = 0.0;
        color[3] = 1.0;
    }
    if (incommand == 6) {
        // green dark
        color[0] = 0.0;
        color[1] = 0.50;
        color[2] = 0.0;
        color[3] = 1.0;
    }
    if (incommand == 7) {
        // red
        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 0.0;
        color[3] = 1.0;
    }
    if (incommand == 8) {
        // transparent
        color[0] = 0.0;
        color[1] = 1.0;
        color[2] = 0.0;
        color[3] = 0.50;
    }
    if (incommand == 19) {
        //
        readConfig();
    }
    if (incommand == 20) {
        // transparent
        viggen_mode = !viggen_mode;
    }
    /* This is our handler for the menu item.  Our inItemRef is the refcon
     * we registered in our XPLMAppendMenuItem calls.  It is either +1000 or
     * -1000 depending on which menu item is picked. */
    //if (gDataRef != NULL) {
    /* We read the data ref, add the increment and set it again.
     * This changes the nav frequency. */
    //XPLMSetDatai(gDataRef,(int) inItemRef);
    //}
}

int ifCharInArray(char* str, char val) {
    char x;
    int pos = 0;
    x = str[pos];
    while (x != '\0') {
        if (str[pos] == val) {
            return pos;
        }
        pos++;
        x = str[pos];
    }
    return -1;
}

char inputbuf[8200];

int dr_gear = 0;
float dr_pitch = 0;
float dr_altitude = 0;

float dr_vx = 0;
float dr_vy = 0;
float dr_vz = 0;

float dr_theta = 0;
float dr_phi = 0;
float dr_psi = 0;


void updateDatarefs() {
    dr_gear = getGear();
    dr_pitch = getPitch();
    dr_altitude = getAltitude();
    dr_vx = getVX();
    dr_vy = getVY();
    dr_vz = getVZ();
    dr_theta = getPitch();
    dr_phi = getRoll();
    dr_psi = getTrueHeading();
}

void drawHUD() {

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //drawLineText("-startmitt-", 512, 512, 1.0, 2);
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    //XPLMSetGraphicsState(0 /*Fog*/, 1 /*TexUnits*/, 0 /*Lighting*/, 1 /*AlphaTesting*/, 1 /*AlphaBlending*/, 0 /*DepthTesting*/, 0 /*DepthWriting*/);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    //DrawTest();
    CalculateCenter();
    //drawLineText("CalculateCenter", 0, 0, 1.0, 1);
    // if (CalculateCenter() == -1) {
    //     return 1;
    // }

    if (getViewType() == 1000) {
        //glTranslatef(offset_x+getPanelL(), offset_y+getPanelT()-512 -35-29, 0);
        //glTranslatef(offset_x + TEXTURE_WIDTH / 2, 0, 0);
        //float fovscale = 30 / getFOV_x();
        //fovscale = 30.0f / 80.0f;
        //glScalef(hud_scale, hud_scale, 0); // gör om 1024 pixlar till den ungefärliga storleken på HUD glaset vi ritar på

    } else {

        //glTranslatef(offset_x, offset_y, 0);
        //glTranslatef(offset_x + TEXTURE_WIDTH / 2, 0, 0);
        //glScalef(hud_scale, hud_scale, 0);
    }
    //drawLineText("getViewType", 0, 0, 1.0, 1);
    if (draw_test) {
        DrawTest();
    }
    if (g_sway) {
        glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
    }
    if (viggen_mode == 1) {
        TranslateToCenter();
        //DrawGlass();
        glPushMatrix();

        DrawViggenMode1();
        glPopMatrix();

    } else if (viggen_mode == 2) {
        TranslateToCenter();
        //DrawGlass();
        glPushMatrix();

        DrawViggenMode2();
        glPopMatrix();

    } else if (viggen_mode == 3) {
        TranslateToCenter();
        //DrawGlass();
        glPushMatrix();

        DrawViggenMode3();
        glPopMatrix();

    } else {
        TranslateToCenter();
        //DrawGlass();
        glPushMatrix();

        //drawLineText("jas", 0, 0, 1.0, 1);
        DrawModesJAS();

        glPopMatrix();
    }

    glPopMatrix();

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    static int heartbeat = 0;
    heartbeat++;
    XPLMSetDatai(drHUDheartbeat, heartbeat);
}

#ifndef NO_FRAMEBUFFER
void drawFramebuffer() {
    GLint xp_rbo, xp_fbo;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &xp_rbo);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &xp_fbo);
    if (!fboInit) {
        debugLog("skapar framebuffer");
        glewInit();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        // Create and bind FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, TEXTURE_WIDTH);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, TEXTURE_WIDTH);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);
        // Create depth renderbuffer
        glGenRenderbuffers(1, &fboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_WIDTH);
        // Create the texture
        XPLMGenerateTextureNumbers((int*)&fboTexture, 1); //glGenTextures(1, &fboTexture);
        XPLMBindTexture2d(fboTexture, 0);                 //glBindTexture(GL_RENDERBUFFER, fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TEXTURE_WIDTH, TEXTURE_WIDTH, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Attached texture to first color attachment and the depth to the depth attachment
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

        // Create and bind FBO_BLUR
        glGenFramebuffers(1, &fbo_blur);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_blur);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, TEXTURE_WIDTH_BLUR);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, TEXTURE_WIDTH_BLUR);
        glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);
        // Create depth renderbuffer
        glGenRenderbuffers(1, &fboDepth_blur);
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepth_blur);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        // Create the texture
        XPLMGenerateTextureNumbers((int*)&fboTexture_blur, 1); //glGenTextures(1, &fboTexture);
        XPLMBindTexture2d(fboTexture_blur, 0);                 //glBindTexture(GL_RENDERBUFFER, fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Attached texture to first color attachment and the depth to the depth attachment
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth_blur);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture_blur, 0);

        GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            fbo = 0;
            fboDepth = 0;
            fboTexture = 0;
            debugLog("skapar framebuffer error?\n");
        } else {
            // Reste FBO Binding
            debugLog("skapar framebuffer klart1\n");
            glBindFramebuffer(GL_FRAMEBUFFER, getFBO()); // ställ tillbaka till vad x-plane hade innan
            debugLog("skapar framebuffer klart2\n");
        }
        fboInit = 1;
        debugLog("skapar framebuffer klart3\n");

        LoadTexture("./mask2.bmp");
        texturemask = LoadTexture("./res/masksvart.bmp");
    } else {

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        //debugLog("render bind framebuffer \n");
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //debugLog("render glDrawBuffers\n");
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_WIDTH);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, TEXTURE_WIDTH, 0, TEXTURE_WIDTH, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //drawLineText("hudstart", 512, 512, 1.0, 1);
        drawHUD();

        glPopMatrix();
        XPLMSetGraphicsState(0, 1, 0, 1, 1, 0, 0);

        glBlendFunc(GL_ZERO, GL_SRC_COLOR); // tar bort färgen

        XPLMBindTexture2d(texturemask, 0);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH, TEXTURE_WIDTH);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH, 0);
        glEnd();

        // unbind FBO

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO());

        // BLUR
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur);
        //debugLog("render bind framebuffer \n");
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        //debugLog("render glDrawBuffers\n");
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, TEXTURE_WIDTH_BLUR, 0, TEXTURE_WIDTH_BLUR, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        XPLMBindTexture2d(fboTexture, 0);
        glGenerateMipmap(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, blur/4);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, 0);
        glEnd();
        
        // rita bilden flera gånger i olika positioner för att jämna ut det mer
        
        glPushMatrix();
        glTranslatef(0.75,0.3,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, 0);
        glEnd();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.4,0.8,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, 0);
        glEnd();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.65,-0.15,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, 0);
        glEnd();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.23,-0.65,0);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(0, 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(0, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, TEXTURE_WIDTH_BLUR);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(TEXTURE_WIDTH_BLUR, 0);
        glEnd();
        glPopMatrix();
        
        // unbind FBO
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO()); // ställ tillbaka till vad x-plane hade innan
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, xp_fbo);

        XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ställ tillbaka till deafault som x-plane 9 vill ha
    }
    /* The actual callback.  First we read the sim's time and the data. */
    //XPLMDebugString("HUDplug: flightloop\n");

    //float elapsed = XPLMGetElapsedTime();

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
}
#endif
float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {

    updateDatarefs();
#ifndef NO_FRAMEBUFFER
    drawFramebuffer();
#else
    fboInit = 1;
#endif
    /* Return 1.0 to indicate that we want to be called again in 1 second. */
    return 0.01;
}

void drawGlassTexture() {
    if (draw_glass == 1 || draw_glass == 3) {
        // Glasskivan
        //glBlendEquationSeparate(GL_MIN, GL_FUNC_ADD);
        //glBlendEquation(GL_MIN);
        glPushMatrix();
        XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
        glEnable(GL_BLEND);
        //glBlendFunc(modes[glass_type], modes[glass_type2]);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
        glColor4f(0.00, 0.00, 0.00, glass_darkness * 0.004 * 10);
        //glTranslatef(512 - hud_x, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);
        glTranslatef(0, -280 * (2.0f / 3.0f), 0.0f);
        if (viggen_mode >= 1) {
            DrawGlassObjectViggen(280);
        } else {
            DrawGlassObject(280);
        }

        glPopMatrix();
        //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        //glBlendEquation(GL_FUNC_ADD);
        XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
        //slut glasskivan
    }
    if (draw_glass == 2 || draw_glass == 1) {
        // Glasskivan
        XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
        glEnable(GL_BLEND);
        #ifdef NO_GLEW
        //glBlendEquation(GL_MIN);
        glColor4f(0.0, 0.0, 0.0, glass_darkness * 0.004 * 10.0f);
        #else
        //glBlendEquationSeparate(GL_MIN, GL_FUNC_ADD);
        glBlendEquation(GL_MIN);
        glColor4f(0.8, 0.7, 0.8, 0.5);
        #endif
        //glBlendEquation(GL_MIN);
        glPushMatrix();
        
        glBlendFunc(modes[glass_type], modes[glass_type2]);
        //glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
        
        //glTranslatef(512 - hud_x, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);

        if (viggen_mode >= 1) {
            if (dr_gear) {
                glTranslatef(0, -40, 0.0f);
            }
            glTranslatef(0, -280.0f * (2.0f / 3.0f) - 25.0f, 0.0f);
            DrawGlassObjectViggen(280);
        } else {
            glTranslatef(0, -280 * (2.0f / 3.0f), 0.0f);
            DrawGlassObject(280);
        }
        glPopMatrix();
        //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        
        #ifdef NO_GLEW
        //glBlendEquation(GL_FUNC_ADD);
        
        #else
        //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendEquation(GL_FUNC_ADD);
        #endif
        XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
        //slut glasskivan
    }
}
void drawHudTexture() {
// Riktiga bilden
#ifdef NO_FRAMEBUFFER

    int screen_width;
    int screen_height;

    XPLMGetScreenSize(&screen_width, &screen_height);

    glPushMatrix();

    float sc = 280.0f / TEXTURE_WIDTH;
    //float screen_w = screen_width / 2.0f / sc;
    float fov = getFOV();
    float fox_pixlar_x = screen_height / fov;

    glScalef(sc, sc, 0);
    glTranslatef(-TEXTURE_WIDTH / 2, -TEXTURE_WIDTH / 2, 0.0f);

    glTranslatef(0, -280 * (2.0f / 3.0f), 0.0f);

    //glTranslatef(screen_w, 0.0f, 0.0f);
    if (viggen_mode == 1) {
        if (dr_gear) {
            glScissor(screen_width / 2 - fox_pixlar_x * VIGGEN_HUD_FOV / 2 * hud_scale,
                      screen_height / 2 - fox_pixlar_x * 20,
                      fox_pixlar_x * VIGGEN_HUD_FOV * hud_scale,
                      fox_pixlar_x * 20);
        } else {
            glScissor(screen_width / 2 - fox_pixlar_x * VIGGEN_HUD_FOV / 2 * hud_scale,
                      screen_height / 2 - fox_pixlar_x * VIGGEN_HUD_FOV * 0.6666f,
                      fox_pixlar_x * VIGGEN_HUD_FOV * hud_scale,
                      fox_pixlar_x * VIGGEN_HUD_FOV);
        }
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }

    drawHUD();

    glPopMatrix();

#else
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // den jag kört
    //glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
    XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
    glBlendFunc(modes[image_blend1], modes[image_blend2]);
    glPushMatrix();
    XPLMBindTexture2d(fboTexture, 0);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTranslatef(0, -280 * (2.0f / 3.0f), 0.0f);

    DrawGlassObject(280);
    glPopMatrix();
#endif
    // Slut riktiga bilden
}
void drawBlurTexture() {
    #ifndef NO_FRAMEBUFFER
    XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
    glBlendFunc(modes[image_blend1], modes[image_blend2]);
    //glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
    glPushMatrix();
    XPLMBindTexture2d(fboTexture_blur, 0);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTranslatef(0, -280 * (2.0f / 3.0f), 0.0f);

    DrawGlassObject(280);
    glPopMatrix();
    #endif
}

int DrawPanelCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {
    if (!fboInit) {
        return 1;
    }
    if (getViewType() != 1000) { // 1000 är 2d panel view
        //return 1;
    }
    int screen_width;
    int screen_height;
    XPLMGetScreenSize(&screen_width, &screen_height);
    float FOV_off_y = getFOVoff_y(); // måste multipliceras med antalet grader per pixel
    float FOV_off_x = getFOVoff_x();
    float fov = getFOV();
    float screen_h = (float)PANEL_WIDTH * ((float)screen_height / (float)screen_width);
    float scale_hud = (screen_h / 280.0f) * (GLASS_FOV / fov);

    glPushMatrix();

    glTranslatef(PANEL_WIDTH / 2, offset_y + 512 * ((float)screen_height / (float)screen_width), 0.0f);

    //drawLineText("scale_hud", 0, 0, 1.0, 1);
    glTranslatef(FOV_off_x * (screen_h / fov), -FOV_off_y * (screen_h / fov), 0.0f);

    glScalef(scale_hud, scale_hud, 0); // skalan baserat på nuvarande FOV

    glScalef(hud_scale, hud_scale, 0); // skalan från configfilen

    drawGlassTexture();
    drawBlurTexture();

    drawHudTexture();

    glPopMatrix();
    //MyDrawCallback(inPhase, inIsBefore, inRefcon);
    return 1;
}

int DrawScreenCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {
    if (!fboInit) {
        return 1;
    }
    if (getViewType() == 1000) { // 1000 är 2d panel view
        return 1;
    }
    int screen_width;
    int screen_height;
    XPLMGetScreenSize(&screen_width, &screen_height);
    float FOV_off_y = getFOVoff_y(); // måste multipliceras med antalet grader per pixel
    float FOV_off_x = getFOVoff_x();
    float fov = getFOV();
    float screen_h = (float)screen_height;
    float scale_hud = (screen_h / 280.0f) * (GLASS_FOV / fov);

    glPushMatrix();
    //drawLineText("DrawScreenCallback", screen_width/2, screen_height/2-50, 1.0, 1);

    //MyDrawCallback(inPhase, inIsBefore, inRefcon);
    glTranslatef(screen_width / 2, offset_y + screen_height / 2, 0.0f);

    //drawLineText("scale_hud", 0, 0, 1.0, 1);
    glTranslatef(FOV_off_x * (screen_h / fov), -FOV_off_y * (screen_h / fov), 0.0f);

    glScalef(scale_hud, scale_hud, 0); // skalan baserat på nuvarande FOV

    glScalef(hud_scale, hud_scale, 0); // skalan från configfilen
    // char temp[512];
    // sprintf(temp, "FOV_off_x %f getFOV(%f) glass:%f screen_height %f", FOV_off_x, fov,GLASS_FOV,  screen_h);
    // drawLineText(temp, 10, 10, 1.0, 1);
    drawGlassTexture();
    drawBlurTexture();

    drawHudTexture();

    // XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
    // glBlendFunc(modes[image_blend1], modes[image_blend2]);
    // glPushMatrix();
    // XPLMBindTexture2d(fboTexture, 0);
    // glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    // glPushMatrix();
    // glTranslatef(0, -512*(2.0f/3.0f), 0.0f);
    // // glBegin(GL_QUADS);
    // // glTexCoord2f(0.0f, 0.0f); glVertex2i( 0, 0);
    // // glTexCoord2f(0.0f, 1.0f); glVertex2i( 0, 512);
    // // glTexCoord2f(1.0f, 1.0f); glVertex2i(512, 512);
    // // glTexCoord2f(1.0f, 0.0f); glVertex2i(512, 0);
    // // glEnd();
    // glPopMatrix();
    // glTranslatef(0, -256, 0.0f);
    // //DrawGlassObject(512);
    // glPopMatrix();

    glPopMatrix();
    return 1;
}

// int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {
//
//     if (!fboInit) {
//         return 1;
//     }
//     // The drawing part.
//     XPLMSetGraphicsState(0,  // No fog, equivalent to glDisable(GL_FOG);
//                          1,  // One texture, equivalent to glEnable(GL_TEXTURE_2D);
//                          0,  // No lighting, equivalent to glDisable(GL_LIGHT0);
//                          0,  // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
//                          1,  // Use alpha blending, e.g. glEnable(GL_BLEND);
//                          0,  // No depth read, e.g. glDisable(GL_DEPTH_TEST);
//                          0); // No depth write, e.g. glDepthMask(GL_FALSE);
//     int screen_width;
//     int screen_height;
//     XPLMGetScreenSize(&screen_width, &screen_height);
//
//     float hud_x = 1024 * 30 / getFOV_x();
//
//     if (draw_glass == 1) {
//         // Glasskivan
//         glPushMatrix();
//         XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//         glEnable(GL_BLEND);
//         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // den jag kört
//         glBlendFunc(modes[glass_type], modes[glass_type2]);
//         //float colorglass[] = {0.2, 0.0, 0.2, 0.5};
//         //colorglass[3] = GetGlassDarkness() * glass_darkness;
//
//         glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
//         //glColor4f(0.2, 0.0, 0.2, 0.5);
//         glTranslatef(512, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);
//         DrawGlassObject(hud_x);
//
//         //XPLMBindTexture2d(fboTexture, 0);
//
//         XPLMSetGraphicsState(0,  // No fog, equivalent to glDisable(GL_FOG);
//                              1,  // One texture, equivalent to glEnable(GL_TEXTURE_2D);
//                              0,  // No lighting, equivalent to glDisable(GL_LIGHT0);
//                              0,  // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
//                              1,  // Use alpha blending, e.g. glEnable(GL_BLEND);
//                              0,  // No depth read, e.g. glDisable(GL_DEPTH_TEST);
//                              0); // No depth write, e.g. glDepthMask(GL_FALSE);
//
//         glPopMatrix();
//         //slut glasskivan
//     }
//     if (draw_glass == 2) {
//         // Glasskivan
//         glBlendEquationSeparate(GL_MIN, GL_FUNC_ADD);
//         //glBlendEquation(GL_MIN);
//         glPushMatrix();
//         XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//         glEnable(GL_BLEND);
//         glBlendFunc(modes[glass_type], modes[glass_type2]);
//         glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
//         glColor4f(0.8, 0.7, 0.8, 0.5);
//         glTranslatef(512, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);
//         DrawGlassObject(hud_x);
//         glPopMatrix();
//         glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//         XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
//         //slut glasskivan
//     }
//
//     // Draw texture.
//
//     // Blur först
//     glPushMatrix();
//     //glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
//     glBlendFunc(modes[0], modes[7]);
//     XPLMBindTexture2d(fboTexture_blur, 0);
//     glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
//     //glTranslatef(512-hud_x/2, 512*((float)screen_height / (float)screen_width) -hud_x/2, 0.0f);
//     glTranslatef(512, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);
//
//     DrawGlassObject(hud_x);
//     glPopMatrix();
//     // Slut Blur
//
//     // Riktiga bilden
//     //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // den jag kört
//     //glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
//     glBlendFunc(modes[image_blend1], modes[image_blend2]);
//     glPushMatrix();
//     XPLMBindTexture2d(fboTexture, 0);
//     glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//     glTranslatef(512, 512 * ((float)screen_height / (float)screen_width) - hud_x * (2.0f / 3.0f), 0.0f);
//     // glBegin(GL_QUADS);
//     // glTexCoord2f(0.0f, 0.0f); glVertex2i( 0, 0);
//     // glTexCoord2f(0.0f, 1.0f); glVertex2i( 0, hud_x);
//     // glTexCoord2f(1.0f, 1.0f); glVertex2i(hud_x, hud_x);
//     // glTexCoord2f(1.0f, 0.0f); glVertex2i(hud_x, 0);
//     // glEnd();
//     DrawGlassObject(hud_x);
//     glPopMatrix();
//
//     // Slut riktiga bilden
//
//     if (draw_test == 1) {
//         hud_x = hud_x / 10;
//         // testa glasskivan
//
//         for (int i = 0; i < 15; i++) {
//             for (int y = 0; y < 15; y++) {
//                 //SetGLTransparentLines();
//                 glBlendEquationSeparate(GL_MIN, GL_FUNC_ADD);
//                 glPushMatrix();
//                 XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//                 glEnable(GL_BLEND);
//                 glBlendFunc(modes[i], modes[y]);
//                 glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
//                 glColor4f(0.9, 0.8, 0.9, 1.0);
//                 glTranslatef(hud_x * i, hud_x * y, 0.0f);
//                 DrawGlassObject(hud_x);
//                 glPopMatrix();
//
//                 glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//                 XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
//                 glEnable(GL_BLEND);
//                 glBlendFunc(modes[image_blend1], modes[image_blend2]);
//                 glPushMatrix();
//                 XPLMBindTexture2d(fboTexture, 0);
//                 glColor4f(1.0f, 0.8f, 1.0f, 1.0f);
//                 glTranslatef(hud_x * i, hud_x * y, 0.0f);
//                 DrawGlassObject(hud_x);
//                 glPopMatrix();
//             }
//         }
//     }
//     if (draw_test == 2) {
//         hud_x = hud_x / 10;
//         // testa överlagringen
//         for (int i = 0; i < 15; i++) {
//             for (int y = 0; y < 15; y++) {
//                 //SetGLTransparentLines();
//                 glEnable(GL_BLEND);
//                 glBlendFunc(modes[glass_type], modes[glass_type2]);
//                 glPushMatrix();
//                 XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
//                 glEnable(GL_BLEND);
//                 glBlendFunc(modes[glass_type], modes[glass_type2]);
//                 glColor4f(0.2, 0.0, 0.2, GetGlassDarkness() * glass_darkness);
//                 //glColor4f(0.2, 0.0, 0.2, 0.5);
//                 glTranslatef(hud_x * i, hud_x * y, 0.0f);
//                 DrawGlassObject(hud_x);
//                 glPopMatrix();
//
//                 XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
//                 glBlendFunc(modes[i], modes[y]);
//                 glPushMatrix();
//                 XPLMBindTexture2d(fboTexture, 0);
//                 glColor4f(1.0f, 0.8f, 1.0f, 1.0f);
//                 glTranslatef(hud_x * i, hud_x * y, 0.0f);
//                 DrawGlassObject(hud_x);
//                 glPopMatrix();
//             }
//         }
//     }
//
//     // XPLMBindTexture2d(g_tex_num, 0);
//     // // Note: if the tex size is not changing, glTexSubImage2D is faster than glTexImage2D.
//     // glTexSubImage2D(GL_TEXTURE_2D,
//     //                 0, // mipmap level
//     //                 0, // x-offset
//     //                 0, // y-offset
//     //                 WIDTH,
//     //                 HEIGHT,
//     //                 GL_RGBA,          // color of data we are seding
//     //                 GL_UNSIGNED_BYTE, // encoding of data we are sending
//     //                 buffer);
//     //
//     //
//     //
//     // XPLMBindTexture2d(fboTexture_blur, 0);
//     // glColor3f(1, 1, 1); // Set color to white.
//     // int x1 = 20;
//     // int y1 = 20;
//     // int x2 = x1 + WIDTH;
//     // int y2 = y1 + HEIGHT;
//     // glBegin(GL_QUADS);
//     // glTexCoord2f(0, 0);
//     // glVertex2f(x1, y1); // We draw one textured quad.  Note: the first numbers 0,1 are texture coordinates, which are ratios.
//     // glTexCoord2f(0, 1);
//     // glVertex2f(x1, y2); // lower left is 0,0, upper right is 1,1.  So if we wanted to use the lower half of the texture, we
//     // glTexCoord2f(1, 1);
//     // glVertex2f(x2, y2); // would use 0,0 to 0,0.5 to 1,0.5, to 1,0.  Note that for X-Plane front facing polygons are clockwise
//     // glTexCoord2f(1, 0);
//     // glVertex2f(x2, y1); // unless you change it; if you change it, change it back!
//     // glEnd();
//     //
//     // glPushMatrix();
//     // XPLMBindTexture2d(fboTexture, 0);
//     //
//     // glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
//     // glTranslatef(20.0f, 200.0f, 0.0f);
//     // glBegin(GL_QUADS);
//     // glTexCoord2f(0.0f, 0.0f); glVertex2i( 0, 0);
//     // glTexCoord2f(1.0f, 0.0f); glVertex2i( 0, 300);
//     // glTexCoord2f(1.0f, 1.0f); glVertex2i(300, 300);
//     // glTexCoord2f(0.0f, 1.0f); glVertex2i(300, 0);
//     // glEnd();
//     // glPopMatrix();
//     //
//     // // XPLMBindTexture2d(g_tex_num, 0);
//     // // glTexSubImage2D(GL_TEXTURE_2D,
//     // //                 0, // mipmap level
//     // //                 0, // x-offset
//     // //                 0, // y-offset
//     // //                 WIDTH,
//     // //                 HEIGHT,
//     // //                 GL_RGBA,          // color of data we are seding
//     // //                 GL_UNSIGNED_BYTE, // encoding of data we are sending
//     // //                 buffer);
//     //
//     //
//     //
//     // snurr = snurr + 0.1f;
//     // glPushMatrix();
//     // glTranslatef(WIDTH, HEIGHT,0);
//     // glRotatef(snurr,0, 0, 1);
//     // glTranslatef(-WIDTH, -HEIGHT,0);
//     // glBegin(GL_QUADS);
//     // glTexCoord2f(0, 0);
//     // glVertex2f(x1, y1); // We draw one textured quad.  Note: the first numbers 0,1 are texture coordinates, which are ratios.
//     // glTexCoord2f(0, 1);
//     // glVertex2f(x1, y2); // lower left is 0,0, upper right is 1,1.  So if we wanted to use the lower half of the texture, we
//     // glTexCoord2f(1, 1);
//     // glVertex2f(x2, y2); // would use 0,0 to 0,0.5 to 1,0.5, to 1,0.  Note that for X-Plane front facing polygons are clockwise
//     // glTexCoord2f(1, 0);
//     // glVertex2f(x2, y1); // unless you change it; if you change it, change it back!
//     // glEnd();
//     //
//     // glPopMatrix();
//     // if (config->visible == 0 || (config->visible == 1 && config->toggleOutside && getViewIsExternal()))
//     //     return 1;
//
//     // // reload aircraft values when needed
//     // if (--acfValuesReloadFrameCount == 0) {
//     //     acfValuesReloadFrameCount = ACF_VALUES_RELOAD_FRAME;
//     //     initAcfValues();
//     // }
//     // Display the window bounds (centered within the window)
//
//     XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     return 1;
// }
