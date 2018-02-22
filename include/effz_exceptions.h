#ifndef EFFZ_EXCEPTIONS_H
#define EFFZ_EXCEPTIONS_H

#include <stdexcept>

namespace eff_z{

	class python_exception : public std::runtime_error{
		public:
			explicit python_exception(const char* what_arg)
				: std::runtime_error(what_arg) {}
	};

	class parsing_exception : public std::runtime_error{
		public:
			explicit parsing_exception(const char* what_arg)
				: std::runtime_error(what_arg) {}
	};
} /* end namespace eff_z */


#endif /* EFFZ_EXCEPTIONS_H */
