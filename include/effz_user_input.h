#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"
#include "effz_exceptions.h"

#include <vector>
#include <string>
#include <unordered_map>


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

	struct occ_nums_ast{
		std::vector<occ_nums_array> parsed_occ_nums;
		std::vector<std::string> parsed_names;
		typedef std::tuple<std::string,occ_nums_array> named_occ_nums_t;
	};
	class occ_nums_parser{
		public:
			occ_nums_parser(const std::string &s, char format);
		private:
			static const std::unordered_map<char,std::string>
				occ_nums_formats;
			static const std::unordered_map<char,std::string>
				occ_nums_tokens;
			std::string s;
			char format;
			occ_nums_ast ast;
			friend occ_nums_array string_to_occ_nums_array(
					const std::string &s);
			friend bool is_valid_pattern(const std::string &s,
					const std::string &pattern);
			void parse_n_format(const std::string &s);
			void parse_N_format(const std::string &s);
			void parse_i_format(const std::string &s);
			void parse_o_format(const std::string &s);
			void parse_O_format(const std::string &s);
	};
} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
