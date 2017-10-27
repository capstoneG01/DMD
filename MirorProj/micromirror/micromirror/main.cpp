/*
*
*  Micromirror Project Main File
*
*  October 02 2017 by stas kanevksy
*
*/

#include "project_constants.h"

#include "usb.h"
#include "API.h"
//project file includes:
#include "helper_functions.h"
//API file includes:
//system includes:
#include <stdio.h>      // standard input / output functions
#include <string>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "BMPParser.h"
#include "pattern.h"
#include "splash.h"
#include "QtCodeCoppy.h"

void sendDataToMirror(void);

int toSplash(Image_t *img, unsigned char **data);

void case1(void);
void case2(void);
void case3(void);
void case4(void);
void case5(void);
void case6(void);
void case7(void);
void case8(void);

struct PatternElement
{
	int bits;
	int color;
	int  exposure;
	int  darkPeriod;
	bool trigIn;
	bool trigOut2;
	std::string name;
	int splashImageIndex;
	int splashImageBitPos;
};

/* @param   LEDSelect - I - Choose the LEDs that are on : b0 = Red, b1 = Green, b2 = Blue
* 0 = No LED(Pass Through)
* 1 = Red
* 2 = Green
* 3 = Yellow(Green + Red)
* 4 = Blue
* 5 = Magenta(Blue + Red)
* 6 = Cyan(Blue + Green)
* 7 = White(Red + Blue + Green)
*/

struct PatternElement pattern1;
struct PatternElement pattern2;
struct PatternElement pattern3;
struct PatternElement pattern4;

int main(int argc, char **argv)
{
	int runProgram = 1;	
	unsigned char inputValue;
	std::cout << "Micromirror main function start " << std::endl;
	//or
	//printf(""Micromirror main function start %d"\n",rollingValue);

	do
	{
		printf("\nPlease select an option:\n");
		printf("1-Open Connection\n");
		printf("2-Load Image\n");
		printf("3-Close Connection\n");
		printf("4-Stop\n");
		printf("5-Pause\n");
		printf("6-Start\n");
		printf("7-Standby Mode\n");
		printf("8-Normal Mode\n");
		printf("O-Open connection, enter normal mode, stop, load image, start\n");
		printf("X-Stop, enter standby mode, quit\n");
		printf("9-Quit\n");
		std::cin >> inputValue;
		switch (inputValue)
		{
		case '1': // Open Connection
			case1();
			break;
		case '2' : // Load Image
			case2();
			break;
		case '3': // Close Connection
			case3();
			break;
		case '4': // Stop
			case4();
			break;
		case '5': // Pause
			case5();
			break;
		case '6': // Start
			case6();
			break;
		case '7': // Standby Mode
			case7();
			break;
		case '8': // Normal Mode
			case8();
			break;
		case '9': // Quit
		case 'q':
		case 'Q':
			runProgram = 0;
			break;
		case 'X': // Stop, enter standby mode, quit
		case 'x':
			case4();
			case7();
			runProgram = 0;
			break;
		case 'O': // Open connection, enter normal mode, stop, load image, start
		case 'o':
			case1();
			case8();
			case4();
			case2();
			case6();
			break;
		default:
			printf("No such option, please try again\n");
			break;
		}
	} while (runProgram);

	/*
	std::cout << "I want to show you some balls... Please Enter a number: ";
	std::cin >> rollingValue;
	print_balls(rollingValue);
	std::cout << "Thank you ! Enter any muner to to close....." << std::endl;
	std::cin >> rollingValue;
	*/
	std::cout << "Micromirror main function End " << std::endl;
	//or
	//printf(""Micromirror main function End %d"\n",rollingValue);
	return 0;
}

void case1() // Open Connection
{
	USB_Init();
	printf("USB Initialized...\n");
	int usb_error_flag;
	for (int i = 0; (i < 6) && (USB_IsConnected() == false); i++) /// will try 5 times to reconnect...
	{
		usb_error_flag = USB_Open(); // -1=Error | 0=Opened
		if (usb_error_flag == 0 && USB_IsConnected() == true)
		{
			printf("USB Opened!\n");
			i = 6; // just in case fall out of for loop	
		}
		else
		{
			printf("Retrying to establish connection...\n", usb_error_flag);
		}
	}
	if (USB_IsConnected() == false)
		printf("Posible USB Connection Failures:,\n Verify that micromirror is connected and powered on!\n Or verify there are no other processess using the micromirror (like T-GUI)!\n Or try reconecting USB cable to the computer!\n Or Hard reset the micromirror!\n", usb_error_flag);
}

