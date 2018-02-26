#include "effz_user_input.h"

#include "effz_exceptions.h"
#include "effz_utility.h"
#include "effz_atomic_data.h"

#include <regex>
#include <tuple>
#include <iterator>
#include <iostream>
#include <sstream>

namespace eff_z{

	occ_nums_array string_to_occ_nums_array(const std::string &s);
	int roman_into_arabic(const std::string &s);
	bool is_cor_num_braces_occ_nums(const std::string &s);
	bool are_wrong_symbols(const std::string &s, const std::string &pattern);
	std::tuple<std::sregex_iterator,std::sregex_iterator>
		get_checked_regex_its(
				const std::string &s,
				const std::regex &pattern,
				const std::regex &delimeter);

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

	int roman_into_arabic(const std::string &s){
		return 0;
	}

	bool is_cor_num_braces_occ_nums(const std::string &s){
			std::regex num_of_2opened_brackets_regex("\\{\\s*\\{");
			std::regex num_of_2closed_brackets_regex("\\}\\s*\\}");

			auto it_2opened_brackets_begin =
				std::sregex_iterator(s.begin(), s.end(),
						num_of_2opened_brackets_regex);
			auto it_2opened_brackets_end = std::sregex_iterator();

			auto it_2closed_brackets_begin =
				std::sregex_iterator(s.begin(), s.end(),
						num_of_2closed_brackets_regex);
			auto it_2closed_brackets_end = std::sregex_iterator();

			std::size_t num_2opened_brackets
				= std::distance(it_2opened_brackets_begin,
						it_2opened_brackets_end);

			std::size_t num_2closed_brackets
				= std::distance(it_2closed_brackets_begin,
						it_2closed_brackets_end);

			if(num_2opened_brackets != num_2closed_brackets){
				return false;
			}

			return true;
	}

