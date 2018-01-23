#include <iostream>
#include <complex>
#include <cmath>
#include <vector>
#include <string>

#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_sf_result.h>
#include <gsl/gsl_sys.h>
#include <sp_func/sp_func.h>
#include <arbcmath.h>

#include "grid.h"
#include "gsl_monte_complex.h"
#include "arb_whittaker.h"
#include "utility.h"
#include "effz_spec_func.h"
#include "effz_zeroth_order.h"
#include "effz_helper_func.h"

#ifndef IMAG_i
#define IMAG_i std::complex<double>(0, 1.)
#endif
#ifndef ALPHA
#define ALPHA 1. / 137.035999139
#endif

typedef std::complex<double> Complex;
typedef std::vector<double> Vector;

using namespace std;


struct h_rnl_params{
	int n;
	int l;
};
struct gf_params{
  double Z;
  Complex E;
  int l;
};
struct reduced_gf_params{
	double delta;
	double e;
	gsl_vector_int *lambda;
	gsl_matrix_int *g;
	int k;
	double t;
};
struct integrand_params{
	double delta;
	gsl_vector_int *lambda;
	gsl_matrix_int *g;
	int k;
	double t;
};
Complex gf(double *r, int dim, void *params);
double h_rnl(double r, void *params);
inline double h_e(int n){
	double nn = (double)n;
	return - 1. / (2. * nn * nn);
}





int main(int argc, char *argv[]){
	std::cout << "Effective Z program\n";
	//std::cout << eff_z::h_rnl(1,0, 3) << "\n";
	//std::cout << eff_z::green_coulomb_h_rad(1., 0, {2.,1.}) << "\n";
	std::vector<std::array<int,4>> g_He = {{1,0,0,1},{1,0,0,-1}};
	std::vector<std::array<int,4>> g_Li = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1}
	};
	std::vector<std::array<int,4>> g_Be = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1},{2,0,0,-1}
	};
	std::vector<std::array<int,4>> g_Ne = {
		{1,0,0,1},{1,0,0,-1},
		{2,0,0,1},{2,0,0,-1},
		{2,1,-1,1},{2,1,0,1},{2,1,1,1},
		{2,1,-1,-1},{2,1,0,-1},{2,1,1,-1}
	};
	std::cout << eff_z::zeroth_order::z_star_0th(2., g_He) << " "
		<< eff_z::zeroth_order::e_0th(2., g_He) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(3., g_Li) << " "
		<< eff_z::zeroth_order::e_0th(3., g_Li) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(4., g_Be) << " "
		<< eff_z::zeroth_order::e_0th(4., g_Be) << "\n";
	std::cout << eff_z::zeroth_order::z_star_0th(10., g_Ne) << " "
		<< eff_z::zeroth_order::e_0th(10., g_Ne) << "\n";
	return 0;

}

Complex gf(double *r, int dim, void *params){
	if(dim!=2){
		gsl_error("dimesion of coordinates array in green func !=2",
			   	__FILE__, __LINE__, GSL_EDOM);
	}
	double r1 = GSL_MAX_DBL(r[0], r[1]);
	double r2 = GSL_MIN_DBL(r[0], r[1]);
	gf_params *par = (gf_params*)params;
	Complex E = par->E;
	double Z = par->Z;
	int l = par->l;
	Complex nu = Z / sqrt(Complex(-2.,0) * E);
	Complex res = nu / Z * spF::gamma((double)l + 1. - nu) / spF::factorial(2 * l + 1)
		* spF::whittakerM(nu, (double)l + 1./2., 2. * Z / nu * r2)
		* spF::whittakerW(nu, (double)l + 1./2., 2. * Z / nu * r1);
		//Complex res = nu / Z * spF::gamma((double)l + 1. - nu) / spF::factorial(2 * l + 1)
		//* arb_whittaker_M(nu, (double)l + 1./2., 2. * Z / nu * r2)
		//* arb_whittaker_W(nu, (double)l + 1./2., 2. * Z / nu * r1);

	return res;
}

double h_rnl(double r, void *params){
	h_rnl_params *par = (h_rnl_params*)params;
	int n = par->n;
	int l = par->l;
	double norm = 1. / gsl_sf_fact(2. * l + 1.) * pow(2. / n, 3. / 2.)
		* sqrt(gsl_sf_fact(n + l) / gsl_sf_fact(n - l - 1.) / (2. * n));
	double x = 2. * r / n;

	return norm * pow(x, l) * exp(-x / 2.) 
			* spF::hypergeometric1F1(-n + l + 1, 2 * l + 2, x);
} 
