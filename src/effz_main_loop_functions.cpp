#include "effz_main_loop_functions.h"

#include <iostream>

namespace eff_z{

	void print_main_menu(){
		std::cout << "*****Effective charge program.*****\n";
		std::cout << "\n";
		std::cout << "1. Numerically calculate energies.\n";
		std::cout << "2. Analytically calculate densities.\n";
		std::cout
			<< "3. Analytically calculate atomic scattering factors.\n";
		std::cout << "0. Exit the effective charge program.\n";
		std::cout << "\n";
		std::cout << "Please enter your choice...\n";
		std::cout << "\n";
	}
	void print_select_order_menu(){
		std::cout << "Please select the accuracy of the calculations.\n";
		std::cout << "\n";
		std::cout << "1. Zeroth-order. Fast. "
			"Provides 6\% accuracy with respect to HF.\n";
		std::cout << "0. Go back to the previous menu.\n";
		std::cout << "\n";
		std::cout << "Please enter your choice...\n";
		std::cout << "\n";
	}
	void print_zeroth_order_energy_menu(){
		std::cout <<"\n";
		std::cout << "Calculation of the energies "
			"in the zeroth-order from a set of occupation "
			"numbers and a charge.\n";
		std::cout <<"\n";
		std::cout << "1. Enter parameters.\n";
		std::cout <<"\n";
		std::cout << "2. Print information about input parameters.\n";
		std::cout <<"\n";
		std::cout << "0. Go back to the previous menu.\n";
		std::cout << "Please enter your parameters...\n";
		std::cout <<"\n";
	}

	void print_occ_nums_format_help(){
		std::cout << "Occupation numbers are specified in one of "
			"the following formats:\n";

		std::cout <<"\n";
		std::cout << "A) Element name. e.g. \" He \".\n";
		std::cout << "\n";

		std::cout << "B) Element name + roman number. e.g. \" Ne I\".\n";
		std::cout << " This means occupation numbers "
			"for Ne minus 1 electron.\n";
		std::cout << "\n";

		std::cout <<"C) Element name + {n,l,m,ms}, "
			"e.g. \" B {2,1,0,1} \".\n";
		std::cout << "or ";
		std::cout << "\" B {{2,1,0,1},{2,1,1,-1}} \".\n Note: more than "
			"two internal brackets are possible.\n";
		std::cout << " In this case we take Boron occupation numbers "
			"and append the corresponding occupation "
			"numbers in brackets.\n";
		std::cout << "\n";

		std::cout << "D) Set of occupation numbers. "
			"e.g. \" {{1,0,0,1},{1,0,0,-1}} \"\n";
		std::cout << "\n";

		std::cout << " IMPORTANT: If there are identical "
			"occupation numbers present the energy is equal to zero.\n";
		std::cout <<"\n";
		std::cout << " IMPORTANT: The occupation numbers are sorted "
			"in the order of increasing n, l, m, ms. ";
		std::cout << "This means that we fill m form -l to l "
			"and ms from -1 to 1";
		std::cout <<"\n";
	}

	int main_menu(char user_input){
		switch (user_input) {
			case '1': {
						  std::cout << "1\n";
						  if(!zeroth_order_energy_loop()){
							  return -1;
						  }
						  break;
					  }
			case '2': {
						  std::cout << "2\n";
						  if(!density_symbolic_loop()){
							  return 0;
						  }
						  break;
					  }
			case '3': {
						  std::cout << "3\n";
						  if(!asf_symbolic_loop()){
							  return 0;
						  }
						  break;
					  }
			case '0': {
						  std::cout << "0\n";
						  return 0; break;
					  }
			default: {
						 std::cout << "Wrong input. Please repeat.\n";
						 std::cout << "default\n";
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
