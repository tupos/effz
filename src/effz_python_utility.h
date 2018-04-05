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

#ifndef EFFZ_PYTHON_UTILITY_H
#define EFFZ_PYTHON_UTILITY_H

#include <Python.h>

#include "effz_exceptions.h"
#include "effz_utility.h"

#include <vector>
#include <array>
#include <iostream>
#include <string>

namespace eff_z{

	void print_PyObject(PyObject *obj);
	template<typename T, typename... Args>
		void print_PyObject(T val, Args... args){
			print_PyObject(val);
			print_PyObject(args...);
		}

	void pprint_sympy_Object(PyObject *obj);
	template<typename T, typename... Args>
		void pprint_sympy_Object(T val, Args... args){
			pprint_sympy_Object(val);
			pprint_sympy_Object(args...);
		}

	std::wstring sympy_Object_to_latex(PyObject *obj);
	template<typename... Args>
		std::vector<std::wstring> sympy_Objects_to_latex(Args&&... args)
		{
			return std::vector<std::wstring>{
					sympy_Object_to_latex(std::forward<Args>(args))...};
		}

	std::wstring sympy_Object_to_string(
			PyObject *obj, const std::string &printer);

	PyObject *
		occ_nums_to_PyObject(const std::vector<std::array<int,4>> &g);

	PyObject* p_matrix_from_int_array
		(int *arr, size_t size1, size_t size2);

	PyObject *get_assumptions0_sympy_Symbol(PyObject *s);

	PyObject *get_sympy_Symbol(const char *s_name);

	template<typename T, typename... Args>
		PyObject *get_sympy_Symbol(
				const char *s_name, T val, Args... args)
		{
			auto args_array = make_array(val, args...);
			PyObject *sympy, *symbol, *symbol_instance;
			try{
				sympy = PyImport_ImportModule("sympy");
				if(!sympy){
					throw python_exception("error finding sympy");
				}

				symbol = PyObject_GetAttrString(sympy, "Symbol");
				Py_DECREF(sympy);
				if(!symbol){
					throw python_exception("error building symbol");
				}
				PyObject *symbol_name = Py_BuildValue("(s)", s_name);
				if(!symbol_name){
					throw python_exception("error building py value");
				}

				PyObject *assumptions = PyDict_New();
				if(!assumptions){
					throw python_exception("error building pythod dict");
				}
				for(auto &key: args_array){
					PyDict_SetItemString(assumptions, key, Py_True);
				}

				symbol_instance = PyObject_Call(
						symbol, symbol_name, assumptions);
				Py_DECREF(assumptions);
				Py_DECREF(symbol_name);
				Py_DECREF(symbol);
				if(!symbol_instance){
					throw python_exception("error building sympy symbol");
				}

			} catch(const python_exception& e){
				PyErr_Print();
				std::cerr << e.what() << "\n";
				return NULL;
			}

			return symbol_instance;

		}
} /* end namespace eff_z */

#endif /* EFFZ_PYTHON_UTILITY_H */