	bool are_wrong_symbols(const std::string &s, const std::string &pattern){
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

	std::tuple<std::sregex_iterator,std::sregex_iterator>
		get_checked_regex_its(
				const std::string &s,
				const std::regex &pattern,
				const std::regex &delimeter)
		{
			auto it_num_occ_nums_begin =
				std::sregex_iterator(s.begin(), s.end(),
						delimeter);
			auto it_num_occ_nums_end = std::sregex_iterator();

			std::size_t num_occ_nums = 0;
			if(it_num_occ_nums_begin != it_num_occ_nums_end){
				num_occ_nums = std::distance(it_num_occ_nums_begin,
						it_num_occ_nums_end);
			}

			auto it_pat_regex_begin =
				std::sregex_iterator(s.begin(), s.end(), pattern);
			auto it_pat_regex_end = std::sregex_iterator();

			if(it_pat_regex_begin == it_pat_regex_end){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::size_t num_found_patterns
				= std::distance(it_pat_regex_begin, it_pat_regex_end);
			std::cout << num_found_patterns << " " << num_occ_nums  << "\n";

			if((num_found_patterns - 1) != num_occ_nums){
				throw parsing_exception("Error in the input of occ_nums");
			}

			return std::make_tuple(it_pat_regex_begin, it_pat_regex_end);
		}

	std::vector<occ_nums_array>
		parse_occ_nums_n_format(const std::string &s){
			if(are_wrong_symbols(s, "[^0-9,]")){
				throw parsing_exception("Error in the input of occ_nums.");
			}
			std::string n_pattern
				= "\\b([1-9][0-9]?|100)\\b";
			std::regex n_pattern_regex(n_pattern);

			std::string num_occ_nums_pattern
				= "\\b\\s*,\\s*(?=\\b([1-9][0-9]?|100)\\b)";
			std::regex num_occ_nums_regex(num_occ_nums_pattern);

			std::tuple<std::sregex_iterator,std::sregex_iterator>
				its = get_checked_regex_its(s, n_pattern_regex,
						num_occ_nums_regex);

			auto it_n_pat_regex_begin = std::get<0>(its);
			auto it_n_pat_regex_end = std::get<1>(its);

			occ_nums_array occ_nums;
			std::vector<occ_nums_array> occ_nums_a;

			std::size_t counter = 0;
			for (std::sregex_iterator i = it_n_pat_regex_begin;
					i != it_n_pat_regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				try{
					int el_num = std::stoi(match_str);
					occ_nums = atomic_data::occ_nums_data::
						g.at(el_num-1);
				} catch (const std::out_of_range &e){
					std::cerr << "Error in the input of occ_nums #"
						<< counter << "\n";
					std::cerr << " " << match_str << "does not exist\n";
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
			std::string N_pattern
				= "\\b[A-Z][a-z]?\\b";
			std::regex N_pattern_regex(N_pattern);

			std::string num_occ_nums_pattern
				= "\\b\\s*,\\s*(?=\\b[A-Z][a-z]?\\b)";
			std::regex num_occ_nums_regex(num_occ_nums_pattern);

			std::tuple<std::sregex_iterator,std::sregex_iterator>
				its = get_checked_regex_its(s, N_pattern_regex,
						num_occ_nums_regex);

			auto it_N_pat_regex_begin = std::get<0>(its);
			auto it_N_pat_regex_end = std::get<1>(its);

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
					std::cerr << " " << match_str << "does not exist\n";
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
			std::string i_pattern
				= "\\b[A-Z][a-z]?\\b\\s*\\b(IX|IV|V?I{1,3})\\b";
			std::regex i_pattern_regex(i_pattern);

			std::string num_occ_nums_pattern
				= "\\b\\s*,\\s*(?=\\b[A-Z][a-z]?\\b\\s*"
				"\\b(IX|IV|V?I{1,3})\\b)";
			std::regex num_occ_nums_regex(num_occ_nums_pattern);

			std::tuple<std::sregex_iterator,std::sregex_iterator>
				its = get_checked_regex_its(s, i_pattern_regex,
						num_occ_nums_regex);

			auto it_i_pat_regex_begin = std::get<0>(its);
			auto it_i_pat_regex_end = std::get<1>(its);

			auto extract_el = [](const std::string& s){
				std::regex el_regex("^[A-Z][a-z]?\\b");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(), el_regex);
				auto it_end = std::sregex_iterator();
				if(std::distance(it_begin, it_end) > 1){
					throw
						parsing_exception("Error in the input of occ_nums.");
				}
				std::smatch match = *it_begin;
				std::string match_str = match.str();
				return parse_occ_nums_N_format(match_str);
			};
			auto extract_roman_n = [](const std::string& s){
				std::regex el_regex("\\b(IX|IV|V?I{1,3})\\s*$");
				auto it_begin = std::sregex_iterator(s.begin(),
						s.end(), el_regex);
				auto it_end = std::sregex_iterator();
				if(std::distance(it_begin, it_end) > 1){
					throw
						parsing_exception("Error in the input of occ_nums.");
				}
				std::smatch match = *it_begin;
				std::string match_str = match.str();
				return parse_occ_nums_N_format(match_str);
			};

			occ_nums_array occ_nums;
			std::vector<occ_nums_array> occ_nums_a;

			std::size_t counter = 0;
			for (std::sregex_iterator i = it_i_pat_regex_begin;
					i != it_i_pat_regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				std::cout << " " << match_str << '\n';
				try{
					//int el_num =
						//atomic_data::element_data::
						//element_names.at(match_str);
					//occ_nums = atomic_data::occ_nums_data::
						//g.at(el_num-1);
				} catch (const std::out_of_range &e){
					std::cerr << "Error in the input of occ_nums #"
						<< counter << "\n";
					std::cerr << " " << match_str << "does not exist\n";
					throw parsing_exception
						("Error in the input of occ_nums");
				}
				occ_nums_a.push_back(occ_nums);
				++counter;
			}

			return occ_nums_a;
		}

	std::vector<occ_nums_array>
		parse_occ_nums_o_format(const std::string &s){
			if(!is_cor_num_braces_occ_nums(s)){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::string o_pattern
				= "\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\}"
				"(\\s*,\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\})*"
				"\\s*\\}";
			std::regex o_pattern_regex(o_pattern);

			std::string num_of_occ_nums_pattern
				= "\\}\\s*\\}\\s*,\\s*\\{\\s*\\{";
			std::regex num_occ_nums_regex(num_of_occ_nums_pattern);

			std::tuple<std::sregex_iterator,std::sregex_iterator>
				its = get_checked_regex_its(s, o_pattern_regex,
						num_occ_nums_regex);

			auto it_o_pat_regex_begin = std::get<0>(its);
			auto it_o_pat_regex_end = std::get<1>(its);

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
			if(!is_cor_num_braces_occ_nums(s)){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::string O_pattern
				= "\\b[A-Z][a-z]?\\b\\s*\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\}"
				"(\\s*,\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*-?\\d\\s*\\})*"
				"\\s*\\}";
			std::regex O_pattern_regex(O_pattern);

			std::string num_of_occ_nums_pattern
				= "\\}\\s*\\}\\s*,\\s*\\b[A-Z][a-z]?\\b\\s*\\{\\s*\\{";
			std::regex num_occ_nums_regex(num_of_occ_nums_pattern);

			std::tuple<std::sregex_iterator,std::sregex_iterator>
				its = get_checked_regex_its(s, O_pattern_regex,
						num_occ_nums_regex);

			auto it_O_pat_regex_begin = std::get<0>(its);
			auto it_O_pat_regex_end = std::get<1>(its);

			std::size_t counter = 0;
			std::vector<occ_nums_array> element_occ_nums;
			std::vector<occ_nums_array> occ_nums_to_append;
			std::vector<occ_nums_array> occ_nums_a;
			occ_nums_array tmp_occ_nums;

			for (std::sregex_iterator i = it_O_pat_regex_begin;
					i != it_O_pat_regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				//std::cout << " " << match_str << '\n';
				try{
					element_occ_nums
						= parse_occ_nums_N_format(match_str);
					occ_nums_to_append
						= parse_occ_nums_o_format(match_str);
					if(element_occ_nums.size() > 1
							|| occ_nums_to_append.size() > 1){
						throw parsing_exception(
								"Error in the input of occ_nums.");
					}
					for(auto &g_i: element_occ_nums.at(0)){
						tmp_occ_nums.push_back(g_i);
					}
					for(auto &g_i: occ_nums_to_append.at(0)){
						tmp_occ_nums.push_back(g_i);
					}
				} catch (const parsing_exception& e){
					std::cerr << "Error in the input of occ_nums #"
						<< counter << "\n";
					std::cerr << e.what();
					std::cerr << " " << match_str << '\n';
					throw parsing_exception
						("Error in the input of occ_nums");
				}
				occ_nums_a.push_back(tmp_occ_nums);
				tmp_occ_nums.clear();
				++counter;
			}

			return occ_nums_a;
	}

} /* end namespace eff_z */
