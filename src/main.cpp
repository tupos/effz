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
	//std::cout << eff_z::zeroth_order::z_star_0th(2., g_He) << " "
	//<< eff_z::zeroth_order::e_0th(2., g_He) << "\n";
	//std::cout << eff_z::zeroth_order::z_star_0th(3., g_Li) << " "
	//<< eff_z::zeroth_order::e_0th(3., g_Li) << "\n";
	//std::cout << eff_z::zeroth_order::z_star_0th(4., g_Be) << " "
	//<< eff_z::zeroth_order::e_0th(4., g_Be) << "\n";
	//std::cout << eff_z::zeroth_order::z_star_0th(10., g_Ne) << " "
	//<< eff_z::zeroth_order::e_0th(10., g_Ne) << "\n";
	std::cout <<
		eff_z::zeroth_order::z_star_0th
		(20., eff_z::atomic_data::occ_nums::g[19]) << " "
		<< eff_z::zeroth_order::e_0th
		(20., eff_z::atomic_data::occ_nums::g[19]) << "\n";
	//eff_z::zeroth_order::density_0th rho_Ne(10.,g_Ne);
	//double r = 0.5;
	//while(r < 10.){
	//std::cout << rho_Ne(r,0.,0.) << "\n";
	//r += 0.5;
	//}

	//std::vector<std::array<int,5>> exchange_quantum_nums;
	//for(int n = 1; n <= 6; ++n){
	//for(int l = 0; l <= n - 1; ++l){
	//for(int n1 = 1; n1 <= 6; ++n1){
	//for(int l1 = 0; l1 <= n1 - 1; ++l1){
	//for(int k = std::abs(l-l1); k <= l+l1; ++k){
	//exchange_quantum_nums
	//.push_back({n,l,n1,l1,k});
	//}
	//}
	//}
	//}
	//}
	//for(auto &g: exchange_quantum_nums){
	//std::cout << "{" << g[0] << "," << g[1] << "," << g[2]
	//<< "," << g[3] << "," << g[4] << "} ";
	//std::cout << eff_z::zeroth_order::i_exchange(g[0],g[1],
	//g[2],g[3],g[4]) << "\n";
	//}

	eff_z::zeroth_order::python_test();
	Py_Initialize();
	PyRun_SimpleString("import sys\n" "import os");
	PyRun_SimpleString("sys.path.append(os.getcwd() + \"/src\")");
	//PyRun_SimpleString("print(sys.path)");
	//PyObject *tmp
	//= eff_z::occ_nums_to_PyObject(eff_z::atomic_data::occ_nums::g[10]);
	//PyObject *tmp1
	//= eff_z::occ_nums_to_PyObject(g_He);
	//PyObject *tmp2
	//= eff_z::occ_nums_to_PyObject(g_Li);
	//eff_z::print_PyObject(tmp, tmp1, tmp2);
	zeroth_order::print_rho_h_l(g_He);
	PyObject *real = get_sympy_Symbol("r", "real", "positive");
	PyObject *symb = get_sympy_Symbol("symb");
	print_PyObject(get_assumptions0_sympy_Symbol(real), get_assumptions0_sympy_Symbol(symb));
	PyObject *rho_he = zeroth_order::computed_rho_h_l(g_He);
	std::wcout << sympy_Object_to_latex(rho_he) << "\n";
	auto vec = sympy_Objects_to_latex(rho_he,rho_he);
	std::wcout << vec[0] << "\n" << vec[1] << "\n";

	if(Py_FinalizeEx() < 0){
		return 120;
	}



	return EXIT_SUCCESS;

} catch(const std::exception &e){
	return EXIT_FAILURE;
} catch(...){
	return EXIT_FAILURE;
}
