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

#include "effz_user_input.h"

#include "effz_exceptions.h"
#include "effz_utility.h"
#include "effz_atomic_data.h"

#include <regex>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <iostream>
#include <sstream>

namespace eff_z{

	namespace{
		occ_nums_array string_to_occ_nums_array(const std::string &s);
		std::size_t roman_to_arabic(const std::string &s);
		bool is_valid_pattern(const std::string &s,
				const std::string &pattern);
	} /* end unnamed namespace */

	void occ_nums_ast::push_back(
			const std::string &s, const occ_nums_array &a){
		named_occ_nums_t pair = std::make_pair(s,a);
		named_occ_nums.push_back(pair);
	}

	void occ_nums_ast::clear(){
		named_occ_nums.clear();
	}

	const occ_nums_array& occ_nums_ast::get_nums(std::size_t i) const {
		return std::get<1>(named_occ_nums.at(i));
	}

	const std::unordered_map<char,std::string>
		occ_nums_parser::occ_nums_formats = {
			{'n',"^\\s*([1-9][0-9]?|100)\\b"
				"(\\s*,\\s*\\b([1-9][0-9]?|100)\\b)*\\s*$"},
			{'N',"^\\s*[A-Z][a-z]?"
				"\\b(\\s*,\\s*\\b[A-Z][a-z]?\\b)*\\s*$"},
			{'i',"^\\s*[A-Z][a-z]?\\b\\s*\\b(XC|XL|L?X{0,3})"
				"(IX|IV|V?I{1,3})\\b"
					"(\\s*,\\s*\\b[A-Z][a-z]?"
					"\\b\\s*\\b(XC|XL|L?X{0,3})"
					"(IX|IV|V?I{1,3})\\b)*\\s*$"},
			{'o',"^\\s*\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\}"
					"(\\s*,\\s*\\{\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\})*\\s*$"},
			{'O',"^\\s*[A-Z][a-z]?\\b\\s*\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\}"
					"(\\s*,\\s*\\b[A-Z][a-z]?\\b\\s*\\{\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\})*\\s*$"}

		};

	const std::unordered_map<char,std::string>
		occ_nums_parser::occ_nums_tokens = {
			{'n',"\\b([1-9][0-9]?|100)\\b"},
			{'N',"\\b[A-Z][a-z]?\\b"},
			{'i',"\\b[A-Z][a-z]?\\b\\s*\\b(XC|XL|L?X{0,3})"
				"(IX|IV|V?I{1,3})\\b"},
			{'o',"\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\}"},
			{'O',"\\b[A-Z][a-z]?\\b\\s*\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\}"
					"(\\s*,\\s*"
					"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
					"\\s*-?\\d\\s*\\})*"
					"\\s*\\}"}
		};

	occ_nums_parser::occ_nums_parser(const std::string &s, char format)
		: s(s), format(format){
			if(!is_valid_pattern(s,occ_nums_formats.at(format))){
				std::string what = "Error in the input string ";
				what += format;
				throw parsing_exception(what.c_str());
			}
			parse();
		}

	occ_nums_ast occ_nums_parser::get_ast() const {
		return ast;
	}

