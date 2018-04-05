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

#include "effz_python_utility.h"

#include "effz_exceptions.h"

#include <iostream>

namespace eff_z{

	void print_PyObject(PyObject *obj){
		PyObject *builtins, *print, *arg, *result;
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

			result = PyObject_CallObject(print, arg);
			Py_DECREF(arg);
			Py_DECREF(print);

			if(!result){
				throw python_exception("python call error");
			}
		} catch(const python_exception& e){
			PyErr_Print();
			std::cerr << e.what() << "\n";
		}
	}

	void pprint_sympy_Object(PyObject *obj){
		PyObject *sympy, *pprint, *arg, *result;
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

			result = PyObject_CallObject(pprint, arg);
			Py_DECREF(arg);
			Py_DECREF(pprint);

			if(!result){
				throw python_exception("python call error");
			}
		} catch(const python_exception& e){
			PyErr_Print();
			std::cerr << e.what() << "\n";
		}
	}

	std::wstring sympy_Object_to_latex(PyObject *obj){
		PyObject *sympy, *pprint, *arg, *string;
		try{
			if(!obj){
				throw python_exception("obj to print is NULL");
			}
			sympy = PyImport_ImportModule("sympy");
			if(!sympy){
				throw python_exception("python alloc error");
			}

			pprint = PyObject_GetAttrString(sympy, "latex");
			Py_DECREF(sympy);
			if(!pprint){
				throw python_exception("python alloc error");
			}

			arg = Py_BuildValue("(O)", obj);
			if(!arg){
				throw python_exception("python alloc error");
			}

			string = PyObject_CallObject(pprint, arg);
			Py_DECREF(arg);
			Py_DECREF(pprint);

			if(!string){
				throw python_exception("python call error");
			}

			Py_UNICODE* c_str = PyUnicode_AS_UNICODE(string);
			std::wstring res(c_str);
			return res;
		} catch(const python_exception& e){
			PyErr_Print();
			std::cerr << e.what() << "\n";
			return std::wstring();
		}
	}

	std::wstring sympy_Object_to_string(
			PyObject *obj, const std::string &printer)
	{
		PyObject *sympy, *pprint, *arg, *string;
		try{
			if(!obj){
				throw python_exception("obj to print is NULL");
			}
			sympy = PyImport_ImportModule("sympy");
			if(!sympy){
				throw python_exception("python alloc error");
			}

			pprint = PyObject_GetAttrString(sympy, printer.c_str());
			Py_DECREF(sympy);
			if(!pprint){
				throw python_exception("python alloc error");
			}

			arg = Py_BuildValue("(O)", obj);
			if(!arg){
				throw python_exception("python alloc error");
			}

			string = PyObject_CallObject(pprint, arg);
			Py_DECREF(arg);
			Py_DECREF(pprint);

			if(!string){
				throw python_exception("python call error");
			}

			Py_UNICODE* c_str = PyUnicode_AS_UNICODE(string);
			std::wstring res(c_str);
			return res;
		} catch(const python_exception& e){
			PyErr_Print();
			std::cerr << e.what() << "\n";
			return std::wstring();
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

		PyObject *get_assumptions0_sympy_Symbol(PyObject *s){
			PyObject *assumptions0;
			try{
				if(!s){
					throw python_exception("symbol is NULL");
				}

				assumptions0 = PyObject_GetAttrString(s, "assumptions0");
				if(!assumptions0){
					throw python_exception("error calling assumptions0");
				}

			} catch(const python_exception& e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}

			return assumptions0;
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
