#include "effz_zeroth_order.h"

#include "effz_spec_func.h"
#include "effz_helper_func.h"
#include "effz_integration.h"
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
						* eff_z::integration::quad( 
								[n1,l1,k](double r1) -> double {
								double h_rnl =
								std::abs(eff_z::h_rnl(n1,l1,r1));
								return h_rnl * h_rnl
								* std::pow(r1,2-(static_cast<double>(k)+1));
								}, {r,30.});
				};
			return eff_z::integration::quad(
					[n,l,&inner_0_r,&inner_r_inf](double r) -> double {
					double h_rnl = eff_z::h_rnl(n,l,r);
					return r * r * h_rnl * h_rnl
					* (inner_0_r(r) + inner_r_inf(r));
					},{0.,30.}
					); 
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
		
	}
}
