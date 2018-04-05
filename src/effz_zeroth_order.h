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

#ifndef EFFZ_ZEROTH_ORDER_H
#define EFFZ_ZEROTH_ORDER_H

#include "effz_typedefs.h"
#include "effz_config.h"

#include <gsl/gsl_sf_coupling.h>
#include <array>
#include <vector>
#include <string>

namespace eff_z{
	namespace zeroth_order{
		double three_j_prod_direct(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k);

		double i_direct(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k);

		class i_direct_database{
			private:
				std::string path_to_data;

				typedef std::tuple<std::array<int,5>,double> elem_t;
				std::vector<elem_t> database;

				friend double i_direct(
						const int n,
						const int l,
						const int n1,
						const int l1,
						const int k);

				void calculate_database();
				void save_database(std::fstream &s);
				void load_database(std::fstream &s);

			public:
				i_direct_database(const std::string &path_to_data
						= config::shared_config().get_database_dir()
						+ "/i_direct_database.txt");

				double get_i_direct(
						const int n,
						const int l,
						const int n1,
						const int l1,
						const int k);

		};

		double three_j_prod_exchange(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k);

		double i_exchange(
				const int n,
				const int l,
				const int n1,
				const int l1,
				const int k);

		class i_exchange_database{
			private:
				std::string path_to_data;

				typedef std::tuple<std::array<int,5>,double> elem_t;
				std::vector<elem_t> database;

				friend double i_exchange(
						const int n,
						const int l,
						const int n1,
						const int l1,
						const int k);

				void calculate_database();
				void save_database(std::fstream &s);
				void load_database(std::fstream &s);

			public:
				i_exchange_database(const std::string &path_to_data
						= config::shared_config().get_database_dir()
						+ "/i_exchange_database.txt");

				double get_i_exchange(
						const int n,
						const int l,
						const int n1,
						const int l1,
						const int k);

		};

		double v_direct(
				const int n,
				const int l,
				const int m,
				const int n1,
				const int l1,
				const int m1);

		double v_exchange(
				const int n,
				const int l,
				const int m,
				const int n1,
				const int l1,
				const int m1);

		double v_direct_total(const occ_nums_array &g);

		double v_exchange_total(const occ_nums_array &g);

		double v_direct_total_par(const occ_nums_array &g);

		double v_exchange_total_par(const occ_nums_array &g);

		double v_total(const occ_nums_array &g);

		double v_total_par(const occ_nums_array &g);

		double a(const occ_nums_array &g);

		double z_star_0th(double z,
				const occ_nums_array &g);

		double e_0th(double z,const occ_nums_array &g);

		double z_star_0th_par(double z, const occ_nums_array &g);

		double e_0th_par(double z,const occ_nums_array &g);

		std::tuple<double,double> z_star_and_e_0th_par(double z,
				const occ_nums_array &g);



		class density_0th{
			private:
				double z;
				occ_nums_array occ_nums;
			public:
				density_0th(const double z,
						const occ_nums_array &occ_nums);
				double operator()(
						const double r,
						const double theta,
						const double phi) const;
		};
	} /*end namespace zeroth_order*/

} /*end namespace eff_z*/

#endif /* EFFZ_ZEROTH_ORDER_H */
