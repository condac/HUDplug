
#include "HUDplug.h"
#include "datarefs.h"
#include "config.h"
#include "statusDisplay.h"
#include "drawFunctions.h"
#include "drawElements.h"
// Downloaded from https://developer.x-plane.com/code-sample/hello-world-sdk-3/

#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include <string.h>

#define WIDTH 128
#define HEIGHT 128
// This is our texture ID.  Texture IDs in OpenGL are just ints...but this is a global for the life of our plugin.
static int g_tex_num = 0;

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
static int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon);
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
GLuint nullptr;

// We use this memory to prep the buffer.  Note that this memory DOES NOT have to be global - the memory is FULLY
// read by OpenGL before glTexSubImage2D or glTexImage2D return, so you could use local or temporary storage, or
// change the image AS SOON as the call returns!  4 bytes for R,G,B,A 32-bit pixels.
static unsigned char buffer[WIDTH * HEIGHT * 4];

static int my_draw_tex(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {
    // A really dumb bitmap generator - just fill R and G with x and Y based color watch, and the B and alpha channels
    // based on mouse position.
    int mx, my, sx, sy;
    XPLMGetMouseLocation(&mx, &my);
    XPLMGetScreenSize(&sx, &sy);
    unsigned char* c = buffer;
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x) {
            *c++ = x * 255 / WIDTH;
            *c++ = y * 255 / HEIGHT;
            *c++ = mx * 255 / sx;
            *c++ = my * 255 / sy;
        }
    XPLMBindTexture2d(g_tex_num, 0);
    // Note: if the tex size is not changing, glTexSubImage2D is faster than glTexImage2D.
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, // mipmap level
                    0, // x-offset
                    0, // y-offset
                    WIDTH,
                    HEIGHT,
                    GL_RGBA,          // color of data we are seding
                    GL_UNSIGNED_BYTE, // encoding of data we are sending
                    buffer);

    // The drawing part.
    XPLMSetGraphicsState(0,  // No fog, equivalent to glDisable(GL_FOG);
                         1,  // One texture, equivalent to glEnable(GL_TEXTURE_2D);
                         0,  // No lighting, equivalent to glDisable(GL_LIGHT0);
                         0,  // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
                         1,  // Use alpha blending, e.g. glEnable(GL_BLEND);
                         0,  // No depth read, e.g. glDisable(GL_DEPTH_TEST);
                         0); // No depth write, e.g. glDepthMask(GL_FALSE);

    glColor3f(1, 1, 1); // Set color to white.
    int x1 = 20;
    int y1 = 20;
    int x2 = x1 + WIDTH;
    int y2 = y1 + HEIGHT;
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x1, y1); // We draw one textured quad.  Note: the first numbers 0,1 are texture coordinates, which are ratios.
    glTexCoord2f(0, 1);
    glVertex2f(x1, y2); // lower left is 0,0, upper right is 1,1.  So if we wanted to use the lower half of the texture, we
    glTexCoord2f(1, 1);
    glVertex2f(x2, y2); // would use 0,0 to 0,0.5 to 1,0.5, to 1,0.  Note that for X-Plane front facing polygons are clockwise
    glTexCoord2f(1, 0);
    glVertex2f(x2, y1); // unless you change it; if you change it, change it back!
    glEnd();
}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
    strcpy(outName, "HUDplug");
    strcpy(outSig, "github.condac.HUDplug");
    strcpy(outDesc, "A plug-in for HUDplug.");

#ifdef XPLM301
    // We're not guaranteed that the main monitor's lower left is at (0, 0)... we'll need to query for the global desktop bounds!
    int global_desktop_bounds[4]; // left, bottom, right, top
    XPLMGetScreenBoundsGlobal(&global_desktop_bounds[0], &global_desktop_bounds[3], &global_desktop_bounds[2], &global_desktop_bounds[1]);

    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    // Set the window bounds such that we stretch the full *width* of the global desktop, and cover the top 200 bx
    params.left = global_desktop_bounds[0];
    params.bottom = global_desktop_bounds[3] - 200;
    params.right = global_desktop_bounds[2];
    params.top = global_desktop_bounds[3];
    params.visible = 1;
    params.drawWindowFunc = draw12;
    params.handleMouseClickFunc = dummy_mouse_handler;
    params.handleRightClickFunc = dummy_mouse_handler;
    params.handleMouseWheelFunc = dummy_wheel_handler;
    params.handleKeyFunc = dummy_key_handler;
    params.handleCursorFunc = dummy_cursor_status_handler;
    params.refcon = NULL;
    params.layer = xplm_WindowLayerFlightOverlay; // stick our window beneath all floating windows (like the X-Plane 11 map)
    params.decorateAsFloatingWindow = 0;

    g_window = XPLMCreateWindowEx(&params);

    //XPLMSetWindowPositioningMode(g_window, xplm_WindowPositionFree, -1);
    // As the X-Plane window resizes, glue our left and right edges to the sides of the screen
    // (causing our width to grow and shrink to match the window size), but keep a constant
    // height for our window (with the same y position relative to the window's top).
    //XPLMSetWindowGravity(g_window, 0, 1, 1, 1);

