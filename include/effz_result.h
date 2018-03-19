#ifndef EFFZ_RESULT_H
#define EFFZ_RESULT_H

#include "effz_zeroth_order.h"
#include "effz_user_input.h"

#include <string>
#include <iostream>

namespace eff_z{
	namespace zeroth_order{
		class base_result{
			public:
				base_result();
				virtual ~base_result() = default;
				virtual void print_result(){
					std::cout << result_string;
				};
			protected:
				std::string result_string;
		};

		class energy_result : public base_result{
			public:
				energy_result(const std::string &name, int z,
						const occ_nums_array &g);
			private:
				const std::string name;
				const int z;
				const occ_nums_array g;
				double z_star;
				double energy;
		};

	} /* end namespace zeroth_order */
} /* end namespace eff_z */


#endif /* EFFZ_RESULT_H */
