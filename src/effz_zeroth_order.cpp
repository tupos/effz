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

#include "effz_zeroth_order.h"

#include "effz_spec_func.h"
#include "effz_utility.h"
#include "effz_integration.h"
#include "effz_parallel_func.h"

#include "cereal/types/vector.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/archives/json.hpp"

#include <iostream>
#include <exception>
#include <fstream>
#include <tuple>
#include <algorithm>

namespace eff_z{
	namespace zeroth_order{

		double three_j_prod_direct(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k)
		{
			const double three_j_1 =
				eff_z::three_j_symbol({{{l,l,k},{0,0,0}}});
			const double three_j_2 =
				eff_z::three_j_symbol({{{l,l,k},{m,-m,0}}});
			const double three_j_3 =
				eff_z::three_j_symbol({{{l1,l1,k},{0,0,0}}});
			const double three_j_4 =
				eff_z::three_j_symbol({{{l1,l1,k},{m1,-m1,0}}});
			int prefactor = ((m+m1)%2 == 0) ? 1 : -1;
			prefactor *= (2 * l + 1) * (2 * l1 + 1);

			return static_cast<double>(prefactor)
				* three_j_1 * three_j_2 * three_j_3 * three_j_4;
		}

		double i_direct(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k
				)
		{
			auto inner_0_r =
				[n1,l1,k](double r) -> double {
					return 1. / std::pow(r, static_cast<double>(k) + 1)
						* eff_z::integration::quad(
								[n1,l1,k](double r1) -> double {
								double h_rnl =
								std::abs(eff_z::h_rnl(n1,l1,r1));
								return h_rnl * h_rnl
								* std::pow(r1,static_cast<double>(k) + 2);
								}, {0.,r});
				};

			auto inner_r_inf =
				[n1,l1,k](double r) -> double {
					return std::pow(r, static_cast<double>(k))
						* eff_z::integration::int_r_inf(
								[n1,l1,k](double r1) -> double {
								double h_rnl =
								std::abs(eff_z::h_rnl(n1,l1,r1));
								return h_rnl * h_rnl
								* std::pow(r1,2-(static_cast<double>(k)+1));
								}, r);
				};

			auto integral = [n,l,&inner_0_r,&inner_r_inf](double r)
				-> double {
					double h_rnl = eff_z::h_rnl(n,l,r);
					return r * r * h_rnl * h_rnl
						* (inner_0_r(r) + inner_r_inf(r));
				};

			return eff_z::integration::int_0_inf(integral);
		}


		void i_direct_database::calculate_database(){

			std::vector<std::array<int,5>> direct_quantum_nums;
			for(int n = 1; n <= 6; ++n){
				for(int l = 0; l <= n - 1; ++l){
					for(int n1 = 1; n1 <= 6; ++n1){
						for(int l1 = 0; l1 <= n1 - 1; ++l1){
							for(int k = 0; k <= std::min(l,l1); ++k){
								direct_quantum_nums
									.push_back({n,l,n1,l1,2*k});
							}
						}
					}
				}
			}

			for(int n = 1; n <= 6; ++n){
				for(int l = 0; l <= n - 1; ++l){
					for(int l1 = 0; l1 <= 2; ++l1){
						for(int k = 0; k <= std::min(l,l1); ++k){
							direct_quantum_nums
								.push_back({n,l,7,l1,2*k});
						}
					}
				}
			}

			for(int n1 = 1; n1 <= 6; ++n1){
				for(int l1 = 0; l1 <= n1 - 1; ++l1){
					for(int l = 0; l <= 2; ++l){
						for(int k = 0; k <= std::min(l,l1); ++k){
							direct_quantum_nums
								.push_back({7,l,n1,l1,2*k});
						}
					}
				}
			}

			for(int l = 0; l <= 2; ++l){
				for(int l1 = 0; l1 <= 2; ++l1){
					for(int k = 0; k <= std::min(l,l1); ++k){
						direct_quantum_nums.push_back({7,l,7,l1,2*k});
					}
				}
			}

			auto f_to_map
				= [](const std::array<int,5> &arr)
				-> std::tuple<std::array<int,5>, double>{
					return std::make_tuple(
							arr,
							i_direct(
								arr[0],
								arr[1],
								arr[2],
								arr[3],
								arr[4])
							);
				};

			database = eff_z::parallel::parallel_table(
					direct_quantum_nums, f_to_map);
		}

