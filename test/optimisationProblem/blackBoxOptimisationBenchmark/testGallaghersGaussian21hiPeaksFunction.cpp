// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("bbob::GallaghersGaussian21hiPeaksFunction", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::GallaghersGaussian21hiPeaksFunction<> gallaghersGaussian21hiPeaksFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_2.input"));

    arma::Mat<double> deltaC21;
    REQUIRE(deltaC21.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_deltaC21_21x1_dim" + std::to_string(numberOfDimensions) +".input"));

    arma::Mat<double> localOptimaY21;
    REQUIRE(localOptimaY21.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_localOptimaY21_" + std::to_string(numberOfDimensions) +"x21.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_gallaghersGaussian21hiPeaksFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    gallaghersGaussian21hiPeaksFunction.setObjectiveValueTranslation(0);
    gallaghersGaussian21hiPeaksFunction.setRotationQ(rotationR);
    gallaghersGaussian21hiPeaksFunction.setLocalParameterTranslations(localOptimaY21);
    gallaghersGaussian21hiPeaksFunction.setLocalParameterConditionings(deltaC21);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(gallaghersGaussian21hiPeaksFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::GallaghersGaussian21hiPeaksFunction<>(5).toString() == "bbob_gallaghers_gaussian_21hi_peaks_function");
  }
}

