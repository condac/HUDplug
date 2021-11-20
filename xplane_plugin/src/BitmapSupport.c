#include "HUDplug.h"
/// Cross Platform Bitmap functions - Sandy Barbour 2003
#if IBM
#    define _CRT_SECURE_NO_WARNINGS 1
#    include <windows.h>
#endif
#include "BitmapSupport.h"
#include "datarefs.h"
#include <stdio.h>
#include <stdlib.h>

/// Functions to handle endian differeneces between windows, linux and mac.
#if APL
short Endian(short Data) {
    unsigned char* pBuffer = (unsigned char*)&Data;
    short Result = (short)(pBuffer[0] & 0xff) + ((short)(pBuffer[1] & 0xff) << 8);
    return (Result);
}

uint32_t Endian(uint32_t Data) {
    unsigned char* pBuffer = (unsigned char*)&Data;

    uint32_t Result = (uint32_t)(pBuffer[0] & 0xff) + ((uint32_t)(pBuffer[1] & 0xff) << 8) + ((uint32_t)(pBuffer[2] & 0xff) << 16) + ((uint32_t)(pBuffer[3] & 0xff) << 24);

    return (Result);
}

void SwapEndian(short* Data) {
    *Data = Endian(*Data);
}

void SwapEndian(uint32_t* Data) {
    *Data = Endian(*Data);
}
#else
/// Only the mac needs these so dummy functions for windows and linux.

#endif

/// Swap the red and blue pixels.
void SwapRedBlue(IMAGEDATA* ImageData) {
    unsigned char* srcPixel;
    //uint32_t count;
    uint32_t x, y;
    unsigned char sTemp;

    /// Does not support 4 channels.
    if (ImageData->Channels == 4)
        return;

    /// Do the swap
    srcPixel = ImageData->pData;
    //count = ImageData->Width * ImageData->Height;
    for (y = 0; y < ImageData->Height; ++y)
        for (x = 0; x < ImageData->Width; ++x) {
            sTemp = srcPixel[0];
            srcPixel[0] = srcPixel[2];
            srcPixel[2] = sTemp;

            srcPixel += 3;
            if (x == (ImageData->Width - 1))
                srcPixel += ImageData->Padding;
        }
}