		void i_direct_database::save_database(std::fstream &s){
				cereal::JSONOutputArchive output(s);
				output(CEREAL_NVP(database));
		}

		void i_direct_database::load_database(std::fstream &s){
				cereal::JSONInputArchive input(s);
				input(CEREAL_NVP(database));
		}

		i_direct_database::i_direct_database(
				const std::string &path_to_data)
			: path_to_data(path_to_data), database() {
				std::fstream s;
				try{
					s.open(path_to_data);
					if(!s.is_open()){
						s.open(path_to_data, s.out | s.trunc);
						//std::cout << "calc" << "\n";
						calculate_database();
						save_database(s);
					} else {
						//std::cout << "load" << "\n";
						load_database(s);
					}
				} catch (const std::exception &e){
					std::cerr << "error happened" << e.what();
					throw;
				}
				s.close();
			}

		double i_direct_database::get_i_direct(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k){
			auto find_i_direct
				= [n,l,n1,l1,k](
						const std::tuple<std::array<int,5>, double> &el)
				-> bool
				{
					return std::get<0>(el)
						== std::array<int,5>({n,l,n1,l1,k});
				};
			auto element = std::find_if(
					database.cbegin(),
					database.cend(),
					find_i_direct);
			if(element != database.cend()){
				return std::get<1>(*element);
			} else {
				return i_direct(n,l,n1,l1,k);
			}
		}

		double i_direct_data_test(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k)
		{
			i_direct_database dat;
			return dat.get_i_direct(n,l,n1,l1,k);
		}

		double three_j_prod_exchange(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k)
		{
			const int q = m1 - m;
			const bool is_q_between =
				eff_z::in_range<int>(-k,k,q);
			if(!is_q_between){
				return 0.;
			}
			int prefactor = ((l + l1 + k)%2 == 0) ? 1 : -1;
			prefactor *= (2 * l + 1) * (2 * l1 + 1);
			const double three_j_1 =
				eff_z::three_j_symbol({{{l1,l,k},{0,0,0}}});
			const double three_j_2 =
				eff_z::three_j_symbol({{{l1,l,k},{-m1,m,q}}});

			return static_cast<double>(prefactor)
				* three_j_1 * three_j_2 * three_j_1 * three_j_2;
		}

		double i_exchange(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k
				)
		{
			auto inner_0_r =
				[n,l,n1,l1,k](double r) -> double {
					return 1. / std::pow(r, static_cast<double>(k) + 1)
						* eff_z::integration::quad(
								[n,l,n1,l1,k](double r1) -> double {
								double h_rnl =
								eff_z::h_rnl(n,l,r1);
								double h_rnl1 =
								eff_z::h_rnl(n1,l1,r1);
								return h_rnl * h_rnl1
								* std::pow(r1,static_cast<double>(k) + 2);
								},
								{0.,r});
				};

			auto inner_r_inf =
				[n,l,n1,l1,k](double r) -> double {
					return std::pow(r, static_cast<double>(k))
						* eff_z::integration::int_r_inf(
								[n,l,n1,l1,k](double r1) -> double {
								double h_rnl =
								eff_z::h_rnl(n,l,r1);
								double h_rnl1 =
								eff_z::h_rnl(n1,l1,r1);
								return h_rnl * h_rnl1
								* std::pow(r1,2-(static_cast<double>(k)+1));
								}, r);
				};

			auto integral =	[n,l,n1,l1,&inner_0_r,&inner_r_inf](double r)
				-> double {
					double h_rnl = eff_z::h_rnl(n,l,r);
					double h_rnl1 = eff_z::h_rnl(n1,l1,r);
					return r * r * h_rnl1 * h_rnl
						* (inner_0_r(r) + inner_r_inf(r));
				};

			return eff_z::integration::int_0_inf(integral);
		}

