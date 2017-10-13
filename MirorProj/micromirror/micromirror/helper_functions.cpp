//project includes:
#include "project_constants.h"
#include "helper_functions.h"

//system includes:
#include <stdio.h>      // standard input / output functions
#include <string.h>     // string function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <stdlib.h>
#include <iostream>
#include <fstream>


void print_balls(int numberOfBalls)
{
	std::cout << "THESE ARE MY BALLS!!!!!!! LOOK AT THEM !!!" << std::endl;
	for (int i = 0; i < numberOfBalls; i++)
	{
		std::cout << " .--. " << std::endl;
		std::cout << "|    |" << std::endl;
		std::cout << " '--' " << std::endl;
	}
}
