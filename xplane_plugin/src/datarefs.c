#include "HUDplug.h"

#include "datarefs.h"

XPLMDataRef drPitch;
XPLMDataRef drRoll;
XPLMDataRef drTrueHeading;
XPLMDataRef drHeading;
XPLMDataRef drVX;
XPLMDataRef drVY;
XPLMDataRef drVZ;
XPLMDataRef drVV;
XPLMDataRef drRadarAltitude;
XPLMDataRef drAltitude;
XPLMDataRef drWindSpeed;
XPLMDataRef drWindDirection;
XPLMDataRef drIAS;
XPLMDataRef drGroundSpeed;

XPLMDataRef drMachSpeed;
//XPLMDataRef  drCurrentView;
XPLMDataRef drViewIsExternal;
XPLMDataRef drBalInd;
XPLMDataRef drYawStr;

// torque
XPLMDataRef drTrq;
XPLMDataRef drEnginesNo;
XPLMDataRef drTrqGreenLo;
XPLMDataRef drTrqGreenHi;
XPLMDataRef drTrqYellowLo;
XPLMDataRef drTrqYellowHi;
XPLMDataRef drTrqRedLo;
XPLMDataRef drTrqRedHi;
XPLMDataRef drMaxTrq;

XPLMDataRef hudVisibleDR = NULL;
XPLMCommandRef toggleHudCommand = NULL;

int numberOfEngines = 1;
float tqGreenLo;
float tqGreenHi;
float tqYellowLo;
float tqYellowHi;
float tqRedLo;
float tqRedHi;
float tqMax;

void debugLog(const char* fmt, ...) {
    char buffer[1024];
    // if I ever send debug string longer than 1024 bytes - "HELIHUD: ",
    // I will never find this error why application crashes :-)
    va_list ap;
    va_start(ap, fmt);
    strcpy(buffer, "HUDplug:  ");
    vsprintf(buffer + 9, fmt, ap);
    va_end(ap);
    XPLMDebugString(buffer);
}

int findDataRef(const char* name, XPLMDataRef* result) {
    *result = XPLMFindDataRef(name);
    if (*result == NULL) {
        debugLog("Error finding XPL variable ");
        debugLog(name);
        debugLog("\n");
        return -1;
    }
    return 0;
}

int registerDataRefs() {
    //  Create our custom integer dataref
    hudVisibleDR = XPLMRegisterDataAccessor("HUDplug/Visible",
                                            xplmType_Int, // The types we support
                                            1,            // Writable
                                            GetHudVisibleCB,
                                            SetHudVisibleCB, // Integer accessors
                                            NULL,
                                            NULL, // Float accessors
                                            NULL,
                                            NULL, // Doubles accessors
                                            NULL,
                                            NULL, // Int array accessors
                                            NULL,
                                            NULL, // Float array accessors
                                            NULL,
                                            NULL, // Raw data accessors
                                            NULL,
                                            NULL); // Refcons not used

    // Find and intialize our dataref
    hudVisibleDR = XPLMFindDataRef("HUDplug/Visible");
    if (hudVisibleDR == NULL)
        return -1;
    else {
        XPLMSetDatai(hudVisibleDR, 0);
        return 0;
    }
}

