
#include "project_constants.h"

#include "usb.h"
#include "API.h"

//project file includes:
#include "helper_functions.h"

//API file includes:
//system includes:
#include <stdio.h>      // standard input / output functions
#include <string>       // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "BMPParser.h"
#include "pattern.h"
#include "splash.h"
#include "CImg.h"
#include "splash.h"
#include "compress.h"
#include "error.h"

int toSplash(Image_t *img, unsigned char **data);

struct PatternElement
{
	int bits;
	int color;
	int  exposure;
	int  darkPeriod;
	bool trigIn;
	bool trigOut2;
	char name[200]; //increase 200 if strings become super long (should not happen!)
	int splashImageIndex;
	int splashImageBitPos;
};

void addPattern()
{
	//Temp
	char imagePath[] = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\G01b.bmp";

	//Temp
	int numPatAdded = 0;

	PatternElement pattern;
	pattern.bits = 8;
	pattern.color = 1;
	pattern.exposure = 100000;
	pattern.darkPeriod = 0;
	pattern.trigIn = false;
	pattern.trigOut2 = true;

	numPatAdded++;

	LCR_ClearPatLut();

	int totalSplashImages = 0;
	int ret;
	char errStr[255];
	int imgCount = 0;
	int bits = 0;
	int totalBits = 0;
	int elemCount = 0;

	if (pattern.bits > 8)
		printf("Error:Bit depth not selected for pattern\n");

	totalBits = totalBits + pattern.bits;

	if (totalBits > 400)
		printf("Error:Total Bit Depth cannot exceed 400\n");

	pattern.splashImageIndex = 0; // imgCount;
	pattern.splashImageBitPos = 0; // bits;		
	totalSplashImages = imgCount + 1;

	if (LCR_AddToPatLut(0, pattern.exposure, true, pattern.bits, pattern.color, pattern.trigIn, pattern.darkPeriod, pattern.trigOut2, pattern.splashImageIndex, pattern.splashImageBitPos)<0)
		printf("Unable to add pattern number to the LUT\n");

	if (LCR_SendPatLut() < 0)
		printf("Sending pattern LUT failed!\n");

	ret = LCR_SetPatternConfig(1, 0);
	if (ret < 0)
		printf("Sending pattern LUT size failed!\n");

	int m_ptnWidth = 1920;
	int m_ptnHeight = 1080;
	int error = 0;
	BOOL firmware = false;

	for (int image = 0; image < totalSplashImages; image++)
	{
		int spalshImageCount;
		if (firmware)
			spalshImageCount = image;
		else
			spalshImageCount = totalSplashImages - 1 - image;

		int bitDepth = 8; //8 -third of screen
		Image_t *imgPrtDest = PTN_Alloc(m_ptnWidth, m_ptnHeight, 24);

		PTN_Fill(imgPrtDest, 0); // fill the image with zero values

		Image_t *imgPrtSrc = PTN_Alloc(m_ptnWidth, m_ptnHeight, 24);

		error = BMP_LoadFromFile(imagePath, imgPrtSrc);
		if (error != 0)
			printf("error in BMP_LoadFromFile\n");



		PTN_Merge(imgPrtDest, imgPrtSrc, 0, bitDepth);
		PTN_SwapColors(imgPrtDest, PTN_COLOR_RED, PTN_COLOR_BLUE, PTN_COLOR_GREEN);



		uint08* splash_block = NULL;
		spalshImageCount = spalshImageCount;
		int splashSize = toSplash(imgPrtDest, &splash_block);
		if (splashSize <= 0) {
			printf("Error in splashSize stuff (toSplash2)");
		}
		else {
			printf("splashSize = %d\n", splashSize);
		}



		bool master = true;
		int splash_size = splashSize;
		int origSize = splash_size;
		int splashImageCount = spalshImageCount;

		LCR_InitPatternMemLoad(master, splashImageCount, splash_size);
		int imgDataDwld = 0;

		while (splash_size > 0)
		{
			int dnldSize = LCR_pattenMemLoad(master, splash_block + (origSize - splash_size), splash_size);
			if (dnldSize < 0)
			{
				printf("Downloading to device failed\n");
			}

			splash_size -= dnldSize;

			if (splash_size < 0)
				splash_size = 0;

			imgDataDwld += dnldSize;
		}
	}
}

int toSplash(Image_t *img, unsigned char **data)
{
	uint08 *splashData = NULL;
	if (splashData == NULL)
	{
		splashData = SPL_AllocSplash(img->Width, img->Height);
		if (splashData == NULL)
			return ERR_OUT_OF_RESOURCE;
	}

	*data = splashData;

	return SPL_ConvImageToSplash(img, SPL_COMP_AUTO, splashData);
}







/******************************************************************************************************************************
***********************************************     UNUSED METHODS/CODE BELOW     ********************************************


printf("Image data loaded:\n");
printf("Buffer:%d\n", imgPrtDest->Buffer);               /**< Pointer to the image buffer
printf("Width:%d\n", imgPrtDest->Width);                 /**< Width of the image in pixels
printf("Height:%d\n", imgPrtDest->Height);               /**< Height of the image in pixels
printf("LineWidth:%d\n", imgPrtDest->LineWidth);         /**< Number of bytes in a line
printf("FullHeight:%d\n", imgPrtDest->FullHeight);       /**< Total height of the original image
printf("BitDepth:%d\n", imgPrtDest->BitDepth);


error = BMP_SaveToFile(imgPrtDest, "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\imgPrtMERGED.bmp");
if (error != 0)
printf("Cant save image\n");
error = BMP_SaveToFile(imgPrtSrc, "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\imgPrtSrcMERGED.bmp");
if (error != 0)
printf("Cant save image\n");

error = BMP_SaveToFile(imgPrtSrc, "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\imgPrtSrcLOADED.bmp");
if (error != 0)
printf("Cant save image\n");
error = BMP_SaveToFile(imgPrtDest, "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\imgPrtDestLOADED.bmp");
if (error != 0)
printf("Cant save image\n");
******************************************************************************************************************************/