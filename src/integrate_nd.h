#ifndef INTEGRATE_ND_H
#define INTEGRATE_ND_H

#include <complex>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_monte.h>

typedef std::complex<double> Complex;

namespace intO {
  struct gsl_complex_monte_function_struct {
    Complex (*f)(double * x_array, size_t dim, void * params);
    size_t dim;
    void * params;
  };

  typedef struct gsl_complex_monte_function_struct gsl_complex_monte_function;

#define GSL_COMPLEX_MONTE_FN_EVAL(F,x) (*((F)->f))(x,(F)->dim,(F)->params)

  double gsl_complex_monte_function_re(double * x_array, size_t dim, void * params);
  double gsl_complex_monte_function_im(double * x_array, size_t dim, void * params);
  
  struct _res{
    double res;
    double error;
  };
  _res integrate_nd(gsl_monte_function *F, double xa, double xb);
  _res* integrate_nd(gsl_complex_monte_function *F, double xa, double xb);

}  // intO


#endif /* INTEGRATE_ND_H */
