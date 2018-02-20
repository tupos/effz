#include "effz_zeroth_order_python.h"

#include "effz_exceptions.h"
#include "effz_python_utility.h"

#include <cstdlib>


namespace eff_z{
	namespace zeroth_order{

		PyObject *get_effz_py_module(){
			const char *python_module_name = "effz_zeroth_order_symbolic";
			PyObject *module = PyImport_ImportModule(python_module_name);
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
					throw python_exception("error finding in py module");
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
					throw python_exception("error finding in py module");
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
					throw python_exception("error finding in py module");
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
					throw python_exception("error finding in py module");
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
					throw python_exception("error finding in py module");
				}
				return asf_h_l;

			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}
		}

		PyObject* computed_rho_h_l
			(const std::vector<std::array<int,4>> &g){
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

		void print_rho_h_l(const std::vector<std::array<int,4>> &g){
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

		//void print_rho_h_l_4args_latex(std::ostream& os,
				//const std::vector<std::array<int,4>> &g){
			//try{
				//PyObject *rho_evaluated = computed_rho_h_l_4args(g);
				//if(!rho_evaluated){
					//throw python_exception("eval error");
				//}

				//Py_DECREF(rho_evaluated);
			//} catch(const python_exception &e){
				//PyErr_Print();
				//std::cerr << e.what() << "\n";
			//}

		//}

		PyObject* computed_rho_h_l_fourier
			(const std::vector<std::array<int,4>> &g){
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

		void print_rho_h_l_fourier(
				const std::vector<std::array<int,4>> &g){
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

		PyObject* computed_asf_h_l
			(const std::vector<std::array<int,4>> &g){
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

		void print_asf_h_l(
				const std::vector<std::array<int,4>> &g){
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

		int python_test()
		{
			putenv((char*)"PYTHONDONTWRITEBYTECODE=1");
			PyObject *pName, *pModule, *p_my_print;
			PyObject *p_my_print_args, *p_my_print_args_val;
			PyObject *p_h_l_rnl, *p_h_l_rnl_args;
			PyObject *p_h_l_rnl_val;
			size_t size1 = 2, size2 = 4;
			int *arr = new int[size1 * size2];
			if(arr == NULL){
				fprintf(stderr, "malloc failure");
				exit(EXIT_FAILURE);
			}
			int a[2][4] = {{1,0,0,1},{1,0,0,-1}};
			for(size_t k = 0; k < size1; ++k){
				for(size_t l = 0; l < size2; ++l){
					arr[size2 * k + l] = a[k][l];
				}
			}

			const char* m_name = "effz_zeroth_order_symbolic";
			const char* f_name = "my_print";

			Py_Initialize();
			PyRun_SimpleString("import sys\n" "import os");
			PyRun_SimpleString("sys.path.append(os.getcwd() + \"/src\")");
			//PyRun_SimpleString("print(sys.path)");

			pName = PyUnicode_DecodeFSDefault(m_name);
			if(pName == NULL){
				fprintf(stderr, "Error decoding python file name.\n");
				Py_XDECREF(pName);
				return 1;
			}

			pModule = PyImport_Import(pName);
			Py_DECREF(pName);

			if(pModule != NULL){
				p_my_print = PyObject_GetAttrString(pModule, f_name);
				p_h_l_rnl = PyObject_GetAttrString(pModule, "h_l_rnl");

				if(p_my_print && PyCallable_Check(p_my_print)
						&& p_h_l_rnl && PyCallable_Check(p_h_l_rnl)){

					p_h_l_rnl_args = PyTuple_New(4);
					PyObject *z = get_sympy_Symbol("z");
					PyObject *r = get_sympy_Symbol("r");
					PyObject *n = Py_BuildValue("i", 1);
					PyObject *l = Py_BuildValue("i", 0);
					p_my_print_args = PyTuple_New(1);
					p_my_print_args_val
						= p_matrix_from_int_array(arr, size1, size2);
					if(!z || !r || !n || !l || !p_my_print_args_val){
						Py_DECREF(p_h_l_rnl_args);
						Py_DECREF(p_my_print_args);
						Py_DECREF(pModule);
						fprintf(stderr, "Error while creating arguments\n");
						return 1;
					}

					PyTuple_SetItem(p_h_l_rnl_args, 0, z);
					PyTuple_SetItem(p_h_l_rnl_args, 1, n);
					PyTuple_SetItem(p_h_l_rnl_args, 2, l);
					PyTuple_SetItem(p_h_l_rnl_args, 3, r);

					p_h_l_rnl_val
						= PyObject_CallObject(p_h_l_rnl, p_h_l_rnl_args);
					PyObject *t
						= PyObject_CallMethod(p_h_l_rnl_val, "simplify","");
					Py_DECREF(p_h_l_rnl_val);
					p_h_l_rnl_val = t;

					if(p_h_l_rnl_val != NULL){
						PyTuple_SetItem(p_my_print_args, 0, p_h_l_rnl_val);
						pprint_sympy_Object(p_h_l_rnl_val);

						p_my_print_args_val
							= PyObject_CallObject(p_my_print, p_my_print_args);

						Py_DECREF(p_my_print_args);
						Py_DECREF(p_h_l_rnl_args);
						Py_DECREF(p_my_print_args_val);
						Py_DECREF(p_h_l_rnl_val);
					} else {
						Py_DECREF(p_my_print);
						Py_DECREF(p_h_l_rnl);
						Py_DECREF(pModule);
						PyErr_Print();
						fprintf(stderr, "Call failed\n");
						return 1;
					}

				} else {
					if(PyErr_Occurred()){
						PyErr_Print();
					}
					fprintf(stderr, "Cannot find function \"%s\"\n", f_name);
				}
				Py_XDECREF(p_my_print);
				Py_XDECREF(p_h_l_rnl);
				Py_DECREF(pModule);
			} else {
				PyErr_Print();
				fprintf(stderr, "Failed to load \"%s\"\n", m_name);
				return 1;
			}
			if(Py_FinalizeEx() < 0){
				return 120;
			}
			delete [] arr;
			return 0;
		}
	} /* end namespace zeroth_order */
} /* end namespace eff_z */
