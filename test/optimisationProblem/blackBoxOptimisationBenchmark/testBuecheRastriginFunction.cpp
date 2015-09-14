// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("bbob::BuecheRastriginFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::BuecheRastriginFunction buecheRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Col<double> translation;
    REQUIRE(translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translationEvenAbs_" + std::to_string(numberOfDimensions) + "x1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_buecheRastriginFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    buecheRastriginFunction.setObjectiveValueTranslation(0);
    buecheRastriginFunction.setParameterTranslation(translation);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(buecheRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::BuecheRastriginFunction(5).toString() == "bbob_bueche_rastrigin_function");
    }
  }
}
