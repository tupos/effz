
bin_PROGRAMS = effz

effz_SOURCES = effz_atomic_data.cpp effz_config.cpp effz_exceptions.cpp\
			   effz_main_loop_functions.cpp effz_python_utility.cpp\
			   effz_result.cpp effz_spec_func.cpp effz_user_input.cpp\
			   effz_utility.cpp effz_zeroth_order.cpp\
			   effz_zeroth_order_python.cpp main.cpp\
			   \
			   effz_atomic_data.h effz_config.h effz_exceptions.h\
			   effz_integration.h effz_main_loop_functions.h\
			   effz_parallel_func.h effz_python_utility.h effz_result.h\
			   effz_spec_func.h effz_typedefs.h effz_user_input.h\
			   effz_utility.h effz_zeroth_order.h\
			   effz_zeroth_order_python.h


effz_CPPFLAGS = -I$(top_srcdir)/src @PYTHONINCLUDE@\
				@GSLCPPFLAGS@ @TBBCPPFLAGS@\
				-DEFFZ_PYTHON_SRC_DIR='"$(effzpythondir)"'

effz_LDFLAGS = @PYTHONLDFLAGS@ @GSLLDFLAGS@ @TBBLDFLAGS@

effz_LDADD = @PYTHONLIBS@

effzpythondir=$(pkgdatadir)/python_src_dir
dist_effzpython_DATA = effz_zeroth_order_symbolic.py
uninstall-hook:
	rm -rf $(pkgdatadir)

noinst_HEADERS = cereal/access.hpp\
				 cereal/types/base_class.hpp\
				 cereal/types/unordered_map.hpp\
				 cereal/types/boost_variant.hpp\
				 cereal/types/utility.hpp\
				 cereal/types/bitset.hpp\
				 cereal/types/array.hpp\
				 cereal/types/polymorphic.hpp\
				 cereal/types/vector.hpp\
				 cereal/types/map.hpp\
				 cereal/types/complex.hpp\
				 cereal/types/tuple.hpp\
				 cereal/types/queue.hpp\
				 cereal/types/list.hpp\
				 cereal/types/forward_list.hpp\
				 cereal/types/functional.hpp\
				 cereal/types/set.hpp\
				 cereal/types/valarray.hpp\
				 cereal/types/string.hpp\
				 cereal/types/concepts/pair_associative_container.hpp\
				 cereal/types/chrono.hpp\
				 cereal/types/deque.hpp\
				 cereal/types/stack.hpp\
				 cereal/types/memory.hpp\
				 cereal/types/common.hpp\
				 cereal/types/unordered_set.hpp\
				 cereal/macros.hpp\
				 cereal/cereal.hpp\
				 cereal/details/static_object.hpp\
				 cereal/details/polymorphic_impl_fwd.hpp\
				 cereal/details/traits.hpp\
				 cereal/details/polymorphic_impl.hpp\
				 cereal/details/helpers.hpp\
				 cereal/details/util.hpp\
				 cereal/archives/json.hpp\
				 cereal/archives/adapters.hpp\
				 cereal/archives/xml.hpp\
				 cereal/archives/portable_binary.hpp\
				 cereal/archives/binary.hpp\
				 cereal/external/rapidjson/fwd.h\
				 cereal/external/rapidjson/memorystream.h\
				 cereal/external/rapidjson/reader.h\
				 cereal/external/rapidjson/prettywriter.h\
				 cereal/external/rapidjson/ostreamwrapper.h\
				 cereal/external/rapidjson/encodedstream.h\
				 cereal/external/rapidjson/filereadstream.h\
				 cereal/external/rapidjson/istreamwrapper.h\
				 cereal/external/rapidjson/stringbuffer.h\
				 cereal/external/rapidjson/internal/ieee754.h\
				 cereal/external/rapidjson/internal/strtod.h\
				 cereal/external/rapidjson/internal/swap.h\
				 cereal/external/rapidjson/internal/regex.h\
				 cereal/external/rapidjson/internal/diyfp.h\
				 cereal/external/rapidjson/internal/biginteger.h\
				 cereal/external/rapidjson/internal/strfunc.h\
				 cereal/external/rapidjson/internal/itoa.h\
				 cereal/external/rapidjson/internal/stack.h\
				 cereal/external/rapidjson/internal/dtoa.h\
				 cereal/external/rapidjson/internal/meta.h\
				 cereal/external/rapidjson/internal/pow10.h\
				 cereal/external/rapidjson/encodings.h\
				 cereal/external/rapidjson/schema.h\
				 cereal/external/rapidjson/stream.h\
				 cereal/external/rapidjson/filewritestream.h\
				 cereal/external/rapidjson/rapidjson.h\
				 cereal/external/rapidjson/document.h\
				 cereal/external/rapidjson/allocators.h\
				 cereal/external/rapidjson/writer.h\
				 cereal/external/rapidjson/error/error.h\
				 cereal/external/rapidjson/error/en.h\
				 cereal/external/rapidjson/memorybuffer.h\
				 cereal/external/rapidjson/pointer.h\
				 cereal/external/rapidjson/msinttypes/inttypes.h\
				 cereal/external/rapidjson/msinttypes/stdint.h\
				 cereal/external/rapidxml/rapidxml_iterators.hpp\
				 cereal/external/rapidxml/rapidxml_print.hpp\
				 cereal/external/rapidxml/rapidxml_utils.hpp\
				 cereal/external/rapidxml/rapidxml.hpp\
				 cereal/external/rapidxml/license.txt\
				 cereal/external/rapidxml/manual.html\
				 cereal/external/base64.hpp
