#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"
#include "effz_exceptions.h"

#include <vector>
#include <string>


namespace eff_z{

	std::vector<int> parse_z_format(const std::string &s);

	char parse_f_format(const std::string &s);

	std::string parse_o_format(const std::string &s);

	std::vector<std::tuple<std::string,occ_nums_array>>
		parse_v_format(const std::string &s,
				char format);

	std::vector<
		std::tuple<
		std::vector<int>,
		std::vector<std::tuple<std::string,occ_nums_array>>,
		std::string>>
			parse_format_string(const std::string &s);

} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
