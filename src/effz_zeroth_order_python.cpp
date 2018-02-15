#include "effz_zeroth_order_python.h"

#include <cstdlib>

PyObject* p_matrix_from_int_array(int *arr, size_t size1, size_t size2){
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
			PyObject* item_ij = PyLong_FromLong(arr[size2 * i + j]);
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

PyObject *p_get_sympy_Symbol(const char *s_name){
	PyObject *sympy = PyImport_ImportModule("sympy");
	if(!sympy)
		return NULL;
	PyObject *symbol = PyObject_GetAttrString(sympy, "Symbol");
	Py_DECREF(sympy);
	if(!symbol)
		return NULL;
	PyObject *symbol_instance
		= PyObject_CallFunction(symbol, "s", s_name);
	Py_DECREF(symbol);
	if(!symbol_instance)
		return NULL;
	return symbol_instance;
}

int p_pprint_sympy_Object(PyObject *obj){
	if(!obj && !PyTuple_Check(obj))
		return -1;

	PyObject *sympy = PyImport_ImportModule("sympy");
	if(!sympy)
		return -1;

	PyObject *pretty_print
		= PyObject_GetAttrString(sympy, "pretty_print");
	Py_DECREF(sympy);
	if(!pretty_print)
		return -1;

	PyObject *res = PyObject_CallObject(pretty_print, obj);
	Py_DECREF(pretty_print);
	if(!res)
		return -1;
	return 0;
}
//int p_effz_print_rho_h_l_4(const int *g, size_t g_len){
	//return 0;
//}

//int p_effz_print_rho_h_l
//(const int *g, size_t g_len, const char* format_s, ...)
//{
	//size_t fs_len = strlen(format_s);
	//if(fs_len > 3)
		//return -1;
	//if(fs_len == 0){
		//if(p_effz_print_rho_h_l_4(g, g_len))
			//return -1;
		//return 0;
	//}
	//size_t l = 0;
	//while(!format_s){

	//}

//}


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
	PyRun_SimpleString("print(sys.path)");

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
			PyObject *z = p_get_sympy_Symbol("z");
			PyObject *r = p_get_sympy_Symbol("r");
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

			p_h_l_rnl_val = PyObject_CallObject(p_h_l_rnl, p_h_l_rnl_args);
			PyObject *t = PyObject_CallMethod(p_h_l_rnl_val, "simplify","");
			Py_DECREF(p_h_l_rnl_val);
			p_h_l_rnl_val = t;

			if(p_h_l_rnl_val != NULL){
				PyTuple_SetItem(p_my_print_args, 0, p_h_l_rnl_val);
				p_pprint_sympy_Object(p_my_print_args);

				p_my_print_args_val
					= PyObject_CallObject(p_my_print, p_my_print_args);

				Py_DECREF(p_my_print_args);
				Py_DECREF(p_h_l_rnl_args);
				/*printf("Result of call: %ld\n", PyLong_AsLong(p_my_print_args_val));*/
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
