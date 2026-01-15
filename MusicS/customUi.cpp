#include <wtypes.h>
#include "global.h"

RECT g_waveRect = {
    WAVE_X,
    WAVE_Y,
    WAVE_X + WAVE_WIDTH,
    WAVE_Y + WAVE_HEIGHT
};

RECT g_barRect = {
    SPEC_X,
    SPEC_Y,
    SPEC_X + SPEC_WIDTH,
    SPEC_Y + SPEC_HEIGHT
};