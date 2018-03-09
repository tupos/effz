#include "effz_main_loop_functions.h"

#include <iostream>

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
			case '0':{
						 this->not_show_menu();
						 std::cout << format_string;
						 break;
					 }
			case '9':{
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
			"3. Analytically calculate atomic scattering factors.\n";
		main_text += menu_text;
		menu_text.swap(main_text);
	};

	base_menu_ptr main_menu::get_next_menu(char choice){
		base_menu_ptr new_menu;
		switch(choice){
			case '1':{
						 std::shared_ptr<main_menu> ptr
							 = std::make_shared<main_menu>();
						 std::shared_ptr<base_menu> ptr1
							 = std::make_shared<zeroth_order_energy_menu>(
									 ptr);
						 new_menu = ptr1;
						 break;
					 }
			case '2':{
						 //new_menu = std::make_unique<
							 //zeroth_order_density_menu>();
						 break;
					 }
			case '3':{
						 //new_menu = std::make_unique<
							 //zeroth_order_asf_menu>();
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
						 //new_menu = std::make_unique<
							 //zeroth_order_energy_menu>();
						 break;
					 }
			default:{
						help_action_handler(choice);
						break;
					}
		}
		return new_menu;
	}

	//zeroth_order_density_menu::zeroth_order_density_menu(){
		//menu_text = "\n*****Calculation of the densities "
			//"in the zeroth-order from a set of occupation "
			//"numbers and a charge.*****\n\n"
			//"1. Enter parameters.\n"
			//"2. Print information about input parameters string.\n"
			//"3. Print information about occupation numbers format.\n"
			//"0. Go back to the previous menu.\n\n"
			//"Please enter your choice...\n";
	//}

	//base_menu_ptr zeroth_order_density_menu::get_next_menu(char choice){
		//base_menu_ptr new_menu;
		//switch(choice){
			//case '1':{
						 //new_menu = std::make_unique<
							 //zeroth_order_energy_menu>();
						 //break;
					 //}
			//case '2':{
						 //if(!density_symbolic_loop()){
							 //return 0;
						 //}
						 //break;
					 //}
			//case '3':{
						 //if(!asf_symbolic_loop()){
							 //return 0;
						 //}
						 //break;
					 //}
			//case '0':{ return 0; break; }
			//default:{
						//std::cout << "Wrong input. Please repeat.\n";
						//break;
					//}
		//}
		//return new_menu;
	//}

	//zeroth_order_asf_menu::zeroth_order_asf_menu(){
		//menu_text = "\n*****Calculation of the ASF "
			//"in the zeroth-order from a set of occupation "
			//"numbers and a charge.*****\n\n"
			//"1. Enter parameters.\n"
			//"2. Print information about input parameters string.\n"
			//"3. Print information about occupation numbers format.\n"
			//"0. Go back to the previous menu.\n\n"
			//"Please enter your choice...\n";
	//}

	//base_menu_ptr zeroth_order_asf_menu::get_next_menu(char choice){
		//base_menu_ptr new_menu;
		//switch(choice){
			//case '1':{
						 //new_menu = std::make_unique<
							 //zeroth_order_energy_menu>();
						 //break;
					 //}
			//case '2':{
						 //if(!density_symbolic_loop()){
							 //return 0;
						 //}
						 //break;
					 //}
			//case '3':{
						 //if(!asf_symbolic_loop()){
							 //return 0;
						 //}
						 //break;
					 //}
			//case '0':{ return 0; break; }
			//default:{
						//std::cout << "Wrong input. Please repeat.\n";
						//break;
					//}
		//}
		//return new_menu;
	//}

	void menus::print_main() const{
		std::cout << main;
	}

	void menus::print_select_order() const{
		std::cout << select_order;
	}

	void menus::print_zeroth_order_energy() const{
		std::cout << zeroth_order_energy;
	}
	const std::string menus::main
		= "*****Effective charge program.*****\n\n"
			"1. Numerically calculate energies.\n"
			"2. Analytically calculate densities.\n"
			"3. Analytically calculate atomic scattering factors.\n"
			"0. Exit the effective charge program.\n\n"
			"Please enter your choice...\n";

	const std::string menus::select_order
		= "Please select the accuracy of the calculations.\n\n"
			"1. Zeroth-order. Fast. Provides 6\% accuracy with "
			"respect to HF.\n"
			"0. Go back to the previous menu.\n\n"
			"Please enter your choice...\n";

	const std::string menus::zeroth_order_energy
		= "\n*****Calculation of the energies "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.*****\n\n"
			"1. Enter parameters.\n"
			"2. Print information about input parameters string.\n"
			"3. Print information about occupation numbers format.\n"
			"0. Go back to the previous menu.\n\n"
			"Please enter your choice...\n";

	void help::print_format_occ_nums() const{
		std::cout << format_occ_nums;
	}

	void help::print_format_string() const{
		std::cout << format_string;
	}

	const std::string help::format_occ_nums
		= "*****OCC_NUMS_FORMAT help*****\n"
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

	const std::string help::format_string
		= "*****FORMAT_STRING_HELP*****\n"
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

	int main_menu1(char user_input){
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
			menus().print_zeroth_order_energy();
			std::cin >> user_input;
			if(!zeroth_order_energy_menu1(user_input))
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
	int zeroth_order_energy_menu1(char user_input){
		switch(user_input){
			case '1': {
						  if(!zeroth_order_energy_enter_parameters_loop())
							  return -1;
						  break;
					  }
			case '2': {
						  help().print_format_string();
						  break;
					  }
			case '3': {
						  help().print_format_occ_nums();
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
