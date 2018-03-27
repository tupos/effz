#ifndef MONTE_COMPLEX_H
#define MONTE_COMPLEX_H

#include <complex>

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_plain.h>
#include <gsl/gsl_monte_miser.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_sf_result.h>

typedef std::complex<double> Complex;

struct gsl_monte_function_complex_struct {
	Complex (*f)(double * x_array, size_t dim, void * params);
	size_t dim;
	void * params;
};

typedef struct gsl_monte_function_complex_struct gsl_monte_function_complex;

#define GSL_MONTE_FN_COMPLEX_EVAL(F,x) (*((F)->f))(x,(F)->dim,(F)->params)

inline double 
gsl_monte_function_complex_re(double * x_array, size_t dim, void * params)
{	
	Complex val =
	   	GSL_MONTE_FN_COMPLEX_EVAL((gsl_monte_function_complex*)params, x_array);
	return val.real();
}
inline double
gsl_monte_function_complex_im(double * x_array, size_t dim, void * params)
{
	Complex val =
	   	GSL_MONTE_FN_COMPLEX_EVAL((gsl_monte_function_complex*)params, x_array);
	return val.imag(); 
}


#endif /* MONTE_COMPLEX_H */
