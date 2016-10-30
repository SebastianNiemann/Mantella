/**
Problems
========

.. cpp:class:: template<T, N> problem

  **Template parameters**
  
    - **T** - A floating point type
    - **N** - The (``unsigned``) number of dimensions 
  
  .. cpp:member:: std::function objective_function
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::array<T, N> lower_bounds
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::array<T, N> upper_bounds
  
    Lorem ipsum dolor sit amet
      
  .. cpp:function:: problem()
  
    Lorem ipsum dolor sit amet
    
.. toctree::
  
  problem/ackley_function
  problem/rastrigin_function
  problem/rosenbrock_function
  problem/sphere_function
  problem/sum_of_different_powers_function
*/
template <typename T, unsigned N>
struct problem {
  std::function<T(
      const std::array<T, N>& parameter)> objective_function;
  
  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;
  
  constexpr problem() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
constexpr problem<T, N>::problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");
  
  // These bounds could be anything as default, as they need user-given, context-dependent knowledge to be useful.
  // However, since we map the bounds later on internally to [0, 1] to ease maintainability in our optimisation 
  // algorithms, we use [0, 1] as default.
  lower_bounds.fill(T(0.0));
  upper_bounds.fill(T(1.0));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("problem", "[problem]") {
  const mant::problem<double, 2> problem;
  
  CHECK((problem.lower_bounds == std::array<double, 2>({0.0, 0.0})));
  CHECK((problem.upper_bounds == std::array<double, 2>({1.0, 1.0})));
}
#endif
