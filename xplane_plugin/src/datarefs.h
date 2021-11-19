
#ifndef DATAREF_H_INCLUDED
#define DATAREF_H_INCLUDED

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMPlugin.h"
#include "XPLMPlanes.h"

void debugLog(const char* fmt, ...);

int findDataRef(const char* name, XPLMDataRef* result);

int registerDataRefs();

int initDataRefs();
void unregisterData();

float getPitch();
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

//int getCurrentView();

int getViewIsExternal();

int GetHudVisibleCB(void* inRefcon);

void SetHudVisibleCB(void* inRefcon, int inValue);

#endif
