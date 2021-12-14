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
XPLMDataRef drElvTrim;

XPLMDataRef drMachSpeed;
XPLMDataRef drAlpha;
XPLMDataRef drBeta;
//XPLMDataRef  drCurrentView;
XPLMDataRef drViewIsExternal;
XPLMDataRef drBalInd;
XPLMDataRef drYawStr;
XPLMDataRef drFOVoff_x;
XPLMDataRef drFOVoff_y;
XPLMDataRef drFOV;
XPLMDataRef drFOV_x;
float fov2;
int fov_notfound = 0;
XPLMDataRef drGForce;
XPLMDataRef drGForceX;
XPLMDataRef drGForceZ;
XPLMDataRef drThrottlePos;

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

XPLMDataRef drGear;
XPLMDataRef drTotalWeight;
XPLMDataRef drSpeedBrake;
XPLMDataRef drSpeedBrakeAnalog;
XPLMDataRef drParkBrakeAnalog;

XPLMDataRef drFuel1;
XPLMDataRef drFuel2;
XPLMDataRef drFuel3;
XPLMDataRef drFuel4;
XPLMDataRef drFuelExtra1;
XPLMDataRef drFuelExtra2;
XPLMDataRef drFuelExtra3;
XPLMDataRef drFuelExtra1m;
XPLMDataRef drFuelExtra2m;
XPLMDataRef drFuelExtra3m;
XPLMDataRef drFuelFlow;
XPLMDataRef drWeaponFired;
XPLMDataRef drWeaponType;

XPLMDataRef drGearDef;
XPLMDataRef drFBO;
XPLMDataRef drPause;
XPLMDataRef dr_nav1_vdef;
XPLMDataRef dr_nav1_hdef;
XPLMDataRef dr_nav1_disph;
XPLMDataRef dr_nav1_dispv;
XPLMDataRef dr_nav1_heading;
XPLMDataRef dr_nav1_id;
XPLMDataRef dr_nav1_distance;
XPLMDataRef dr_nav1_eta;

XPLMDataRef hudVisibleDR = NULL;
XPLMDataRef stabilisatorStatusDR = NULL;
XPLMDataRef glasDarknessDR = NULL;
XPLMDataRef fuelDR = NULL;

XPLMDataRef JASLampsAirbrakeDR = NULL;
XPLMDataRef JASLampsSpakDR = NULL;
XPLMDataRef JASLampsAttDR = NULL;
XPLMDataRef JASLampsHojdDR = NULL;

int JASLampsSpak = 0;
int JASLampsAtt = 0;
int JASLampsHojd = 0;

XPLMDataRef JASButtonSpakDR = NULL;
XPLMDataRef JASButtonAttDR = NULL;
XPLMDataRef JASButtonHojdDR = NULL;

int JASButtonSpak = 0;
int JASButtonAtt = 0;
int JASButtonHojd = 0;

float fuelTotal = 0.0;
float glassDarknessValue = 0.2;
int stabilisatorStatusValue = 0;
XPLMCommandRef toggleHudCommand = NULL;

