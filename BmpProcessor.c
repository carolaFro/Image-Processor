/**
 * This files contains the definitinitions of the functions
 * used to handle BMP files.
 *
 * Completion time: 2h
 *
 * @author Carolina Rojas
 * @version 1.0
 */

/////////////////// INCLUDES ////////////////////
#include "BmpProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////// MACROS ////////////////////
#define HEADER_SIGNATURE_B 0x42
#define HEADER_SIGNATURE_M 0x4d
#define BMP_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40
#define RESERVED1 0x0
#define RESERVED2 0x0
#define FILE_OFFSET BMP_HEADER_SIZE + DIB_HEADER_SIZE
#define PLANES 1
#define BITS_PER_PIXELS 24 // 3 bytes, 8 bits per color
#define BYTES_PER_PIXELS BITS_PER_PIXELS / 8
////////////////////////////////////////////////

void readBMPHeader(FILE *file, struct BMP_Header *header)
{
    fread(&header->signature, sizeof(char) * 2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

void writeBMPHeader(FILE *file, struct BMP_Header *header)
{
    fwrite(&header->signature, sizeof(char) * 2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

void readDIBHeader(FILE *file, struct DIB_Header *header)
{
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bitsPerPixel, sizeof(short), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->imageSize, sizeof(int), 1, file);
    fread(&header->horizRes, sizeof(int), 1, file);
    fread(&header->vertRes, sizeof(int), 1, file);
    fread(&header->colorNum, sizeof(int), 1, file);
    fread(&header->importantColorNum, sizeof(int), 1, file);
}

void writeDIBHeader(FILE *file, struct DIB_Header *header)
{
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bitsPerPixel, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->imageSize, sizeof(int), 1, file);
    fwrite(&header->horizRes, sizeof(int), 1, file);
    fwrite(&header->vertRes, sizeof(int), 1, file);
    fwrite(&header->colorNum, sizeof(int), 1, file);
    fwrite(&header->importantColorNum, sizeof(int), 1, file);
}

void makeBMPHeader(struct BMP_Header *header, int width, int height)
{
    header->signature[0] = HEADER_SIGNATURE_B;
    header->signature[1] = HEADER_SIGNATURE_M;
    header->size = (BMP_HEADER_SIZE + DIB_HEADER_SIZE + (((width % 4) + width) * height) * BYTES_PER_PIXELS);
    header->reserved1 = RESERVED1;
    header->reserved2 = RESERVED2;
    header->offset_pixel_array = FILE_OFFSET;
}

void makeDIBHeader(struct DIB_Header *header, int width, int height)
{
    header->size = DIB_HEADER_SIZE;
    header->width = width;
    header->height = height;
    header->planes = PLANES;
    header->bitsPerPixel = BITS_PER_PIXELS;
    header->compression = 0;
    header->imageSize = ((width % 4) + width) * height *
                       (BYTES_PER_PIXELS);
    header->horizRes = 0;
    header->vertRes = 0;
    header->colorNum = 0;
    header->importantColorNum = 0;
}

void readPixelsBMP(FILE *file, struct Pixel **pArr, int width, int height)
{
    // Calculate the padding to ensure each row of pixels is a multiple of 4 bytes
    int padding = (4 - ((width * BYTES_PER_PIXELS) % 4)) % 4;

    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            fread(&pArr[i][j], sizeof(struct Pixel), 1, file);
        }
        // Read and discard padding bytes
        fseek(file, padding, SEEK_CUR);
    }
}

void writePixelsBMP(FILE *file, struct Pixel **pArr, int width, int height)
{
    int padding = (4 - ((width * BYTES_PER_PIXELS) % 4)) % 4;

    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            fwrite(&pArr[i][j], sizeof(struct Pixel), 1, file);
        }
        // Write padding bytes
        for (int p = 0; p < padding; p++)
        {
            fputc(0, file);
        }
    }
}