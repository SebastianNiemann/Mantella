#pragma once

// C++ standard library
#include <memory>
#include <unordered_set>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

// TODO Add restarting
namespace mant {
  class HookeJeevesAlgorithm : public OptimisationAlgorithm {
   public:
    explicit HookeJeevesAlgorithm();

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setInitialStepSize(
        const double initialStepSize);

    void setStepSizeDecrease(
        const double stepSizeDecrease);

   protected:
    double initialStepSize_;
    double stepSizeDecrease_;

    double stepSize_;
    
    std::unordered_set<arma::Col<double>, Hash, IsEqual> previousBestParameters_;
  };
}
