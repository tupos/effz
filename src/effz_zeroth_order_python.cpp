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

#include "effz_zeroth_order_python.h"

#include "effz_exceptions.h"
#include "effz_python_utility.h"

#include <iostream>
#include <functional>


namespace eff_z{
	namespace zeroth_order{
		namespace{
			PyObject* get_effz_py_module();
			PyObject* get_h_l_rnl();
			PyObject* get_rho_h_l_p();
			PyObject* get_rho_h_l();
			PyObject* get_rho_h_l_fourier();
			PyObject* get_asf_h_l();


			PyObject *get_effz_py_module(){
				const char *python_module_name
					= "effz_zeroth_order_symbolic";
				PyObject *module
					= PyImport_ImportModule(python_module_name);
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					return module;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}

			PyObject* get_h_l_rnl(){
				PyObject *module = get_effz_py_module();
				PyObject *h_l_rnl;
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					h_l_rnl = PyObject_GetAttrString(module, "h_l_rnl");
					Py_DECREF(module);
					if(!h_l_rnl){
						throw python_exception(
								"error finding in py module");
					}
					return h_l_rnl;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}

			PyObject* get_rho_h_l_p(){
				PyObject *module = get_effz_py_module();
				PyObject *rho_h_l_p;
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					rho_h_l_p = PyObject_GetAttrString(module, "rho_h_l_p");
					Py_DECREF(module);
					if(!rho_h_l_p){
						throw python_exception(
								"error finding in py module");
					}
					return rho_h_l_p;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}

			PyObject* get_rho_h_l(){
				PyObject *module = get_effz_py_module();
				PyObject *rho_h_l;
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					rho_h_l = PyObject_GetAttrString(module, "rho_h_l");
					Py_DECREF(module);
					if(!rho_h_l){
						throw python_exception(
								"error finding in py module");
					}
					return rho_h_l;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}

			PyObject* get_rho_h_l_fourier(){
				PyObject *module = get_effz_py_module();
				PyObject *rho_h_l_fourier;
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					rho_h_l_fourier
						= PyObject_GetAttrString(module, "rho_h_l_fourier");
					Py_DECREF(module);
					if(!rho_h_l_fourier){
						throw python_exception(
								"error finding in py module");
					}
					return rho_h_l_fourier;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}

			PyObject* get_asf_h_l(){
				PyObject *module = get_effz_py_module();
				PyObject *asf_h_l;
				try{
					if(!module){
						throw python_exception("error finding py module");
					}
					asf_h_l = PyObject_GetAttrString(module, "asf_h_l");
					Py_DECREF(module);
					if(!asf_h_l){
						throw python_exception(
								"error finding in py module");
					}
					return asf_h_l;

				} catch(const python_exception &e){
					PyErr_Print();
					std::cerr << e.what() << "\n";
					return NULL;
				}
			}
		} /* end namespace */

		PyObject* computed_rho_h_l(const occ_nums_array &g){
			PyObject *g_py = occ_nums_to_PyObject(g);
			try{
				if(!g_py){
					throw python_exception(
							"error converting occ nums to python");
				}
				PyObject *rho_h_l = get_rho_h_l();
				if(!rho_h_l){
					throw python_exception("error getting rho_h_l");
				}
				PyObject *arg = PyTuple_New(5);

				PyObject *z
					= get_sympy_Symbol("z", "real", "positive");
				PyObject *r
					= get_sympy_Symbol("r", "real", "positive");
				PyObject *theta = get_sympy_Symbol("theta", "real");
				PyObject *phi = get_sympy_Symbol("phi", "real");

				if(!arg || !z || !r || !theta ||! phi){
					throw python_exception("alloc error");
				}
				PyTuple_SET_ITEM(arg, 0, z);
				PyTuple_SET_ITEM(arg, 1, g_py);
				PyTuple_SET_ITEM(arg, 2, r);
				PyTuple_SET_ITEM(arg, 3, theta);
				PyTuple_SET_ITEM(arg, 4, phi);

				if(!arg){
					Py_DECREF(rho_h_l);
					throw python_exception("alloc error");
				}

				PyObject *rho_evaluated
					= PyObject_CallObject(rho_h_l, arg);
				Py_DECREF(rho_h_l);
				Py_DECREF(arg);
				if(!rho_evaluated){
					throw python_exception("eval error");
				}
				return rho_evaluated;
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}
		}

