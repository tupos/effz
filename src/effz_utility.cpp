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

#include "effz_utility.h"

#include <locale>
#include <codecvt>

namespace eff_z{

	void print_occ_nums(std::ostream &stream, const occ_nums_array &g){
		stream << "{\n";
		stream << "  ";
		int i = 0;
		int k = g.size() - 1;
		for(const auto &g_i: g){
			if(i == 4){
				i = 0;
				stream << "\n  ";
			}
			stream << "{";
			int j = 0;
			for(const auto &g_ij: g_i){
				stream << g_ij;
				if(j != 3){stream << ",";}
				++j;
			}
			stream << "}";
			if(k != 0){
				stream << ",";
			} else {stream << "\n";}
			++i;
			--k;
		}
		stream << "}\n";
	}

	std::string wstr_to_str(const std::wstring &wstr){
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

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
		for (int i = 0; i < size; ++i)
			fprintf(file, "%f ", gsl_vector_get(v, i));
		fclose(file);

		return 0;
	}
} /* end namespace eff_z */
