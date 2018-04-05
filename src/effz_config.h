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

#ifndef EFFZ_CONFIG_H
#define EFFZ_CONFIG_H

#include <config.h>

#include <string>

namespace eff_z{


	class config{
		public:
			static config& shared_config();

			config(const config&) = delete;
			void operator=(const config&) = delete;

			const std::string& get_checked_python_src_dir() const;
			const std::string& get_checked_database_dir() const;

			const std::string& get_python_src_dir() const;
			const std::string& get_database_dir() const;

			void check_dirs() const;
			const std::string& get_python_path_cmd() const;

		private:
			config();

			const std::string home_dir;
			const std::string python_src_dir;
			const std::string database_dir;
			const std::string python_path_cmd;
			const std::string&
				get_checked_dir(const std::string &dir_name) const;
	};

} /* end namespace eff_z */

#endif /* EFFZ_CONFIG_H */
