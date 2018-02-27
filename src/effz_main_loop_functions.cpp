#include "effz_main_loop_functions.h"

#include <iostream>

namespace eff_z{

	void print_main_menu(){
		std::cout << "*****Effective charge program.*****\n\n"
			"1. Numerically calculate energies.\n"
			"2. Analytically calculate densities.\n"
			"3. Analytically calculate atomic scattering factors.\n"
			"0. Exit the effective charge program.\n\n"
			"Please enter your choice...\n";
	}

	void print_select_order_menu(){
		std::cout << "Please select the accuracy of the calculations.\n\n"
			"1. Zeroth-order. Fast. Provides 6\% accuracy with "
			"respect to HF.\n"
			"0. Go back to the previous menu.\n\n"
			"Please enter your choice...\n";
	}

	void print_zeroth_order_energy_menu(){
		std::cout <<"\n*****Calculation of the energies "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.*****\n\n"
			"1. Enter parameters.\n"
			"2. Print information about input parameters string.\n"
			"3. Print information about occupation numbers format.\n"
			"0. Go back to the previous menu.\n\n"
			"Please enter your choice...\n";
	}

	void print_occ_nums_format_help(){
		std::cout << "*****OCC_NUMS_FORMAT help*****\n"
			"Occupation numbers are specified in one of "
			"the following formats:\n\n"
			"\"n\" --- numerical format, e.g., \"5\" "
			"using occupation numbers of the B element.\n\n"
			"\"N\" --- named format, e.g. \"He\""
			"using occupation numbers of the He element.\n\n"
			"\"i\" --- ionization format, i.e., "
			"element name + roman number. e.g. \"Ne II\".\n"
			" This means occupation numbers for Ne minus 1 electron.\n\n"
			"\"O\" --- occupation numbers and an element name format "
			"i.e., element name + {{n,l,m,ms}}, e.g. \"B {{2,1,0,1}}\" "
			"or \"B {{2,1,0,1},{2,1,1,-1}}\". Note: more than "
			"two internal brackets are possible. "
			"In this case we take Boron occupation numbers "
			"and append the corresponding occupation "
			"numbers in brackets.\n\n"
			"\"o\" --- a set of occupation numbers format "
			"e.g. \"{{1,0,0,1},{1,0,0,-1}}\"\n\n"
			" IMPORTANT: If there are identical "
			"occupation numbers present the energy is equal to zero.\n\n"
			" IMPORTANT: The occupation numbers are sorted "
			"in the order of increasing n, l, m, ms. "
			"This means that we fill m form -l to l "
			"and ms from -1 to 1\n";
	}

	void print_parameters_format_string_help(){
		std::cout << "*****FORMAT_STRING_HELP*****\n"
			"In order to perform computation the "
			"parameters should be given in a form of a format string.\n"
			"The format string can be specified in the file or "
			"in the standard input, i.e., terminal window or in "
			"the interactive mode of a program.\n"
			"The format string looks like:\n\n"
			"   -z Z1, Z2, ... -f OCC_NUMS_FORMAT "
			"-v OCC_NUMS1, OCC_NUMS2, ...; REPEAT_FORMAT_STRING\n\n"
			"EXPLANATION.\n \"-z\" specifies the charges "
			"all arguments except of Z1 are optional. "
			"Z1, Z2, ... are numbers.\n"
			"\"-f\" specifies the format of the occupation numbers "
			"and can be one of the following "
			"\"n\", \"N\", \"i\", \"O\", \"o\" "
			"for more information read OCC_NUMS_FORMAT help.\n"
			"\"-v\" specifies the occupation numbers in the format "
			"OCC_NUMS_FORMAT. All arguments except of OCC_NUMS1 "
			"are optional.\n"
			"The semicolon at the end separates format strings, "
			"since multiple strings can be provided. All format strings "
			"except of the first one are optional.\n\n"
			"RESULTS OF THE COMPUTATION.\n"
			"Provided the format string above, the program computes "
			"for all charges Z1,Z2,... corresponding characteristics. "
			"thus yielding num_charges * num_occ_nums values, "
			"by default the values are output to the screen. "
			"If to the format string before the \";\" the flag "
			"\"-o\" is provided with a file path, i.e., \n\n"
			"    -z Z1, Z2, ... -f OCC_NUMS_FORMAT "
			"-v OCC_NUMS1, OCC_NUMS2, ... -o PATH_TO_OUTPUT_FILE;\n\n"
			"the results will be stored in the file on the disk.\n";
	}

	int main_menu(char user_input){
		switch (user_input) {
			case '1': {
						  if(!zeroth_order_energy_loop()){
							  return -1;
						  }
						  break;
					  }
			case '2': {
						  if(!density_symbolic_loop()){
							  return 0;
						  }
						  break;
					  }
			case '3': {
						  if(!asf_symbolic_loop()){
							  return 0;
						  }
						  break;
					  }
			case '0': { return 0; break; }
			default: {
						 std::cout << "Wrong input. Please repeat.\n";
						 break;
					 }
		}
		return -1;
	}
	int zeroth_order_energy_loop(){
		char user_input;
		while(1){
			print_zeroth_order_energy_menu();
			std::cin >> user_input;
			if(!zeroth_order_energy_menu(user_input))
				break;
		}
		return 0;
	}
	int density_symbolic_loop(){
		return 0;
	}
	int asf_symbolic_loop(){
		return 0;
	}
	int zeroth_order_energy_menu(char user_input){
		switch(user_input){
			case '1': {
						  if(!zeroth_order_energy_enter_parameters_loop())
							  return -1;
						  break;
					  }
			case '2': {
						  print_parameters_format_string_help();
						  break;
					  }
			case '3': {
						  print_occ_nums_format_help();
						  break;
					  }
			case '0': { return 0; }
			default: {
						 std::cout << "Wrong input. Please repeat.\n";
						 break;
					 }
		}
		return -1;
	}
	int zeroth_order_energy_enter_parameters_loop(){
		return 0;

	}
} /* end namespace eff_z */
