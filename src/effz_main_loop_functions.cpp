/*
Copyright 2018 Oleg Skoromnik

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "effz_main_loop_functions.h"

#include "effz_user_input.h"
#include "effz_result.h"

#include <iostream>
#include <limits>

namespace eff_z{
	void base_menu::base_action_handler(char choice){
		switch(choice){
			case 'q':{
						 this->quit();
						 break;
					 }
			case 'p':{
						 this->print_menu();
						 break;
					 }
			default:{
						this->not_show_menu();
						std::cout << "Wrong input. Please repeat.\n";
						break;
					}
		}
	}

	base_menu_with_help::base_menu_with_help() :
		format_string("*****OCC_NUMS_FORMAT help*****\n"
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
			"and ms from -1 to 1\n"),
		format_occ_nums("*****FORMAT_STRING_HELP*****\n"
			"In order to perform computation the "
			"parameters should be given in a form of a format string.\n"
			"The format string can be specified in the file or "
			"in the standard input, i.e., terminal window or in "
			"the interactive mode of a program.\n"
			"The format string looks like:\n\n"
			"   -z Z1, Z2, ... -f OCC_NUMS_FORMAT "
			"-v OCC_NUMS1, OCC_NUMS2, ...; REPEAT_FORMAT_STRING\n\n"
			"EXPLANATION.\n \"-z\" specifies the charges. "
			"All arguments except of Z1 are optional. "
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
			"the results will be stored in the file on the disk.\n") {
				std::string help_text =
					"0. Print information about input parameters string.\n"
					"9. Print information about occupation numbers "
					"format.\n";
				help_text += menu_text;
				menu_text.swap(help_text);
			}

	void base_menu_with_help::help_action_handler(char choice){
		switch(choice){
			case '9':{
						 this->not_show_menu();
						 std::cout << format_string;
						 break;
					 }
			case '0':{
						 this->not_show_menu();
						 std::cout << format_occ_nums;
						 break;
					 }
			default:{
						base_action_handler(choice);
						break;
					}
		}
	}

	base_menu_with_previous::base_menu_with_previous(
			base_menu_ptr prev_menu) : prev_menu(prev_menu){
		std::string prev_text = "b. Go to the previous menu.\n";
		prev_text += menu_text;
		menu_text.swap(prev_text);
	}

	base_menu_ptr base_menu_with_previous::prev_action_handler(
			char choice){
		base_menu_ptr next_menu;
		if(choice == 'b'){
			next_menu.swap(prev_menu);
		}
		return next_menu;
	}

	main_menu::main_menu() {
		std::string main_text =
			"*****Effective charge program.*****\n\n"
			"1. Numerically calculate energies.\n"
			"2. Analytically calculate densities.\n"
			"3. Analytically calculate atomic scattering factors (asf).\n";
		main_text += menu_text;
		menu_text.swap(main_text);
	};

	base_menu_ptr main_menu::get_next_menu(char choice){
		base_menu_ptr new_menu;
		switch(choice){
			case '1':{
						 new_menu = std::make_shared<main_menu>();
						 base_menu_ptr ptr
							 = std::make_shared<zeroth_order_energy_menu>(
									 new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			case '2':{
						 new_menu = std::make_shared<main_menu>();
						 base_menu_ptr ptr
							 = std::make_shared<zeroth_order_density_menu>(
									 new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			case '3':{
						 new_menu = std::make_shared<main_menu>();
						 base_menu_ptr ptr
							 = std::make_shared<zeroth_order_asf_menu>(
									 new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			default:{
						base_action_handler(choice);
						break;
					}
		}
		return new_menu;
	}

	zeroth_order_energy_menu::zeroth_order_energy_menu(
			base_menu_ptr prev_menu) : base_menu_with_previous(prev_menu) {
		std::string zeroth_order_energy_str
			= "\n*****Calculation of the energies "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.*****\n\n"
			"1. Enter parameters.\n";
		zeroth_order_energy_str += menu_text;
		menu_text.swap(zeroth_order_energy_str);
	};

	base_menu_ptr zeroth_order_energy_menu::get_next_menu(char choice){
		base_menu_ptr new_menu;
		switch(choice){
			case '1':{
						 std::cin.ignore(
								 std::numeric_limits<
								 std::streamsize>::max(), '\n');
						 for(;;){
							 try{
								 std::cout <<
									 "Please enter parameter string:\n";
								 std::string parameter_str;
								 std::getline(std::cin,parameter_str);
								 zeroth_order::
									 parse_calc_and_print_0th_results_energy(std::cout,parameter_str);

								 break;
							 } catch (const parsing_exception &e){
								 std::cout << e.what() << "\n";
							 }
						 }
						 new_menu
							 = std::make_shared<zeroth_order_energy_menu>(
									 prev_menu);
						 base_menu_ptr ptr = std::make_shared<
							 result_menu_with_prev>(new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			default:{
						if(!(new_menu = prev_action_handler(choice)))
							help_action_handler(choice);
						break;
					}
		}
		return new_menu;
	}

	zeroth_order_density_menu::zeroth_order_density_menu(
			base_menu_ptr prev_menu) : base_menu_with_previous(prev_menu) {
		std::string zeroth_order_density_str
			= "\n*****Calculation of the densities "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.*****\n\n"
			"1. Enter parameters.\n";
		zeroth_order_density_str += menu_text;
		menu_text.swap(zeroth_order_density_str);
	};

	base_menu_ptr zeroth_order_density_menu::get_next_menu(char choice){
		base_menu_ptr new_menu;
		switch(choice){
			case '1':{
						 std::cin.ignore(
								 std::numeric_limits<
								 std::streamsize>::max(), '\n');
						 for(;;){
							 try{
								 std::cout <<
									 "Please enter parameter string:\n";
								 std::string parameter_str;
								 std::getline(std::cin,parameter_str);
								 zeroth_order::
									 parse_calc_and_print_0th_results_density(std::cout,parameter_str);

								 break;
							 } catch (const parsing_exception &e){
								 std::cout << e.what() << "\n";
							 }
						 }
						 new_menu
							 = std::make_shared<zeroth_order_density_menu>(
									 prev_menu);
						 base_menu_ptr ptr = std::make_shared<
							 result_menu_with_prev>(new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			default:{
						if(!(new_menu = prev_action_handler(choice)))
							help_action_handler(choice);
						break;
					}
		}
		return new_menu;
	}

	zeroth_order_asf_menu::zeroth_order_asf_menu(
			base_menu_ptr prev_menu) : base_menu_with_previous(prev_menu) {
		std::string zeroth_order_asf_str
			= "\n*****Calculation of the energies "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.*****\n\n"
			"1. Enter parameters.\n";
		zeroth_order_asf_str += menu_text;
		menu_text.swap(zeroth_order_asf_str);
	};

	base_menu_ptr zeroth_order_asf_menu::get_next_menu(char choice){
		base_menu_ptr new_menu;
		switch(choice){
			case '1':{
						 std::cin.ignore(
								 std::numeric_limits<
								 std::streamsize>::max(), '\n');
						 for(;;){
							 try{
								 std::cout <<
									 "Please enter parameter string:\n";
								 std::string parameter_str;
								 std::getline(std::cin,parameter_str);
								 zeroth_order::
									 parse_calc_and_print_0th_results_asf(
											 std::cout,parameter_str);

								 break;
							 } catch (const parsing_exception &e){
								 std::cout << e.what() << "\n";
							 }
						 }
						 new_menu
							 = std::make_shared<zeroth_order_asf_menu>(
									 prev_menu);
						 base_menu_ptr ptr = std::make_shared<
							 result_menu_with_prev>(new_menu);
						 new_menu.swap(ptr);
						 break;
					 }
			default:{
						new_menu = prev_action_handler(choice);
						help_action_handler(choice);
						break;
					}
		}
		return new_menu;
	}

	result_menu_with_prev::result_menu_with_prev(base_menu_ptr prev_menu)
		: base_menu_with_previous(prev_menu)
	{
		std::string result_menu_str
			= "The calculation results have been printed.\n"
			"What do you want to do next?\n"
			"m. Go tho the main menu.\n";
		result_menu_str += menu_text;
		menu_text.swap(result_menu_str);
	}

	base_menu_ptr result_menu_with_prev::get_next_menu(char choice)
	{
		base_menu_ptr next_menu;
		if(choice == 'm'){
			next_menu = std::make_shared<main_menu>();
			return next_menu;
		}
		if(!(next_menu = prev_action_handler(choice)))
			base_action_handler(choice);
		return next_menu;
	}
} /* end namespace eff_z */
