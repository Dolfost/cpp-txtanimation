#ifndef TXTANIAMTION_HPP
#define TXTANIAMTION_HPP

#include <iostream>
#include <fstream>
#include <cstring>

#include "common/common.hpp"

#include "include/fcheck/fcheck.hpp"
#include "include/clrscr/clrscr.hpp"
#include "include/sleep/sleep.hpp"

void run_animation(const char filename[], const short clearmode) {
	txt__anim__main__data *main_data = new txt__anim__main__data;
	std::ifstream inFile;
	inFile.open(filename);
	is_open(inFile, filename, true);

	txt__anim__common__data *common_data = new txt__anim__common__data;

	strncpy(main_data->filename, filename, TA_TEMP_SIZE);

	ta_read_header(inFile, main_data);

	char temp[main_data->block_size][TA_TEMP_SIZE];	// temps
//	char *rubbish = new char[TA_TEMP_SIZE];
	unsigned short *count = new unsigned short;

	while (!inFile.eof()) {	// <core>
		for (*count = 0; *count < main_data->block_size; ++*count) {
			inFile.getline(temp[*count], TA_TEMP_SIZE);
			ifstrm_status(inFile, main_data->filename, true, false, false); }

		clrscr(clearmode);
//		inFile.getline(rubbish, TA_TEMP_SIZE);
		inFile.ignore(TA_TEMP_SIZE, '\n');
		ifstrm_status(inFile, main_data->filename, true, false, false);

		for (*count = 0; *count < main_data->block_size; ++*count) {
			std::cout << temp[*count] << "\n"; }

		sleep(main_data->delay);
	}

	delete main_data;
	delete common_data;
//	delete [] rubbish;
	delete count;
}


#endif

// last edit: 18/09/021
// developed by https://github.com/Dolfost
