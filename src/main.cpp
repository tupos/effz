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

#include <Python.h>
#include <config.h>

#include "effz_main_loop_functions.h"
#include "effz_config.h"

#include <cstdlib>
#include <csignal>

using namespace eff_z;


void effz_sigint(int signal){
	std::cout << "exited with ctrl-c: " << signal << "\n";
	std::_Exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) try {

	std::signal(SIGINT,effz_sigint);

	config::shared_config().check_dirs();

	putenv((char*)"PYTHONDONTWRITEBYTECODE=1");
	Py_Initialize();
	PyRun_SimpleString("import sys\nimport os");
	PyRun_SimpleString(config::shared_config()
			.get_python_path_cmd().c_str());

#ifdef DEBUG
	PyRun_SimpleString("print(sys.path)");
#endif

	char user_input;
	base_menu_ptr current_menu = std::make_shared<main_menu>();
	while(!current_menu->is_quit_selected()){
		if(current_menu->is_show_menu()){
			current_menu->print_menu();
		}
		std::cin >> user_input;
		base_menu_ptr new_menu
			= current_menu->get_next_menu(user_input);
		if(new_menu){
			current_menu.swap(new_menu);
		}
	}
#ifdef EFFZ_PYTHON_VERSION_SIX
	if(Py_FinalizeEx() < 0){
		return 120;
	}
#elif defined(EFFZ_PYTHON_VERSION_FIVE)\
	|| defined(EFFZ_PYTHON_VERSION_FOUR)
	Py_Finalize();
#endif

	return EXIT_SUCCESS;

} catch(const std::exception &e){
	std::cerr << e.what() << "\n";
	return EXIT_FAILURE;
} catch(...){
	return EXIT_FAILURE;
}
