#include "integrate_nd.h"

using namespace intO;

namespace intO {

  struct par_bottom{
    gsl_monte_function *F;
    double *r_lim;
    double *r;
    int N_r;
  };
  double gsl_complex_monte_function_re(double * x_array, size_t dim, void * params){
    return GSL_COMPLEX_MONTE_FN_EVAL(
				     (gsl_complex_monte_function*)params,
				     x_array).real();
  }
  double gsl_complex_monte_function_im(double * x_array, size_t dim, void * params){
    return GSL_COMPLEX_MONTE_FN_EVAL(
				     (gsl_complex_monte_function*)params,
				     x_array).imag();
  }
  int int_routine(const gsl_function *f, double xa, double xb,
		  double *res, double *error){
    // int w_size = 5000;
    gsl_integration_workspace *w
      = gsl_integration_workspace_alloc(30000);
    // gsl_integration_qags(f, xa, xb, 0, 1.e-2, 20000, w, res, error);
    gsl_integration_qag(f, xa, xb, 1.e-3, 0, 30000, 3, w,
    			res, error);
    gsl_integration_workspace_free(w);

    // gsl_integration_glfixed_table *w = gsl_integration_glfixed_table_alloc(10);
    // *res = gsl_integration_glfixed(f, xa, xb, w);
    // gsl_integration_glfixed_table_free(w);
    
    return 0;
  }
  double integrand_nd(double x, void *params){
    par_bottom *par = (par_bottom *)params;
    double r0 = par->r_lim[0];
    double rf = par->r_lim[1];
    int N_r = par->N_r;
    int N_total = (par->F)->dim;
    double *r_old = par->r;
    double r_new[N_r + 1];
    for (int i = 0; i < N_r; i++) {
      r_new[i] = r_old[i];
    }
    r_new[N_r] = x;
    par_bottom par_new = {par->F, par->r_lim, r_new, N_r + 1};
    if(N_r == N_total - 1){
      
      return GSL_MONTE_FN_EVAL(par_new.F, r_new);
    }
    gsl_function f = {&integrand_nd, &par_new};
    double res;
    double error;
    int_routine(&f, r0, rf, &res, &error);

    return res;
  }
  _res integrate_nd(gsl_monte_function *F, double xa, double xb){
    if(F->dim == 1){
      std::cout << "Integral dimension should be larger that 1." << std::endl;
      exit(1);
    }
    double x_lim[2] = {xa, xb};
    par_bottom par = {F, x_lim, NULL, 0};
    _res ans;

    gsl_function f = {&integrand_nd, &par};
    int_routine(&f, xa, xb, &ans.res, &ans.error);
    
    return ans;

  }

  _res* integrate_nd(gsl_complex_monte_function *F, double xa, double xb){
    if(F->dim == 1){
      std::cout << "Integral dimension should be larger that 1." << std::endl;
      exit(1);
    }
    
    gsl_monte_function re = {&gsl_complex_monte_function_re, F->dim, F};
    gsl_monte_function im = {&gsl_complex_monte_function_im, F->dim, F};

    _res *ans = (_res *)malloc(sizeof(_res)*2);

    ans[0] = integrate_nd(&re, xa, xb);
    ans[1] = integrate_nd(&im, xa, xb);

    return ans;

  }
  

}  // end namespace intO
