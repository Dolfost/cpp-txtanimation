#ifndef TXTANIAMTION_PARSER_HPP
#define TXTANIAMTION_PARSER_HPP

#include <iostream>
#include <fstream>
#include <cstring>

#include "common/common.hpp"
#include "txtanimation.hpp"

#include "include/fcheck/fcheck.hpp"
#include "include/clrscr/clrscr.hpp"
#include "include/colorit/colorit.hpp"

// colors
#define TAP_GRN  112
#define TAP_GRN1 118
#define TAP_MAG  202
#define TAP_CYN  225
#define TAP_RED  196
#define TAP_YEL  220

struct txt__anim__parser__data {
	unsigned long lines = 0;
	unsigned int blocks = 0;
	unsigned int expected_blocks = 0;
	unsigned long current_line = 0;
};

// lines counting
unsigned long ta_count_lines(char filename[]) {
	std::ifstream inFile;
	inFile.open(filename);
	is_open(inFile, filename, true);

	char *rubbish = new char[TA_TEMP_SIZE];
	unsigned long count = 0;

	while (!inFile.eof()) {
		inFile.getline(rubbish, TA_TEMP_SIZE);
		ifstrm_status(inFile, filename, true, false, false);
		++count;
	}

	--count;

	inFile.close();
	delete [] rubbish;

	return count;
}

void ta_parser_fatal_error_msg(void) {
	std::cout << "\nlast error status:\t" << setFore(TAP_RED) << Style.BOLD
		  << "fatal" << Style.RST << setFore(TAP_RED) << "\nAbort\n" 
		  << Style.RST;
	exit(EXIT_FAILURE);
}
// <debug> printing debug info
void ta_parser_info(txt__anim__common__data *common_data, txt__anim__main__data *main_data,
		    txt__anim__parser__data *parser_data, char temp[][TA_TEMP_SIZE]) {
	std::cout << Style.ITLC << Fore.GRN << "\nInfo:" << Style.RST <<
		     "\nFilename:\t\t" << main_data->filename
		  << "\nDelay:\t\t\t" << main_data->delay
		  << "\nBlock size:\t\t" << main_data->block_size
		  << "\nBlocks expected:\t" << parser_data->expected_blocks
		  << "\nBlocks read:\t\t" << parser_data->blocks
		  << "\nLines read:\t\t" << parser_data->lines
		  << "\n\n" << Style.ITLC << setFore(TAP_GRN) << "Last memory cells content:"
		  << Style.RST << "\n";

	unsigned short *count = new unsigned short;
	for (*count = 0; *count < main_data->block_size; ++*count)
		std::cout << "temp[" << *count << "]:\t " << temp[*count] << "\n";
	delete count;
}

