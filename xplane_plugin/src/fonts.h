#ifndef _fonts_h_
#define _fonts_h_

#include "XPLMGraphics.h"

typedef struct HUDFontProperties {
    char* fileName;
    char firstChar;
    XPLMTextureID texId;
    int dispListBase;
    int rowsCount;
    int colsCount;
    int charWidth;
    int charHeight;
    int spacing;
    int texSize;
} HUDFontProperties;

extern HUDFontProperties fontBig;

// HUDFontProperties fontSmall = {
//                                 "arial.bmp",  //fileName
//                                 ' ', // firstChar
//                                 0, //texId
//                                 0, //dispListBase
//                                 6, //rowsCount
//                                 16, //colsCount
//                                 16, //charWidth
//                                 19, //charHeight
//                                 -2, //spacing
//                                 256 //texSize
//                               };
extern HUDFontProperties fontSmall;
extern HUDFontProperties fontMain;

void LoadFontTextures(void);
void CreateHUDFont(HUDFontProperties* f);
void DrawHUDText(const char* pValue, HUDFontProperties* f, float pX, float pY, int pAllign, float* color);
void DrawHUDText2(const char* pValue, HUDFontProperties* f, int pX, int pY, char pAllign, float* color);
void DrawHUDNumber(int pValue, HUDFontProperties* f, int pDigits, int pX, int pY, char pAllign, float* color);
void DrawTexts(void);
void InitFonts(void);
int LoadHUDFontTexture(HUDFontProperties* f);
HUDFontProperties* GetBigFont(void);
HUDFontProperties* GetSmallFont(void);
int getTextWidth(HUDFontProperties* f, int numberOfChars);
void SetGLText(void);
void SetGLTransparentLines(void);

#endif