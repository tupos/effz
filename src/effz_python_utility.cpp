#include "effz_python_utility.h"

#include "effz_exceptions.h"

#include <iostream>

namespace eff_z{

	void print_PyObject(PyObject *obj){
		PyObject *builtins, *print, *arg;
		try{
			if(!obj){
				throw python_exception("obj to print is NULL");
			}
			builtins = PyImport_ImportModule("builtins");
			if(!builtins){
				throw python_exception("python alloc error");
			}

			print = PyObject_GetAttrString(builtins, "print");
			Py_DECREF(builtins);
			if(!print){
				throw python_exception("python alloc error");
			}

			arg = Py_BuildValue("(O)", obj);
			if(!arg){
				throw python_exception("python alloc error");
			}

			if(!PyObject_CallObject(print, arg)){
				throw python_exception("python call error");
			}
			Py_DECREF(arg);
			Py_DECREF(print);
		} catch(const python_exception& e){
			Py_XDECREF(arg);
			Py_XDECREF(print);
			Py_XDECREF(builtins);
			PyErr_Print();
			std::cerr << e.what() << "\n";
		}
	}

	void pprint_sympy_Object(PyObject *obj){
		PyObject *sympy, *pprint, *arg;
		try{
			if(!obj){
				throw python_exception("obj to print is NULL");
			}
			sympy = PyImport_ImportModule("sympy");
			if(!sympy){
				throw python_exception("python alloc error");
			}

			pprint = PyObject_GetAttrString(sympy, "pretty_print");
			Py_DECREF(sympy);
			if(!pprint){
				throw python_exception("python alloc error");
			}

			arg = Py_BuildValue("(O)", obj);
			if(!arg){
				throw python_exception("python alloc error");
			}

			if(!PyObject_CallObject(pprint, arg)){
				throw python_exception("python call error");
			}
			Py_DECREF(arg);
			Py_DECREF(pprint);
		} catch(const python_exception& e){
			Py_XDECREF(arg);
			Py_XDECREF(pprint);
			Py_XDECREF(sympy);
			PyErr_Print();
			std::cerr << e.what() << "\n";
		}
	}

	PyObject *
		occ_nums_to_PyObject(const std::vector<std::array<int,4>> &g){
			Py_ssize_t len1 = g.size();
			PyObject *list = PyList_New(len1);
			try{
				if(!list)
					throw python_exception("error allocating python list");
				for(Py_ssize_t i = 0; i < len1; ++i){
					PyObject *g_i = PyList_New(4);
					if(!g_i){
						Py_DECREF(list);
						throw python_exception
							("error allocating python list");
					}
					for(Py_ssize_t j = 0; j < 4; ++j){
						PyObject *g_ij = PyLong_FromLong(g.at(i)[j]);
						if(!g_ij){
							Py_DECREF(g_i);
							Py_DECREF(list);
							throw python_exception(
									"error allocating python long");
						}
						PyList_SET_ITEM(g_i, j, g_ij);
					}
					PyList_SET_ITEM(list, i, g_i);
				}
			} catch(const python_exception &e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}

			return list;
		}

		PyObject* p_matrix_from_int_array
			(int *arr, size_t size1, size_t size2)
			{
				if(!arr || !size1 || !size2)
					return NULL;
				Py_ssize_t len1 = size1;
				Py_ssize_t len2 = size2;
				PyObject *list = PyList_New(len1);
				if(!list){
					goto error;
				}
				for(Py_ssize_t i = 0; i < len1; ++i){
					PyObject* row = PyList_New(len2);
					if(row == NULL)
						goto error;
					for(Py_ssize_t j = 0; j < len2; ++j){
						PyObject* item_ij
							= PyLong_FromLong(arr[size2 * i + j]);
						if(item_ij == NULL)
							goto error;
						PyList_SET_ITEM(row, j, item_ij);
					}
					PyList_SET_ITEM(list, i, row);
				}
				return list;
error:
				PyErr_Print();
				fprintf(stderr, "Error\n");
				return NULL;
			}

		PyObject *get_sympy_Symbol(const char *s_name){
			PyObject *sympy, *symbol, *symbol_instance;
			try{
				sympy = PyImport_ImportModule("sympy");
				if(!sympy){
					throw python_exception("error finding sympy");
				}

				symbol = PyObject_GetAttrString(sympy, "Symbol");
				Py_DECREF(sympy);
				if(!symbol){
					throw python_exception("error finding sympy");
				}

				symbol_instance
					= PyObject_CallFunction(symbol, "s", s_name);
				Py_DECREF(symbol);
				if(!symbol_instance){
					throw python_exception("error finding sympy");
				}

			} catch(const python_exception& e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}

			return symbol_instance;
		}
} /* end namespace eff_z */
