#include <optimisationProblem/benchmark/linearSlope.hpp>

#include <cmath>
using std::sqrt;
using std::pow;

#include <random>
using std::bernoulli_distribution;

#include <armadillo>
using arma::sign;
using arma::find;
using arma::accu;
using arma::abs;
using arma::dot;

#include <helper/random.hpp>

namespace hop {
  LinearSlope::LinearSlope(const unsigned int &numberOfDimensions) : BenchmarkProblem(numberOfDimensions), _scaling(getScaling(10)), _one(numberOfDimensions) {
    _one.fill(bernoulli_distribution(0.5)(Random::RNG) ? 5.0 : -5.0);
    _scaling %= sign(_one);
    _partiallyObjectiveValue = 5.0 * accu(abs(_scaling));
  }

  double LinearSlope::getObjectiveValueImplementation(const Col<double> &parameter) const {
    Col<double> z(parameter);

    Col<uword> outOfBound = find(_one % z < 25.0);
    z.elem(outOfBound) %= _one.elem(outOfBound);

    return _partiallyObjectiveValue - dot(_scaling, z);
  }
}
