
#ifndef DATAREF_H_INCLUDED
#define DATAREF_H_INCLUDED

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMPlugin.h"
#include "XPLMPlanes.h"

int findDataRef(const char* name, XPLMDataRef* result);

GLuint getFBO();

int registerDataRefs();

int initDataRefs();
void unregisterData();

float getFRP();

float getAlphaA();
float getBetaA();
float getFOV();

float getFOV_x();
float getFOVoff_y();
float getFOVoff_x();
extern float dr_pitch;
float getPitch();
float getPitchTrim();
float getRoll();
float getHeading();
float getTrueHeading();
float getTrueSpeed();
extern float dr_vx;
float getVX();
extern float dr_vy;
float getVY();
extern float dr_vz;
float getVZ();
extern float dr_theta;
float getTheta();
extern float dr_psi;
float getPsi();
extern float dr_phi;
float getPhi();
float getVV();
float getRadarAltitude();
extern float dr_altitude;
float getAltitude();
float getWindSpeed();
float getWindDirection();
float getIAS();
float getBalance();
float getYawStringAngle();

float getGroundSpeed();
float getMachSpeed();
float getGForce();
float getGForceX();
float getGForceZ();
float getTotalWeight();
float getThrottlePos();

float getILSv();
float getILSh();
int ifILSEnabled();

float getNAV1Heading();
float getNAV1Distance();
float getNAV1ETA();

float getPrickX();
float getPrickY();
float getPrickActive();
float getNAVxHeading();
int getNAV1Id(const char* text);
float getBanaX();
float getBanaY();
float getBanaDist();

int getSpeedBrake();
int getParkBrake();
int getStabStatus();
extern int dr_gear;
int getGear();
int getPause();
int getMkvLarm();
int getMkvNeedMore();
float getMkvGneed();

float getFuelFlow();
float getTotalFuel();
int isFuelTank(int index);
float getFuelInTank(int index);
int markKontakt();

void setStabStatus(int value);

//int getCurrentView();

int getViewIsExternal();

int GetHudVisibleCB(void* inRefcon);

void SetHudVisibleCB(void* inRefcon, int inValue);
int GetStabilisatorStatusCB(void* inRefcon);

void SetStabilisatorStatusCB(void* inRefcon, int inValue);

float GetGlassDarkness();
float GetGlassDarknessCB(void* inRefcon);

void SetGlassDarknessCB(void* inRefcon, float inValue);

float GetJASFuelCB(void* inRefcon);
void SetJASFuelCB(void* inRefcon, float inValue);

int GetJASLampsAirbrakeCB(void* inRefcon);
void SetJASLampsAirbrakeCB(void* inRefcon, int inValue);

int GetJASLampsSpakCB(void* inRefcon);
void SetJASLampsSpakCB(void* inRefcon, int inValue);

int GetJASLampsAttCB(void* inRefcon);
void SetJASLampsAttCB(void* inRefcon, int inValue);

int GetJASLampsHojdCB(void* inRefcon);
void SetJASLampsHojdCB(void* inRefcon, int inValue);

int GetJASLampsMaster1CB(void* inRefcon);
void SetJASLampsMaster1CB(void* inRefcon, int inValue);

int GetJASLampsMaster2CB(void* inRefcon);
void SetJASLampsMaster2CB(void* inRefcon, int inValue);

void setWarning(int inValue);

int GetJASButtonSpakCB(void* inRefcon);
void SetJASButtonSpakCB(void* inRefcon, int inValue);

int GetJASButtonAttCB(void* inRefcon);
void SetJASButtonAttCB(void* inRefcon, int inValue);

int GetJASButtonHojdCB(void* inRefcon);
void SetJASButtonHojdCB(void* inRefcon, int inValue);

int GetJASAutoModeCB(void* inRefcon);
void SetJASAutoModeCB(void* inRefcon, int inValue);

float GetJASAutoAttCB(void* inRefcon);
void SetJASAutoAttCB(void* inRefcon, float inValue);

float GetJASAutoAltCB(void* inRefcon);
void SetJASAutoAltCB(void* inRefcon, float inValue);

float getProjectionMatrix(int index);
float getProjectionMatrix3d(int index);
float getModelMatrix(int index);

int getViewType();
int getWindow();

int getWindowWidth();

float getPanelL();
float getPanelR();
float getPanelT();
float getPanelB();

extern float dr_truetheta;
extern float dr_truepsi;
extern float dr_truephi;
 float getTruePsi();
 float getTruePhi();
 float getTrueTheta();
extern float dr_vectorAlpha;
extern float dr_vectorBeta;

#endif
