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

#include "effz_config.h"


#include <iostream>
#include <system_error>
#include <cerrno>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>

namespace eff_z{

	enum class dir_prop {blk, chr, dir, fifo, reg, lnk, not_exist};
	class file_sys{
		public:
			dir_prop dir_type(const std::string &dir_name) const;
			bool exists(const std::string &fs_obj) const;
			void print_dir_type(const std::string &fs_obj) const;
			void mkdir(const std::string &dir_name) const;
			void mkdir_p(const std::string &dir_name) const;
	};

	dir_prop file_sys::dir_type(const std::string &dir_name) const{
#ifdef _WIN32
		//TODO add windows implementation here
#else
		struct stat info;
		if(!stat(dir_name.c_str(), &info)){
			if(S_ISBLK(info.st_mode)) {return dir_prop::blk;}
			else if(S_ISCHR(info.st_mode)) {return dir_prop::chr;}
			else if(S_ISDIR(info.st_mode)) {return dir_prop::dir;}
			else if(S_ISFIFO(info.st_mode)) {return dir_prop::fifo;}
			else if(S_ISREG(info.st_mode)) {return dir_prop::reg;}
			else if(S_ISLNK(info.st_mode)) {return dir_prop::lnk;}
			else {
				throw std::runtime_error("Unknown file type");
			}
		} else{
			if(errno == ENOENT){
				return dir_prop::not_exist;
			} else {
				throw std::system_error(errno, std::generic_category(),
						"Error checking for dir existance");
			}
		}
#endif
	}

	bool file_sys::exists(const std::string &fs_obj) const{
		dir_prop dt;
		try{
			dt = dir_type(fs_obj);
		} catch (const std::exception &e){
			std::cerr << e.what() << "\n";
			throw std::exception();
		}
		switch(dt){
			case dir_prop::not_exist : { return false; break;}
			case dir_prop::dir :
			case dir_prop::blk :
			case dir_prop::chr :
			case dir_prop::fifo :
			case dir_prop::reg :
			case dir_prop::lnk : { return true; break;}
			default: throw std::runtime_error("Unknown file type"); break;
		}
	}

	void file_sys::print_dir_type(const std::string &fs_obj) const{
		dir_prop dt;
		try{
			dt = dir_type(fs_obj);
		} catch (const std::exception &e){
			std::cerr << e.what() << "\n";
			throw std::exception();
		}
		switch(dt){
			case dir_prop::not_exist : {std::cout << "not_exist\n"; break;}
			case dir_prop::dir : {std::cout << "dir\n"; break;}
			case dir_prop::blk : {std::cout << "blk\n"; break;}
			case dir_prop::chr : {std::cout << "chr\n"; break;}
			case dir_prop::fifo : {std::cout << "fifo\n"; break;}
			case dir_prop::reg : {std::cout << "reg\n"; break;}
			case dir_prop::lnk : {std::cout << "lnk\n"; break;}
		}
	}

	void file_sys::mkdir(const std::string &dir_name) const
	{
		mode_t nMode = 0755; // UNIX style permissions
		int nError = 0;
#ifdef _WIN32
		//TODO add windows implementation here
		//nError = _mkdir(dir_name.c_str()); // can be used on Windows
#else
		nError = ::mkdir(dir_name.c_str(),nMode); // can be used on non-Windows
#endif
		if(nError != 0) {
			throw std::system_error(errno, std::generic_category(),
					"Error creating dir");
		}
	}

	void file_sys::mkdir_p(const std::string &dir_name) const
	{
		//TODO remove call to std::system
#ifdef _WIN32
		//TODO add windows implementation here
		//nError = _mkdir(dir_name.c_str()); // can be used on Windows
#else
		std::string command = std::string("mkdir -p ") + dir_name;
		int error = std::system(command.c_str());
		if(error != 0) {
			throw std::runtime_error("std::system mkdir -p failed");
		}
#endif
	}

 /*
  *config implementation *******************************
  */
	config& config::shared_config()
	{
		static config instance;
		return instance;
	}

	config::config() : home_dir(EFFZ_HOME_DIRECTORY), /* exported from
														 configure.ac	*/
	python_src_dir(
			std::string(EFFZ_PYTHON_SRC_DIR)), /* exported from
												  src/Makefile.am */
	database_dir(home_dir + "/effz/database_dir"),
	python_path_cmd(
			std::string("sys.path.append(\"") + python_src_dir + "\")") {}

	const std::string& config::get_checked_dir(
			const std::string &dir_name) const
	{
		file_sys fs;
		dir_prop dt = fs.dir_type(dir_name);
		switch(dt){
			case dir_prop::dir : { break; }
			case dir_prop::not_exist : {
										   try{ fs.mkdir_p(dir_name); }
										   catch(const std::exception &e){
											   std::cerr <<
												   e.what() << "\n";
											   throw std::exception();
										   }
										   break;
									   }
			case dir_prop::blk :
			case dir_prop::chr :
			case dir_prop::fifo :
			case dir_prop::reg :
			case dir_prop::lnk : {
									 std::string e("Can not create ");
									 e += dir_name + " dir."
										 + " Non dir dype object exists";
									 throw std::runtime_error(e);
								 }
		}
		return dir_name;
	}

	const std::string& config::get_checked_python_src_dir() const
	{
		return get_checked_dir(python_src_dir);
	}

	const std::string& config::get_checked_database_dir() const
	{
		return get_checked_dir(database_dir);
	}

	const std::string& config::get_python_src_dir() const
	{
		return python_src_dir;
	}

	const std::string& config::get_database_dir() const
	{
		return database_dir;
	}

	void config::check_dirs() const{
		//get_checked_python_src_dir();
		get_checked_database_dir();
	}
	const std::string& config::get_python_path_cmd() const
	{
		return python_path_cmd;
	}
} /* end namespace eff_z */
