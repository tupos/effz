#include "effz_result.h"

#include "effz_zeroth_order.h"

namespace eff_z{
	namespace zeroth_order{
		base_result::base_result()
			: result_string(
					"********************************\n"
					"***The calculation result is:***\n") { }

		energy_result::energy_result(const std::string &name, int z,
				const occ_nums_array &g)
			: name(name), z(z), g(g)
		{

		}

	} /* end namespace zeroth_order */
} /* end namespace eff_z */
