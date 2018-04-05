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

#include "effz_result.h"

#include "effz_zeroth_order.h"
#include "effz_utility.h"

#include <sstream>

namespace eff_z{
	namespace zeroth_order{
		base_result::base_result()
			: result_string(
					"*************************************\n"
					"*****The calculation result is:******\n") { }

		energy_result::energy_result(const std::string &name, int z,
				const occ_nums_array &g)
			: name(name), z(z), g(g)
		{
			auto z_and_e = z_star_and_e_0th_par(z,g);
			z_star = std::get<0>(z_and_e);
			energy = std::get<1>(z_and_e);
			std::stringstream ss;
			ss << "*** " << "\"" << name << "\"" << " ***\n";
			ss << "*** z = " << z << " ***\n";
			ss << "*** z_star = " << z_star << " ***\n";
			ss << "*** energy = " << energy << " ***\n";
			ss << "*** occupation numbers start ********\n";
			print_occ_nums(ss, g);
			ss << "*** occupation numbers end **********\n";
			ss << "*************************************\n";
			result_string += ss.str();
		}

		density_result::density_result(const std::string &name, int z,
				const occ_nums_array &g)
			: name(name), z(z), g(g), density(g)
		{
			std::string density_latex = density.get_density_latex_str();
			std::string density_pretty = density.get_density_pretty_str();
			std::stringstream ss;
			ss << "*** " << "\"" << name << "\"" << " ***\n";
			ss << "*** z = " << z << " ***\n";
			ss << "*** density pretty start ************\n";
			ss << density_pretty << "\n";
			ss << "*** density pretty end **************\n";
			ss << "*** density latex start *************\n";
			ss << density_latex << "\n";
			ss << "*** density latex end ***************\n";
			ss << "*** occupation numbers start ********\n";
			print_occ_nums(ss, g);
			ss << "*** occupation numbers end **********\n";
			ss << "*************************************\n";
			result_string += ss.str();
		}

		asf_result::asf_result(const std::string &name, int z,
				const occ_nums_array &g)
			: name(name), z(z), g(g), asf(g)
		{
			std::string asf_latex = asf.get_asf_latex_str();
			std::string asf_pretty = asf.get_asf_pretty_str();
			std::stringstream ss;
			ss << "*** " << "\"" << name << "\"" << " ***\n";
			ss << "*** z = " << z << " ***\n";
			ss << "*** asf pretty start ************\n";
			ss << asf_pretty << "\n";
			ss << "*** asf pretty end **************\n";
			ss << "*** asf latex start *************\n";
			ss << asf_latex << "\n";
			ss << "*** asf latex end ***************\n";
			ss << "*** occupation numbers start ********\n";
			print_occ_nums(ss, g);
			ss << "*** occupation numbers end **********\n";
			ss << "*************************************\n";
			result_string += ss.str();
		}

		void parse_calc_and_print_0th_results_energy(
				std::ostream &stream, const std::string &s)
		{
			f_strings_parser parser(s);
			auto data = parser.get_parsed_data();

			for(const auto &dat:data){
				auto charges = dat.z;
				auto nums = dat.on_ast;

				for(auto &num: charges){
					for(auto &occ_nums: nums.named_occ_nums){
						zeroth_order::energy_result
							res(std::get<0>(occ_nums), num,
									std::get<1>(occ_nums));
						res.print_result(stream);
					}
				}
			}
		}
		void parse_calc_and_print_0th_results_density(
				std::ostream &stream, const std::string &s)
		{
			f_strings_parser parser(s);
			auto data = parser.get_parsed_data();

			for(const auto &dat:data){
				auto charges = dat.z;
				auto nums = dat.on_ast;

				for(auto &num: charges){
					for(auto &occ_nums: nums.named_occ_nums){
						zeroth_order::density_result
							res(std::get<0>(occ_nums), num,
									std::get<1>(occ_nums));
						res.print_result(stream);
					}
				}
			}
		}

		void parse_calc_and_print_0th_results_asf(
				std::ostream &stream, const std::string &s)
		{
			f_strings_parser parser(s);
			auto data = parser.get_parsed_data();

			for(const auto &dat:data){
				auto charges = dat.z;
				auto nums = dat.on_ast;

				for(auto &num: charges){
					for(auto &occ_nums: nums.named_occ_nums){
						zeroth_order::asf_result
							res(std::get<0>(occ_nums), num,
									std::get<1>(occ_nums));
						res.print_result(stream);
					}
				}
			}
		}
	} /* end namespace zeroth_order */
} /* end namespace eff_z */
