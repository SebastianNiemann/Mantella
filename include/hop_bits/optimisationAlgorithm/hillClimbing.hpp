#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class HillClimbing : public OptimisationAlgorithm {
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      HillClimbing(const HillClimbing&) = delete;
      HillClimbing& operator=(const HillClimbing&) = delete;

      void setMaximalStepSize(
          const arma::Col<double>& maximalStepSize);

      std::string to_string() const override;

    protected:
      arma::Col<double> maximalStepSize_;

      void optimiseImplementation() override;
  };
}