int numberOfEngines = 1;
float tqGreenLo;
float tqGreenHi;
float tqYellowLo;
float tqYellowHi;
float tqRedLo;
float tqRedHi;
float tqMax;

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
        //return 0;
    }
    glasDarknessDR = XPLMRegisterDataAccessor("HUDplug/glass_darkness",
                                              xplmType_Float, // The types we support
                                              1,              // Writable
                                              NULL,
                                              NULL, // Integer accessors
                                              GetGlassDarknessCB,
                                              SetGlassDarknessCB, // Float accessors
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
    glasDarknessDR = XPLMFindDataRef("HUDplug/glass_darkness");
    if (glasDarknessDR == NULL)
        return -1;
    else {
        XPLMSetDataf(glasDarknessDR, 0.3);
        //return 0;
    }
    fuelDR = XPLMRegisterDataAccessor("JAS/fuel",
                                      xplmType_Float, // The types we support
                                      1,              // Writable
                                      NULL,
                                      NULL, // Integer accessors
                                      GetJASFuelCB,
                                      SetJASFuelCB, // Float accessors
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
    fuelDR = XPLMFindDataRef("JAS/fuel");
    if (fuelDR == NULL)
        return -1;
    else {
        XPLMSetDataf(fuelDR, 0.3);
        //return 0;
    }
    JASLampsAirbrakeDR = XPLMRegisterDataAccessor("JAS/lamps/airbrake",
                                                  xplmType_Int, // The types we support
                                                  1,            // Writable
                                                  GetJASLampsAirbrakeCB,
                                                  SetJASLampsAirbrakeCB, // Integer accessors
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
    JASLampsAirbrakeDR = XPLMFindDataRef("JAS/lamps/airbrake");
    if (JASLampsAirbrakeDR == NULL)
        return -1;
    else {
        //XPLMSetDataf(JASLampsAirbrakeDR, 0.3);
        //return 0;
    }
    JASLampsSpakDR = XPLMRegisterDataAccessor("JAS/lamps/spak",
                                              xplmType_Int, // The types we support
                                              1,            // Writable
                                              GetJASLampsSpakCB,
                                              SetJASLampsSpakCB, // Integer accessors
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
    JASLampsSpakDR = XPLMFindDataRef("JAS/lamps/spak");
    if (JASLampsSpakDR == NULL)
        return -1;
    else {
        //XPLMSetDataf(JASLampsAirbrakeDR, 0.3);
        //return 0;
    }
    JASLampsAttDR = XPLMRegisterDataAccessor("JAS/lamps/att",
                                             xplmType_Int, // The types we support
                                             1,            // Writable
                                             GetJASLampsAttCB,
                                             SetJASLampsAttCB, // Integer accessors
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
    JASLampsAttDR = XPLMFindDataRef("JAS/lamps/att");
    if (JASLampsAttDR == NULL)
        return -1;
    else {
        //XPLMSetDataf(JASLampsAirbrakeDR, 0.3);
        //return 0;
    }
    JASLampsHojdDR = XPLMRegisterDataAccessor("JAS/lamps/hojd",
                                              xplmType_Int, // The types we support
                                              1,            // Writable
                                              GetJASLampsHojdCB,
                                              SetJASLampsHojdCB, // Integer accessors
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
    JASLampsHojdDR = XPLMFindDataRef("JAS/lamps/hojd");
    if (JASLampsHojdDR == NULL) {
        return -1;
    }

    JASButtonHojdDR = XPLMRegisterDataAccessor("JAS/button/hojd",
                                               xplmType_Int, // The types we support
                                               1,            // Writable
                                               GetJASButtonHojdCB,
                                               SetJASButtonHojdCB, // Integer accessors
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
    JASButtonHojdDR = XPLMFindDataRef("JAS/button/hojd");
    if (JASButtonHojdDR == NULL) {
        return -1;
    }

    JASButtonAttDR = XPLMRegisterDataAccessor("JAS/button/att",
                                              xplmType_Int, // The types we support
                                              1,            // Writable
                                              GetJASButtonAttCB,
                                              SetJASButtonAttCB, // Integer accessors
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
    JASButtonAttDR = XPLMFindDataRef("JAS/button/att");
    if (JASButtonAttDR == NULL) {
        return -1;
    }

    JASButtonSpakDR = XPLMRegisterDataAccessor("JAS/button/spak",
                                               xplmType_Int, // The types we support
                                               1,            // Writable
                                               GetJASButtonSpakCB,
                                               SetJASButtonSpakCB, // Integer accessors
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
    JASButtonSpakDR = XPLMFindDataRef("JAS/button/spak");
    if (JASButtonSpakDR == NULL) {
        return -1;
    }

    stabilisatorStatusDR = XPLMRegisterDataAccessor("HUDplug/stabilisatorStatus",
                                                    xplmType_Int, // The types we support
                                                    1,            // Writable
                                                    GetStabilisatorStatusCB,
                                                    SetStabilisatorStatusCB, // Integer accessors
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
    stabilisatorStatusDR = XPLMFindDataRef("HUDplug/stabilisatorStatus");
    if (stabilisatorStatusDR == NULL)
        return -1;
    else {
        XPLMSetDatai(stabilisatorStatusDR, 0);
        return 0;
    }
}

int initDataRefs() {
    int lTmp;
    /* Also look up our data refs. */
    lTmp = 0;
    //lTmp += findDataRef("sim/flightmodel/position/theta", &drPitch);
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
    lTmp += findDataRef("sim/flightmodel/controls/elv_trim", &drElvTrim);

    lTmp += findDataRef("sim/flightmodel/misc/machno", &drMachSpeed);
    lTmp += findDataRef("sim/flightmodel/position/groundspeed", &drGroundSpeed);

    lTmp += findDataRef("sim/flightmodel/position/alpha", &drAlpha);
    lTmp += findDataRef("sim/flightmodel/position/beta", &drBeta);

    lTmp += findDataRef("sim/flightmodel/forces/g_nrml", &drGForce);
    lTmp += findDataRef("sim/flightmodel/forces/g_side", &drGForceX);
    lTmp += findDataRef("sim/flightmodel/forces/g_axil", &drGForceZ);
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

    lTmp += findDataRef("sim/cockpit/switches/gear_handle_status", &drGear);
    lTmp += findDataRef("sim/flightmodel/weight/m_total", &drTotalWeight);
    lTmp += findDataRef("sim/cockpit2/engine/actuators/throttle_ratio_all", &drThrottlePos);
    lTmp += findDataRef("sim/cockpit/warnings/annunciators/speedbrake", &drSpeedBrake);
    lTmp += findDataRef("sim/cockpit2/controls/speedbrake_ratio", &drSpeedBrakeAnalog);
    lTmp += findDataRef("sim/cockpit2/controls/parking_brake_ratio", &drParkBrakeAnalog);

    lTmp += findDataRef("sim/flightmodel/engine/ENGN_FF_", &drFuelFlow);
    lTmp += findDataRef("sim/flightmodel/weight/m_fuel_total", &drFuel1);
    lTmp += findDataRef("sim/weapons/fuel_warhead_mass_now", &drFuelExtra1);
    lTmp += findDataRef("sim/weapons/fuel_warhead_mass_max", &drFuelExtra1m);

    lTmp += findDataRef("sim/weapons/firing", &drWeaponFired);
    lTmp += findDataRef("sim/weapons/type", &drWeaponType);

    lTmp += findDataRef("sim/cockpit/radios/nav1_vdef_dot", &dr_nav1_vdef);
    lTmp += findDataRef("sim/cockpit/radios/nav1_hdef_dot", &dr_nav1_hdef);
    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_bearing_deg_mag", &dr_nav1_heading);
    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_nav_id", &dr_nav1_id);
    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_dme_distance_nm", &dr_nav1_distance);
    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_dme_time_min", &dr_nav1_eta);

    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_display_horizontal", &dr_nav1_disph);
    lTmp += findDataRef("sim/cockpit2/radios/indicators/nav1_display_vertical", &dr_nav1_dispv);

    lTmp += findDataRef("sim/flightmodel/parts/tire_vrt_def_veh", &drGearDef);
    lTmp += findDataRef("sim/graphics/view/current_gl_fbo", &drFBO);
    lTmp += findDataRef("sim/time/paused", &drPause);

    lTmp += findDataRef("sim/graphics/view/field_of_view_vertical_deg", &drFOVoff_y);
    lTmp += findDataRef("sim/graphics/view/field_of_view_horizontal_deg", &drFOVoff_x);
    lTmp += findDataRef("sim/graphics/view/field_of_view_deg", &drFOV_x);

    if (findDataRef("sim/graphics/view/vertical_field_of_view_deg", &drFOV) == -1) {
        fov2 = 30;
        fov_notfound = 1;
    }
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
float getPitchTrim() {
    return XPLMGetDataf(drElvTrim);
}
float getTrueHeading() {
    return XPLMGetDataf(drTrueHeading);
}
float getAlphaA() {
    return XPLMGetDataf(drAlpha);
}
float getBetaA() {
    return XPLMGetDataf(drBeta);
}
float getGForce() {
    return XPLMGetDataf(drGForce);
}
float getGForceX() {
    return XPLMGetDataf(drGForceX);
}
float getGForceZ() {
    return XPLMGetDataf(drGForceZ);
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
float getFOVoff_y() {
    return XPLMGetDataf(drFOVoff_y);
}
float getFOVoff_x() {
    return XPLMGetDataf(drFOVoff_x);
}
float getFOV() {
    if (fov_notfound) {

        int screen_width;
        int screen_height;

        XPLMGetScreenSize(&screen_width, &screen_height);
        float w = screen_width;
        float h = screen_height;
        float ratio = w / h;
        fov2 = XPLMGetDataf(drFOV_x) / ratio;
        return fov2;
    }
    return XPLMGetDataf(drFOV);
}

float getTotalWeight() {
    return XPLMGetDataf(drTotalWeight);
}
float getThrottlePos() {
    return XPLMGetDataf(drThrottlePos);
}
int getGear() {
    return XPLMGetDatai(drGear);
}
int getSpeedBrake() {
    if (XPLMGetDataf(drSpeedBrakeAnalog) > 0.0) {
        return 1;
    } else {
        return 0;
    }
    return XPLMGetDatai(drSpeedBrake);
}
int getParkBrake() {
    if (XPLMGetDataf(drParkBrakeAnalog) > 0.0) {
        return 1;
    } else {
        return 0;
    }
}
int getStabStatus() {
    return XPLMGetDatai(stabilisatorStatusDR);
}
int getPause() {
    return XPLMGetDatai(drPause);
}
void setStabStatus(int value) {
    XPLMSetDatai(stabilisatorStatusDR, value);
}

float getArrayValuef(XPLMDataRef dataref, int index) {
    float value[1];
    XPLMGetDatavf(dataref, value, index, 1);
    return value[0];
}
int getArrayValuei(XPLMDataRef dataref, int index) {
    int value[1];
    XPLMGetDatavi(dataref, value, index, 1);
    return value[0];
}
int isFuelTank(int index) {
    if (getArrayValuei(drWeaponType, index) == 23 && getArrayValuei(drWeaponFired, 0) == 0) {
        return 1;
    }
    return 0;
}
float getFuelInTank(int index) {
    if (isFuelTank(index) == 1) {
        return getArrayValuef(drFuelExtra1, index);
    }
    return 0.0;
}
float getTotalFuel() {
    float total = 0.0;
    total = total + XPLMGetDataf(drFuel1);
    /* Vi kan inte veta om extra tankarna är bensin eller bomber.
    För att kolla detta så kikar vi på om extra tanken väger mindre än maxvikten,
    detta betyder att det försvunnit bensin och då är det en tank
    */
    total = total + getFuelInTank(0);
    total = total + getFuelInTank(2);
    total = total + getFuelInTank(3);
    total = total + getFuelInTank(4);
    total = total + getFuelInTank(5);

    return total;
}

int markKontakt() {

    if (getArrayValuef(drGearDef, 0) > 0.0 || getArrayValuef(drGearDef, 1) > 0.0 || getArrayValuef(drGearDef, 2) > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

float getFuelFlow() {
    float value[1];
    XPLMGetDatavf(drFuelFlow, value, 0, 1);
    return value[0];
}

float getILSv() {
    return XPLMGetDataf(dr_nav1_vdef);
}
float getILSh() {
    return XPLMGetDataf(dr_nav1_hdef);
}
int ifILSEnabled() {
    if (XPLMGetDatai(dr_nav1_disph) == 1 && XPLMGetDatai(dr_nav1_dispv) == 1) {
        return 1;
    } else {
        return 0;
    }
}
float getNAV1Heading() {
    return XPLMGetDataf(dr_nav1_heading);
}
float getNAV1Distance() {
    return XPLMGetDataf(dr_nav1_distance);
}
float getNAV1ETA() {
    return XPLMGetDataf(dr_nav1_eta) * 60;
}
int getNAV1Id(const char* text) {
    return XPLMGetDatab(dr_nav1_id, (void*)text, 0, 150);
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
float GetJASFuelCB(void* inRefcon) {
    fuelTotal = (getTotalFuel() / 3000.0) * 100;
    return fuelTotal;
}

void SetJASFuelCB(void* inRefcon, float inValue) {
    fuelTotal = inValue;
}

int GetJASLampsAirbrakeCB(void* inRefcon) {
    return getSpeedBrake();
}

void SetJASLampsAirbrakeCB(void* inRefcon, int inValue) {
}

int GetJASLampsSpakCB(void* inRefcon) {
    return JASLampsSpak;
}
void SetJASLampsSpakCB(void* inRefcon, int inValue) {
    JASLampsSpak = inValue;
}

int GetJASLampsAttCB(void* inRefcon) {
    return JASLampsAtt;
}
void SetJASLampsAttCB(void* inRefcon, int inValue) {
    JASLampsAtt = inValue;
}

int GetJASLampsHojdCB(void* inRefcon) {
    return JASLampsHojd;
}
void SetJASLampsHojdCB(void* inRefcon, int inValue) {
    JASLampsHojd = inValue;
}

int GetJASButtonSpakCB(void* inRefcon) {
    return JASButtonSpak;
}
void SetJASButtonSpakCB(void* inRefcon, int inValue) {
    JASButtonSpak = inValue;
}

int GetJASButtonAttCB(void* inRefcon) {
    return JASButtonAtt;
}
void SetJASButtonAttCB(void* inRefcon, int inValue) {
    JASButtonAtt = inValue;
}

int GetJASButtonHojdCB(void* inRefcon) {
    return JASButtonHojd;
}
void SetJASButtonHojdCB(void* inRefcon, int inValue) {
    JASButtonHojd = inValue;
}

int GetStabilisatorStatusCB(void* inRefcon) {
    return stabilisatorStatusValue;
}

void SetStabilisatorStatusCB(void* inRefcon, int inValue) {
    stabilisatorStatusValue = inValue;
    //visible = inValue
}

float GetGlassDarkness() {
    return glassDarknessValue;
}

float GetGlassDarknessCB(void* inRefcon) {
    return glassDarknessValue;
}

void SetGlassDarknessCB(void* inRefcon, float inValue) {
    glassDarknessValue = inValue;
    //visible = inValue
}
GLuint getFBO() {
    return XPLMGetDatai(drFBO);
}
