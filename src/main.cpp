#include <Python.h>

#include "effz_main_loop_functions.h"
#include "effz_config.h"

#ifdef DEBUG
#include "effz_debug.h"
#endif

#include <cstdlib>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void my_handler(int s){
           printf("Caught signal %d\n",s);
           exit(1);

}

using namespace eff_z;


int main(int argc, char *argv[]) try {

	config::shared_config().check_dirs();

	putenv((char*)"PYTHONDONTWRITEBYTECODE=1");
	Py_Initialize();
	PyRun_SimpleString("import sys\nimport os");
	PyRun_SimpleString(config::shared_config()
			.get_python_path_cmd().c_str());

#ifdef DEBUG
	PyRun_SimpleString("print(sys.path)");
#endif
struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = my_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

	char user_input;
	base_menu_ptr current_menu = std::make_shared<main_menu>();
	while(!current_menu->is_quit_selected()){
		if(current_menu->is_show_menu()){
			current_menu->print_menu();
		}
		std::cin >> user_input;
		base_menu_ptr new_menu
			= current_menu->get_next_menu(user_input);
sigaction(SIGINT, &sigIntHandler, NULL);
		if(new_menu){
			current_menu.swap(new_menu);
		}
	}

	if(Py_FinalizeEx() < 0){
		return 120;
	}

#ifdef DEBUG
	test_output();
#endif

	return EXIT_SUCCESS;

} catch(const std::exception &e){
	std::cerr << e.what() << "\n";
	return EXIT_FAILURE;
} catch(...){
	return EXIT_FAILURE;
}