		void i_exchange_database::calculate_database(){

			std::vector<std::array<int,5>> exchange_quantum_nums;
			for(int n = 1; n <= 6; ++n){
				for(int l = 0; l <= n - 1; ++l){
					for(int n1 = 1; n1 <= 6; ++n1){
						for(int l1 = 0; l1 <= n1 - 1; ++l1){
							for(int k = std::abs(l-l1); k <= l+l1; ++k){
								exchange_quantum_nums
									.push_back({n,l,n1,l1,k});
							}
						}
					}
				}
			}

			for(int n = 1; n <= 6; ++n){
				for(int l = 0; l <= n - 1; ++l){
					for(int l1 = 0; l1 <= 1; ++l1){
						for(int k = std::abs(l-l1); k <= l+l1; ++k){
							exchange_quantum_nums
								.push_back({n,l,7,l1,k});
						}
					}
				}
			}

			for(int n1 = 1; n1 <= 6; ++n1){
				for(int l1 = 0; l1 <= n1 - 1; ++l1){
					for(int l = 0; l <= 1; ++l){
						for(int k = std::abs(l-l1); k <= l+l1; ++k){
							exchange_quantum_nums
								.push_back({7,l,n1,l1,k});
						}
					}
				}
			}

			for(int l = 0; l <= 1; ++l){
				for(int l1 = 0; l1 <= 1; ++l1){
					for(int k = std::abs(l-l1); k <= l+l1; ++k){
						exchange_quantum_nums.push_back({7,l,7,l1,k});
					}
				}
			}

			auto f_to_map
				= [](const std::array<int,5> &arr)
				-> std::tuple<std::array<int,5>, double>{
					return std::make_tuple(
							arr,
							i_exchange(
								arr[0],
								arr[1],
								arr[2],
								arr[3],
								arr[4])
							);
				};

			database = eff_z::parallel::parallel_table(
					exchange_quantum_nums, f_to_map);
		}

		void i_exchange_database::save_database(std::fstream &s){
				cereal::JSONOutputArchive output(s);
				output(CEREAL_NVP(database));
		}

		void i_exchange_database::load_database(std::fstream &s){
				cereal::JSONInputArchive input(s);
				input(CEREAL_NVP(database));
		}

		i_exchange_database::i_exchange_database(
				const std::string &path_to_data)
			: path_to_data(path_to_data), database() {
				std::fstream s;
				try{
					s.open(path_to_data);
					if(!s.is_open()){
						s.open(path_to_data, s.out | s.trunc);
						//std::cout << "calc" << "\n";
						calculate_database();
						save_database(s);
					} else {
						//std::cout << "load" << "\n";
						load_database(s);
					}
				} catch (const std::exception &e){
					std::cerr << "error happened " << e.what();
					throw;
				}
				s.close();
			}

		double i_exchange_database::get_i_exchange(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k){
			auto find_i_exchange
				= [n,l,n1,l1,k](
						const std::tuple<std::array<int,5>, double> &el)
				-> bool
				{
					return std::get<0>(el)
						== std::array<int,5>({n,l,n1,l1,k});
				};
			auto element = std::find_if(
					database.cbegin(),
					database.cend(),
					find_i_exchange);
			if(element != database.cend()){
				return std::get<1>(*element);
			} else {
				return i_exchange(n,l,n1,l1,k);
			}
		}

		double v_direct(
				const int n,
				const int l,
				const int m,
				const int n1,
				const int l1,
				const int m1)
		{
			int k_boundary = std::min(l,l1);
			double sum = 0.;
			for(int k = 0; k <= k_boundary; ++k){
				sum += 0.5 * i_direct(n,l,n1,l1,2 * k)
					* three_j_prod_direct(l,m,l1,m1,2 * k);
			}
			return sum;
		}

		double v_exchange(
				const int n,
				const int l,
				const int m,
				const int n1,
				const int l1,
				const int m1)
		{
			int k_min = std::abs(l1 - l);
			int k_max = l1 + l;
			double sum = 0.;
			for(int k = k_min; k <= k_max; ++k){
				sum += 0.5 * i_exchange(n,l,n1,l1,k)
					* three_j_prod_exchange(l,m,l1,m1,k);
			}
			return sum;
		}

		double v_direct_total(const occ_nums_array &g){
			i_direct_database i_d;
			double sum = 0.;
			for(auto &g_i: g){
				for(auto &g_j: g){
					int n = g_i[0], l = g_i[1], m = g_i[2];
					int n1 = g_j[0], l1 = g_j[1], m1 = g_j[2];

					int k_boundary = std::min(l,l1);
					double sum_d = 0.;
					for(int k = 0; k <= k_boundary; ++k){
						sum_d += 0.5 * i_d.get_i_direct(n,l,n1,l1,2 * k)
							* three_j_prod_direct(l,m,l1,m1,2 * k);
					}

					sum += sum_d;
				}
			}
			return sum;
		}

