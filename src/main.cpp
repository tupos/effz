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

#include "grid.h"
#include "sp_func/sp_func.h"
#include "gsl_monte_complex.h"
#include "arbcmath.h"
#include "arb_whittaker.h"
#include "utility.h"

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
Complex reduced_gf(double *r, int dim, void *params);
Complex integrand(double *r, size_t dim, void *params); 
int integrate_monte_complex(gsl_monte_function_complex *F, double *xa, 
							double *xb, gsl_sf_result *res);
Complex integrand_with_t(double *x, size_t dim, void *params);

void display_results (std::string title, gsl_sf_result *res){
  printf ("%s ==================\n", title.c_str());
  printf ("result = % .6f + i %.6f\n", res[0].val, res[1].val);
  printf ("error = % .6f + i %.6f\n", res[0].err, res[1].err);
}





int main(int argc, char *argv[]){
	int g_he_array[8] = {1, 0, 0, 1, 
						 1, 0, 0, -1};
	gsl_matrix_int_view g_he_array_view =
					gsl_matrix_int_view_array(g_he_array, 2, 4);
	gsl_matrix_int g_he = g_he_array_view.matrix; 

	int g_li_array[12] = {1, 0, 0, 1, 
						  1, 0, 0, -1,
						  2, 0, 0, 1};
	gsl_matrix_int_view g_li_array_view =
					gsl_matrix_int_view_array(g_li_array, 3, 4);
	gsl_matrix_int g_li = g_li_array_view.matrix; 

	
	// -------------------------------------------------------------------
	


	int lambda_he_1212_arr[4] = {0, 1, 0, 1};
	gsl_vector_int_view lambda_he_1212_view = 
		gsl_vector_int_view_array(lambda_he_1212_arr, 4);
	gsl_vector_int lambda_he_1212 = lambda_he_1212_view.vector;

		
	
	double r_i[4] = {0.0001, 0.0001, 0.0001, 0.0001};
	double r_f[4] = {100., 100., 100., 100.};

	double x_i[5] = {0.0001, 0.0001, 0.0001, 0.0001, 0.};
	double x_f[5] = {100., 100., 100., 100., 5.};
	//double x_tmp[5] = {1., 0.2, 0.3, 0.4, 0};
	//gsl_sf_result ans[2];
	//integrand_params he_1212_params = {0.0001, &lambda_he_1212,
										//&g_he, 0, 0.6};

	//gsl_monte_function_complex F = {&integrand, 4, &he_1212_params};
	//gsl_monte_function_complex G = {&integrand_with_t, 5, &he_1212_params};
	
	//integrate_monte_complex(&F, r_i, r_f, ans);
	
	//integrate_monte_complex(&G, x_i, x_f, ans);

	//display_results("miser", ans);
	//std::cout << integrand(r_i, 4, &he_1212_params);
	int grid_size = 5;
	Grid grid_t(grid_size);
	const double abs_t = 5.;
	const double tb[2] = {0., abs_t}; 
	grid_t.init_unif_x0_xf(tb[0], tb[1]);
	grid_t.print();
	gsl_matrix *f_from_t = gsl_matrix_alloc(grid_size, 3);
#pragma omp parallel for
	for (int i = 0; i < grid_size; i++) {
		double t = grid_t.at(i);
		integrand_params he_1212_params = {0.001, &lambda_he_1212,
										&g_he, 0, t};
		gsl_sf_result ans[2];

		gsl_monte_function_complex F = {&integrand, 4, &he_1212_params};
		integrate_monte_complex(&F, r_i, r_f, ans);
		gsl_matrix_set(f_from_t, i, 0, t);
		gsl_matrix_set(f_from_t, i, 1, ans[0].val);
		gsl_matrix_set(f_from_t, i, 2, ans[1].val);

	}
	print_gsl_matrix(f_from_t);
	print_matrix(f_from_t, "res.txt");
	gsl_matrix_free(f_from_t);

	return 0;

}
Complex integrand_with_t(double *x, size_t dim, void *params){
	integrand_params *par = (integrand_params *)params;
	par->t = x[4];
	double r[4];
	for(int i = 0; i < 4; ++i)
		r[i] = x[i];
	return integrand(r, 4, par);
}
int integrate_monte_complex(gsl_monte_function_complex *F, double *xa, 
							double *xb, gsl_sf_result *ans)
{
	gsl_monte_function re = {&gsl_monte_function_complex_re, F->dim, F};
    gsl_monte_function im = {&gsl_monte_function_complex_im, F->dim, F};

    const gsl_rng_type *T;
	gsl_rng *r;

	size_t calls = 500000;
	gsl_rng_env_setup();
	
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);

	gsl_monte_miser_state *s = gsl_monte_miser_alloc (F->dim);

	gsl_monte_miser_integrate(&re, xa, xb, F->dim, calls, r, s,
							   &ans[0].val, &ans[0].err);

	gsl_monte_miser_init(s);

	gsl_monte_miser_integrate(&im, xa, xb, F->dim, calls, r, s,
							   &ans[1].val, &ans[1].err);

	gsl_monte_miser_free (s);
	gsl_rng_free(r);
	//gsl_monte_plain_state *s = gsl_monte_plain_alloc (F->dim);

	//gsl_monte_plain_integrate(&re, xa, xb, F->dim, calls, r, s,
							   //&ans[0].val, &ans[0].err);

	//gsl_monte_plain_init(s);

	//gsl_monte_plain_integrate(&im, xa, xb, F->dim, calls, r, s,
							   //&ans[1].val, &ans[1].err);

	//gsl_monte_plain_free (s);
	
    return GSL_SUCCESS;

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

