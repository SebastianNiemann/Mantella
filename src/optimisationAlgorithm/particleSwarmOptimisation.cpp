#include "mantella_bits/optimisationAlgorithm/particleSwarmOptimisation.hpp"

// C++ standard library
#include <random>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/probability.hpp"
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  ParticleSwarmOptimisation::ParticleSwarmOptimisation()
      : OptimisationAlgorithm() {
    setNextParametersFunction([this] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& differences) {
      // TODO Check: Always or only per population?
      if (activeParticleIndex_ == 0 && randomiseTopology_) {
        neighbourhoodTopology_ = neighbourhoodTopologyFunction_(numberOfParticles_);
        randomiseTopology_ = false;
      }
      
      if (differences(0) >= 0) {
        randomiseTopology_ = true;
      }
      
      
      if (differences(0) < localBestObjectiveValues_(activeParticleIndex_)) {
        localBestObjectiveValues_(activeParticleIndex_) = differences(0);
        localBestSolutions_.col(activeParticleIndex_) = parameters.col(0);
      }
      
      activeParticleIndex_ = (activeParticleIndex_ + 1) % numberOfParticles_;
      
      arma::uword neighbourhoodBestParticleIndex;
      arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(neighbourhoodTopology_.col(activeParticleIndex_));
      static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

      arma::Col<double> attractionCenter;
      if (neighbourhoodParticlesIndecies(neighbourhoodBestParticleIndex) == activeParticleIndex_) {
        attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(activeParticleIndex_) - particle_)) / 2.0;
      } else {
        attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(activeParticleIndex_) - particle_) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex) - particle_)) / 3.0;
      }

      const arma::Col<double>& velocity =
      maximalAcceleration_ * arma::randu<arma::Col<double>>(parameters.n_rows) * velocities_.col(activeParticleIndex_) + randomNeighbour(attractionCenter, 0, arma::norm(attractionCenter));

      particle_.col(activeParticleIndex_) += velocity;
      velocities_.col(activeParticleIndex_) = velocity;
        
      return particle_.col(activeParticleIndex_);
    });
    
    setBoundaryHandlingFunction([this] (
        const arma::Mat<double>& parameters) {
      arma::Mat<double> boundedParameters = parameters;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<arma::uword>& belowLowerBound = arma::find(parameters.col(n) < 0);
        const arma::Col<arma::uword>& aboveUpperBound = arma::find(parameters.col(n) > 1);

        static_cast<arma::Col<double>>(velocities_.col(n)).elem(belowLowerBound) *= -0.5;
        static_cast<arma::Col<double>>(velocities_.col(n)).elem(aboveUpperBound) *= -0.5;

        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(belowLowerBound).fill(0);
        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(aboveUpperBound).fill(1);
      }
      
      return boundedParameters;
    });
    
    setNeighbourhoodTopologyFunction([this] (
        const arma::uword numberOfParticles) {
       arma::Mat<arma::uword> neighbourhoodTopology = (arma::randu<arma::Mat<double>>(numberOfParticles, numberOfParticles) <= std::pow(1.0 - 1.0 / static_cast<double>(numberOfParticles), 3.0));
       neighbourhoodTopology.diag().ones();
       
       return neighbourhoodTopology;
    });
  }

  void ParticleSwarmOptimisation::optimise(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::Mat<double>& initialParameters) {
    if (!std::isfinite(maximalAcceleration_)) {
      setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    } else {
      // TODO verify 
    }
    
    if (!std::isfinite(maximalLocalAttraction_)) {
      setMaximalLocalAttraction(0.5 + std::log(2.0));
    } else {
      // TODO verify 
    }
    
    if (!std::isfinite(maximalGlobalAttraction_)) {
      setMaximalGlobalAttraction(maximalLocalAttraction_);
    } else {
      // TODO verify 
    }
    
    numberOfParticles_ = initialParameters.n_cols;
    activeParticleIndex_ = 0;
    
    velocities_ = arma::randu<arma::Mat<double>>(optimisationProblem->numberOfDimensions_, numberOfParticles_) * 2 - 1;
    velocities_ -= initialParameters;
    
    localBestSolutions_ = initialParameters;
    localBestObjectiveValues_.fill(arma::datum::inf);
    
    randomiseTopology_ = false;
    neighbourhoodTopology_ = neighbourhoodTopologyFunction_(numberOfParticles_);
    
    OptimisationAlgorithm::optimise(optimisationProblem, initialParameters);
  }

  void ParticleSwarmOptimisation::setMaximalAcceleration(
      const double maximalAcceleration) {
    maximalAcceleration_ = maximalAcceleration;
  }

  void ParticleSwarmOptimisation::setMaximalLocalAttraction(
      const double maximalLocalAttraction) {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  void ParticleSwarmOptimisation::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }
}
