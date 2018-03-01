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

	occ_nums_array string_to_occ_nums_array(const std::string &s);
	std::size_t roman_to_arabic(const std::string &s);
	bool is_wrong_pattern(const std::string &s, const std::string &pattern);

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
		typedef std::unordered_map<char,int>::key_type Key;
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

	bool is_wrong_pattern(const std::string &s, const std::string &pattern){
		std::regex pat_regex(pattern);
		auto it_pat_regex_begin = std::sregex_iterator(s.begin(), s.end(),
				pat_regex);
		auto it_pat_regex_end = std::sregex_iterator();

		if(it_pat_regex_begin != it_pat_regex_end){
			return true;
		} else{
			return false;
		}
	}

	std::vector<int> parse_z_format(const std::string &s){
		if(!is_wrong_pattern(s, "^([1-9][0-9]?|100)\\b"
					"(\\s*,\\s*\\b([1-9][0-9]?|100)\\b)*$")){
			throw
				parsing_exception("Error in the input string.");
		}
		std::string z_pattern
			= "\\b([1-9][0-9]?|100)\\b";
		std::regex z_pattern_regex(z_pattern);

		auto it_z_pat_regex_begin = std::sregex_iterator(s.begin(),
				s.end(), z_pattern_regex);
		auto it_z_pat_regex_end = std::sregex_iterator();

		std::vector<int> nums;
		for (std::sregex_iterator i = it_z_pat_regex_begin;
				i != it_z_pat_regex_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			try{
				int num = std::stoi(match_str);
				nums.push_back(num);
			} catch (const std::out_of_range &e){
				std::cerr << "Error in the input.\n";
				throw parsing_exception
					("Error in the input.");
			} catch (const std::invalid_argument &e){
				std::cerr << e.what() << "\n";
				throw parsing_exception
					("Error in the input.");
			}
		}
		return nums;
	}

	std::vector<occ_nums_array>
		parse_occ_nums_n_format(const std::string &s){
			std::vector<int> el_nums;
			try{
				el_nums = parse_z_format(s);
			} catch (const parsing_exception &e){
				std::cerr << e.what() << "\n";
				throw parsing_exception("Error in the input string");
			}

			std::vector<occ_nums_array> occ_nums_a;
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
				occ_nums_a.push_back(occ_nums);
				++counter;
			}
			return occ_nums_a;
		}

	std::vector<occ_nums_array>
		parse_occ_nums_N_format(const std::string &s){
			if(!is_wrong_pattern(s,
						"^[A-Z][a-z]?\\b(\\s*,\\s*\\b[A-Z][a-z]?\\b)*$")){
				throw parsing_exception(
						"Error in the input string N format.");
			}
			std::string N_pattern
				= "\\b[A-Z][a-z]?\\b";
			std::regex N_pattern_regex(N_pattern);

			auto it_N_pat_regex_begin = std::sregex_iterator(s.begin(),
					s.end(), N_pattern_regex);
			auto it_N_pat_regex_end = std::sregex_iterator();

			occ_nums_array occ_nums;
			std::vector<occ_nums_array> occ_nums_a;

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
					std::cerr << " " << match_str << " does not exist\n";
					throw parsing_exception
						("Error in the input of occ_nums");
				}
				occ_nums_a.push_back(occ_nums);
				++counter;
			}

			return occ_nums_a;
		}

	std::vector<occ_nums_array>
		parse_occ_nums_i_format(const std::string &s){
			if(!is_wrong_pattern(s,
						"^[A-Z][a-z]?\\b\\s*\\b(XC|XL|L?X{0,3})"
						"(IX|IV|V?I{1,3})\\b"
						"(\\s*,\\s*\\b[A-Z][a-z]?\\b\\s*\\b(XC|XL|L?X{0,3})"
						"(IX|IV|V?I{1,3})\\b)*$")){
				throw parsing_exception("Error in the input string.");
			}

			std::string i_pattern
				= "\\b[A-Z][a-z]?\\b\\s*\\b(XC|XL|L?X{0,3})"
				"(IX|IV|V?I{1,3})\\b";
			std::regex i_pattern_regex(i_pattern);

			auto it_i_pat_regex_begin = std::sregex_iterator(s.begin(),
					s.end(), i_pattern_regex);
			auto it_i_pat_regex_end = std::sregex_iterator();

			auto extract_el = [](const std::string& s){
				std::regex el_regex("^[A-Z][a-z]?\\b");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(), el_regex);
				auto it_end = std::sregex_iterator();
				std::smatch match = *it_begin;
				std::string match_str = match.str();
				return parse_occ_nums_N_format(match_str).at(0);
			};
			auto extract_roman_n = [](const std::string& s){
				std::regex
					el_regex("\\b(XC|XL|L?X{0,3})(IX|IV|V?I{1,3})\\s*$");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(), el_regex);
				auto it_end = std::sregex_iterator();
				std::smatch match = *it_begin;
				std::string match_str = match.str();
				return roman_to_arabic(match_str);
			};

			occ_nums_array occ_nums;
			std::size_t roman_n = 0;
			std::vector<occ_nums_array> occ_nums_a;

			std::size_t counter = 0;
			for (std::sregex_iterator i = it_i_pat_regex_begin;
					i != it_i_pat_regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				//std::cout << " " << match_str << '\n';
				try{
					occ_nums = extract_el(match_str);
					roman_n = extract_roman_n(match_str);
					if(occ_nums.size() <= roman_n - 1){
						throw parsing_exception("Ionization degree is"
								" larger than the number of electrons.");
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
				occ_nums_a.push_back(occ_nums);
				++counter;
			}

			return occ_nums_a;
		}

	std::vector<occ_nums_array>
		parse_occ_nums_o_format(const std::string &s){
			if(!is_wrong_pattern(s,
						"^\\{\\s*"
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
						"\\s*\\})*$")){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::string o_pattern
				= "\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\}"
				"(\\s*,\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\})*"
				"\\s*\\}";
			std::regex o_pattern_regex(o_pattern);

			auto it_o_pat_regex_begin = std::sregex_iterator(s.begin(),
					s.end(), o_pattern_regex);
			auto it_o_pat_regex_end = std::sregex_iterator();

			std::size_t counter = 0;
			occ_nums_array occ_nums;
			std::vector<occ_nums_array> occ_nums_a;

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
				occ_nums_a.push_back(occ_nums);
				++counter;
			}

			return occ_nums_a;
		}

	std::vector<occ_nums_array>
		parse_occ_nums_O_format(const std::string &s){
			if(!is_wrong_pattern(s,
						"^[A-Z][a-z]?\\b\\s*\\{\\s*"
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
						"\\s*\\})*$")){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::string O_pattern
				= "\\b[A-Z][a-z]?\\b\\s*\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\}"
				"(\\s*,\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\})*"
				"\\s*\\}";
			std::regex O_pattern_regex(O_pattern);

			auto it_O_pat_regex_begin = std::sregex_iterator(s.begin(),
					s.end(), O_pattern_regex);
			auto it_O_pat_regex_end = std::sregex_iterator();

			auto extract_el = [](const std::string &s){
				std::regex el_name_regex("\\b[A-Z][a-z]?\\b");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(),el_name_regex);
				std::smatch match = *it_begin;
				return parse_occ_nums_N_format(match.str()).at(0);
			};
			auto extract_occ_nums = [](const std::string &s){
				std::regex el_occ_nums_regex("\\{\\s*"
						"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
						"\\s*-?\\d\\s*\\}"
						"(\\s*,\\s*"
						"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,"
						"\\s*-?\\d\\s*\\})*"
						"\\s*\\}");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(), el_occ_nums_regex);
				std::smatch match = *it_begin;
				return parse_occ_nums_o_format(match.str()).at(0);
			};

			std::size_t counter = 0;
			occ_nums_array element_occ_nums;
			occ_nums_array occ_nums_to_append;
			std::vector<occ_nums_array> occ_nums_a;

			for (std::sregex_iterator i = it_O_pat_regex_begin;
					i != it_O_pat_regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				//std::cout << " " << match_str << '\n';
				try{
					element_occ_nums = extract_el(match_str);
					occ_nums_to_append = extract_occ_nums(match_str);
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
				occ_nums_a.push_back(element_occ_nums);
				++counter;
			}

			return occ_nums_a;
	}

	std::tuple<std::vector<int>,std::vector<occ_nums_array>>
		parse_format_string(const std::string &s){
			std::unordered_set<std::string> flags
				= {"-z", "-f", "-v", "-o"};
			std::string strings_pattern
				= "^(-[zfvo]\\s*[^zvfo;-]+){3,4}"
				"(\\s*;\\s*(-[zfvo]\\s*[^zvfo;-]+){3,4})*$";
			std::string format_string
				= "^(-[zfvo]\\s*[^zvfo;-]+){3,4}";

		}

} /* end namespace eff_z */
