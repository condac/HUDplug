
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

// modifications to use standalone from glut by Bengt

//#include "glutint.h"
#include "HUDplug.h"
#include "glutstroke.h"
#include "glut_mroman.cc"

float scalefactor = 0.2;

void glutStrokeCharacter2(int c) {
    const StrokeCharRec* ch;
    const StrokeRec* stroke;
    const CoordRec* coord;
    StrokeFontPtr fontinfo = (StrokeFontPtr)&glutStrokeMonoRoman;
    int i, j;

    // #if defined(_WIN32)
    //     fontinfo = (StrokeFontPtr)__glutFont(font);
    // #else
    //     fontinfo = (StrokeFontPtr)font;
    // #endif

    if (c < 0 || c >= fontinfo->num_chars)
        return;
    ch = &(fontinfo->ch[c]);
    if (ch) {
        for (i = ch->num_strokes, stroke = ch->stroke; i > 0; i--, stroke++) {
            glLineWidth(line_text_scale);
            glBegin(GL_LINE_STRIP);
            for (j = stroke->num_coords, coord = stroke->coord; j > 0; j--, coord++) {
                glVertex2f(coord->x, coord->y);
            }
            glEnd();
        }
        glTranslatef(ch->right, 0.0, 0.0);
    }
}

float glutStrokeWidth(int c) {
    StrokeFontPtr fontinfo = (StrokeFontPtr)&glutStrokeMonoRoman;
    const StrokeCharRec* ch;

    // #if defined(_WIN32)
    //     fontinfo = (StrokeFontPtr)__glutFont(font);
    // #else
    //     fontinfo = (StrokeFontPtr)font;
    // #endif

    if (c < 0 || c >= fontinfo->num_chars)
        return 0;
    ch = &(fontinfo->ch[c]);
    if (ch)
        return ch->right;
    else
        return 0;
}
float glutStrokeLength(const char* string) {
    int c;
    float length;
    StrokeFontPtr fontinfo = (StrokeFontPtr)&glutStrokeMonoRoman;
    const StrokeCharRec* ch;
    int i = 0;
    //
    // #if defined(_WIN32)
    //     fontinfo = (StrokeFontPtr)__glutFont(font);
    // #else
    //     fontinfo = (StrokeFontPtr)font;
    // #endif

    length = 0;
    //for (; *string != '\0'; string++) {
    while (string[i] != '\0') {
        c = string[i];
        if (c >= 0 && c < fontinfo->num_chars) {
            ch = &(fontinfo->ch[c]);
            if (ch) {
                length += ch->right;
            }
        }
        i++;
    }
    return length;
}

float textHeight(float size) {
    return 1.0 * glutStrokeWidth('W') * size * text_scale * scalefactor;
}

void drawLineText(const char* text, float x, float y, float size, int align) {
    int width = glutStrokeWidth('W');
    float length = glutStrokeLength(text);
    float halign = 0.0;
    float valign = 0.0;
    int i = 0;

    if (align == 1) {
        halign = -length / 2;
    }
    if (align == 2) {
        halign = -length;
    }

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(scalefactor * size * text_scale, scalefactor * size * text_scale, 0.0);
    glTranslatef(halign, valign, 0.0);
    while (text[i] != '\0') {
        glutStrokeCharacter2(text[i]);
        i++;
    }
    glPopMatrix();
}