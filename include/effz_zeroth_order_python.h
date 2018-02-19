#ifndef EFFZ_ZEROTH_ORDER_PYTHON_H
#define EFFZ_ZEROTH_ORDER_PYTHON_H

#include <Python.h>


#include <vector>
#include <array>


namespace eff_z{
	namespace zeroth_order{
		PyObject* get_h_l_rnl();
		PyObject* get_rho_h_l_p();
		PyObject* get_rho_h_l();
		PyObject* get_rho_h_l_fourier();
		PyObject* get_asf_h_l();

		void print_rho_h_l_4(const std::vector<std::array<int,4>> &g);

		int python_test();

	} /* end namespace zeroth_order */
} /* end namespace eff_z */

#endif /* EFFZ_ZEROTH_ORDER_PYTHON_H */
