#ifndef EFFZ_ZEROTH_ORDER_PYTHON_H
#define EFFZ_ZEROTH_ORDER_PYTHON_H

#include <Python.h>

#include "effz_typedefs.h"

#include <vector>
#include <array>
#include <string>
#include <memory>


namespace eff_z{
	namespace zeroth_order{

		PyObject* computed_rho_h_l(const occ_nums_array &g);
		PyObject* computed_rho_h_l_fourier(const occ_nums_array &g);
		PyObject* computed_asf_h_l(const occ_nums_array &g);
		void print_rho_h_l(const occ_nums_array &g);
		void print_rho_h_l_fourier(const occ_nums_array &g);
		void print_asf_h_l(const occ_nums_array &g);

		class symbolic_density{
			public:
				symbolic_density(const occ_nums_array &g);
			private:
				std::string density_latex;
				std::shared_ptr<PyObject> density_ptr;

		};


	} /* end namespace zeroth_order */
} /* end namespace eff_z */

#endif /* EFFZ_ZEROTH_ORDER_PYTHON_H */
