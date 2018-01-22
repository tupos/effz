#ifndef EFFZ_HELPER_FUNC_H
#define EFFZ_HELPER_FUNC_H

#include <vector>
#include <numeric>
#include <iostream>

namespace eff_z{

	namespace helper_func{

		template <typename T>
		bool in_range(T begin, T end, T num){
			std::vector<T> range(std::abs(end - begin) + 1);
			std::iota(range.begin(), range.end(), begin);
			return std::any_of(range.cbegin(), range.cend(),
					[num](T n){return n == num;}
					);
		}

	} /*end namespace helper_func*/

} /*end namespace eff_z*/

#endif /* EFFZ_HELPER_FUNC_H */
