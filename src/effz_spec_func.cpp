/*
Copyright 2018 Oleg Skoromnik

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "effz_spec_func.h"

#include <gsl/gsl_sf_coulomb.h>
#include <gsl/gsl_sf_coupling.h>
#include <gsl/gsl_sf_legendre.h>

#ifndef EFFZ_ZEROTH_ORDER
#include <sp_func/whittakerM.h>
#include <sp_func/whittakerW.h>
#include <sp_func/gamma.h>
#endif

#include <cmath>



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
#ifndef EFFZ_ZEROTH_ORDER
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
		///[> arb_whittaker_M(nu, (double)l + 1./2., 2. * z / nu * r2)
		///[> arb_whittaker_W(nu, (double)l + 1./2., 2. * z / nu * r1);

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
#endif
	double three_j_symbol(
			const std::array<std::array<int,3>,2> &jm){
		int two_ja = 2 * jm[0][0];
		int two_jb = 2 * jm[0][1];
		int two_jc = 2 * jm[0][2];
		int two_ma = 2 * jm[1][0];
		int two_mb = 2 * jm[1][1];
		int two_mc = 2 * jm[1][2];
		return gsl_sf_coupling_3j(
				two_ja,two_jb,two_jc,
				two_ma,two_mb,two_mc);

	}
	std::complex<double> sph_harm_y(
			const int l,
			const int m,
			const double theta,
			const double phi
			)
	{
		using namespace std::complex_literals;
		const double x = cos(theta);
		std::complex<double> e = exp(
				std::complex<double>(1i * static_cast<double>(m) * phi));
		const int ll = (l <= -1) ? -(l+1) : l;
		int mm = m;
		int prefactor = 1;
		if(m < 0){
			mm = -m;
			prefactor = ((mm % 2) == 0) ? 1 : -1;
		}
		return e * gsl_sf_legendre_sphPlm(ll,mm,x)
			* static_cast<double>(prefactor);
	}
} /* end namespace eff_z*/