void case2() // Load Image
{
	
	int imageCount;
	int error_flag = 0; 
	std::string path1;
	std::string path2;
	std::string path3;
	std::string path4;

	int numPatAdded = 0;

	if (LCR_SetMode(0x3) < 0) 		
	{
		//TODO::WE SHOULD MAKE THESE CONSTANTS IN OUR DEFINE>H FILE
		// 0 is  "Video Mode"
		// 1 is  "Pre stored Pattern"
		// 2 is  "Video Pattern Mode"
		// 3 is  "Pattern on the fly mode"
		printf("Unable to switch to Pattern on the fly mode\n");
		error_flag = 1;
	}
	else
	{
		printf("Mode switched to: Pattern on the fly\n");
	}
	/*
	if (!error_flag) // get path to images
	{
		imageCount = 1; 
		path1 = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\image4.bmp";
		path2 = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\image4.bmp";
		path3 = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\image4.bmp";
		path4 = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\image4.bmp";
	}
	LCR_ClearPatLut(); // clear image LUT




	//for (int i2 = 0; i2 < imageCount; i2++) // create pattern elements
	//{
		//set pattern variables for the images
		pattern1.bits = 1;
		pattern1.color = 1;
		pattern1.exposure = 2000000;
		pattern1.darkPeriod = 1000000;
		pattern1.trigIn = false;
		pattern1.trigOut2 = true;
		pattern1.name = path1;
		//numPatAdded++;
		//
		pattern2.bits = 1;
		pattern2.color = 1;
		pattern2.exposure = 200000;
		pattern2.darkPeriod = 100000;
		pattern2.trigIn = false;
		pattern2.trigOut2 = true;
		pattern2.name = path2;
		//numPatAdded++;
		//
		pattern3.bits = 1;
		pattern3.color = 1;
		pattern3.exposure = 200000;
		pattern3.darkPeriod = 100000;
		pattern3.trigIn = false;
		pattern3.trigOut2 = true;
		pattern3.name = path3;
		//numPatAdded++;
		//
		pattern4.bits = 1;
		pattern4.color = 1;
		pattern4.exposure = 200000;
		pattern4.darkPeriod = 100000;
		pattern4.trigIn = false;
		pattern4.trigOut2 = true;
		pattern4.name = path4;
	//}
		//numPatAdded++;

		if (LCR_AddToPatLut(numPatAdded, pattern1.exposure, true, pattern1.bits, pattern1.color, pattern1.trigIn, pattern1.darkPeriod, pattern1.trigOut2, 0, 0)<0)
		{
			printf("Unable to add pattern number %d to the LUT", numPatAdded);
			error_flag = 1;
			//i = imageCount; // drop out of for loop
		}
		else
			numPatAdded++;
		
		if (LCR_AddToPatLut(numPatAdded, pattern2.exposure, true, pattern2.bits, pattern2.color, pattern2.trigIn, pattern2.darkPeriod, pattern2.trigOut2, 0, 0)<0)
		{
			printf("Unable to add pattern number %d to the LUT", numPatAdded);
			error_flag = 1;
			//i = imageCount; // drop out of for loop
		}
		else
			numPatAdded++;

		if (LCR_AddToPatLut(numPatAdded, pattern3.exposure, true, pattern3.bits, pattern3.color, pattern3.trigIn, pattern3.darkPeriod, pattern3.trigOut2, 0, 0)<0)
		{
			printf("Unable to add pattern number %d to the LUT", numPatAdded);
			error_flag = 1;
			//i = imageCount; // drop out of for loop
		}
		else
			numPatAdded++;

		if (LCR_AddToPatLut(numPatAdded, pattern4.exposure, true, pattern4.bits, pattern4.color, pattern4.trigIn, pattern4.darkPeriod, pattern4.trigOut2, 0, 0)<0)
		{
			printf("Unable to add pattern number %d to the LUT", numPatAdded);
			error_flag = 1;
			//i = imageCount; // drop out of for loop
		}
		else
			numPatAdded++;
	

	//send images:
	if (!(error_flag) && LCR_SendPatLut() < 0)
	{
		printf("Sending pattern LUT failed!\n");
		error_flag = 1;
	}
	
	if (!(error_flag)) 
		error_flag = LCR_SetPatternConfig(imageCount, 0);// set to repeat images
	//error_flag = LCR_SetPatternConfig(imageCount, show_this_ammount);// set to run 'show_this_ammount' of images and then stop.

	if (error_flag < 0) {
		printf("Could not configure pattern repeat/non-repeat\n");
		printf("error_flag = %d\n", error_flag);
	}

	sendDataToMirror();
	*/
	addPattern();
}
 

