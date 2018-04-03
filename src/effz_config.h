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

		private:
			config();

			const std::string home_dir;
			const std::string python_src_dir;
			const std::string database_dir;
			const std::string&
				get_checked_dir(const std::string &dir_name) const;
	};

} /* end namespace eff_z */

#endif /* EFFZ_CONFIG_H */
