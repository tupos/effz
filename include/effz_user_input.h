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
		void push_back(const std::string &s, const occ_nums_array &a);
		typedef std::tuple<std::string,occ_nums_array> named_occ_nums_t;
	};
	class occ_nums_parser{
		public:
			occ_nums_parser(const std::string &s, char format);
			occ_nums_ast get_ast();
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
			occ_nums_ast parse_n_format(const std::string &s);
			occ_nums_ast parse_N_format(const std::string &s);
			occ_nums_ast parse_i_format(const std::string &s);
			occ_nums_ast parse_o_format(const std::string &s);
			occ_nums_ast parse_O_format(const std::string &s);
			occ_nums_ast parse();
			typedef
				occ_nums_ast(occ_nums_parser::*parse_f)(const std::string&);
			const std::unordered_map<char,parse_f> occ_nums_f_map
				= {
					{'n', &occ_nums_parser::parse_n_format},
					{'N', &occ_nums_parser::parse_N_format},
					{'i', &occ_nums_parser::parse_i_format},
					{'O', &occ_nums_parser::parse_O_format},
					{'o', &occ_nums_parser::parse_o_format}
				};
	};
} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
