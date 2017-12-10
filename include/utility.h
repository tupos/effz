#ifndef UTILITY_H
#define UTILITY_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>

void print_gsl_matrix_int(gsl_matrix_int *m);
void print_gsl_vector_int(gsl_vector_int *v);
void print_gsl_matrix(gsl_matrix *m);
int print_matrix(const gsl_matrix* m, const char* fname);
int print_vector(const gsl_vector* v, const char* fname);


inline gsl_matrix_int 
*matrix_to_gsl_matrix_int(const int *arr, int size1, int size2){
	gsl_matrix_int *m = gsl_matrix_int_alloc(size1, size2);
	for(int i = 0; i < size1; ++i){
		for(int j = 0; j < size2; ++j){
			int value = *(arr + i*size2 + j);
			gsl_matrix_int_set(m, i, j, value);
		}
	}
	return m;
}

inline gsl_vector_int
*array_to_gsl_vector_int(const int *arr, int size){
	gsl_vector_int *v = gsl_vector_int_alloc(size);
	for(int i = 0; i < size; ++i){
		gsl_vector_int_set(v, i, arr[i]);
	}
	return v;
}



#endif /* UTILITY_H */