/// Generic bitmap loader to handle all platforms
int BitmapLoader(const char* FilePath, IMAGEDATA* ImageData, int pChannels) {
    BMPFILEHEADER Header;
    BMPINFOHEADER ImageInfo;
    uint32_t Padding;
    FILE* BitmapFile = NULL;
    int RetCode = 0;
    int lTexDataSize;
    unsigned char* lLineData;
    unsigned char* lPtr;
    int lBytesWritten = 0;

    ImageData->pData = NULL;

    BitmapFile = fopen(FilePath, "rb");
    if (BitmapFile != NULL) {
        //debugLog("true fopen \n");

        if (fread(&Header, sizeof(Header), 1, BitmapFile) == 1) {
            //debugLog("true fread1 \n");
            if (fread(&ImageInfo, sizeof(ImageInfo), 1, BitmapFile) == 1) {
                //debugLog("true fread2 \n");
                //debugLog("Header.bfType %c  %c %d %d %d \n", (Header.bfType & 0xff), (Header.bfType >> 8), ImageInfo.biBitCount, ImageInfo.biWidth, ImageInfo.biHeight);
/// Handle Header endian.
#if APL
                SwapEndian(&Header.bfSize);
                SwapEndian(&Header.bfOffBits);

                /// Handle ImageInfo endian.
                SwapEndian(&ImageInfo.biWidth);
                SwapEndian(&ImageInfo.biHeight);
                SwapEndian(&ImageInfo.biBitCount);
#endif
                //debugLog("Header.bfType %c  %c %d %d %d \n", (Header.bfType & 0xff), (Header.bfType >> 8), ImageInfo.biBitCount, ImageInfo.biWidth, ImageInfo.biHeight);
                /// Make sure that it is a bitmap.
#if APL && defined(__POWERPC__)

                if (((Header.bfType & 0xff) == 'M') && (((Header.bfType >> 8) & 0xff) == 'B') &&
#else
                if (((Header.bfType & 0xff) == 'B') && (((Header.bfType >> 8) & 0xff) == 'M') &&
#endif

                    (ImageInfo.biBitCount == 24) && (ImageInfo.biWidth > 0) && (ImageInfo.biHeight > 0)) {
                    //debugLog("true header1 \n");
                    /// Sandy Barbour - I have found that "Header.bfSize" does not always
                    /// agree with the actual file size and can sometimes be
                    /// "ImageInfo.biSize"  smaller. So add it in for good measure
                    if ((Header.bfSize + ImageInfo.biSize - Header.bfOffBits) >= (ImageInfo.biWidth * ImageInfo.biHeight * 3)) {
                        //debugLog("true header2 \n");
                        Padding = (ImageInfo.biWidth * 3 + 3) & ~3;
                        Padding -= ImageInfo.biWidth * 3;
                        

                        ImageData->Width = ImageInfo.biWidth;
                        ImageData->Height = ImageInfo.biHeight;
                        ImageData->Padding = Padding;
                        
                        debugLog("Padding %d biwidth %d offbits %d \n", Padding, ImageInfo.biWidth, Header.bfOffBits);

                        /// Allocate memory for the actual image.
                        if (pChannels < 1 || pChannels > 4)
                            pChannels = 3;
                        ImageData->Channels = pChannels;
                        lTexDataSize = ImageInfo.biWidth * ImageInfo.biHeight * ImageData->Channels + ImageInfo.biHeight * Padding;
#ifdef DEBUG
                        debugLog("Loading texture from file %s.\n", FilePath);
                        debugLog("Width %d, height %d, using %d channels.\n", ImageInfo.biWidth, ImageInfo.biHeight, ImageData->Channels);
                        debugLog("Allocating texture size: %d\n", lTexDataSize);
#endif
                        ImageData->pData = (unsigned char*)malloc(lTexDataSize);
                        lLineData = (unsigned char*)malloc(ImageInfo.biWidth * 3 + Padding);
                        if (ImageData->pData != NULL && lLineData != NULL) {
                            //debugLog("true ImageData \n");
                            /// Get the actual image - line by line
                            
                            // Close file and open and move to acual start position of pixelarray as defined by header data Header.bfOffBits
                            debugLog("tell innan %d \n", ftell(BitmapFile));
                            fclose(BitmapFile);
                            BitmapFile = fopen(FilePath, "rb");
                            char junk[1000];
                            fread(junk, Header.bfOffBits, 1, BitmapFile);
                            debugLog("tell efter %d \n", ftell(BitmapFile));
                            lPtr = ImageData->pData;
                            RetCode = 1;
                            for (int i = 0; i < ImageInfo.biHeight; i++) {
                                //debugLog("for biHeight \n");
                                // read each line and convert 3 BMP channels to pChanells
                                if (fread(lLineData, ImageInfo.biWidth * 3 + Padding, 1, BitmapFile) != 1) {
                                    //debugLog("if fread  lLineData \n");
                                    RetCode = 0;
                                    break;
                                }
                                for (int j = 0; j < ImageInfo.biWidth; j++) {
                                    unsigned char pix = (lLineData[j * 3]) ? 255 : 0;
                                    switch (pChannels) {
                                    case 4:
                                        lPtr[0] = pix;
                                        lPtr[1] = pix;
                                        lPtr[2] = pix;
                                        lPtr[3] = pix;
                                        break;
                                    case 3:
                                        lPtr[0] = 255; // lLineData[j*3];
                                        lPtr[1] = 0;   // lLineData[j*3+1];
                                        lPtr[2] = 255; // lLineData[j*3+2];
                                        break;
                                    case 1:
                                        lPtr[0] = lLineData[j * 3];
                                        break;
                                    }
                                    lPtr += pChannels;
                                    lBytesWritten += pChannels;
                                }
                            }
                        }
                        if (lLineData != NULL)
                            free(lLineData);
                    }
                }
            } else {
                debugLog("else fread2 \n");
            }
        } else {
            debugLog("else fread1 \n");
        }
    } else {
        debugLog("else bitmaploader \n");
    }
    if (BitmapFile != NULL)
        fclose(BitmapFile);
#ifdef DEBUG
    debugLog("Written %d bytes.\n", lBytesWritten);
#endif
    return RetCode;
}
