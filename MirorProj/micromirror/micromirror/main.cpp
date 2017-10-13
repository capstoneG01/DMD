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

void sendDataToMirror(void);

void case1(void);
void case2(void);
void case3(void);
void case4(void);
void case5(void);


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
		printf("\nPlease select an option:\n1-Open Connection\n2-Load Image\n3-Close Connection\n4-Stop\n5-Pause\n6-Quit\n");
		std::cin >> inputValue;
		switch (inputValue)
		{
		case '1':
			case1();
			break;
		case '2' :
			case2();
			break;
		case '3':
			case3();
			break;
		case '6':
		case 'q':
		case 'Q':
			runProgram = 0;
			break;
		case '4':
			case4();
			break;
		case '5':
			case5();
			break;
		default:
			printf("No such option, pelase try again\n");
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




void case1()
{
	USB_Init();
	printf("USB Initialized...\n");
	int usb_error_flag;
	for (int i = 0; (i < 6) && (USB_IsConnected() == false); i++) /// will try 5 times to reconnect..
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
void case3()
{
	USB_Close();
	USB_Exit();
	printf("USB Closed.\n");
}
void case2()
{
	int imageCount;
	int error_flag = 0; 
	std::string path1;
	std::string path2;
	std::string path3;
	std::string path4;

	int numPatAdded = 1;

	if (LCR_SetMode(0x3) < 0) 		
	{
		//TODO::WE SHOULD MAKE THESE CONSTANTS IN OUR DEFINE>H FILW
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

	if (!error_flag) // get path to images
	{
		imageCount = 4; 
		path1 = "C:\\Users\\kanevsks\\Desktop\\MirorProj\\micromirror\\image1.bmp";
		path2 = "C:\\Users\\kanevsks\\Desktop\\MirorProj\\micromirror\\image2.bmp";
		path3 = "C:\\Users\\kanevsks\\Desktop\\MirorProj\\micromirror\\image3.bmp";
		path4 = "C:\\Users\\kanevsks\\Desktop\\MirorProj\\micromirror\\image4.bmp";
	}
	LCR_ClearPatLut(); // clear image LUT




	//for (int i2 = 0; i2 < imageCount; i2++) // create pattern elements
	//{
		//set pattern variables for the images
		pattern1.bits = 1;
		pattern1.color = 1;
		pattern1.exposure = 200000;
		pattern1.darkPeriod = 100000;
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





		sendDataToMirror();






















	

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

	if (LCR_PatternDisplay(0x2) < 0)
		printf("Unable to start pattern display");

}
 
void case4() 
{
	if (LCR_PatternDisplay(0x0) < 0)
		printf("Unable to stop pattern display");
}

void case5()
{
	if (LCR_PatternDisplay(0x1) < 0)
		printf("Unable to pause pattern display");
}

	

void sendDataToMirror() 
{
	int m_ptnWidth = 1920;
	int m_ptnHeight = 1080;
	uint08  * pointerToImageArray;


	int splashImageCount;
	int origSize;
	int splash_size;



	LCR_InitPatternMemLoad(true, splashImageCount, splash_size);
	
	int dnldSize = LCR_pattenMemLoad(true, pointerToImageArray + (origSize - splash_size), splash_size);
	if (dnldSize < 0)
	{
		printf("Unable Download image !!");
	}




	/*for (int image = 0; image < totalSplashImages; image++)
	{

		int spalshImageCount;

		spalshImageCount = totalSplashImages - 1 - image;

		PtnImage merge_image(m_ptnWidth, m_ptnHeight, 24);

		merge_image.fill(0);


		for (int i = 0; i < m_elements.size(); i++)
		{
			if (m_elements[i].splashImageIndex != spalshImageCount)
				continue;
			int bitpos = m_elements[i].splashImageBitPos;
			int bitdepth = m_elements[i].bits;
			PtnImage image(m_elements[i].name);
			merge_image.merge(image, bitpos, bitdepth);
		}

		merge_image.swapColors(PTN_COLOR_RED, PTN_COLOR_BLUE, PTN_COLOR_GREEN);
		uint08* splash_block = NULL;

		PtnImage merge_image_master(m_ptnWidth, m_ptnHeight, 24);
		PtnImage merge_image_slave(m_ptnWidth, m_ptnHeight, 24);
		merge_image_master = merge_image;
		merge_image_slave = merge_image;
		if (m_dualAsic)
		{

			merge_image_master.crop(0, 0, m_ptnWidth / 2, m_ptnHeight);
			merge_image_slave.crop(m_ptnWidth / 2, 0, m_ptnWidth / 2, m_ptnHeight);

			if (firmware == true)
			{
				if (m_firmware->addSplash(&merge_image_master) < 0)
				{
					showError(GET_ERR_STR());
					return -1;
				}
				if (m_firmwareSlave != NULL)
				{
					if (m_firmwareSlave->addSplash(&merge_image_slave) < 0)
					{
						showError(GET_ERR_STR());
						return -1;
					}
				}
			}

			else
			{
				uint08* splash_block_master = NULL;
				uint08* splash_block_slave = NULL;

				int splashSizeMaster = merge_image_master.toSplash(&splash_block_master);
				int splashSizeSlave = merge_image_slave.toSplash(&splash_block_slave);

				if (splashSizeMaster <= 0 || splashSizeSlave <= 0)
					return -1;

				if (uploadPatternToEVM(true, spalshImageCount, splashSizeMaster, splash_block_master) == -1)
					return -1;

				if (uploadPatternToEVM(false, spalshImageCount, splashSizeSlave, splash_block_slave) == -1)
					return -1;

			}
		}
		else
		{
			if (firmware == true)
			{
				if (m_firmware->addSplash(&merge_image) < 0)
				{
					showError(GET_ERR_STR());
					return -1;
				}
			}
			else

			{
				int splashSize = merge_image.toSplash(&splash_block);
				if (splashSize <= 0)
					return -1;
				if (uploadPatternToEVM(true, spalshImageCount, splashSize, splash_block) < 0)
					return -1;
			}
		}
	}
	*/

}




