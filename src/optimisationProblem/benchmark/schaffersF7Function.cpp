#include <hop_bits/optimisationProblem/benchmark/schaffersF7Function.hpp>

#include <cmath>

namespace hop {
  SchaffersF7Function::SchaffersF7Function(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _delta(getScaling(std::sqrt(10.0))) {

  }

  double SchaffersF7Function::getObjectiveValueImplementation(const arma::Col<double> &parameter) const {
    arma::Col<double> z = arma::square(_delta % (_rotationQ * getAsymmetricTransformation(0.5, _rotationR * (parameter - _translation))));
    arma::Col<double> s = arma::pow(z.subvec(0, z.n_elem - 2) + z.subvec(1, z.n_elem - 1), 0.25);

    return std::pow(arma::mean(s % (1 + arma::square(arma::sin(50 * arma::pow(s, 0.4))))), 2) + 10.0 * getPenality(parameter);
  }
}
