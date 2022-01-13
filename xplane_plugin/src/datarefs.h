
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

float getAlphaA();
float getBetaA();
float getFOV();
float getFOVoff_y();
float getFOVoff_x();
float getPitch();
float getPitchTrim();
float getRoll();
float getHeading();
float getTrueHeading();
float getVX();
float getVY();
float getVZ();
float getVV();
float getRadarAltitude();
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
int getNAV1Id(const char* text);

int getSpeedBrake();
int getParkBrake();
int getStabStatus();
int getGear();
int getPause();
int getMkvLarm();

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

#endif
