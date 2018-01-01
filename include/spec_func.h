#ifndef SPEC_FUNC_H
#define SPEC_FUNC_H

#include <complex>
#include <array>

namespace eff_z {

	inline double h_rnl(const int n, const int l, const double r);

	struct h_rnl_params{
		int n;
		int l;
	};
	inline double h_rnl_gsl(double r, void *h_rnl_params_);

	inline double h_l_rnl(
			const double z,
			const int n,
			const int l,
			const double r
			);

	struct h_l_rnl_params{
		double z;
		int n;
		int l;
	};
	inline double h_l_rnl_gsl(double r, void *h_l_rnl_params_);

	inline std::complex<double> green_coulomb_h_rad(
			const std::complex<double> E,
			const int l,
			const std::array<double, 2> &r
			);

	struct gf_h_params{
		std::complex<double> E;
		int l;
	};
	inline std::complex<double> green_coulomb_h_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_params_
			);

	std::complex<double> green_coulomb_rad(
			const double z,
			const std::complex<double> E,
			const int l,
			const std::array<double, 2> &r
			);

	struct gf_h_l_params{
		double z;
		std::complex<double> E;
		int l;
	};
	inline std::complex<double> green_coulomb_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_l_params_
			);

} /*end namespace eff_z*/

#endif /* SPEC_FUNC_H */
