#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
    public:
      explicit HookeJeevesAlgorithm(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      HookeJeevesAlgorithm(const HookeJeevesAlgorithm&) = delete;
      HookeJeevesAlgorithm& operator=(const HookeJeevesAlgorithm&) = delete;

      void setInitialStepSize(
          const arma::Col<double>& stepSize);

      std::string to_string() const override;

    protected:
      arma::Col<double> stepSize_;

      void optimiseImplementation() override;
  };
}