int initDataRefs() {
    int lTmp;
    /* Also look up our data refs. */
    lTmp = 0;
    lTmp += findDataRef("sim/flightmodel/position/theta", &drPitch);
    lTmp += findDataRef("sim/flightmodel/position/phi", &drRoll);
    lTmp += findDataRef("sim/flightmodel/position/magpsi", &drHeading);
    lTmp += findDataRef("sim/flightmodel/position/psi", &drTrueHeading);
    lTmp += findDataRef("sim/flightmodel/position/local_vx", &drVX);
    lTmp += findDataRef("sim/flightmodel/position/local_vy", &drVY);
    lTmp += findDataRef("sim/flightmodel/position/local_vz", &drVZ);
    lTmp += findDataRef("sim/flightmodel/position/vh_ind_fpm", &drVV);
    lTmp += findDataRef("sim/flightmodel/position/y_agl", &drRadarAltitude);
    lTmp += findDataRef("sim/flightmodel/misc/h_ind", &drAltitude);
    lTmp += findDataRef("sim/cockpit2/gauges/indicators/wind_speed_kts", &drWindSpeed);
    lTmp += findDataRef("sim/cockpit2/gauges/indicators/wind_heading_deg_mag", &drWindDirection);
    //lTmp += findDataRef("sim/weather/wind_direction_degt[0]", &drWindDirection);
    lTmp += findDataRef("sim/flightmodel/position/indicated_airspeed", &drIAS);
    
    lTmp += findDataRef("sim/cockpit2/gauges/indicators/mach_pilot", &drMachSpeed);
    lTmp += findDataRef("sim/flightmodel/position/groundspeed", &drGroundSpeed);
    //lTmp += findDataRef("sim/flightmodel/position/magnetic_variation", &drMagVar);
    //lTmp += findDataRef("sim/graphics/view/view_type", &drCurrentView);
    lTmp += findDataRef("sim/graphics/view/view_is_external", &drViewIsExternal);
    lTmp += findDataRef("sim/cockpit2/gauges/indicators/slip_deg", &drBalInd);
    lTmp += findDataRef("sim/flightmodel2/misc/yaw_string_angle", &drYawStr);
    lTmp += findDataRef("sim/flightmodel/engine/ENGN_TRQ", &drTrq); //NewtonMeters
    lTmp += findDataRef("sim/aircraft/engine/acf_num_engines", &drEnginesNo);
    lTmp += findDataRef("sim/aircraft/limits/green_lo_TRQ", &drTrqGreenLo); // ft-lbs
    lTmp += findDataRef("sim/aircraft/limits/green_hi_TRQ", &drTrqGreenHi);
    lTmp += findDataRef("sim/aircraft/limits/yellow_lo_TRQ", &drTrqYellowLo);
    lTmp += findDataRef("sim/aircraft/limits/yellow_hi_TRQ", &drTrqYellowHi);
    lTmp += findDataRef("sim/aircraft/limits/red_lo_TRQ", &drTrqRedLo);
    lTmp += findDataRef("sim/aircraft/limits/red_hi_TRQ", &drTrqRedHi);
    lTmp += findDataRef("sim/aircraft/controls/acf_trq_max_eng", &drMaxTrq);

    lTmp += registerDataRefs();

    return lTmp;
}

void unregisterData() {
    XPLMUnregisterDataAccessor(hudVisibleDR);
}

float getPitch() {
    return XPLMGetDataf(drPitch);
}
float getRoll() {
    return XPLMGetDataf(drRoll);
}
float getHeading() {
    return XPLMGetDataf(drHeading);
}
float getTrueHeading() {
    return XPLMGetDataf(drTrueHeading);
}
float getVX() {
    return XPLMGetDataf(drVX);
}
float getVY() {
    return XPLMGetDataf(drVY);
}
float getVZ() {
    return XPLMGetDataf(drVZ);
}
float getVV() {
    return XPLMGetDataf(drVV);
}
float getRadarAltitude() {
    return XPLMGetDataf(drRadarAltitude);
}
float getAltitude() {
    return XPLMGetDataf(drAltitude);
}
float getWindSpeed() {
    return XPLMGetDataf(drWindSpeed);
}
float getWindDirection() {
    return XPLMGetDataf(drWindDirection);
}
float getIAS() {
    return XPLMGetDataf(drIAS);
}
float getGroundSpeed() {
    return XPLMGetDataf(drGroundSpeed);
}
float getMachSpeed() {
    return XPLMGetDataf(drMachSpeed);
}
float getBalance() {
    return XPLMGetDataf(drBalInd);
}
float getYawStringAngle() {
    return XPLMGetDataf(drYawStr);
}

//int getCurrentView() {
//  return XPLMGetDatai(drCurrentView);
//}

int getViewIsExternal() {
    return XPLMGetDatai(drViewIsExternal);
}

int GetHudVisibleCB(void* inRefcon) {
    return 1;
}

void SetHudVisibleCB(void* inRefcon, int inValue) {
    //visible = inValue
}