#endif
    // Position the window as a "free" floating window, which the user can drag around
    //XPLMSetWindowPositioningMode(g_window, xplm_WindowPositionFree, -1);
    // Limit resizing our window: maintain a minimum width/height of 100 boxels and a max width/height of 300 boxels
    //XPLMSetWindowResizingLimits(g_window, 200, 200, 300, 300);
    //XPLMSetWindowTitle(g_window, "HUDplug");

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
    //xplm_Phase_Window
    //xplm_Phase_LastCockpit
    XPLMRegisterDrawCallback(MyDrawCallback,
                             xplm_Phase_Window, /* Draw when sim is doing windows */
                             0,                 /* Before plugin windows */
                             NULL);             /* No refcon needed */
                                                //   XPLMRegisterDrawCallback(MyDrawCallback,
                                                //                         40, /* Draw when sim is doing windows */
                                                //                         0,                 /* Before plugin windows */
                                                //                         NULL);             /* No refcon needed */
                                                // XPLMRegisterDrawCallback(MyDrawCallback,
                                                //                          45, /* Draw when sim is doing windows */
                                                //                          0,                 /* Before plugin windows */
                                                //                          NULL);             /* No refcon needed */
                                                //  XPLMRegisterDrawCallback(MyDrawCallback,
                                                //                           50, /* Draw when sim is doing windows */
                                                //                           0,                 /* Before plugin windows */
                                                //                           NULL);             /* No refcon needed */
                                                //
                                                //   XPLMRegisterDrawCallback(MyDrawCallback,
                                                //                            55, /* Draw when sim is doing windows */
                                                //                            0,                 /* Before plugin windows */
                                                //                            NULL);             /* No refcon needed */
                                                // GLuint fbo_;
                                                // // create a framebuffer object
                                                // glGenFramebuffers(1, &fbo_);
                                                // glBindFramebuffer(GL_FRAMEBUFFER_EXT, fbo_);
                                                //return (g_window != NULL);

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

    XPLMRegisterDrawCallback(my_draw_tex, xplm_Phase_Gauges, 0, NULL);

    // mitt försök att rita i en framebuffer:
    // debugLog("skapa framebuffer\n");
    // XPLMGenerateTextureNumbers((int*)(&hud_texture), 1);
    // XPLMBindTexture2d(hud_texture, 0);
    // 
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // debugLog("texturklart\n");
    // // store whatever rbo and fbo X-Plane is using right now
    // GLint xp_rbo, xp_fbo;
    // glGetIntegerv(GL_RENDERBUFFER_BINDING, &xp_rbo);
    // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &xp_fbo);
    // debugLog("renderbuffers1\n");
    // // create a renderbuffer object to store depth info
    // //glGenRenderbuffersEXT(1, &rbo_);
    // debugLog("renderbuffers12\n");
    // //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rbo_);
    // debugLog("renderbuffers13\n");
    // //glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL, 1024, 768);
    // debugLog("renderbuffers2\n");
    // //glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, xp_rbo);
    // debugLog("renderbuffers\n");
    // // create a framebuffer object
    // debugLog("framebuffer\n");
    // //glGenFramebuffersEXT(1, &fbo_);
    // glGenFramebuffers(1, &fbo_);
    // debugLog("framebuffer1\n");
    // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_);
    // debugLog("framebuffer2\n");
    // // attach the texture to FBO color attachment point
    // glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, hud_texture, 0);
    // debugLog("framebuffer3\n");
    // // attach the renderbuffer to depth attachment point
    // glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT, rbo_);
    // debugLog("klart\n");
    // // check FBO status
    // GLenum status_ = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    // if (status_ != GL_FRAMEBUFFER_COMPLETE_EXT)
    //     debugLog("No possible FBO, sorry");
    // 
    // // switch back to window-system-provided framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER_EXT, xp_fbo);
    return 1; //g_window != NULL;
}

PLUGIN_API void XPluginStop(void) {
    // Since we created the window, we'll be good citizens and clean it up
#ifdef XPLM301
    XPLMDestroyWindow(g_window);
    g_window = NULL;
#endif
    XPLMUnregisterDrawCallback(MyDrawCallback, xplm_Phase_Window, 1, NULL);
    XPLMUnregisterDrawCallback(my_draw_tex, xplm_Phase_Gauges, 0, NULL);
    XPLMBindTexture2d(g_tex_num, 0);
    GLuint t = g_tex_num;
    glDeleteTextures(1, &t);
}

