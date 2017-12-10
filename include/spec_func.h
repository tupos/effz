#ifndef SPEC_FUNC_H
#define SPEC_FUNC_H

#include <complex>

namespace eff_z {

	std::complex<double> green_coulomb_h_rad(
			std::complex<double> const E,
			std::array<double, 2> const &r
			);
	std::complex<double> green_coulomb_h_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_param
			);
	std::complex<double> green_coulomb_rad(
			double const Z,
			std::complex<double> const E,
			std::array<double, 2> const &r
			);
	std::complex<double> green_coulomb_rad_gsl(
			double *r,
			size_t dim,
			void *gf_param
			);

} /*end namespace eff_z*/

#endif /* SPEC_FUNC_H */
