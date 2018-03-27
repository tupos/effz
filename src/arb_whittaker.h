#ifndef ARB_WHITTAKER_H
#define ARB_WHITTAKER_H

#include <complex>
#include <cmath>
#include "arbcmath.h"

typedef std::complex<double> Complex;

inline Complex arb_whittaker_M(Complex k, Complex mu, Complex z){
  return exp(-1./2. * z) * pow(z, 1./2. + mu)
    * ac_hyp1f1(1./2. + mu - k, 1. + 2. * mu, z);
}

inline Complex arb_whittaker_W(Complex k, Complex mu, Complex z){
  return exp(-1./2. * z) * pow(z, 1./2. + mu)
    * ac_hyperu(1./2. + mu - k, 1. + 2. * mu, z);
}



#endif /* ARB_WHITTAKER_H */
