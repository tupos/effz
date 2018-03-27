#ifndef EFFZ_MAIN_LOOP_FUNCTIONS_H
#define EFFZ_MAIN_LOOP_FUNCTIONS_H

#include <string>
#include <iostream>
#include <memory>

namespace eff_z{
	class base_menu{
		public:
			typedef std::shared_ptr<base_menu> base_menu_ptr;
			base_menu() : menu_text(
					"p. Print this menu.\n"
					"q. Exit the effective charge program.\n\n"
					"Please enter your choice...\n"),
			is_quit(false), show_menu(true) {}
			virtual ~base_menu() = default;
			virtual base_menu_ptr get_next_menu(char choice) = 0;
			virtual void print_menu(){
				std::cout << menu_text << "\n";
			};
			inline bool is_quit_selected() const{
				return is_quit;
			};
			inline bool is_show_menu(){
				return show_menu;
			}
		protected:
			std::string menu_text;
			bool is_quit;
			bool show_menu;
			inline void quit(){
				is_quit = true;
			}
			inline void not_show_menu(){
				show_menu = false;
			}
			void base_action_handler(char choice);
	};
	typedef base_menu::base_menu_ptr base_menu_ptr;

	class base_menu_with_help : virtual public base_menu{
		public:
			base_menu_with_help();
			base_menu_ptr get_next_menu(char choice) override = 0;
		protected:
			void help_action_handler(char choice);
		private:
			const std::string format_string;
			const std::string format_occ_nums;
	};

	class base_menu_with_previous : virtual public base_menu{
		public:
			base_menu_with_previous(base_menu_ptr prev_menu);
			base_menu_ptr get_next_menu(char choice) override = 0;
		protected:
			base_menu_ptr prev_action_handler(char choice);
			base_menu_ptr prev_menu;
	};

	class main_menu : public base_menu{
		public:
			main_menu();
			base_menu_ptr get_next_menu(char choice) final;
	};

	class zeroth_order_energy_menu :
		public base_menu_with_previous, public base_menu_with_help{
		public:
			explicit zeroth_order_energy_menu(base_menu_ptr prev_menu);
			base_menu_ptr get_next_menu(char choice) final;
	};

	class zeroth_order_density_menu :
		public base_menu_with_previous, public base_menu_with_help{
		public:
			explicit zeroth_order_density_menu(base_menu_ptr prev_menu);
			base_menu_ptr get_next_menu(char choice) final;
	};

	class zeroth_order_asf_menu :
		public base_menu_with_previous, public base_menu_with_help{
		public:
			explicit zeroth_order_asf_menu(base_menu_ptr prev_menu);
			base_menu_ptr get_next_menu(char choice) final;
	};

	class result_menu_with_prev : public base_menu_with_previous{
		public:
			explicit result_menu_with_prev(base_menu_ptr prev_menu);
			base_menu_ptr get_next_menu(char choice) final;
	};
} /* end namespace eff_z */

#endif /* EFFZ_MAIN_LOOP_FUNCTIONS_H */