void run_animation_parser(char *filename, short clearmode) {
	txt__anim__parser__data *parser_data = new txt__anim__parser__data; 
	
	std::cout << setFore(TAP_MAG) << S.BLD << "TXT-animation Parser\n" << Style.RST 
		  << "\n\n" << Style.BLD << setFore(TAP_CYN) << "Starting..."
		  << Style.RST << "\n";

	txt__anim__main__data *main_data = new txt__anim__main__data;
	
	std::ifstream inFile;
	inFile.open(filename);

	if (!inFile.is_open()) {	// if could not open a file
		std::cout << "\n" << Style.BLD << setFore(TAP_RED) << "ERROR:" << Style.RST
			  << " could not open the file \"" << filename << "\".\n";
		ta_parser_fatal_error_msg(); }
	
	txt__anim__common__data *common_data = new txt__anim__common__data;

	strncpy(main_data->filename, filename, TA_TEMP_SIZE);

	parser_data->lines = ta_count_lines(main_data->filename); 

	if (parser_data->lines == 0) {
		std::cout << "\n"<< setFore(TAP_RED) << Style.BOLD << "ERROR:" << Style.RST
			  << " file \"" << main_data->filename << "\" is empty.\n";
		ta_parser_fatal_error_msg(); }

	// getting data from header
	if (!ta_read_header(inFile, main_data)) {
		std::cout << "\n" << Style.BLD << setFore(TAP_RED) << "ERROR:" << Style.RST
			  << " could not read header in file \"" << main_data->filename << "\".\n";
		ta_parser_fatal_error_msg(); } 
	
	// error check too small file
	if (parser_data->lines < (main_data->block_size + common_data->header_size + 1)) {
		std::cout << "\n" << Style.BLD << setFore(TAP_RED) << "ERROR:" << Style.RST 
			  << " the number of lines in file \"" << main_data->filename
			  << "\"\ncannot contain even one block of the size specified in the header.\n"
			     "\n" << Style.ITLC << setFore(TAP_YEL) << "Info:\n" << Style.RST <<
			     "Lines:\t\t\t" << parser_data->lines
			  << "\nBlock size:\t\t" << main_data->block_size
			  << "\nHeader size <const>:\t" << common_data->header_size
			  << "\n";
		ta_parser_fatal_error_msg(); }

	// make expectations
	std::cout << "Assuming the possible number of blocks...\n";
	parser_data->expected_blocks =
	((parser_data->lines - common_data->header_size) / (main_data->block_size + 1));

	std::cout << "Running file \"" << main_data->filename << "\"... <"
		  << main_data->delay << "/" << main_data->block_size << ">\n";

	// temps
	char temp[main_data->block_size][TA_TEMP_SIZE];
	char *rubbish = new char[TA_TEMP_SIZE];
	unsigned short *count = new unsigned short;

	// skipping the header in current_line
	parser_data->current_line += common_data->header_size + 1;
	while (!inFile.eof()) {	// <core>
		for (*count = 0; *count < main_data->block_size; ++*count) {
			++parser_data->current_line;	// counting current line

			if (parser_data->current_line > parser_data->lines)
				--parser_data->current_line;

			std::cout << "Reading the line: " << parser_data->current_line << "/"
				  << parser_data->lines << "\r";

			inFile.getline(temp[*count], TA_TEMP_SIZE);
			if (ifstrm_status(inFile, main_data->filename, false, false, false)) {
				ta_parser_fatal_error_msg(); 
			}
		}
		// counting readed blocks and line that will be eaten by inFile.get()
		++parser_data->blocks;
		++parser_data->current_line;

		inFile.getline(rubbish, TA_TEMP_SIZE);
		if (ifstrm_status(inFile, main_data->filename, false, false, false))
			ta_parser_fatal_error_msg();
	} std::cout << "\n"; // new line after current_line and \r

	delete count;	// freeing memory
	delete [] rubbish;
	
	// error checking the wrong number of blocks
	std::cout << "Equalization of the expected and explicit number of blocks...\n";
	if (parser_data->expected_blocks != parser_data->blocks) {
		std::cout << "\n" << Style.BOLD << setFore(TAP_RED) << "ERROR:" << Style.RST
			  << " number of blocks in file \"" << main_data->filename << "\" is less than\n"
			     "expected number blocks.\n" 
			  << Style.ITLC << setFore(TAP_YEL) << "\nInfo:\n" << Style.RST <<
			     "Lines:\t\t\t" << parser_data->lines
			  << "\nBlock size:\t\t" << main_data->block_size
			  << "\nHeader size <const>:\t" << common_data->header_size
			  << "\nBlocks expected:\t" << parser_data->expected_blocks
			  << "\nBlocks readed:\t\t" << parser_data->blocks << "\n";
		ta_parser_fatal_error_msg();
	}

	std::cout << setFore(TAP_GRN1) << Style.BOLD << "\nAll looks fine\n" << Style.RST;
	ta_parser_info(common_data, main_data, parser_data, temp);

	char *choise = new char;

	// run animation loop
	std::cout << "\nRun animation \"" << main_data->filename << "\"? <y/n> ";
	while (*choise != 'n') {
		std::cin.get(*choise).get();
		while (*choise != 'y' && *choise != 'n') {

			std::cout << setFore(TAP_RED) << Style.BOLD 
				  << "Please, enter a valid ansver:" << Style.RST << " <y/n> ";
			std::cin.get(*choise).get();
		}
		if (*choise == 'n')
			break;

		run_animation(main_data->filename, clearmode);
		ta_parser_info(common_data, main_data, parser_data, temp);

		std::cout << "\nRun animation \"" << main_data->filename << "\" again? <y/n> ";
	}

	// freeing memory
	delete main_data;
	delete common_data;
	delete parser_data;
	delete choise;	
}

#endif

// last edit: 21/09/21
// developed by https://github.com/Dolfost
