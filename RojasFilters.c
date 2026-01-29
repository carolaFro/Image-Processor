/**
* This is the implementation of a threaded program that applies
* two different filters, blur box and swisscheese, to an image.
*
* Completion time: 18h
*
* @author Carolina Rojas Matos
* @version 11/10/2023
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <math.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 	14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 	4096
#define THREAD_COUNT 		4
//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
struct BMP_Header bmpHeader;
struct DIB_Header dibHeader;

typedef struct ThreadData
{
	int startRow;
	int endRow;
	int width;
	int height;
	struct Pixel **pArr;
} ThreadData;
//TODO: finish me


////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

int check_file(char *file_name)
{
	char *dot = strrchr(file_name, '.');
	
	if (dot != NULL)
	{
		// Extract the file extension (excluding the period)
		char *fileExtension = dot + 1;

		// Compare the file extension to the expected extension (e.g., "txt")
		if (strcmp(fileExtension, "bmp") == 0)
		{
			printf("Valid InputFile\n");
			return 0;
		}
		else
		{
			printf("File extension not supported: %s\n", fileExtension);
		}
	}
	else
	{
		printf("File has no extension.\n");
	}

	return 1;
}

// Helper function to handler pixel array
void set_pArr2(struct Pixel **pArr, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		pArr[i] = (struct Pixel *)malloc(width * sizeof(struct Pixel));

		// test for allocation errors to avoid mem corruption
		if (pArr[i] == NULL)
		{
			// Memory allocation failed, free previously allocated memory and return NULL
			for (int j = 0; j < i; j++)
			{
				free(pArr[j]);
			}
			free(pArr);
			return;
		}
	}
}

void freePixelArray(struct Pixel **pArr, int height)
{
	if (pArr == NULL || *pArr == NULL)
	{
		return;
	}
	// Free the pixel array
	for (int i = 0; i < height; i++)
	{
		free(pArr[i]);
	}

	free(pArr);
}

void threadingFuction(struct Pixel **pArr, int height, int width, void *(*filterFunc)(void *))
{
	pthread_t threads[THREAD_COUNT];
	ThreadData threadData[THREAD_COUNT];

	// Initialize and create threads
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threadData[i].startRow = (height / THREAD_COUNT) * i;
		threadData[i].endRow = (i == THREAD_COUNT - 1) ? height : (height / THREAD_COUNT) * (i + 1);
		threadData[i].width = width;
		threadData[i].height = height;
		threadData[i].pArr = pArr;

		pthread_create(&threads[i], NULL, filterFunc, &threadData[i]);
	}

	// Wait for threads to finish
	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		pthread_join(threads[i], NULL);
	}
}

void *boxBlurFilter(void *args)
{
	ThreadData *data = (ThreadData *)args;
	int startRow = data->startRow;
	int endRow = data->endRow;
	int width = data->width;
	int height = data->height;
	struct Pixel **pArr = data->pArr;
	struct Pixel **newArr = (struct Pixel **)malloc(height * sizeof(struct Pixel *));

	// Initialize newArr
	for (int i = 0; i < height; i++)
	{
		newArr[i] = (struct Pixel *)malloc(width * sizeof(struct Pixel));
	}

	// Apply the box blur to each pixel in the segment
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int sumR = 0, sumG = 0, sumB = 0;
			int count = 0;
			for (int di = -1; di <= 1; di++)
			{
				for (int dj = -1; dj <= 1; dj++)
				{
					int ni = i + di;
					int nj = j + dj;
					if (ni >= 0 && ni < height && nj >= 0 && nj < width)
					{
						sumR += pArr[ni][nj].red;
						sumG += pArr[ni][nj].green;
						sumB += pArr[ni][nj].blue;
						count++;
					}
				}
			}
			newArr[i][j].red = sumR / count;
			newArr[i][j].green = sumG / count;
			newArr[i][j].blue = sumB / count;
		}
	}

	// Copy the blurred segment back into the original pixel array
	for (int i = startRow; i < endRow; i++)
	{
		memcpy(pArr[i], newArr[i], width * sizeof(struct Pixel));
		free(newArr[i]); // Free the row after copying
	}
	free(newArr); // Free the temporary array

	return 0;
}

void createHoles(struct Pixel **pArr, int numHoles, int holeRadius, int width, int height)
{
	for (int n = 0; n < numHoles; n++)
	{
		int centerX = rand() % width;
		int centerY = rand() % height;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int dx = j - centerX;
				int dy = i - centerY;
				if (dx * dx + dy * dy <= holeRadius * holeRadius)
				{
					pArr[i][j].red = 0;
					pArr[i][j].green = 0;
					pArr[i][j].blue = 0;
				}
			}
		}
	}
}

void *swissCheeseFilter(void *args)
{
	ThreadData *data = (ThreadData *)args;
	int width = data->width;
	int height = data->height;
	struct Pixel **pArr = data->pArr;

	int smallerSide = (width < height) ? width : height;
	int numHoles = (int)(smallerSide * 0.08); // 8% of the smaller side

	// Define hole size parameters
	int smallHoleRadius = width / 30;	
	int averageHoleRadius = width / 15;
	int largeHoleRadius = width / 8;	

	// The number of each type of hole
	int numSmallHoles = numHoles / 5;	
	int numAverageHoles = numHoles / 10; 
	int numLargeHoles = numHoles / 12;	

	// Apply yellow tint across the entire image
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pArr[i][j].red = (pArr[i][j].red + 15 > 255) ? 255 : pArr[i][j].red + 15;
			pArr[i][j].green = (pArr[i][j].green + 15 > 255) ? 255 : pArr[i][j].green + 15;
		}
	}

	createHoles(pArr, numSmallHoles, smallHoleRadius, width, height);
	createHoles(pArr, numAverageHoles, averageHoleRadius, width, height);
	createHoles(pArr, numLargeHoles, largeHoleRadius, width, height);
	return 0;
}

int main(int argc, char* argv[]) 
{
	char *in;
	char *out;
	char *filter;
	int opt;

	if (argc < 2)
	{
		// Print an error message and exit if the number of arguments is incorrect
		fprintf(stderr, "Usage: %s [-i inputFile] [-o outputFile] [-f filter]\n",
				argv[0]);
		exit(1); // Return an error code
	}
	char *check = argv[2];
	// Open the file for reading and check it is the correct file
	if (check_file(check) == 1)
	{
		fprintf(stderr, "Usage: %s [-i inputFile] [-o outputFile] [-f filter]\n",
				argv[0]);
		exit(1); // Return an error code
	}

	while ((opt = getopt(argc, argv, "i:o:f:")) != -1)
	{
		switch (opt)
		{
		case 'i':
			in = optarg;
			break;
		case 'o':
			out = optarg;
			break;
		case 'f':
			filter = optarg;
			break;
		default:
			fprintf(stderr, "-%s Invalid\n", optarg);
		}
	}

	printf("\nfiles %s %s\n", in, out);
	FILE *inputFile = fopen(in, "rb");

	if (inputFile == NULL)
	{
		// Failed to open the file, handle the error as needed
		perror("Error opening the input file");
		exit(1);
	}

	readBMPHeader(inputFile, &bmpHeader);
	readDIBHeader(inputFile, &dibHeader);

	struct Pixel **pArr = NULL;
	pArr = (struct Pixel **)malloc(dibHeader.height * sizeof(struct Pixel *));
	if (pArr != NULL)
	{
		set_pArr2(pArr, dibHeader.width, dibHeader.height); // this function should go to image.h
																	//  but we cant change the headerfiles
	}

	// extract pixelarr info

	readPixelsBMP(inputFile, pArr, dibHeader.width, dibHeader.height);

	if (strcmp(filter, "b") == 0)
	{
		threadingFuction(pArr, dibHeader.width, dibHeader.height, boxBlurFilter);
	}
	else if (strcmp(filter, "c") == 0)
	{
		threadingFuction(pArr, dibHeader.width, dibHeader.height, swissCheeseFilter);
	}
	//
	// Create image struct

	FILE *outputFile = fopen(out, "wb");
	// populate new file
	writeBMPHeader(outputFile, &bmpHeader);
	writeDIBHeader(outputFile, &dibHeader);
	writePixelsBMP(outputFile, pArr, dibHeader.width, dibHeader.height);

	freePixelArray(pArr, dibHeader.height);

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}