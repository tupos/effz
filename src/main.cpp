#include <Python.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <tuple>

#include "effz_main_loop_functions.h"
#include "effz_atomic_data.h"
#include "effz_zeroth_order.h"
#include "effz_python_utility.h"
#include "effz_zeroth_order_python.h"
#include "effz_user_input.h"


using namespace eff_z;
using namespace zeroth_order;
using namespace atomic_data;


int main(int argc, char *argv[]) try {

	//char user_input;
	//while(1){
		//print_main_menu();
		//std::cin >> user_input;
		////std::cout << user_input << "\n";
		//if(!main_menu(user_input))
			//break;
	//}
	//std::vector<int> nums = parse_z_format("1,2,3,4,5");
	//for(auto &num: nums){
		//std::cout << num << " ";
	//}
	//std::cout << "\n";

	//char r = parse_f_format("i");
	//std::cout << r << "\n";

	//std::vector<occ_nums_array> arr
		//= parse_occ_nums_i_format("He II");
	//std::vector<occ_nums_array> arr
		//= parse_occ_nums_N_format("He, Li, F");
	//std::vector<occ_nums_array> arr
		//= parse_occ_nums_o_format("{{1,0,0,1},{1,0,0,1}},{{1,0,0,1}}}");
	//std::vector<occ_nums_array> arr
		//= parse_v_format("H {{1,0,0,-1}}, Be {{1,0,0,1},{1,0,0,1}}"
				//", Li {{2,0,0,1},{2,0,0,-1},{3,0,0,1}}",'O');
	std::string format_string
		= "-f N -z 1,2,3,4 -v He, Li, F;"
		"-f o -z 1 -v {{1,0,0,1}};"
		"-f i -z 4 -v Li II;";
	std::cout << format_string << "\n";
	auto data = parse_format_string(format_string);
	for(const auto &dat:data){
		auto charges = std::get<0>(dat);
		auto nums = std::get<1>(dat);

		for(auto &num: charges){
			std::cout << num << " ";
		}
		std::cout << "\n";
		for(auto &occ_nums: nums){
			std::cout << "\"" << std::get<0>(occ_nums) << "\" ";
			std::cout << "{";
			for(auto &g_i: std::get<1>(occ_nums)){
				std::cout << "{";
				for(auto &g_ij: g_i){
					std::cout << g_ij << ",";
				}
				std::cout << "},";
			}
			std::cout << "}\n";
		}

	}
	//std::cout << eff_z::zeroth_order::z_star_0th(2., occ_nums_data::g_He)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(2., occ_nums_data::g_He) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(3., occ_nums_data::g_Li)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(3., occ_nums_data::g_Li) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(4., occ_nums_data::g_Be)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(4., occ_nums_data::g_Be) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(5., occ_nums_data::g_B)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(5., occ_nums_data::g_B) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(6., occ_nums_data::g_C)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(6., occ_nums_data::g_C) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(7., occ_nums_data::g_N)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(7., occ_nums_data::g_N) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(8., occ_nums_data::g_O)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(8., occ_nums_data::g_O) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(9., occ_nums_data::g_F)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(9., occ_nums_data::g_F) << "\n";

	//std::cout << eff_z::zeroth_order::z_star_0th(10., occ_nums_data::g_Ne)
		//<< " "
		//<< eff_z::zeroth_order::e_0th(10., occ_nums_data::g_Ne) << "\n";

	//std::cout <<
		//eff_z::zeroth_order::z_star_0th
		//(20., atomic_data::occ_nums_data::g[19]) << " "
		//<< eff_z::zeroth_order::e_0th
		//(20., atomic_data::occ_nums_data::g[19]) << "\n";

	//putenv((char*)"PYTHONDONTWRITEBYTECODE=1");
	//Py_Initialize();
	//PyRun_SimpleString("import sys\n" "import os");
	//PyRun_SimpleString("sys.path.append(os.getcwd() + \"/src\")");
	////PyRun_SimpleString("print(sys.path)");
	//zeroth_order::print_rho_h_l(occ_nums_data::g_He);
	//PyObject *real = get_sympy_Symbol("r", "real", "positive");
	//PyObject *symb = get_sympy_Symbol("symb");
	//print_PyObject(get_assumptions0_sympy_Symbol(real), get_assumptions0_sympy_Symbol(symb));
	//PyObject *rho_he = zeroth_order::computed_rho_h_l(occ_nums_data::g_He);
	//std::wcout << sympy_Object_to_latex(rho_he) << "\n";
	//PyObject *rho_be_fourier
		//= zeroth_order::computed_rho_h_l_fourier(occ_nums_data::g_Be);
	//std::wcout << sympy_Object_to_latex(rho_be_fourier) << "\n";
	//pprint_sympy_Object(rho_be_fourier);
	//PyObject *asf_be
		//= zeroth_order::computed_asf_h_l(occ_nums_data::g_Be);
	//std::wcout << sympy_Object_to_latex(asf_be) << "\n";
	//pprint_sympy_Object(asf_be);

	//if(Py_FinalizeEx() < 0){
		//return 120;
	//}




	return EXIT_SUCCESS;

} catch(const std::exception &e){
	std::cerr << e.what() << "\n";
	return EXIT_FAILURE;
} catch(...){
	return EXIT_FAILURE;
}
