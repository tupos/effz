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

	} /* end namespace zeroth_order */
} /* end namespace eff_z */
