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

			const std::string& get_python_src_dir() const;
			const std::string& get_database_src_dir() const;
		private:
			config();

			static const std::string home_dir;
			static const std::string python_src_dir;
			static const std::string database_dir;

			bool is_program_folder_exist() const;
			void mkdir_program_folder() const;
	};

} /* end namespace eff_z */

#endif /* EFFZ_CONFIG_H */
