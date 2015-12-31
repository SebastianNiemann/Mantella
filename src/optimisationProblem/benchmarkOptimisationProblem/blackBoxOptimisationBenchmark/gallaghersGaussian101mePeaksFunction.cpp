#include "mantella_bits/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <algorithm>
#include <limits>

// Mantella
#include "mantella_bits/probability.hpp"

// This implementation contains a lot of *magic numbers* and behaviour, introduced by the black-box optimisation benchmark, but only partially explained in the paper.
// So don't expect use to explain the unexplained.
// @see N. Hansen et al., Real-Parameter Black-Box Optimization Benchmarking 2010: Experimental Setup. Research Report RR-7215, INRIA, 2010.
namespace mant {
  namespace bbob {
    GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const arma::uword numberOfDimensions)
        : BlackBoxOptimisationBenchmark(numberOfDimensions),
          weight_(arma::join_cols(arma::Col<double>({10.0}), arma::linspace<arma::Col<double>>(1.1, 9.1, 100))),
          localParameterConditionings_(getRandomLocalParameterMediumConditionings(101)),
          localParameterTranslations_(arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 10.0 - 5.0),
          rotationQ_(randomRotationMatrix(numberOfDimensions_)) {
      localParameterTranslations_.col(0) = 0.8 * localParameterTranslations_.col(0);

#if defined(SUPPORT_MPI)
      MPI_Bcast(localParameterConditionings_.memptr(), static_cast<int>(localParameterConditionings_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(localParameterTranslations_.memptr(), static_cast<int>(localParameterTranslations_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Bcast(rotationQ_.memptr(), static_cast<int>(rotationQ_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

      setObjectiveFunction([this](
                               const arma::Col<double>& parameter) {
          assert(parameter.n_elem == numberOfDimensions_);
            
          double maximalValue = std::numeric_limits<double>::lowest();
          for (arma::uword n = 0; n < 101; ++n) {
            const arma::Col<double>& localParameterTranslation = parameter - localParameterTranslations_.col(n);
            maximalValue = std::max(maximalValue, weight_(n) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(localParameterTranslation, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(n)) * rotationQ_ * localParameterTranslation)));
          }

          return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
      },
          "BBOB Gallagher's Gaussian 101-me Peaks Function");
    }
  }
}
