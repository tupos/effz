#ifndef EFFZ_MAIN_LOOP_FUNCTIONS_H
#define EFFZ_MAIN_LOOP_FUNCTIONS_H

#include <string>
#include <iostream>
#include <memory>

namespace eff_z{
	class base_menu{
		public:
			base_menu() : menu_text("base_menu"), is_quit(false) {}
			virtual ~base_menu() = default;
			virtual std::unique_ptr<base_menu>
				get_next_menu(char choice) = 0;
			virtual void print_menu(){
				std::cout << menu_text << "\n";
			};
			inline bool is_quit_selected() const{
				return is_quit;
			};
			inline void quit(){
				is_quit = true;
			}
		protected:
			std::string menu_text;
			bool is_quit;
	};
	typedef std::unique_ptr<base_menu> base_menu_ptr;

	class main_menu : public base_menu{
		public:
			main_menu();
			base_menu_ptr get_next_menu(char choice) final;
	};
	class zeroth_order_energy_menu : public base_menu{
		public:
			zeroth_order_energy_menu();
			base_menu_ptr get_next_menu(char choice) final;
	};
	class zeroth_order_density_menu : public base_menu{
		public:
			zeroth_order_density_menu();
			base_menu_ptr get_next_menu(char choice) final;
	};
	class zeroth_order_asf_menu : public base_menu{
		public:
			zeroth_order_asf_menu();
			base_menu_ptr get_next_menu(char choice) final;
	};

	class menus{
		public:
			void print_main() const;
			void print_select_order() const;
			void print_zeroth_order_energy() const;
		private:
			static const std::string main;
			static const std::string select_order;
			static const std::string zeroth_order_energy;
	};

	class help{
		public:
			void print_format_occ_nums() const;
			void print_format_string() const;
		private:
			static const std::string format_occ_nums;
			static const std::string format_string;
	};


	int main_menu1(char user_input);

	int zeroth_order_energy_loop();

	int density_symbolic_loop();

	int asf_symbolic_loop();

	int zeroth_order_energy_menu1(char user_input);

	int zeroth_order_energy_enter_parameters_loop();
} /* end namespace eff_z */

#endif /* EFFZ_MAIN_LOOP_FUNCTIONS_H */
