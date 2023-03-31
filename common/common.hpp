#ifndef TXTANIMATION_COMMON_HPP
#define TXTANIMATION_COMMON_HPP

#include <iostream>
#include <fstream>

#include "../include/fcheck/fcheck.hpp"

#define TA_TEMP_SIZE 4096 // maximum template size;

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

struct txt__anim__common__data {
	const unsigned short header_size = 1;
};

struct txt__anim__main__data {
	char filename[TA_TEMP_SIZE];
	float delay = 0;
	unsigned short block_size = 0;
};

// <core> header reading
bool ta_read_header(std::ifstream& inFile, txt__anim__main__data *main_data) { 
	bool badflag = false;
	char *rubbish = new char[TA_TEMP_SIZE];

	inFile >> main_data->delay;

	if (ifstrm_status(inFile, main_data->filename, false, true, true))
		badflag = true;

	inFile >> main_data->block_size;
	if (ifstrm_status(inFile, main_data->filename, false, true, true))
		badflag = true;

	inFile.getline(rubbish, TA_TEMP_SIZE);
	inFile.getline(rubbish, TA_TEMP_SIZE);

	delete [] rubbish;

	if (ifstrm_status(inFile, main_data->filename, false, true, true))
		badflag = true;
	
	if (badflag)
		return false;
	return true;
}

#endif

// last edit: 22/07/21
