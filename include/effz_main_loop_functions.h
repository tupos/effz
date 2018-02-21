#ifndef EFFZ_MAIN_LOOP_FUNCTIONS_H
#define EFFZ_MAIN_LOOP_FUNCTIONS_H

namespace eff_z{
	void print_main_menu();

	void print_select_order_menu();

	void print_zeroth_order_energy_menu();

	void print_occ_nums_format_help();

	int main_menu(char user_input);

	int zeroth_order_energy_loop();

	int density_symbolic_loop();

	int asf_symbolic_loop();

	int zeroth_order_energy_menu(char user_input);

	int zeroth_order_energy_enter_parameters_loop();
} /* end namespace eff_z */

#endif /* EFFZ_MAIN_LOOP_FUNCTIONS_H */
