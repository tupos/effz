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
