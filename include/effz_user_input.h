#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"

#include <vector>
#include <string>


namespace eff_z{

	occ_nums_array string_to_occ_nums_array(const std::string &s);

	bool is_cor_num_braces_occ_nums(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_o_format(const std::string &s);

} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
