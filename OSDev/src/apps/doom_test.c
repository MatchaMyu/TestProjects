#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell/console.h"
#include "isr.h"
#include "doomgeneric.h"

#ifndef DOOMGENERIC_RESX
#define DOOMGENERIC_RESX 640
#endif  // DOOMGENERIC_RESX

#ifndef DOOMGENERIC_RESY
#define DOOMGENERIC_RESY 400
#endif  // DOOMGENERIC_RESY

void doom_test(void)
{
    console_write("Calling DoomGeneric...\n");

    doomgeneric_Create(0, NULL);

    console_write("DoomGeneric returned.\n");
}
