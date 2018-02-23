#include "effz_user_input.h"

#include "effz_exceptions.h"
#include "effz_utility.h"

#include <regex>
#include <iterator>
#include <iostream>
#include <sstream>

namespace eff_z{

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
			std::regex num_of_occ_nums_regex(num_of_occ_nums_pattern);

			auto it_num_occ_nums_begin =
				std::sregex_iterator(s.begin(), s.end(),
						num_of_occ_nums_regex);
			auto it_num_occ_nums_end = std::sregex_iterator();

			std::size_t num_occ_nums = 0;
			if(it_num_occ_nums_begin != it_num_occ_nums_end){
				num_occ_nums = std::distance(it_num_occ_nums_begin,
						it_num_occ_nums_end);
			}

			auto it_o_pat_regex_begin =
				std::sregex_iterator(s.begin(), s.end(), o_pattern_regex);
			auto it_o_pat_regex_end = std::sregex_iterator();

			if(it_o_pat_regex_begin == it_o_pat_regex_end){
				throw parsing_exception("Error in the input of occ_nums");
			}

			std::size_t num_found_patterns
				= std::distance(it_o_pat_regex_begin, it_o_pat_regex_end);

			if((num_found_patterns - 1) != num_occ_nums){
				throw parsing_exception("Error in the input of occ_nums");
			}

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

} /* end namespace eff_z */
