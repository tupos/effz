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
