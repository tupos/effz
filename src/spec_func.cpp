#include "spec_func.h"

#include <algorithm>
#include <gsl/gsl_sf_coulomb.h>

#include <sp_func/whittakerM.h>
#include <sp_func/whittakerW.h>
#include <sp_func/gamma.h>



namespace eff_z {

	double h_rnl(const int n, const int l, const double r){
		return gsl_sf_hydrogenicR(n, l, 1., r);
	}

	double h_rnl_gsl(double r, void *h_rnl_params_){
		h_rnl_params *params = (h_rnl_params*)h_rnl_params_;
		return gsl_sf_hydrogenicR(params->n, params->l, 1., r);
	}

	double h_l_rnl(
			const double z,
			const int n,
			const int l,
			const double r
			)
	{
		return gsl_sf_hydrogenicR(n, l, z, r);
	}

	double h_l_rnl_gsl(double r, void *h_l_rnl_params_){
		h_l_rnl_params *params = (h_l_rnl_params*)h_l_rnl_params_;
		return gsl_sf_hydrogenicR(params->n, params->l, params->z, r);
	}

	std::complex<double> green_coulomb_h_rad(
			const std::complex<double> E,
			const int l,
			const std::array<double, 2> &r
			)
	{
		return green_coulomb_rad(1., E, l, r);
	}

	std::complex<double> green_coulomb_h_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_params_
			)
	{
		gf_h_params *params = (gf_h_params *)gf_h_params_;
		const std::array<double, 2> r_a{r[0], r[1]};
		return green_coulomb_rad(1., params->E, params->l, r_a);
	}

	std::complex<double> green_coulomb_rad(
			const double z,
			const std::complex<double> E,
			const int l,
			const std::array<double, 2> &r
			)
	{
		double r1 = std::max(r[0], r[1]);
		double r2 = std::min(r[0], r[1]);
		std::complex<double> nu = z / sqrt(std::complex<double>(-2.,0) * E);
		std::complex<double> res = nu / z
			* spF::gamma((double)l + 1. - nu) / spF::factorial(2 * l + 1)
			* spF::whittakerM(nu, (double)l + 1./2., 2. * z / nu * r2)
			* spF::whittakerW(nu, (double)l + 1./2., 2. * z / nu * r1);
		//Complex res = nu / z * spF::gamma((double)l + 1. - nu) / spF::factorial(2 * l + 1)
		//* arb_whittaker_M(nu, (double)l + 1./2., 2. * z / nu * r2)
		//* arb_whittaker_W(nu, (double)l + 1./2., 2. * z / nu * r1);

		return res;
	}

	std::complex<double> green_coulomb_rad_gsl(
			double *r,
			size_t dim,
			void *gf_h_l_params_
			)
	{
		const gf_h_l_params *params = (gf_h_l_params *) gf_h_l_params_;
		const std::array<double, 2> r_a{r[0], r[1]};
		return green_coulomb_rad(params->z, params->E, params->l, r_a);

	}
} /* end namespace eff_z*/
