#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"

#include <vector>
#include <string>
#include <regex>


namespace eff_z{

	std::vector<int> parse_z_format(const std::string &s);

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

	template<typename CharT, typename Iterator>
		class basic_parse{
			public:
				typedef CharT value_type;
				typedef Iterator const_iterator;
				basic_parse(
						const value_type &s,
						const value_type &validate_pattern,
						const value_type &extract_pattern) :
					string(string), validate_pattern(validate_pattern),
					extract_pattern(extract_pattern),
					validate_regex(validate_pattern),
					extract_regex(extract_pattern) {}

			private:
				value_type string;
				value_type validate_pattern;
				value_type extract_pattern;
				std::basic_regex<value_type> validate_regex;
				std::basic_regex<value_type> extract_regex;
				const_iterator valid_iter;
				const_iterator extract_iter;


		};
} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
