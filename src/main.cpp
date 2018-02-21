#include <Python.h>
#include <iostream>
#include <vector>
#include <array>

#include "effz_atomic_data.h"
#include "effz_zeroth_order.h"
#include "effz_python_utility.h"
#include "effz_zeroth_order_python.h"


using namespace eff_z;


int main(int argc, char *argv[]) try {

	std::cout << "Effective Z program\n";

	std::vector<std::array<int,4>> g_He = {{1,0,0,1},{1,0,0,-1}};
	std::vector<std::array<int,4>> g_Li = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1}
	};
	std::vector<std::array<int,4>> g_Be = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1},{2,0,0,-1}
	};
	std::vector<std::array<int,4>> g_Bo = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1},{2,0,0,-1},
		{2,1,-1,1}
	};
	std::vector<std::array<int,4>> g_Ne = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1},{2,0,0,-1},
		{2,1,-1,1},{2,1,0,1},{2,1,1,1},
		{2,1,-1,-1},{2,1,0,-1},{2,1,1,-1}
	};
	std::cout << eff_z::zeroth_order::z_star_0th(5., g_Bo) << " "
		<< eff_z::zeroth_order::e_0th(5., g_Bo) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(2., g_He) << " "
	<< eff_z::zeroth_order::e_0th(2., g_He) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(3., g_Li) << " "
	<< eff_z::zeroth_order::e_0th(3., g_Li) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(4., g_Be) << " "
	<< eff_z::zeroth_order::e_0th(4., g_Be) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(10., g_Ne) << " "
	<< eff_z::zeroth_order::e_0th(10., g_Ne) << "\n";
	std::cout <<
		eff_z::zeroth_order::z_star_0th
		(20., eff_z::atomic_data::occ_nums::g[19]) << " "
		<< eff_z::zeroth_order::e_0th
		(20., eff_z::atomic_data::occ_nums::g[19]) << "\n";

	putenv((char*)"PYTHONDONTWRITEBYTECODE=1");
	Py_Initialize();
	PyRun_SimpleString("import sys\n" "import os");
	PyRun_SimpleString("sys.path.append(os.getcwd() + \"/src\")");
	//PyRun_SimpleString("print(sys.path)");
	zeroth_order::print_rho_h_l(g_He);
	PyObject *real = get_sympy_Symbol("r", "real", "positive");
	PyObject *symb = get_sympy_Symbol("symb");
	print_PyObject(get_assumptions0_sympy_Symbol(real), get_assumptions0_sympy_Symbol(symb));
	PyObject *rho_he = zeroth_order::computed_rho_h_l(g_He);
	std::wcout << sympy_Object_to_latex(rho_he) << "\n";
	PyObject *rho_be_fourier = zeroth_order::computed_rho_h_l_fourier(g_Be);
	std::wcout << sympy_Object_to_latex(rho_be_fourier) << "\n";
	pprint_sympy_Object(rho_be_fourier);
	PyObject *asf_be = zeroth_order::computed_asf_h_l(g_Be);
	std::wcout << sympy_Object_to_latex(asf_be) << "\n";
	pprint_sympy_Object(asf_be);

	if(Py_FinalizeEx() < 0){
		return 120;
	}



	return EXIT_SUCCESS;

} catch(const std::exception &e){
	return EXIT_FAILURE;
} catch(...){
	return EXIT_FAILURE;
}
