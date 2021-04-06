#include <hatch/core/async.hh>
#include <gtest/gtest.h>

#include <iostream>

namespace hatch {

  class AsyncTest : public ::testing::Test {
  protected:
    std::unique_ptr<promise<bool, int, double>> _simple_promise;
    future<bool, int, double> _simple_future_zero;
    future<double> _simple_future_one;

    std::unique_ptr<promise<bool, int>> _complex_promise;
    future<bool, int> _complex_future_zero;
    future<int> _complex_future_one;
    future<int> _complex_future_two;
    future<int> _complex_future_three;

  protected:
    void SetUp() override {
      _simple_promise = std::make_unique<promise<bool, int, double>>();
      _simple_future_zero = _simple_promise->awaited();
      _simple_future_one = _simple_future_zero.then(
        [](bool b, int i, double f) -> double {
          if (b) {
            return i * f;
          } else {
            throw std::runtime_error("simple");
          }
        }
      );

      _complex_promise = std::make_unique<promise<bool, int>>();
      _complex_future_zero = _complex_promise->awaited();
      _complex_future_one = _complex_future_zero.then(
        [](bool b, int i) -> std::tuple<bool, int> {
          return std::make_tuple(b, 3*i);
        }
      ).then(
        [](bool b, int i) -> int {
          if (b) {
            return 2*i;
          } else {
            throw std::runtime_error("complex");
          }
        }
      );
      _complex_future_two = _complex_future_one.then(
        [](int i) -> int {
          return 5 * i;
        }
      ).recover([](std::exception_ptr p) -> int {
        try {
          std::rethrow_exception(p);
        } catch (std::runtime_error& e) {
          return strlen(e.what());
        } catch(...) {
          return -1;
        }
      });
      _complex_future_three = _complex_future_one.then(
        [](int i) -> int {
          return 7 * i;
        }
      ).then(
        [](int i) -> int {
          return 13 * i;
        }
      );
    }
  };

  TEST_F(AsyncTest, SimpleSetupTest) {
    EXPECT_TRUE(_simple_promise->is_pending());
    EXPECT_TRUE(_simple_future_zero.is_pending());
    EXPECT_TRUE(_simple_future_one.is_pending());
  }

  TEST_F(AsyncTest, SimpleCompletionTestValues) {
    _simple_promise->complete(true, 2, 22.2);
    EXPECT_TRUE(_simple_promise->is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(_simple_future_one.is_completed());
    EXPECT_EQ(_simple_future_one.get(), 44.4);
  }

  TEST_F(AsyncTest, SimpleCompletionTestTuple) {
    _simple_promise->complete(std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(_simple_promise->is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(_simple_future_one.is_completed());
    EXPECT_EQ(_simple_future_one.get(), 44.4);
  }

  TEST_F(AsyncTest, SimpleFailureTest) {
    _simple_promise->complete(false, 6, 10.2);
    EXPECT_TRUE(_simple_promise->is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(false, 6, 10.2));
    EXPECT_TRUE(_simple_future_one.is_failed());
    try {
      _simple_future_one.get();
    } catch (std::runtime_error& e) {
      EXPECT_STREQ(e.what(), "simple");
    } catch (...) {
      FAIL() << "Simple failure test threw wrong type";
    }
  }

  TEST_F(AsyncTest, SimpleMovePromiseTest) {
    promise<bool, int, double> p = std::move(*_simple_promise);
    EXPECT_TRUE(_simple_promise->is_moved());
    EXPECT_TRUE(p.is_pending());

    p.complete(true, 2, 22.2);
    EXPECT_TRUE(p.is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(_simple_future_one.is_completed());
    EXPECT_EQ(_simple_future_one.get(), 44.4);
  }

  TEST_F(AsyncTest, SimpleMoveFutureTest) {
    future<double> f = std::move(_simple_future_one);
    EXPECT_TRUE(_simple_future_one.is_detached());
    EXPECT_TRUE(f.is_pending());

    _simple_promise->complete(true, 2, 22.2);
    EXPECT_TRUE(_simple_promise->is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(f.is_completed());
    EXPECT_EQ(f.get(), 44.4);
  }

  TEST_F(AsyncTest, SimpleCopyFutureTest) {
    future<double> f = _simple_future_one;
    EXPECT_TRUE(_simple_future_one.is_pending());
    EXPECT_TRUE(f.is_pending());

    _simple_promise->complete(true, 2, 22.2);
    EXPECT_TRUE(_simple_promise->is_completed());
    EXPECT_TRUE(_simple_future_zero.is_completed());
    EXPECT_EQ(_simple_future_zero.get(), std::make_tuple(true, 2, 22.2));
    EXPECT_TRUE(_simple_future_one.is_completed());
    EXPECT_EQ(_simple_future_one.get(), 44.4);
    EXPECT_TRUE(f.is_completed());
    EXPECT_EQ(f.get(), 44.4);
  }

  TEST_F(AsyncTest, ComplexCompletionTest) {
    _complex_promise->complete(std::make_tuple(true, 2));
    EXPECT_TRUE(_complex_future_zero.is_completed());
    EXPECT_TRUE(_complex_future_one.is_completed());
    EXPECT_TRUE(_complex_future_two.is_completed());
    EXPECT_TRUE(_complex_future_three.is_completed());
    EXPECT_EQ(_complex_future_zero.get(), std::make_tuple(true, 2));
    EXPECT_EQ(_complex_future_one.get(), 12);
    EXPECT_EQ(_complex_future_two.get(), 60);
    EXPECT_EQ(_complex_future_three.get(), 1092);
  }

  TEST_F(AsyncTest, ComplexFailureTest) {
    _complex_promise->complete(std::make_tuple(false, 2));
    EXPECT_TRUE(_complex_future_zero.is_completed());
    EXPECT_TRUE(_complex_future_one.is_failed());
    EXPECT_TRUE(_complex_future_two.is_completed());
    EXPECT_TRUE(_complex_future_three.is_failed());
    EXPECT_EQ(_complex_future_zero.value(), std::make_tuple(false, 2));
    try {
      _complex_future_one.get();
    } catch (std::runtime_error& e) {
      EXPECT_STREQ(e.what(), "complex");
    } catch (...) {
      FAIL() << "complex failure test threw wrong type";
    }
    EXPECT_EQ(_complex_future_two.value(), 7);
    try {
      _complex_future_three.get();
    } catch (std::runtime_error& e) {
      EXPECT_STREQ(e.what(), "complex");
    } catch (...) {
      FAIL() << "complex failure test threw wrong type";
    }
  }

}
