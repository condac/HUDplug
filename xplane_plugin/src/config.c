#define _GNU_SOURCE
#include <stdio.h>
#include "HUDplug.h"
#include "config.h"
#include "drawFunctions.h"

//master_struct masters[MAXMASTERS];

char filename[512];

float landing_speed1 = 140;
float landing_speed2 = 180;
float landing_weight1 = 18000;
float landing_weight2 = 48000;

float glass_width = 1000;
float glass_height = 560;
float glass_x;
float glass_y;
float glass_darkness = 0.2;
int draw_glass = 1;
int glass_type = 6;
int glass_type2 = 4;
int th1 = 0;

void readConfig() {
    debugLog("readConfig \n");
    //FILE* configFile;
    char lDirName[512];
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
    sprintf(filename, "%sconfig.txt", lDirName);

    debugLog("read file %s \n", filename);

    int nrOfLines = 0;
    //XPLMDebugString("HUDplug.readConfig: getNrOfLines\n");
    nrOfLines = getNrOfLines(filename);
    char test[100];
    sprintf(test, "%d", nrOfLines);
    XPLMDebugString(test);
    XPLMDebugString("HUDplug:config: Done\n");
}

void parseLine(char* line) {

    if (strncmp("viggen_mode=", line, 12) == 0) {
        //Viggen mode
        int i = 0;
        XPLMDebugString(line + 12);
        sscanf(line + 12, "%d", &i);
        viggen_mode = i;
    }
    if (strncmp("text_scale=", line, 11) == 0) {
        //text_scale
        float i = 0;
        XPLMDebugString(line + 11);
        sscanf(line + 11, "%f", &i);
        text_scale = i;
    }
    if (strncmp("hud_scale=", line, 10) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 10);
        sscanf(line + 10, "%f", &i);
        hud_scale = i;
    }
    if (strncmp("landing_speed1=", line, 15) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 15);
        sscanf(line + 15, "%f", &i);
        landing_speed1 = i;
    }
    if (strncmp("landing_speed2=", line, 15) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 15);
        sscanf(line + 15, "%f", &i);
        landing_speed2 = i;
    }
    if (strncmp("landing_weight1=", line, 16) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 16);
        sscanf(line + 16, "%f", &i);
        landing_weight1 = i;
    }
    if (strncmp("landing_weight2=", line, 16) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 16);
        sscanf(line + 16, "%f", &i);
        landing_weight2 = i;
    }
    if (strncmp("draw_glass=", line, 11) == 0) {
        //hud_scale
        int i = 0;
        XPLMDebugString(line + 11);
        sscanf(line + 11, "%d", &i);
        draw_glass = i;
    }
    if (strncmp("glass_type=", line, 11) == 0) {
        //hud_scale
        int i = 0;
        XPLMDebugString(line + 11);
        sscanf(line + 11, "%d", &i);
        glass_type = i;
    }
    if (strncmp("glass_type2=", line, 12) == 0) {
        //hud_scale
        int i = 0;
        XPLMDebugString(line + 12);
        sscanf(line + 12, "%d", &i);
        glass_type2 = i;
    }
    if (strncmp("th1=", line, 4) == 0) {
        //hud_scale
        int i = 0;
        XPLMDebugString(line + 4);
        sscanf(line + 4, "%d", &i);
        th1 = i;
    }
    if (strncmp("glass_darkness=", line, 15) == 0) {
        //hud_scale
        float i = 0;
        XPLMDebugString(line + 15);
        sscanf(line + 15, "%f", &i);
        glass_darkness = i;
    }
}

#if defined(WINDOWS) || defined(WINDOWS64)
/*
 POSIX getline replacement for non-POSIX systems (like Windows)
 Differences:
     - the function returns int64_t instead of ssize_t
     - does not accept NUL characters in the input file
 Warnings:
     - the function sets EINVAL, ENOMEM, EOVERFLOW in case of errors. The above are not defined by ISO C17,
     but are supported by other C compilers like MSVC
 */
int64_t getline(char** restrict line, size_t* restrict len, FILE* restrict fp) {
    // Check if either line, len or fp are NULL pointers
    if (line == NULL || len == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    // Use a chunk array of 128 bytes as parameter for fgets
    char chunk[128];

    // Allocate a block of memory for *line if it is NULL or smaller than the chunk array
    if (*line == NULL || *len < sizeof(chunk)) {
        *len = sizeof(chunk);
        if ((*line = malloc(*len)) == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }
    // "Empty" the string
    (*line)[0] = '\0';

    while (fgets(chunk, sizeof(chunk), fp) != NULL) {
        // Resize the line buffer if necessary
        size_t len_used = strlen(*line);
        size_t chunk_used = strlen(chunk);

        if (*len - len_used < chunk_used) {
            // Check for overflow
            if (*len > SIZE_MAX / 2) {
                errno = EOVERFLOW;
                return -1;
            } else {
                *len *= 2;
            }

            if ((*line = realloc(*line, *len)) == NULL) {
                errno = ENOMEM;
                return -1;
            }
        }
        // Copy the chunk to the end of the line buffer
        memcpy(*line + len_used, chunk, chunk_used);
        len_used += chunk_used;
        (*line)[len_used] = '\0';

        // Check if *line contains '\n', if yes, return the *line length
        if ((*line)[len_used - 1] == '\n') {
            return len_used;
        }
    }

    return -1;
}
#endif

int getNrOfLines(char* filename) {
    int banan = 0;
    FILE* configFile;
    if ((configFile = fopen(filename, "r")) == NULL) {
        display("Error! opening configfile");
        return 0;
    } else {

        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, configFile)) != -1) {
            banan++;
            parseLine(line);
        }
        fclose(configFile);
        return banan;
    }
    return banan;
}

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
