/**
 * Definition for an image ADT.
 *
 * Completion time: 3h30m
 *
 * @author Carolina Rojas, Ruben Acuna
 * @version 10/20/2023
 */

////////////////////////////////////////////////////////////////////////////////
// Include Files
#include "PixelProcessor.h"
#include <stdlib.h>

//////////////////// MACROS ////////////////////
#define RGB_MAX_VAL 255
////////////////////////////////////////////////

// Helper function that applies the shift and clapms
void shiftAndClamp(unsigned char *channel, int shift)
{
    if (shift <= 0)
    {
        *channel = (unsigned char)(*channel > abs(shift) ? *channel - abs(shift) : 0);
    }
    else
    {
        *channel = (unsigned char)(*channel + shift > RGB_MAX_VAL ? RGB_MAX_VAL : *channel + shift);
    }
}

void colorShiftPixels(struct Pixel **pArr, int width, int height, int rShift, int gShift, int bShift)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            shiftAndClamp(&pArr[i][j].red, rShift);
            shiftAndClamp(&pArr[i][j].green, gShift);
            shiftAndClamp(&pArr[i][j].blue, bShift);
        }
    }
}
