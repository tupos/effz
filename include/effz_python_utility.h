#ifndef EFFZ_PYTHON_UTILITY_H
#define EFFZ_PYTHON_UTILITY_H

#include <Python.h>

#include <vector>
#include <array>
#include <iostream>

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

	PyObject *
		occ_nums_to_PyObject(const std::vector<std::array<int,4>> &g);

	PyObject* p_matrix_from_int_array
		(int *arr, size_t size1, size_t size2);

	PyObject *get_sympy_Symbol(const char *s_name);
} /* end namespace eff_z */

#endif /* EFFZ_PYTHON_UTILITY_H */
