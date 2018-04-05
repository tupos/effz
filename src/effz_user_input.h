/*
Copyright 2018 Oleg Skoromnik

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef EFFZ_USER_INPUT_H
#define EFFZ_USER_INPUT_H

#include "effz_typedefs.h"
#include "effz_exceptions.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>


namespace eff_z{

	struct occ_nums_ast{
		typedef std::tuple<std::string,occ_nums_array> named_occ_nums_t;
		std::vector<named_occ_nums_t> named_occ_nums;

		void push_back(const std::string &s, const occ_nums_array &a);
		void clear();
		const occ_nums_array& get_nums(std::size_t i) const;
	};
	class occ_nums_parser{
		public:
			occ_nums_parser(const std::string &s, char format);
			occ_nums_ast get_ast() const;
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
				> parse_f_pointer;
			const std::unordered_map<char,parse_f_pointer> occ_nums_f_map
				= {
					{'n', &occ_nums_parser::parse_n_format},
					{'N', &occ_nums_parser::parse_N_format},
					{'i', &occ_nums_parser::parse_i_format},
					{'O', &occ_nums_parser::parse_O_format},
					{'o', &occ_nums_parser::parse_o_format}
				};
	};

	struct f_string_ast{
		char format;
		std::vector<int> z;
		std::string output_file_path;
		occ_nums_ast on_ast;
	};
	class f_string_parser{
		public:
			f_string_parser(const std::string &s);
			f_string_ast get_ast() const;
		private:
			std::string format_s;

			f_string_ast ast;

			friend class occ_nums_parser;

			static const std::string flags[];
			static const std::unordered_map<std::string,std::string>
				validation_patterns;
			static const std::unordered_map<std::string,std::string>
				tokens;

			friend bool is_valid_pattern(const std::string &s,
					const std::string &pattern);
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
			void parse_single_string();
	};

	class f_strings_parser{
		public:
			f_strings_parser(const std::string &format_strings);
			std::vector<f_string_ast> get_parsed_data() const;
		private:
			std::string format_strings;
			std::vector<f_string_ast> parsed_data;
			friend class f_string_parser;
			friend bool is_valid_pattern(const std::string &s,
					const std::string &pattern);
			static const std::string validation_pattern;
			static const std::string format_string_token;
			void parse_strings();
	};
} /* end namespace eff_z */

#endif /* EFFZ_USER_INPUT_H */
