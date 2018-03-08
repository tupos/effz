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
				std::function<
				occ_nums_ast(occ_nums_parser*,const std::string&)
				> parse_f;
			const std::unordered_map<char,parse_f> occ_nums_f_map
				= {
					{'n', &occ_nums_parser::parse_n_format},
					{'N', &occ_nums_parser::parse_N_format},
					{'i', &occ_nums_parser::parse_i_format},
					{'O', &occ_nums_parser::parse_O_format},
					{'o', &occ_nums_parser::parse_o_format}
				};
	};
	struct f_string_ast{

	};
	class f_string_parser{
		public:
			f_string_parser(const std::string &s);
		private:
			std::string s;

			char format;
			std::vector<int> z;
			std::string output;
			occ_nums_ast ast;

			friend class occ_nums_parser;

			static const std::string flags[];
			static const std::string s_validation_pattern;
			static const std::unordered_map<std::string,std::string>
				f_validation_pattern;
			static const std::string flag_stm_token;
			static const std::unordered_map<std::string,std::string>
				flag_s_token;
			static const std::string flag_token;

			bool are_bad_flags();

			void parse_f(const std::string &s);
			void parse_z(const std::string &s);
			void parse_o(const std::string &s);
			void parse_v(const std::string &s);
			typedef
				std::function<
				void(f_string_parser*,const std::string&)
				> parse_pointer;
			const std::unordered_map<std::string,parse_pointer>
				parse_map
				= {
					{"-f", &f_string_parser::parse_f},
					{"-z", &f_string_parser::parse_z},
					{"-o", &f_string_parser::parse_o},
					{"-v", &f_string_parser::parse_v},
				};
			void parse();
	};
} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
