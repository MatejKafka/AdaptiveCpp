/*
 * This file is part of AdaptiveCpp, an implementation of SYCL and C++ standard
 * parallelism for CPUs and GPUs.
 *
 * Copyright The AdaptiveCpp Contributors
 *
 * AdaptiveCpp is released under the BSD 2-Clause "Simplified" License.
 * See file LICENSE in the project root for full license details.
 */
// SPDX-License-Identifier: BSD-2-Clause

#include <algorithm>
#include <execution>
#include <pstl/glue_execution_defs.h>
#include <utility>
#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/mpl.hpp>

#include "pstl_test_suite.hpp"

BOOST_FIXTURE_TEST_SUITE(pstl_is_sorted_until, enable_unified_shared_memory)

template <class Policy, class Generator, class Compare = std::less<>>
void test_is_sorted_until(Policy&& pol, std::size_t problem_size,
                          Generator&& gen, Compare comp = {}) {
  std::vector<int> data(problem_size);
  for(int i = 0; i < problem_size; ++i)
    data[i] = gen(i);

  std::cout<< "\n";
  for (auto & x: data)
    std::cout << x << " ";
  std::cout<< "\n";

  // auto ret =
  //     std::is_sorted_until(pol, data.begin(), data.end());
  // auto ret_host =
  //     std::is_sorted_until(data.begin(), data.end());

  // std::cout << "\n Result: host, pol: " << *ret_host << "  " << *ret << "\n";
  // BOOST_CHECK(ret == ret_host);

  auto ret = std::is_sorted_until(pol, data.begin(), data.end(), comp);
  auto ret_host = std::is_sorted_until(data.begin(), data.end(), comp);

  std::cout << " Comp = std::less:  " << *ret_host << "  " << *ret << "\n";
  BOOST_CHECK(ret == ret_host);

  auto cmp = std::less_equal<>{};
  ret = std::is_sorted_until(pol, data.begin(), data.end(), cmp);
  ret_host = std::is_sorted_until(data.begin(), data.end(), cmp);

  std::cout << " std::less_equal:   " << *ret_host << "  " << *ret << "\n";
  BOOST_CHECK(ret == ret_host);

}


// template<class Policy>
// void empty_tests(Policy&& pol) {
//   test_is_sorted_until(pol, 0, [](int i){return i;});
// }

template<class Policy>
void single_element_tests(Policy&& pol) {
  test_is_sorted_until(pol, 1, [](int i){return i;});
}

template<class Policy>
void medium_size_tests(Policy&& pol) {
  test_is_sorted_until(pol, 20, [](int i){return 10-i;});
  test_is_sorted_until(pol, 20, [](int i){return (i > 5 ? i : 20-i);});
  test_is_sorted_until(pol, 20, [](int i){return (i <= 5 ? i : 20-i);});
  test_is_sorted_until(pol, 20, [](int i){return (i == 0 ? i : 20-i);});
  test_is_sorted_until(pol, 20, [](int i){return i;});
  test_is_sorted_until(pol, 20, [](int i){return 42;});
}

// BOOST_AUTO_TEST_CASE(par_unseq_empty) {
//   empty_tests(std::execution::par_unseq);
// }

BOOST_AUTO_TEST_CASE(par_unseq_single_element) {
  std::cout<< "\n Is_sorted_until: returns the last iterator. The corresponding dereferenenced values are shown below.\n";
  single_element_tests(std::execution::par_unseq);
}

BOOST_AUTO_TEST_CASE(par_unseq_medium_size) {
  medium_size_tests(std::execution::par_unseq);
}



// BOOST_AUTO_TEST_CASE(par_empty) {
//   empty_tests(std::execution::par);
// }

BOOST_AUTO_TEST_CASE(par_single_element) {
  single_element_tests(std::execution::par);
}

BOOST_AUTO_TEST_CASE(par_medium_size) {
  medium_size_tests(std::execution::par);
}


BOOST_AUTO_TEST_SUITE_END()