Complex reduced_gf(double *r, int dim, void *params){
	reduced_gf_params *par = (reduced_gf_params *)params;

	Complex e1 = par->t + (par->e - IMAG_i * par->delta) / 2.;

	if(par->k == 0){
		int lambda_length = (par->lambda)->size;
		Complex sub = 0.;

		for(int i = 0; i < lambda_length; ++i){
			int lambdai = gsl_vector_int_get(par->lambda,i);
			int ni = gsl_matrix_int_get(par->g, lambdai, 0);
			int li = gsl_matrix_int_get(par->g, lambdai, 1);

			h_rnl_params _h_rnl_params = {ni, li};

			sub += r[0] * r[1]
				* h_rnl(r[0], &_h_rnl_params)
				* h_rnl(r[1], &_h_rnl_params)
				/ (e1 - h_e(ni));

		}
		gf_params _gf_params = {1., e1, 0};
		return gf(r, dim, &_gf_params) + sub;
	}

	gf_params _gf_params = {1., e1, par->k};
	return gf(r, dim, &_gf_params);
}

Complex integrand(double *r, size_t dim, void *params){
	double r_12_s = GSL_MIN_DBL(r[0], r[1]);	
	double r_12_g = GSL_MAX_DBL(r[0], r[1]);	
	double r_12_p_s = GSL_MIN_DBL(r[2], r[3]);
	double r_12_p_g = GSL_MAX_DBL(r[2], r[3]);

	integrand_params *par = (integrand_params *)params;

	int length_g = ( par->g )->size1;
	int length_lambda = ( par->lambda )->size;
	int lambda_0 = gsl_vector_int_get(par->lambda, 0);
	int lambda_1 = gsl_vector_int_get(par->lambda, 1);

	double prod = 1.;
	for(int i = 0; i < 4; ++i){
		int lambdai = gsl_vector_int_get(par->lambda, i);
		int ni = gsl_matrix_int_get(par->g, lambdai, 0);
		int li = gsl_matrix_int_get(par->g, lambdai, 1);

		h_rnl_params _h_rnl_params = {ni, li};

		prod *= r[i] * h_rnl(r[i], &_h_rnl_params); 
	}

	int k = par->k;
	prod *= pow(r_12_s, k) / pow(r_12_g, k+1) 
			* pow(r_12_p_s, k) / pow(r_12_p_g, k+1) 
			/ (2. * k + 1.);
	
	int ms[2] = {gsl_matrix_int_get(par->g, lambda_0, 3),
	   				gsl_matrix_int_get(par->g, lambda_1, 3)};
	
	gsl_vector_int *lambda_sub_tplus = gsl_vector_int_alloc(length_g);
	gsl_vector_int *lambda_sub_tminus = gsl_vector_int_alloc(length_g);

	int indx_lambda_sub_tplus = 0;
	int indx_lambda_sub_tminus = 0;
	for(int i = 0; i < length_g; ++i){
		int ms_i = gsl_matrix_int_get(par->g, i, 3);

		if(ms[0] == ms_i){
			gsl_vector_int_set(lambda_sub_tplus,
				   	indx_lambda_sub_tplus, i);
			indx_lambda_sub_tplus++;
		}

		if(ms[1] == ms_i){
			gsl_vector_int_set(lambda_sub_tminus,
				   	indx_lambda_sub_tminus, i);
			indx_lambda_sub_tminus++;
		}
	}
	gsl_vector_int_view l_sub_t =
	   					gsl_vector_int_subvector(lambda_sub_tplus, 0,
		   										indx_lambda_sub_tplus);
	gsl_vector_int_view l_sub_mt =
	   				gsl_vector_int_subvector(lambda_sub_tminus, 0,
			   									indx_lambda_sub_tminus);
	
	int n_lambda_0 = gsl_matrix_int_get(par->g, lambda_0, 0);
	int n_lambda_1 = gsl_matrix_int_get(par->g, lambda_1, 0);
	double e0 = h_e(n_lambda_0) + h_e(n_lambda_1);

	reduced_gf_params r_gf_params_tplus = {par->delta, 
										   e0, 
										   &l_sub_t.vector, 
										   par->g, 
										   par->k, 
										   par->t};
	reduced_gf_params r_gf_params_tminus = {par->delta, 
											e0, 
											&l_sub_mt.vector, 
											par->g, 
											par->k, 
											-( par->t )};
	double r_t[2] = {r[0], r[2]};
	double r_mt[2] = {r[1], r[3]};
	Complex res = 1. / (2. * M_PI * IMAG_i) * prod
			* reduced_gf(r_t, 2, &r_gf_params_tplus)
			* reduced_gf(r_mt, 2, &r_gf_params_tminus);
	return res; 
}



