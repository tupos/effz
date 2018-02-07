#ifndef EFFZ_ZEROTH_ORDER_H
#define EFFZ_ZEROTH_ORDER_H

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
						= "./data/i_direct_database.txt");

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
						= "./data/i_exchange_database.txt");

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

		double v_direct_total(const std::vector<std::array<int,4>> &g);

		double v_exchange_total(const std::vector<std::array<int,4>> &g);

		double v_direct_total_par(const std::vector<std::array<int,4>> &g);

		double v_exchange_total_par(const std::vector<std::array<int,4>> &g);

		double v_total(const std::vector<std::array<int,4>> &g);

		double v_total_par(const std::vector<std::array<int,4>> &g);

		double a(const std::vector<std::array<int,4>> &g);

		double z_star_0th(double z,
				const std::vector<std::array<int,4>> &g);

		double e_0th(double z,const std::vector<std::array<int,4>> &g);

		double z_star_0th_par(double z,
				const std::vector<std::array<int,4>> &g);

		double e_0th_par(double z,const std::vector<std::array<int,4>> &g);

		class density_0th{
			private:
				double z;
				std::vector<std::array<int,4>> occ_nums;
			public:
				density_0th(const double z,
						const std::vector<std::array<int,4>> &occ_nums);
				double operator()(
						const double r,
						const double theta,
						const double phi) const;
		};
	} /*end namespace zeroth_order*/

} /*end namespace eff_z*/

#endif /* EFFZ_ZEROTH_ORDER_H */