void case3() // Close Connection
{
	USB_Close();
	USB_Exit();
	printf("USB Closed\n");
}

void case4() // Stop
{
	if (LCR_PatternDisplay(0x0) < 0)
		printf("Unable to stop pattern display");
}

void case5() // Pause
{
	if (LCR_PatternDisplay(0x1) < 0)
		printf("Unable to pause pattern display");
}

void case6() // Start
{

	if (LCR_PatternDisplay(0x2) < 0)
		printf("Unable to start pattern display");
}

void case7() // Standby Mode
{
	if (LCR_SetPowerMode(1) < 0)
		printf("Unable to go to stand by");
}

void case8() // Normal Mode
{
	if (LCR_SetPowerMode(0) < 0)
		printf("Unable to power on the board into normal mode");
}

void sendDataToMirror() 
{
	int m_ptnWidth = 1920;
	int m_ptnHeight = 1080;
	uint08 *splashData;
	bool master = true;
	int error = 0;
	int imageCount = 1 ;
	int bitDepth = 24;
	int bitPos = 0;
	int multiplier = 1;

	//Image_t  pattern1Image_tSpace ;
	
	//Image_t *pattern1Image_t = &pattern1Image_tSpace;

	Image_t *pattern1Image_t = PTN_Alloc(m_ptnWidth, m_ptnHeight, bitDepth);
	PTN_Fill(pattern1Image_t, 0); // fill the image with zero values

	char path1[] = "C:\\Users\\kanevsks\\Desktop\\git\\MirorProj\\micromirror\\image4.bmp";
	const char *pathptr = path1;
	error = BMP_LoadFromFile(pathptr, pattern1Image_t);
	if (error != 0)
	{
		printf("error in BMP_LoadFromFile\n");
	}


	Image_t *pattern1Image_t2 = PTN_Alloc(0, 0, 0); // create an empty image space
	//init image space created
	pattern1Image_t2->Width = m_ptnWidth;
	pattern1Image_t2->Height = m_ptnHeight;
	pattern1Image_t2->LineWidth = m_ptnWidth * multiplier;
	pattern1Image_t2->Buffer = pattern1Image_t->Buffer;
	pattern1Image_t2->FullHeight = m_ptnHeight;
	pattern1Image_t2->BitDepth = 1;

	error = PTN_Merge(pattern1Image_t, pattern1Image_t2, bitPos, pattern1Image_t2->BitDepth);
	if (error != 0)
	{
		printf("error in PTN_Merge\n");
	}

	error = PTN_SwapColors(pattern1Image_t, PTN_COLOR_RED, PTN_COLOR_BLUE, PTN_COLOR_GREEN);

	if (error != 0)
	{
		printf("error in PTN_SwapColors\n");
	}



	
	printf("%d\n", pattern1Image_t->Buffer); /**< Pointer to the image buffer */
	printf("%d\n",pattern1Image_t->Width);             /**< Width of the image in pixels */
	printf("%d\n", pattern1Image_t->Height);            /**< Height of the image in pixels */
	printf("%d\n", pattern1Image_t->LineWidth);         /**< Number of bytes in a line */
	printf("%d\n", pattern1Image_t->FullHeight);        /**< Total height of the original image */
	printf("%d\n", pattern1Image_t->BitDepth);

	uint08* splash_block_master = NULL;
	uint08* splash_block_slave = NULL;

	int splashSizeMaster = toSplash(pattern1Image_t,&splash_block_master);
	printf("splashSizeMaster: %d\n", splashSizeMaster);
	//int splashSizeSlave = merge_image_slave.toSplash(&splash_block_slave);

	//if (splashSizeMaster <= 0 || splashSizeSlave <= 0)
		//return -1;

	//if (uploadPatternToEVM(true       ,    spalshImageCount,                        splashSizeMaster,     splash_block_master) == -1)
	//int                   (bool master,    int splashImageCount,                    int splash_size,      uint08* splash_block)
	//this does the following:


	int origSize = splashSizeMaster;
	int splashImageCount = 0;
	LCR_InitPatternMemLoad(master, splashImageCount, splashSizeMaster);

	int dnldSize = LCR_pattenMemLoad(master, splash_block_master + (origSize - splashSizeMaster), splashSizeMaster);
	if (dnldSize < 0)
	{
		printf("error in LCR_pattenMemLoad\n");
	}

	//splashSizeMaster -= dnldSize;

}




int toSplash(Image_t *img,unsigned char **data)
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