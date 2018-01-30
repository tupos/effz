#include "effz_zeroth_order.h"

#include "effz_spec_func.h"
#include "effz_helper_func.h"
#include "effz_integration.h"
#include "effz_parallel_func.h"
#include <iostream>

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

		double three_j_prod_exchange(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k)
		{
			const int q = m1 - m;
			const bool is_q_between =
				eff_z::helper_func::in_range<int>(-k,k,q);
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

		double v_direct_total(const std::vector<std::array<int,4>> &g){
			double sum = 0.;
			for(auto &g_i: g){
				for(auto &g_j: g){
					sum += v_direct(
							g_i[0], g_i[1], g_i[2],
							g_j[0], g_j[1], g_j[2]);
				}
			}
			return sum;
		}

		double v_exchange_total(const std::vector<std::array<int,4>> &g){
			double sum = 0.;
			for(auto &g_i: g){
				for(auto &g_j: g){
					sum +=
						(g_i[3] != g_j[3]) ?
						0
						:
						v_exchange(g_i[0], g_i[1], g_i[2],
								g_j[0], g_j[1], g_j[2]);
				}
			}
			return sum;
		}

		double v_direct_total_par(const std::vector<std::array<int,4>> &g){
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

			//typedef std::vector<std::array<int,6>>::const_iterator
				//occ_num_iter;

			//const tbb::blocked_range<occ_num_iter>
				//occ_num_range(
						//occ_nums_array.cbegin(),occ_nums_array.cend());

			//tbb::parallel_for(occ_num_range,
					//[&](const tbb::blocked_range<occ_num_iter>& r){
					//for(occ_num_iter it = r.begin(); it != r.end(); ++it){
					//v_direct((*it)[0], (*it)[1], (*it)[2], (*it)[3],
							//(*it)[4], (*it)[5]);
					//}
					//});

			return sum;
		}

		double v_exchange_total_par(const std::vector<std::array<int,4>> &g){
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

		double v_total(const std::vector<std::array<int,4>> &g){
			return v_direct_total(g) - v_exchange_total(g);
		}

		double v_total_par(const std::vector<std::array<int,4>> &g){
			return v_direct_total_par(g) - v_exchange_total_par(g);
		}

		double a(const std::vector<std::array<int,4>> &g){
			double sum = 0.;
			for(auto &g_i: g){
				double n = static_cast<double>(g_i[0]);
				sum += 1. / (2. * n * n);
			}
			return sum;
		}

		double z_star_0th(double z,
				const std::vector<std::array<int,4>> &g){
			return z - v_total(g) / (2. * a(g));
		}

		double e_0th(double z,const std::vector<std::array<int,4>> &g){
			double z_star = z_star_0th(z,g);
			return -a(g) * z_star * z_star;
		}

		double z_star_0th_par(
				double z, const std::vector<std::array<int,4>> &g){
			return z - v_total_par(g) / (2. * a(g));
		}

		double e_0th_par(double z,
				const std::vector<std::array<int,4>> &g){
			double z_star = z_star_0th_par(z,g);
			return -a(g) * z_star * z_star;
		}


		density_0th::density_0th(const double z,
				const std::vector<std::array<int,4>> &occ_nums)
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

	}
}
