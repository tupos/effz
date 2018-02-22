#include "effz_user_input.h"

#include "effz_exceptions.h"

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
		int counter = 0;
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
				if(i == 4){
					i = 0;
				}
				arr[i] = n;
				if(i == 3){
					occ_nums.push_back(arr);
				}
				++i;
			}
		}
		return occ_nums;
	}

	std::vector<occ_nums_array>
		parse_occ_nums_o_format(const std::string &s1){
			std::string o_pattern
				= "\\{\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*(1|-1)\\s*\\}"
				"(\\s*,\\s*"
				"\\{\\s*\\d\\s*,\\s*\\d\\s*,\\s*\\d\\s*,\\s*(1|-1)\\s*\\})*"
				"\\s*\\}";
			std::regex o_pattern_regex(o_pattern);
			std::string s = "{{1,0,0,1},{2,3,4,-1}}, {{1,0,0,1}}";
			auto regex_begin =
				std::sregex_iterator(s.begin(), s.end(), o_pattern_regex);
			auto regex_end = std::sregex_iterator();

			if(regex_begin == regex_end)
				throw parsing_exception("Error in the input of occ_nums");

			for (std::sregex_iterator i = regex_begin;
					i != regex_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				occ_nums_array arr = string_to_occ_nums_array(match_str);
				std::cout << "  " << match_str << '\n';
				std::cout << "{";
				for(auto &g_i: arr){
					std::cout << "{";
					for(auto &g_ij: g_i){
						std::cout << g_ij << ",";
					}
					std::cout << "}";
				}
				std::cout << "}\n";
			}

			return std::vector<occ_nums_array>();

		}

} /* end namespace eff_z */
