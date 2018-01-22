#include "utility.h"

void print_gsl_matrix_int(gsl_matrix_int *m){
	int size1 = m->size1;
	int size2 = m->size2;	
	for(int i = 0; i < size1; ++i){
		for(int j = 0; j < size2; ++j){
			int m_ij = gsl_matrix_int_get(m, i, j);
			printf("%d ", m_ij); 
		}
		printf("\n");
	}
}
void print_gsl_vector_int(gsl_vector_int *v){
	int size = v->size;
	for(int i = 0; i < size; ++i){
		int v_i = gsl_vector_int_get(v, i);
		printf("%d ", v_i); 
	}
	printf("\n");
}
void print_gsl_matrix(gsl_matrix *m){
	int size1 = m->size1;
	int size2 = m->size2;	
	for(int i = 0; i < size1; ++i){
		for(int j = 0; j < size2; ++j){
			double m_ij = gsl_matrix_get(m, i, j);
			printf("%f ", m_ij); 
		}
		printf("\n");
	}
}
int print_matrix(const gsl_matrix* m, const char* fname) {

	FILE* file;

	if ((file = fopen(fname, "w")) == NULL) {
		return 1;
	}
	int status = gsl_matrix_fprintf(file, m, "%f");
	if(status){
		return GSL_EFAILED;
	}
	fclose(file);

	return GSL_SUCCESS;
}
int print_vector(const gsl_vector* v, const char* fname) {
	int size = v->size;

	FILE* file;

	if ((file = fopen(fname, "w")) == NULL) {
		return 1;
	}
	// fprintf(file, "v_{i}\n");
	for (int i = 0; i < size; ++i) fprintf(file, "%f ", gsl_vector_get(v, i));
	fclose(file);

	return 0;
}