		double v_exchange_total(const occ_nums_array &g){
			i_exchange_database i_e;
			double sum = 0.;
			for(auto &g_i: g){
				for(auto &g_j: g){
					if(g_i[3] != g_j[3]){continue;}

					int n = g_i[0], l = g_i[1], m = g_i[2];
					int n1 = g_j[0], l1 = g_j[1], m1 = g_j[2];

					int k_min = std::abs(l1 - l);
					int k_max = l1 + l;

					double sum_e = 0.;
					for(int k = k_min; k <= k_max; ++k){
						sum_e += 0.5 * i_e.get_i_exchange(n,l,n1,l1,k)
							* three_j_prod_exchange(l,m,l1,m1,k);
					}
					sum += sum_e;
				}
			}
			return sum;
		}

		double v_direct_total_par(const occ_nums_array &g){
			double sum = 0.;
			std::vector<std::array<int,6>>
				occ_nums_array;
			for(auto &g_i: g){
				for(auto &g_j: g){
					occ_nums_array.push_back({ g_i[0], g_i[1], g_i[2],
							g_j[0], g_j[1], g_j[2]});
				}
			}
			auto lambda = [](const std::array<int,6>& g_i){
				return v_direct(g_i[0], g_i[1], g_i[2], g_i[3],
						g_i[4], g_i[5]);
			};
			sum = eff_z::parallel::parallel_sum<double>(
					occ_nums_array, lambda);

			return sum;
		}

		double v_exchange_total_par(
				const occ_nums_array &g){
			double sum = 0.;
			std::vector<std::array<int,8>>
				occ_nums_array;
			for(auto &g_i: g){
				for(auto &g_j: g){
					occ_nums_array.push_back({
							g_i[0], g_i[1], g_i[2], g_i[3],
							g_j[0], g_j[1], g_j[2], g_j[3]});
				}
			}
			auto lambda = [](const std::array<int,8>& g_i){
				return (g_i[3] != g_i[7]) ? 0 :
					v_exchange(g_i[0], g_i[1], g_i[2], g_i[4],
							g_i[5], g_i[6]);
			};
			sum = eff_z::parallel::parallel_sum<double>(
					occ_nums_array, lambda);

			return sum;
		}

		double v_total(const occ_nums_array &g){
			return v_direct_total(g) - v_exchange_total(g);
		}

		double v_total_par(const occ_nums_array &g){
			return v_direct_total_par(g) - v_exchange_total_par(g);
		}

		double a(const occ_nums_array &g){
			double sum = 0.;
			for(auto &g_i: g){
				double n = static_cast<double>(g_i[0]);
				sum += 1. / (2. * n * n);
			}
			return sum;
		}

		double z_star_0th(double z, const occ_nums_array &g){
			return z - v_total(g) / (2. * a(g));
		}

		double e_0th(double z,const occ_nums_array &g){
			double z_star = z_star_0th(z,g);
			return -a(g) * z_star * z_star;
		}

		double z_star_0th_par(double z, const occ_nums_array &g){
			return z - v_total_par(g) / (2. * a(g));
		}

		double e_0th_par(double z, const occ_nums_array &g){
			double z_star = z_star_0th_par(z,g);
			return -a(g) * z_star * z_star;
		}

		std::tuple<double,double> z_star_and_e_0th_par(double z,
				const occ_nums_array &g){
			//double z_star = z_star_0th_par(z,g);
			double z_star = z_star_0th(z,g);
			return std::make_tuple(z_star, -a(g) * z_star * z_star);
		}


		density_0th::density_0th(const double z,
				const occ_nums_array &occ_nums)
			: z(z), occ_nums(occ_nums) {};

		double density_0th::operator()(
				const double r,
				const double theta,
				const double phi) const {
			double sum = 0.;
			for(const auto &g_i: occ_nums){
				double h_rnl = eff_z::h_l_rnl(z,g_i[0],g_i[1], r);
				double abs_y_lm =
					std::abs(eff_z::sph_harm_y(
								g_i[1], g_i[2], theta, phi));
				sum += h_rnl * h_rnl * abs_y_lm * abs_y_lm;
			}
			return sum;
		}

	} /* end namespace zeroth_order */

} /* end namespace eff_z */