PLUGIN_API void XPluginDisable(void) {
}
PLUGIN_API int XPluginEnable(void) {
    return 1;
}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {
}

void draw_hello_world(XPLMWindowID in_window_id, void* in_refcon) {
    // Mandatory: We *must* set the OpenGL state before drawing
    // (we can't make any assumptions about it)
    XPLMSetGraphicsState(
        0 /* no fog */, 0 /* 0 texture units */, 0 /* no lighting */, 0 /* no alpha testing */, 1 /* do alpha blend */, 1 /* do depth testing */, 0 /* no depth writing */
    );

    int l, t, r, b;
    XPLMGetWindowGeometry(in_window_id, &l, &t, &r, &b);

    float col_white[] = {1.0, 1.0, 1.0}; // red, green, blue

    XPLMDrawString(col_white, l + 10, t - 20, "Hello world!", NULL, xplmFont_Proportional);
    XPLMDrawString(col_white, l + 10, t - 20, "Hello world2!", NULL, xplmFont_Proportional);
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

float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {

    /* The actual callback.  First we read the sim's time and the data. */
    //XPLMDebugString("HUDplug: flightloop\n");

    //float elapsed = XPLMGetElapsedTime();

    /* Return 1.0 to indicate that we want to be called again in 1 second. */
    return 0.01;
}
int fboInit = 0;
int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {

    if (!fboInit) {
        debugLog("skapar framebuffer");
        GLenum err = glewInit();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        // Create and bind FBO
        glGenFramebuffers(1, &fbo);
        // Create depth renderbuffer
        glGenRenderbuffers(1, &fboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
        // Create the texture
        //glGenTextures(1, &fboTexture);
        //glBindTexture(GL_RENDERBUFFER, fboTexture);
        XPLMGenerateTextureNumbers(&fboTexture, 1);
        XPLMBindTexture2d(fboTexture, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Attached texture to first color attachment and the depth to the depth attachment
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepth);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
        GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            fbo = 0;
            fboDepth = 0;
            fboTexture = 0;
            debugLog("skapar framebuffer error?\n");
        } else {
            // Reste FBO Binding
            debugLog("skapar framebuffer klart1\n");
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFBO());
            debugLog("skapar framebuffer klart2\n");
        }
        fboInit = 1;
        debugLog("skapar framebuffer klart3\n");
    }
    debugLog("render loop\n");
    GLint xp_fbo;
    //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &xp_fbo);
    // set rendering destination to FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    debugLog("render bind framebuffer \n");
    //glDrawBuffers(1, fbo);
    debugLog("render glDrawBuffers\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Draw things into FBO
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2i( 0, 0);
    glVertex2i( 0, 256);
    glVertex2i(256, 256);
    glVertex2i(256, 0);
    glEnd();
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, xp_fbo);
    
    
    // if (config->visible == 0 || (config->visible == 1 && config->toggleOutside && getViewIsExternal()))
    //     return 1;

    // // reload aircraft values when needed
    // if (--acfValuesReloadFrameCount == 0) {
    //     acfValuesReloadFrameCount = ACF_VALUES_RELOAD_FRAME;
    //     initAcfValues();
    // }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    //XPLMSetGraphicsState(0 /*Fog*/, 1 /*TexUnits*/, 0 /*Lighting*/, 1 /*AlphaTesting*/, 1 /*AlphaBlending*/, 0 /*DepthTesting*/, 0 /*DepthWriting*/);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    //DrawTest();
    CalculateCenter();
    // if (CalculateCenter() == -1) {
    //     return 1;
    // }

    glTranslatef(offset_x, offset_y, 0);
    glScalef(hud_scale, hud_scale, 0);

    if (viggen_mode == 1) {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }
        DrawViggen();
        glPopMatrix();

    } else if (viggen_mode == 2) {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }
        DrawViggenMode2();
        glPopMatrix();

    } else {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }

        DrawModesJAS();
        // DrawCompass();
        //
        // TranslateToCenter();
        // DrawVector();
        //
        // TranslateToCenter();
        // DrawSpeed();
        // DrawAlpha();
        //
        // TranslateToCenter();
        // DrawAltitude();
        // DrawGroundCollision();
        // DrawFuelTime();
        //
        // TranslateToCenter();
        // DrawHorizionLines();
        glPopMatrix();
    }

    glPopMatrix();
    // /* Do the actual drawing.  use GL_LINES to draw sets of discrete lines.
    //  * Each one will go 100 meters in any direction from the plane. */
    // TranslateToCenter();
    // if (config->visPitchRoll) {
    //     DrawCenterBox();
    //     DrawNoseBox(getPitch(), getRoll());
    // }
    // if (config->visMovementArrow)
    //     DrawMovementArrow(getTrueHeading(), getVX(), getVY(), getVZ());
    // // VSI & ball covered inside
    // DrawVerticalSpeedIndicator(getVV());
    // if (config->visLandingBars)
    //     DrawLandingBars(getRadarAltitude());
    // if (config->visIas)
    //     DrawSpeedIndicator(getIAS());
    // if (config->visWind)
    //     DrawWind(getWindDirection(), getWindSpeed(), getHeading());
    // DrawBalanceIndicator(getBalance(), getYawStringAngle());
    // if (config->visTorque) {
    //     float lTorqs[10];
    //     getTorque(lTorqs);
    //     DrawTorque(lTorqs);
    // }
    // DrawTexts();
    //glBindFramebuffer(GL_FRAMEBUFFER, getFBO());

    
    debugLog("render glBindFramebuffer\n");
    glBindFramebuffer(GL_FRAMEBUFFER, getFBO());
    glPushMatrix();
    debugLog("render glBindTexture\n");
    //glBindTexture(GL_TEXTURE_2D, fboTexture);
    XPLMBindTexture2d(fboTexture, 0);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glTranslatef(20.0f, 20.0f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i( 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( 0, 300);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(300, 300);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(300, 0);
    glEnd();
    glPopMatrix();
    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, xp_fbo);
    debugLog("render glBindFramebuffer tillbaka\n");
    //glBindFramebuffer(GL_FRAMEBUFFER, getFBO());
    debugLog("render loop klart med framebuffer");
    
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    static int heartbeat = 0;
    heartbeat++;
    XPLMSetDatai(drHUDheartbeat, heartbeat);
    return 1;
}

