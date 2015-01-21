#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2009/linearSlope.hpp>

// Cereal
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

namespace mant {
  namespace bbob2009 {
    LinearSlope::LinearSlope(
        const unsigned int& numberOfDimensions) 
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setOne(one_);
    }

    void LinearSlope::setOne(const arma::Col<double>& one) {
      if (one.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the one vector (" + std::to_string(one.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      one_ = one;
      xOpt_ = 5.0 * one_;
      scaling_ = arma::sign(one_) % getScaling(10.0);
      partiallyObjectiveValue_ = 5.0 * arma::accu(arma::abs(scaling_));
    }

    double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      arma::Col<double> z = parameter;

      const arma::Col<unsigned int>& outOfBound = arma::find(xOpt_ % z >= 25.0);
      z.elem(outOfBound) = xOpt_.elem(outOfBound);

      return partiallyObjectiveValue_ - arma::dot(scaling_, z);
    }

    std::string LinearSlope::to_string() const  {
      return "LinearSlope";
    }
  }
}

CEREAL_REGISTER_TYPE(mant::bbob2009::LinearSlope)