	occ_nums_ast occ_nums_parser::parse_n_format(const std::string &s){
		std::regex z_pattern_regex(occ_nums_tokens.at('n'));

		auto it_z_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), z_pattern_regex);
		auto it_z_pat_regex_end = std::sregex_iterator();

		std::vector<int> el_nums;
		for (std::sregex_iterator i = it_z_pat_regex_begin;
				i != it_z_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			try{
				int num = std::stoi(match_str);
				el_nums.push_back(num);
			} catch (const std::out_of_range &e){
				std::cerr << "Error in the input string n format.\n";
				throw parsing_exception
					("Error in the input string n format.");
			} catch (const std::invalid_argument &e){
				std::cerr << e.what() << "\n";
				throw parsing_exception
					("Error in the input string n format.");
			}
		}

		occ_nums_array occ_nums;
		std::size_t counter = 0;
		for(auto &el_num: el_nums){
			try{
				occ_nums = atomic_data::occ_nums_data::
					g.at(el_num-1);
			} catch (const std::out_of_range &e){
				std::cerr << "Error in the input of occ_nums #"
					<< counter << "\n";
				std::cerr << " " << el_num << " does not exist\n";
				throw parsing_exception
					("Error in the input of occ_nums");
			}
			ast.push_back(std::to_string(el_num), occ_nums);
			++counter;
		}
		return ast;
	}

	occ_nums_ast occ_nums_parser::parse_N_format(const std::string &s){
		std::regex N_pattern_regex(occ_nums_tokens.at('N'));

		auto it_N_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), N_pattern_regex);
		auto it_N_pat_regex_end = std::sregex_iterator();

		occ_nums_array occ_nums;
		std::size_t counter = 0;
		for (std::sregex_iterator i = it_N_pat_regex_begin;
				i != it_N_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			try{
				int el_num =
					atomic_data::element_data::
					element_names.at(match_str);
				occ_nums = atomic_data::occ_nums_data::
					g.at(el_num-1);
			} catch (const std::out_of_range &e){
				std::cerr << "Error in the input of occ_nums #"
					<< counter << "\n";
				std::cerr << " " << match_str
					<< " does not exist\n";
				throw parsing_exception
					("Error in the input of occ_nums");
			}
			ast.push_back(match_str, occ_nums);
			++counter;
		}
		return ast;
	}

	occ_nums_ast occ_nums_parser::parse_i_format(const std::string &s){
		auto extract_roman_n = [](const std::string& s){
			std::regex
				roman_regex("\\b(XC|XL|L?X{0,3})"
						"(IX|IV|V?I{1,3})\\s*$");
			std::smatch match;
			std::regex_search(s,match,roman_regex);
			std::string match_str = match.str();
			return roman_to_arabic(match_str);
		};

		std::regex i_pattern_regex(occ_nums_tokens.at('i'));

		auto it_i_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), i_pattern_regex);
		auto it_i_pat_regex_end = std::sregex_iterator();

		occ_nums_array occ_nums;
		std::size_t roman_n = 0;
		occ_nums_ast res;

		std::size_t counter = 0;
		for (std::sregex_iterator i = it_i_pat_regex_begin;
				i != it_i_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			//std::cout << " " << match_str << '\n';
			try{
				occ_nums = parse_N_format(match_str).get_nums(0);
				roman_n = extract_roman_n(match_str);
				if(occ_nums.size() <= roman_n - 1){
					throw parsing_exception(
							"Ionization degree is"
							" larger than the number "
							"of electrons.");
				}
				occ_nums.erase(occ_nums.end() - (roman_n - 1),
						occ_nums.end());
			} catch (const parsing_exception &e){
				std::cerr << "Error in the input of occ_nums #"
					<< counter << "\n";
				std::cerr << e.what() << "\n";
				throw parsing_exception
					("Error in the input of occ_nums i format.");
			}
			res.push_back(match_str,occ_nums);
			ast.clear();
			++counter;
		}
		ast = res;
		return ast;
	}

	occ_nums_ast occ_nums_parser::parse_o_format(const std::string &s){

		std::regex o_pattern_regex(occ_nums_tokens.at('o'));

		auto it_o_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), o_pattern_regex);
		auto it_o_pat_regex_end = std::sregex_iterator();

		std::size_t counter = 0;
		occ_nums_array occ_nums;

		for (std::sregex_iterator i = it_o_pat_regex_begin;
				i != it_o_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			try{
				occ_nums = string_to_occ_nums_array(match_str);
			} catch (const parsing_exception& e){
				std::cerr << "Error in the input of occ_nums #"
					<< counter << "\n";
				std::cerr << e.what();
				std::cerr << " " << match_str << '\n';
				throw parsing_exception
					("Error in the input of occ_nums");
			}
			ast.push_back(match_str,occ_nums);
			++counter;
		}
		return ast;
	}

	occ_nums_ast occ_nums_parser::parse_O_format(const std::string &s){

		std::regex O_pattern_regex(occ_nums_tokens.at('O'));

		auto it_O_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), O_pattern_regex);
		auto it_O_pat_regex_end = std::sregex_iterator();

		std::size_t counter = 0;
		occ_nums_array element_occ_nums;
		occ_nums_array occ_nums_to_append;
		occ_nums_ast res;

		for (std::sregex_iterator i = it_O_pat_regex_begin;
				i != it_O_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			//std::cout << " " << match_str << '\n';
			try{
				element_occ_nums = parse_N_format(match_str).get_nums(0);
				occ_nums_to_append = parse_o_format(match_str).get_nums(0);
				for(auto &g_i: occ_nums_to_append){
					element_occ_nums.push_back(g_i);
				}
			} catch (const parsing_exception& e){
				std::cerr << "Error in the input of occ_nums #"
					<< counter << "\n";
				std::cerr << e.what();
				std::cerr << " " << match_str << '\n';
				throw parsing_exception
					("Error in the input of occ_nums");
			}
			res.push_back(match_str,element_occ_nums);
			ast.clear();
			++counter;
		}
		ast = res;

		return ast;
	}

	occ_nums_ast occ_nums_parser::parse(){
		return occ_nums_f_map.at(format)(this,s);
	}


	f_string_parser::f_string_parser(const std::string &format_s)
		: format_s(format_s), ast(){
			if(!is_valid_pattern(format_s,
						validation_patterns.at("whole_s"))){
				throw parsing_exception("Error in the input string");
			}
			if(are_bad_flags()){
				throw parsing_exception("Error wrong flags");
			}
			parse_single_string();
		}

	f_string_ast f_string_parser::get_ast() const {
		return ast;
	}

	const std::string f_string_parser::flags[] = {"-z", "-f", "-v","-o"};

	const std::unordered_map<std::string,std::string>
		f_string_parser::validation_patterns = {
			{"whole_s","^\\s*(-[zfvo]\\s*[^;]+?){3,4};?$"},
			{"-z","^\\s*([1-9][0-9]?|100)\\b"
				"(\\s*,\\s*\\b([1-9][0-9]?|100)\\b)*\\s*$"},
			{"-f","^\\s*[nNiOo]\\s*$"},
			{"-o",""}
		};

	const std::unordered_map<std::string,std::string>
		f_string_parser::tokens = {
			{"flag_stm","-[zfvo]\\s*[^;]+?(?=-[zfvo]\\s*|;|$)"},
			{"flag","-[zvfo]"},
			{"-f_value","[nNiOo]"},
			{"-z_value","\\b([1-9][0-9]?|100)\\b"}
		};

	bool f_string_parser::are_bad_flags(){
		std::unordered_map<std::string,int> flags_num;
		for(const auto &f: flags){
			flags_num.insert({f,0});
		}
		std::regex flags_regex(tokens.at("flag"));
		auto flags_begin = std::sregex_iterator(format_s.begin(),
				format_s.end(), flags_regex);
		auto flags_end = std::sregex_iterator();
		for(auto it = flags_begin; it != flags_end; ++it){
			std::smatch match = *it;
			std::string match_str = match.str();
			//std::cout << match_str << "\n";
			flags_num.at(match_str)++;
		}
		for(auto it = flags_num.cbegin(); it != flags_num.cend(); ++it){
			if((*it).second > 1){
				return true;
			}
			if((*it).first != "-o" && (*it).second != 1){
				return true;
			}
		}
		return false;
	}

	void f_string_parser::parse_f(const std::string &s){
		if(!is_valid_pattern(s, validation_patterns.at("-f"))){
			throw parsing_exception(
					"Error in the -f part of the string.");
		}

		std::regex f_pattern_regex(tokens.at("-f_value"));
		std::smatch match;
		std::regex_search(s,match,f_pattern_regex);

		ast.format = match.str().at(0);
	}

	void f_string_parser::parse_z(const std::string &s){
		if(!is_valid_pattern(s, validation_patterns.at("-z"))){
			throw parsing_exception(
					"Error in the -z part of the string.");
		}
		std::regex z_pattern_regex(tokens.at("-z_value"));

		auto it_z_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), z_pattern_regex);
		auto it_z_pat_regex_end = std::sregex_iterator();

		for (std::sregex_iterator i = it_z_pat_regex_begin;
				i != it_z_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			try{
				int num = std::stoi(match_str);
				ast.z.push_back(num);
			} catch (const std::out_of_range &e){
				std::cerr << "Error in the input -z.\n";
				throw parsing_exception
					("Error in the input -z.");
			} catch (const std::invalid_argument &e){
				std::cerr << e.what() << "\n";
				throw parsing_exception
					("Error in the input -z.");
			}
		}
	}

	void f_string_parser::parse_o(const std::string &s){
		ast.output_file_path = "";
	}

	void f_string_parser::parse_v(const std::string &s){
		ast.on_ast = occ_nums_parser(s,ast.format).get_ast();
	}

	void f_string_parser::parse_single_string(){
		auto extract_flag_and_rest = [](const std::string &s){
			std::regex flag_regex("^-[zfvo]");
			std::smatch match;
			std::regex_search(s,match,flag_regex);
			std::string flag = match.str();
			std::string rest = match.suffix().str();
			return std::make_tuple(flag,rest);
		};

		std::regex flag_stm_regex(tokens.at("flag_stm"));

		auto it_f_stm_begin = std::sregex_iterator(format_s.begin(),
				format_s.end(), flag_stm_regex);
		auto it_f_stm_end = std::sregex_iterator();

		std::string occ_nums_s;
		for(auto i = it_f_stm_begin; i != it_f_stm_end; ++i){
			std::smatch match = *i;
			//std::cout << "     " << match.str() << "\n";
			auto flag_rest = extract_flag_and_rest(match.str());
			std::string flag = std::get<0>(flag_rest);
			std::string rest = std::get<1>(flag_rest);
			//std::cout << flag << "\n";
			//std::cout << rest << "\n";
			if(flag != "-v"){
				parse_map.at(flag)(this,rest);
			} else if(flag == "-v"){
				occ_nums_s = rest;
			}
		}
		parse_map.at("-v")(this,occ_nums_s);
	}


	f_strings_parser::f_strings_parser(const std::string &format_strings)
		: format_strings(format_strings), parsed_data()
	{
		if(!is_valid_pattern(format_strings,validation_pattern)){
			throw parsing_exception("Error in the input string.");
		}
		parse_strings();
	}

	std::vector<f_string_ast> f_strings_parser::get_parsed_data() const{
		return parsed_data;
	}

	const std::string f_strings_parser::validation_pattern
		= "^\\s*(-[zfvo]\\s*[^;]+?){3,4}"
		"(\\s*;\\s*(-[zfvo]\\s*[^;]+?){3,4})*\\s*;?$";

	const std::string f_strings_parser::format_string_token
		= "(-[zfvo]\\s*[^;]+?){3,4}(?=;|$)";

	void f_strings_parser::parse_strings(){
		std::regex format_string_regex(format_string_token);
		auto it_f_string_begin = std::sregex_iterator(
				format_strings.begin(),
				format_strings.end(),format_string_regex);
		auto it_f_string_end = std::sregex_iterator();

		for(auto i = it_f_string_begin;
				i != it_f_string_end; ++i){
			std::smatch match = *i;
			std::string f_string = match.str();
			//std::cout << "  " << f_string <<"\n";
			f_string_parser parser(f_string);
			f_string_ast ast = parser.get_ast();
			parsed_data.push_back(ast);
		}
	}

	namespace{

		occ_nums_array string_to_occ_nums_array(const std::string &s){
			std::istringstream input(s);
			occ_nums_array occ_nums;
			std::array<int,4> arr;
			int i = 0;
			for(;;){
				int n;
				input >> n;
				if(input.bad()){
					throw parsing_exception("Error parsing occ_nums");
				}
				if(input.eof()){
					break;
				} else if(input.fail()){
					input.clear();
					input.get();
				} else {
					arr[i] = n;
					++i;
					if(i == 4){
						occ_nums.push_back(arr);
						i = 0;
					}
				}
			}
			std::size_t counter = 0;
			for(auto &g_i: occ_nums){
				int n = g_i[0];
				int l = g_i[1];
				int m = g_i[2];
				int ms = g_i[3];
				if(!in_range(1, 7, n)){
					std::string error = "Wrong input of occ_num #";
					error += std::to_string(counter);
					error += ", n wrong";
					throw parsing_exception(error.c_str());
				}
				if(!in_range(0, n-1, l)){
					std::string error = "Wrong input of occ_num #";
					error += std::to_string(counter);
					error += ", l wrong";
					throw parsing_exception(error.c_str());
				}
				if(!in_range(-l,l,m)){
					std::string error = "Wrong input of occ_num #";
					error += std::to_string(counter);
					error += ", m is not -l:l";
					throw parsing_exception(error.c_str());
				}
				if(ms != 1 && ms != -1){
					std::string error = "Wrong input of occ_num #";
					error += std::to_string(counter);
					error += ", ms is not -1:1";
					throw parsing_exception(error.c_str());
				}
				++counter;
			}
			return occ_nums;
		}

		std::size_t roman_to_arabic(const std::string &s){
			if(s.empty()){
				return 0;
			}
			std::regex not_roman_regex("[^IVXLCDM]*$");
			auto it_begin
				= std::sregex_iterator(s.begin(), s.end(), not_roman_regex);
			auto it_end = std::sregex_iterator();
			if(it_begin != it_end){
				throw parsing_exception("Non Roman digits occured.");
			}
			std::regex roman_regex("^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})"
					"(IX|IV|V?I{0,3})$");
			std::string check_s = std::regex_replace(s, roman_regex, "");
			if(check_s != ""){
				throw parsing_exception("Not a Roman number");
			}

			std::unordered_map<char,int> conv_map
				= {{'I',1},{'V',5},{'X',10},{'L',50},
					{'C',100},{'D',500},{'M',1000}};
			auto convert = [&conv_map]
				(const std::string::const_reverse_iterator &it){
					return conv_map.at(*it);
				};
			int total = 0;
			char prev = '*';
			for(auto it = s.rbegin(); it != s.rend(); ++it){
				if(convert(it) < total && *it != prev){
					total -= convert(it);
					prev = *it;
				} else {
					total += convert(it);
					prev = *it;
				}
			}
			return total;
		}

		bool is_valid_pattern(const std::string &s,
				const std::string &pattern){
			std::regex pat_regex(pattern);
			auto it_pat_regex_begin = std::sregex_iterator(s.begin(),
					s.end(), pat_regex);
			auto it_pat_regex_end = std::sregex_iterator();

			if(it_pat_regex_begin != it_pat_regex_end){
				return true;
			} else{
				return false;
			}
		}
	} /* end anonymous namespace */
} /* end namespace eff_z */
