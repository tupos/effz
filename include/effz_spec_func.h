#ifndef EFFZ_SPEC_FUNC_H
#define EFFZ_SPEC_FUNC_H

#include <complex>
#include <array>


namespace eff_z {
	/*
	 *abbreviations 
	 *    h_*** --- hydrogen_***, i.e. charge = 1
	 *    h_l_*** --- hydrogen_like_***, i.e. charge != 1
	 *    ***_gsl function with a signature of a gsl one
	 */

	double h_rnl(const int n, const int l, const double r);

	struct h_rnl_params{
		int n;
		int l;
	};
	double h_rnl_gsl(double r, void *h_rnl_params_);

	double h_l_rnl(
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
	double h_l_rnl_gsl(double r, void *h_l_rnl_params_);

	std::complex<double> green_coulomb_h_rad(
			const std::complex<double> E,
			const int l,
			const std::array<double, 2> &r
			);

	struct gf_h_params{
		std::complex<double> E;
		int l;
	};
	std::complex<double> green_coulomb_h_rad_gsl(
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
	std::complex<double> green_coulomb_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_l_params_
			);

	inline double h_E(int n){
		double nn = (double)n;
		return - 1. / (2. * nn * nn);
	}

	inline double h_l_E(double z, int n){
		double nn = (double)n;
		return - z * z / (2. * nn * nn);
	}

	double three_j_symbol(const std::array<std::array<int,3>,2> &jm);


} /*end namespace eff_z*/

#endif /* EFFZ_SPEC_FUNC_H */
