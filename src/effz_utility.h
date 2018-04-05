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

#ifndef EFFZ_UTILITY_H
#define EFFZ_UTILITY_H

#include "effz_typedefs.h"

#include <iostream>
#include <array>
#include <type_traits>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_math.h>

namespace eff_z{
	template <typename T>
		bool in_range(T begin, T end, T num){
			std::vector<T> range(std::abs(end - begin) + 1);
			std::iota(range.begin(), range.end(), begin);
			return std::any_of(range.cbegin(), range.cend(),
					[num](T n){return n == num;}
					);
		}
	template<typename... Ts>
		constexpr auto make_array(Ts&&... ts)
		-> std::array<std::common_type_t<Ts...>,sizeof...(ts)>
		{
			return { std::forward<Ts>(ts)... };
		}

	void print_occ_nums(std::ostream &stream, const occ_nums_array &g);
	std::string wstr_to_str(const std::wstring &wstr);

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


} /* end namespace eff_z */

#endif /* EFFZ_UTILITY_H */
