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

		template<typename T, typename Function>
			class parallel_table_data{
				private:
					std::vector<T> input;
					Function f;
				public:
					typedef typename
						std::result_of<Function(T)>::type return_type;
					tbb::concurrent_vector<return_type> output;

					typedef typename std::vector<T>::const_iterator vec_it;
					void operator()
						(const tbb::blocked_range<vec_it> &range){
							tbb::concurrent_vector<return_type> vec
								= output;
							for(vec_it it = range.begin();
									it != range.end();
									++it){
								vec.push_back(f(*it));
							}
							output = vec;
					}

					parallel_table_data(parallel_table_data &tab,
							tbb::split)
						: input(tab.input), f(tab.f), output() {};

					void join(const parallel_table_data &tab){
						for(auto &el: tab.output){
							output.push_back(el);
						}
					}

					parallel_table_data(
							const std::vector<T> &input,
							Function f
							) : input(input), f(f), output() {}
			};

		template<typename T, typename Function>
			auto parallel_table(const std::vector<T> &input, Function f){
				typedef typename std::vector<T>::const_iterator vec_it;
				tbb::blocked_range<vec_it>
					range(input.cbegin(), input.cend());
				parallel_table_data<T,Function> tab_dat(input, f);
				parallel_reduce(range, tab_dat);

				typedef typename
					std::result_of<Function(T)>::type return_type;
				std::vector<return_type> output;
				for(auto &el: tab_dat.output){
					output.push_back(el);
				}

				return output;
			}

	} /* end namespace parallel */

} /* end namespace eff_z */

#endif /* EFFZ_PARALLEL_FUNC_H */
