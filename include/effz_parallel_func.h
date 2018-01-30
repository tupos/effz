#ifndef EFFZ_PARALLEL_FUNC_H
#define EFFZ_PARALLEL_FUNC_H

#include <tbb/tbb.h>

namespace eff_z{

	namespace parallel{

		template<typename T, typename Container, typename Function>
			class parallel_sum_data{
				private:
					Container cont;
					Function f;
				public:
					typedef typename Container::const_iterator cont_it;
					T my_sum;

					void operator()(const tbb::blocked_range<cont_it> &r){
						T sum = my_sum;
						for(cont_it it = r.begin(); it != r.end(); ++it){
							sum += f(*it);
						}
						my_sum = sum;
					}

					parallel_sum_data(const parallel_sum_data &s,
							tbb::split)
						: cont(s.cont), f(s.f), my_sum(){}

					parallel_sum_data(Container cont, Function f)
						: cont(cont), f(f), my_sum() {}

					void join(const parallel_sum_data &s){
						my_sum += s.my_sum;
					}
			};

		template<typename T, typename Container, typename Function>
			T parallel_sum(
					const Container& cont, const Function& f){
				parallel_sum_data<T,Container,Function>
					ps(cont, f);

				typedef typename Container::const_iterator cont_it;
				const tbb::blocked_range<cont_it> cont_range(
						cont.cbegin(), cont.cend());

				tbb::parallel_reduce(cont_range, ps);

				return ps.my_sum;
			}
	} /* end namespace parallel */

} /* end namespace eff_z */

#endif /* EFFZ_PARALLEL_FUNC_H */
