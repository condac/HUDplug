#define _GNU_SOURCE
#include <stdio.h>
#include "HUDplug.h"
#include "config.h"

//master_struct masters[MAXMASTERS];

char* filename = "Resources/plugins/HUDplug/config.txt";

void readConfig() {
   //FILE* configFile;
    
    int  nrOfLines = 0;
    XPLMDebugString("HUDplug.readConfig: getNrOfLines\n");
    nrOfLines = getNrOfLines(filename);
    char test[100];
    sprintf(test, "%d", nrOfLines);
    XPLMDebugString(test);
    XPLMDebugString("HUDplug:HUDplug: Done\n");
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
        }
        fclose(configFile);
        return banan;
    }
    return banan;
}

