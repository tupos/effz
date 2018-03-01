#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"
#include "effz_exceptions.h"

#include <vector>
#include <string>


namespace eff_z{

	std::vector<int> parse_z_format(const std::string &s);

	char parse_f_format(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_n_format(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_N_format(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_i_format(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_o_format(const std::string &s);

	std::vector<occ_nums_array>
		parse_occ_nums_O_format(const std::string &s);

	std::vector<occ_nums_array> parse_v_format(const std::string &s,
			char format);

} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
