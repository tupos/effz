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

#ifndef EFFZ_RESULT_H
#define EFFZ_RESULT_H

#include "effz_zeroth_order.h"
#include "effz_user_input.h"
#include "effz_zeroth_order_python.h"

#include <string>
#include <iostream>

namespace eff_z{
	namespace zeroth_order{
		class base_result{
			public:
				base_result();
				virtual ~base_result() = default;
				virtual void print_result(std::ostream &stream){
					stream << result_string;
				};
			protected:
				std::string result_string;
		};

		class energy_result : public base_result{
			public:
				energy_result(const std::string &name, int z,
						const occ_nums_array &g);
			private:
				const std::string name;
				const int z;
				const occ_nums_array g;
				double z_star;
				double energy;
		};
		class density_result : public base_result{
			public:
				density_result(const std::string &name, int z,
						const occ_nums_array &g);
			private:
				const std::string name;
				const int z;
				const occ_nums_array g;
				symbolic_density density;
		};
		class asf_result : public base_result{
			public:
				asf_result(const std::string &name, int z,
						const occ_nums_array &g);
			private:
				const std::string name;
				const int z;
				const occ_nums_array g;
				symbolic_asf asf;
		};

		void parse_calc_and_print_0th_results_energy(
				std::ostream &stream, const std::string &s);
		void parse_calc_and_print_0th_results_density(
				std::ostream &stream, const std::string &s);
		void parse_calc_and_print_0th_results_asf(
				std::ostream &stream, const std::string &s);

	} /* end namespace zeroth_order */
} /* end namespace eff_z */


#endif /* EFFZ_RESULT_H */
