from sympy import *

x, t, z, nu, r = symbols('x t z nu r')
init_printing(use_unicode=True)
a = integrate(exp(-r) * r**2, r)
print(a)
