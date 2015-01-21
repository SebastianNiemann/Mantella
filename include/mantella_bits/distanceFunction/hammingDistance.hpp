#pragma once

// C++ Standard Library
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella_bits/distanceFunction.hpp>
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  class HammingDistance : public DistanceFunction<unsigned int> {
    protected:
      inline unsigned int getDistanceImplementation(
          const arma::Col<unsigned int>& parameter) const  override;

      inline arma::Col<unsigned int> getNeighbourImplementation(
          const arma::Col<unsigned int>& parameter,
          const unsigned int& minimalDistance,
          const unsigned int& maximalDistance) const  override;
  };

  //
  // Implementation
  //

  inline unsigned int HammingDistance::getDistanceImplementation(
      const arma::Col<unsigned int>& parameter) const  {
    return arma::accu(parameter != 0);
  }

  inline arma::Col<unsigned int> HammingDistance::getNeighbourImplementation(
      const arma::Col<unsigned int>& parameter,
      const unsigned int& minimalDistance,
      const unsigned int& maximalDistance) const  {
    if(minimalDistance > std::min(getDistanceImplementation(parameter), parameter.n_elem - getDistanceImplementation(parameter))) {
          throw std::logic_error("The minimal distance (" + std::to_string(minimalDistance) + ") must be lower than or equal to the absolute maximal distance (" + std::to_string(std::min(getDistanceImplementation(parameter), parameter.n_elem - getDistanceImplementation(parameter))) + ").");
    }

    arma::Col<unsigned int> flippedParameter = parameter;

    const arma::Col<unsigned int>& elementsToFlip = getRandomPermutation(parameter.n_elem, std::uniform_int_distribution<unsigned int>(minimalDistance, maximalDistance)(Rng::getGenerator()));
    const arma::Col<unsigned int>& flippedElements = parameter.elem(elementsToFlip);

    flippedParameter.elem(flippedElements != 0).fill(0);
    flippedParameter.elem(flippedElements == 0).fill(1);
    return flippedParameter;
  }
}
