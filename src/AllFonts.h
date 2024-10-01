#ifndef ALL_FONTS_H
#define ALL_FONTS_H

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono24pt7b.h>
#include <Fonts/TESTE.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>

#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSerif18pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

typedef struct {
    const char* name;
    const char* message;
    const GFXfont font;
} FontInfo;

FontInfo fonts[] = {
    {"font1", "Teste Fonte MONO 9px", teste},
    {"font2", "Teste Fonte SANS 9px", FreeSans9pt7b},
    {"font3", "Teste Fonte SANS 9px - BOLD", FreeSansBold9pt7b},
    {"font4", "Teste Fonte Serif 9px", FreeSerif9pt7b}
};


#endif
