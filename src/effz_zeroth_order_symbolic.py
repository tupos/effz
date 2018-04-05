#/*
#Copyright 2018 Oleg Skoromnik
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#*/

from sympy import *
init_printing()

z, r, q, s = symbols('z r q s', positive = True)
theta, phi = symbols('theta phi', real = True)
n = symbols('n', integer = True, postive = True)
l, m = symbols('l m', integer = True)
a_bohr = 0.52917721067

def h_l_rnl(z,n,l,r):
    z1, r1 = symbols('z1 r1', real = True)
    n1, l1 = symbols('n1 l1', integer = True)
    x = Integer(2) * z1 * r1 / n1
    norm = (
            1 / factorial(Integer(2) * l1 + Integer(1))
            * (2*z1/n1)**(Rational(3,2))
            * sqrt(
                factorial(n1+l)
                /factorial(n1-l1-Integer(1))
                /(Integer(2)*n1)
                )
            )
    f = norm * x**l1 * exp(-x/2) * hyper((-n1+l1+1,),[2*l1+2],x)
    return f.subs([(z1,z),(n1,n),(l1,l),(r1,r)])

def rho_h_l_p(z,n,l,m,r,theta,phi):
    theta1,phi1 = symbols('theta1 phi1', real = True)
    m1 = symbols('m1', integer = True)
    res = (
        (abs(Ynm(l,m1,theta1,phi1))**2).subs(
            [(m1,m),(theta1,theta),(phi1,phi)]).expand(func=True)
        * abs(h_l_rnl(z,n,l,r))**2
    )
    return res

def rho_h_l(z,g,r,theta,phi):
    s1 = Integer(0)
    for g_i in g:
        n=g_i[0]; l=g_i[1]; m=g_i[2]
        s1 += rho_h_l_p(z,n,l,m,r,theta,phi)
    return s1.simplify()

def rho_h_l_fourier(z,g,q):
    return integrate(
                    r*r * rho_h_l(z,g,r,theta,phi)
                    * exp(I*q*r*cos(theta)) * sin(theta),
                    (phi,0,2*pi),(theta,0,pi),(r,0,oo)
            ).simplify().factor()

def asf_h_l(z,g,s):
    return rho_h_l_fourier(z,g,q).subs(q,Integer(4)*pi*a_bohr*s)

def multiply(a,b):
    print("Will compute", a, "*", b)
    c = 0
    for i in range(0,a):
        c = c + b
    return c

def my_print(a):
    print(type(a))
    print(a)
