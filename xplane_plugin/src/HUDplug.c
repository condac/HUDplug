
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

//#ifndef XPLM300
//      #error This is made to be compiled against the XPLM300 SDK
//#endif

// An opaque handle to the window we will create

#ifdef XPLM301
static XPLMWindowID g_window;



void				draw12(XPLMWindowID in_window_id, void * in_refcon);

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


    //xplm_Phase_Window // den vi brukar köra
    // XPLMRegisterDrawCallback(MyDrawCallback,
    //                          xplm_Phase_Window, /* Draw when sim is doing windows */
    //                          0,                 /* Before plugin windows */
    //                          NULL);             /* No refcon needed */
    //xplm_Phase_LastCockpit

    //xplm_Phase_Panel
    XPLMRegisterDrawCallback(MyDrawCallback,
                             xplm_Phase_Panel, /* Draw when sim is doing windows */
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
    return 1; //g_window != NULL;
}

PLUGIN_API void XPluginStop(void) {
    // Since we created the window, we'll be good citizens and clean it up
#ifdef XPLM301
    XPLMDestroyWindow(g_window);
    g_window = NULL;
#endif
    XPLMUnregisterDrawCallback(MyDrawCallback, xplm_Phase_Window, 1, NULL);
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

int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void* inRefcon) {

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

void	draw12(XPLMWindowID in_window_id, void * in_refcon)
{
    return;
	XPLMSetGraphicsState(
			0 /* no fog */,
			0 /* 0 texture units */,
			0 /* no lighting */,
			0 /* no alpha testing */,
			1 /* do alpha blend */,
			1 /* do depth testing */,
			0 /* no depth writing */
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
