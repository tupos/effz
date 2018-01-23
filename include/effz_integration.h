#ifndef EFFZ_INTEGRATION_H
#define EFFZ_INTEGRATION_H

#include <iostream>
#include <cmath>
#include <functional>
#include <memory>
#include <utility>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>

namespace eff_z{

	namespace integration{

		template <typename F>
			class gsl_single_quad
			{
				private:
					F f;
					int limit;
					std::unique_ptr<
						gsl_integration_workspace,
						std::function<void(gsl_integration_workspace*)>
							> workspace;

					static double gsl_wrapper(double x, void * p)
					{
						gsl_single_quad *t =
							reinterpret_cast<gsl_single_quad*>(p);
						return t->f(x);
					}

				public:
					gsl_single_quad(F f, int limit)
						:
							f(f),
							limit(limit),
							workspace(
									gsl_integration_workspace_alloc(limit),
									gsl_integration_workspace_free) {}

					double integrate(
							const double min,
							const double max,
							const double epsabs,
							const double epsrel)
					{
						gsl_function gsl_f;
						gsl_f.function = &gsl_wrapper;
						gsl_f.params = this;

						double result, error;
						if (!std::isinf(min) && !std::isinf(max))
						{
							gsl_integration_qag(
									&gsl_f, min, max,
									epsabs, epsrel, limit,
									GSL_INTEG_GAUSS21,
									workspace.get(), &result, &error);
						}
						else if(std::isinf(min) && !std::isinf(max))
						{
							gsl_integration_qagil(
									&gsl_f, max,
									epsabs, epsrel, limit,
									workspace.get(), &result, &error);
						}
						else if(!std::isinf(min) && std::isinf(max))
						{
							gsl_integration_qagiu(
									&gsl_f, min,
									epsabs, epsrel, limit,
									workspace.get(), &result, &error);
						}
						else
						{
							gsl_integration_qagi(
									&gsl_f, epsabs, epsrel, limit,
									workspace.get(), &result, &error);
						}

						return result;
					}
			};

		template <typename F>
			double quad(
					F func,
					const std::pair<double,double> &range,
					const double epsabs = 1.49e-8,
					const double epsrel = 1.49e-8,
					const int limit = 250)
			{
				return gsl_single_quad<F>(func, limit).integrate(
						range.first,
						range.second,
						epsabs,
						epsrel);
			}

	} /*namespace integration*/
} /*namespace eff_z*/


#endif /* EFFZ_INTEGRATION_H */