void draw12(XPLMWindowID in_window_id, void* in_refcon) {
    return;
    XPLMSetGraphicsState(
        0 /* no fog */, 0 /* 0 texture units */, 0 /* no lighting */, 0 /* no alpha testing */, 1 /* do alpha blend */, 1 /* do depth testing */, 0 /* no depth writing */
    );

    int b[4];
    XPLMGetWindowGeometry(in_window_id, &b[0], &b[3], &b[2], &b[1]);

    // Draw our window's translucent background overlay
    XPLMDrawTranslucentDarkBox(b[0], b[3], b[2], b[1]);

    // Display the window bounds (centered within the window)
    char scratch_buffer[150];
    sprintf(scratch_buffer, "Window bounds: %d %d %d %d", b[0], b[1], b[2], b[3]);
    float col_white[] = {1.0, 1.0, 1.0};
    int text_width = XPLMMeasureString(xplmFont_Proportional, scratch_buffer, strlen(scratch_buffer));
    float text_midpoint_x = (b[2] + b[0]) / 2;
    XPLMDrawString(col_white, text_midpoint_x - text_width / 2, (b[3] + b[1]) / 2, scratch_buffer, NULL, xplmFont_Proportional);

    glColor4f(1.0, 0.0, 0.0, 0.9);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);

    glVertex2f(0, 0);
    glVertex2f(10, 0);
    glVertex2f(0, 10);
    glVertex2f(-20, 0);
    glVertex2f(0, -20);
    glVertex2f(-100, -100);
    glVertex2f(100, -100);
    glVertex2f(100, 100);

    glEnd();
    //glEnable(GL_BLEND);
    XPLMSetGraphicsState(0 /*Fog*/, 0 /*TexUnits*/, 0 /*Lighting*/, 0 /*AlphaTesting*/, 1 /*AlphaBlending*/, 0 /*DepthTesting*/, 0 /*DepthWriting*/);
    glEnable(GL_BLEND);
    glColor4f(0.0, 0.0, 1.0, 0.9);

    glLineWidth(4);
    glBegin(GL_LINE_LOOP);

    glVertex2f(100, 100);
    glVertex2f(10, 0);
    glVertex2f(0, 10);
    glVertex2f(-20, 0);
    glVertex2f(0, -20);
    glVertex2f(-100, -100);
    glVertex2f(100, -100);
    glVertex2f(100, 100);

    glEnd();
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0); // turn off blending

    glPushMatrix();
    DrawTest();
    CalculateCenter();

    glTranslatef(offset_x, offset_y, 0);
    glScalef(hud_scale, hud_scale, 0);

    if (viggen_mode == 1) {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }
        DrawViggen();
        glPopMatrix();

    } else if (viggen_mode == 2) {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }
        DrawViggenMode2();
        glPopMatrix();

    } else {
        TranslateToCenter();
        DrawGlass();
        glPushMatrix();
        if (g_sway) {
            glTranslatef(-getGForceX() * 10 * g_sway, -getGForce() * 5 * g_sway, 0);
        }
        if (draw_test) {
            DrawTest();
        }

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
