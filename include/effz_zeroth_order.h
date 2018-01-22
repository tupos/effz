#ifndef EFFZ_ZEROTH_ORDER_H
#define EFFZ_ZEROTH_ORDER_H

#include <gsl/gsl_sf_coupling.h>

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

		double three_j_prod_exchange(
				const int l,
				const int m,
				const int l1,
				const int m1,
				const int k);
	} /*end namespace zeroth_order*/
} /*end namespace eff_z*/

#endif /* EFFZ_ZEROTH_ORDER_H */
