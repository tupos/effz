#ifndef EFFZ_ATOMIC_DATA_H
#define EFFZ_ATOMIC_DATA_H

#include "effz_typedefs.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <string>

namespace eff_z {

	namespace atomic_data {

		class occ_nums_data{
			public:
				static const std::vector<occ_nums_array> g;
				static const occ_nums_array g_He;
				static const occ_nums_array g_Li;
				static const occ_nums_array g_Be;
				static const occ_nums_array g_B;
				static const occ_nums_array g_C;
				static const occ_nums_array g_N;
				static const occ_nums_array g_O;
				static const occ_nums_array g_F;
				static const occ_nums_array g_Ne;
		};

		struct element_data{
			static const std::unordered_map<std::string, int>
				element_names;
		};


	} /*end namespace eff_z*/
} /*end namespace eff_z*/


#endif /* EFFZ_ATOMIC_DATA_H */