		PyObject* computed_rho_h_l_fourier(const occ_nums_array &g){
			PyObject *g_py = occ_nums_to_PyObject(g);
			try{
				if(!g_py){
					throw python_exception(
							"error converting occ nums to python");
				}
				PyObject *rho_h_l_fourier = get_rho_h_l_fourier();
				if(!rho_h_l_fourier){
					throw python_exception(
							"error getting rho_h_l_fourier");
				}
				PyObject *arg = PyTuple_New(3);

				PyObject *z
					= get_sympy_Symbol("z", "real", "positive");
				PyObject *q
					= get_sympy_Symbol("q", "real", "positive");

				if(!arg || !z || !q){
					throw python_exception("alloc error");
				}
				PyTuple_SET_ITEM(arg, 0, z);
				PyTuple_SET_ITEM(arg, 1, g_py);
				PyTuple_SET_ITEM(arg, 2, q);

				if(!arg){
					Py_DECREF(rho_h_l_fourier);
					throw python_exception("alloc error");
				}

				PyObject *rho_fourier_evaluated
					= PyObject_CallObject(rho_h_l_fourier, arg);
				Py_DECREF(rho_h_l_fourier);
				Py_DECREF(arg);
				if(!rho_fourier_evaluated){
					throw python_exception("eval error");
				}
				return rho_fourier_evaluated;
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}
		}

		PyObject* computed_asf_h_l(const occ_nums_array &g){
			PyObject *g_py = occ_nums_to_PyObject(g);
			try{
				if(!g_py){
					throw python_exception(
							"error converting occ nums to python");
				}
				PyObject *asf_h_l = get_asf_h_l();
				if(!asf_h_l){
					throw python_exception(
							"error getting rho_h_l_fourier");
				}
				PyObject *arg = PyTuple_New(3);

				PyObject *z
					= get_sympy_Symbol("z", "real", "positive");
				PyObject *s
					= get_sympy_Symbol("s", "real", "positive");

				if(!arg || !z || !s){
					throw python_exception("alloc error");
				}
				PyTuple_SET_ITEM(arg, 0, z);
				PyTuple_SET_ITEM(arg, 1, g_py);
				PyTuple_SET_ITEM(arg, 2, s);

				if(!arg){
					Py_DECREF(asf_h_l);
					throw python_exception("alloc error");
				}

				PyObject *asf_h_l_evaluated
					= PyObject_CallObject(asf_h_l, arg);
				Py_DECREF(asf_h_l);
				Py_DECREF(arg);
				if(!asf_h_l_evaluated){
					throw python_exception("eval error");
				}
				return asf_h_l_evaluated;
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}
		}

		void print_rho_h_l(const occ_nums_array &g){
			try{
				PyObject *rho_evaluated = computed_rho_h_l(g);
				if(!rho_evaluated){
					throw python_exception("eval error");
				}
				pprint_sympy_Object(rho_evaluated);
				Py_DECREF(rho_evaluated);
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
			}
		}

		void print_rho_h_l_fourier(const occ_nums_array &g){
			try{
				PyObject *rho_fourier_evaluated
					= computed_rho_h_l_fourier(g);
				if(!rho_fourier_evaluated){
					throw python_exception("eval error");
				}
				pprint_sympy_Object(rho_fourier_evaluated);
				Py_DECREF(rho_fourier_evaluated);
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
			}
		}

		void print_asf_h_l(const occ_nums_array &g){
			try{
				PyObject *asf_h_l_evaluated
					= computed_asf_h_l(g);
				if(!asf_h_l_evaluated){
					throw python_exception("eval error");
				}
				pprint_sympy_Object(asf_h_l_evaluated);
				Py_DECREF(asf_h_l_evaluated);
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
			}
		}

		symbolic_density::symbolic_density(const occ_nums_array &g)
			: g(g), density_ptr(computed_rho_h_l(g), Py_DecRef),
			density_latex_str(sympy_Object_to_latex(density_ptr.get())),
			density_pretty_str(
					sympy_Object_to_string(density_ptr.get(),"pretty")) { }

		std::string symbolic_density::get_density_latex_str(){
			return eff_z::wstr_to_str(density_latex_str);
		}

		std::string symbolic_density::get_density_pretty_str(){
			return eff_z::wstr_to_str(density_pretty_str);
		}

		symbolic_asf::symbolic_asf(const occ_nums_array &g)
			: g(g), asf_ptr(computed_asf_h_l(g), Py_DecRef),
			asf_latex_str(sympy_Object_to_latex(asf_ptr.get())),
			asf_pretty_str(
					sympy_Object_to_string(asf_ptr.get(),"pretty")) { }

		std::string symbolic_asf::get_asf_latex_str(){
			return eff_z::wstr_to_str(asf_latex_str);
		}

		std::string symbolic_asf::get_asf_pretty_str(){
			return eff_z::wstr_to_str(asf_pretty_str);
		}
	} /* end namespace zeroth_order */
} /* end namespace eff_z */
