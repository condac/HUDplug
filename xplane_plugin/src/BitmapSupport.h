/// Cross Platform Bitmap functions and bitmap data structures - Sandy Barbour 2003

#ifndef BitmapSupportH
#define BitmapSupportH
#include <stdint.h>
/// These need to be aligned
#pragma pack(push, ident, 2)

typedef struct tagBMPFILEHEADER {
    short bfType;
    uint32_t bfSize;
    short bfReserved1;
    short bfReserved2;
    uint32_t bfOffBits;
} BMPFILEHEADER;

typedef struct tagBMPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    short biPlanes;
    short biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPINFOHEADER;

typedef struct tagIMAGEDATA {
    unsigned char* pData;
    uint32_t Width;
    uint32_t Height;
    uint32_t Padding;
    short Channels;
} IMAGEDATA;

#pragma pack(pop, ident)

int BitmapLoader(const char* FilePath, IMAGEDATA* ImageData, int pChannels);

#if APL
void SwapEndian(short* Data);

void SwapEndian(uint32_t* Data);
void SwapEndian(int32_t* Data);
void SwapRedBlue(IMAGEDATA* ImageData);
#endif

#endif
