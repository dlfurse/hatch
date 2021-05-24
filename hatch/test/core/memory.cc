#include <hatch/core/memory.hh>
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <memory>

#include <cmath>

namespace hatch {
  class MemoryTest : public ::testing::Test {
  protected:
    class numeric {
    public:
      int _a;
      int _b;
      double _num;
    };

    //std::unique_ptr<allocator<numeric>> _allocator;

  protected:
//    void SetUp() override {
//      _allocator = std::make_unique<allocator<numeric>>(16, 4);
//    }
  };

  TEST_F(MemoryTest, DummyTest) {
    numeric one{1, 3, 0.2};
    numeric two{2, 4, 0.5};
    EXPECT_EQ((one._a + two._b)*(one._b + two._a), 10 * two._num / one._num);
  }

//  TEST_F(MemoryTest, SimpleCreateTest) {
//    auto ptr = _allocator->create(5, 7, 9.11);
//    EXPECT_EQ(ptr.null(), false);
//    EXPECT_EQ(_allocator->allocated(), 1);
//
//    EXPECT_EQ(ptr->_a, 5);
//    EXPECT_EQ(ptr->_b, 7);
//    EXPECT_EQ(ptr->_num, 9.11);
//
//    _allocator->destroy(ptr);
//    EXPECT_EQ(ptr.null(), true);
//    EXPECT_EQ(_allocator->allocated(), 0);
//  }
//
//  TEST_F(MemoryTest, SimpleCopyTest) {
//    auto ptr = _allocator->create(5, 7, 9.11);
//    auto cpy = ptr;
//    EXPECT_EQ(ptr.null(), false);
//    EXPECT_EQ(cpy.null(), false);
//    EXPECT_EQ(_allocator->allocated(), 1);
//
//    EXPECT_EQ(cpy->_a, 5);
//    EXPECT_EQ(cpy->_b, 7);
//    EXPECT_EQ(cpy->_num, 9.11);
//
//    _allocator->destroy(cpy);
//    EXPECT_EQ(ptr.null(), true);
//    EXPECT_EQ(cpy.null(), true);
//    EXPECT_EQ(_allocator->allocated(), 0);
//  }
//
//  TEST_F(MemoryTest, SimpleMoveTest) {
//    auto ptr = _allocator->create(5, 7, 9.11);
//    auto mvd = std::move(ptr);
//    EXPECT_EQ(ptr.null(), true);
//    EXPECT_EQ(mvd.null(), false);
//    EXPECT_EQ(_allocator->allocated(), 1);
//
//    EXPECT_EQ(mvd->_a, 5);
//    EXPECT_EQ(mvd->_b, 7);
//    EXPECT_EQ(mvd->_num, 9.11);
//
//    _allocator->destroy(mvd);
//    EXPECT_EQ(ptr.null(), true);
//    EXPECT_EQ(mvd.null(), true);
//    EXPECT_EQ(_allocator->allocated(), 0);
//  }
//
//  TEST_F(MemoryTest, SimpleScopeTest) {
//    {
//      auto ptr = _allocator->create(5, 7, 9.11);
//      auto cpy = ptr;
//      auto mvd = std::move(ptr);
//      EXPECT_EQ(ptr.null(), true);
//      EXPECT_EQ(cpy.null(), false);
//      EXPECT_EQ(mvd.null(), false);
//      EXPECT_EQ(_allocator->allocated(), 1);
//
//      EXPECT_EQ(cpy->_a, 5);
//      EXPECT_EQ(cpy->_b, 7);
//      EXPECT_EQ(cpy->_num, 9.11);
//
//      EXPECT_EQ(mvd->_a, 5);
//      EXPECT_EQ(mvd->_b, 7);
//      EXPECT_EQ(mvd->_num, 9.11);
//    }
//    EXPECT_EQ(_allocator->allocated(), 0);
//  }
//
//  TEST_F(MemoryTest, ComplexGrowShrinkTest) {
//    std::vector<pointer<numeric>> ptrs;
//    ptrs.reserve(1024);
//
//    EXPECT_EQ(_allocator->allocated(), 0);
//    EXPECT_EQ(_allocator->capacity(), 16);
//    EXPECT_EQ(_allocator->growth_increment(), 16);
//    EXPECT_EQ(_allocator->shrink_threshold(), 16);
//
//    auto adder = [&](int index) {
//      static auto num = std::atan(1.);
//      ptrs.push_back(std::move(_allocator->create(2 * index, 2 * index + 1, index * num)));
//    };
//
//
//    for (auto index = 0; index < 15; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 16);
//      EXPECT_EQ(_allocator->growth_increment(), 16);
//      EXPECT_EQ(_allocator->shrink_threshold(), 16);
//    }
//
//    adder(15);
//    EXPECT_EQ(_allocator->allocated(), 16);
//    EXPECT_EQ(_allocator->capacity(), 32);
//    EXPECT_EQ(_allocator->growth_increment(), 32);
//    EXPECT_EQ(_allocator->shrink_threshold(), 16);
//
//
//    for (auto index = 16; index < 31; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 32);
//      EXPECT_EQ(_allocator->growth_increment(), 32);
//      EXPECT_EQ(_allocator->shrink_threshold(), 16);
//    }
//
//    adder(31);
//    EXPECT_EQ(_allocator->allocated(), 32);
//    EXPECT_EQ(_allocator->capacity(), 64);
//    EXPECT_EQ(_allocator->growth_increment(), 64);
//    EXPECT_EQ(_allocator->shrink_threshold(), 16);
//
//
//    for (auto index = 32; index < 63; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 64);
//      EXPECT_EQ(_allocator->growth_increment(), 64);
//      EXPECT_EQ(_allocator->shrink_threshold(), 16);
//    }
//
//    adder(63);
//    EXPECT_EQ(_allocator->allocated(), 64);
//    EXPECT_EQ(_allocator->capacity(), 128);
//    EXPECT_EQ(_allocator->growth_increment(), 128);
//    EXPECT_EQ(_allocator->shrink_threshold(), 32);
//
//
//    for (auto index = 64; index < 127; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 128);
//      EXPECT_EQ(_allocator->growth_increment(), 128);
//      EXPECT_EQ(_allocator->shrink_threshold(), 32);
//    }
//
//    adder(127);
//    EXPECT_EQ(_allocator->allocated(), 128);
//    EXPECT_EQ(_allocator->capacity(), 256);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 64);
//
//
//    for (auto index = 128; index < 255; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 256);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 64);
//    }
//
//    adder(255);
//    EXPECT_EQ(_allocator->allocated(), 256);
//    EXPECT_EQ(_allocator->capacity(), 512);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 128);
//
//    for (auto index = 256; index < 511; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 512);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 128);
//    }
//
//    adder(511);
//    EXPECT_EQ(_allocator->allocated(), 512);
//    EXPECT_EQ(_allocator->capacity(), 768);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 256);
//
//    for (auto index = 512; index < 767; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 768);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 256);
//    }
//
//    adder(767);
//    EXPECT_EQ(_allocator->allocated(), 768);
//    EXPECT_EQ(_allocator->capacity(), 1024);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 256);
//
//    for (auto index = 768; index < 1023; index++) {
//      adder(index);
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 1024);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 256);
//    }
//
//    std::random_shuffle(ptrs.begin(), ptrs.end());
//
//    for (auto index = 1022; index >= 256; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 1024);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 256);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 256);
//    EXPECT_EQ(_allocator->capacity(), 512);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 128);
//    ptrs.pop_back();
//
//    for (auto index = 254; index >= 128; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 512);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 128);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 128);
//    EXPECT_EQ(_allocator->capacity(), 256);
//    EXPECT_EQ(_allocator->growth_increment(), 256);
//    EXPECT_EQ(_allocator->shrink_threshold(), 64);
//    ptrs.pop_back();
//
//    for (auto index = 126; index >= 64; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 256);
//      EXPECT_EQ(_allocator->growth_increment(), 256);
//      EXPECT_EQ(_allocator->shrink_threshold(), 64);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 64);
//    EXPECT_EQ(_allocator->capacity(), 128);
//    EXPECT_EQ(_allocator->growth_increment(), 128);
//    EXPECT_EQ(_allocator->shrink_threshold(), 32);
//    ptrs.pop_back();
//
//    for (auto index = 62; index >= 32; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 128);
//      EXPECT_EQ(_allocator->growth_increment(), 128);
//      EXPECT_EQ(_allocator->shrink_threshold(), 32);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 32);
//    EXPECT_EQ(_allocator->capacity(), 64);
//    EXPECT_EQ(_allocator->growth_increment(), 64);
//    EXPECT_EQ(_allocator->shrink_threshold(), 16);
//    ptrs.pop_back();
//
//    for (auto index = 30; index >= 16; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 64);
//      EXPECT_EQ(_allocator->growth_increment(), 64);
//      EXPECT_EQ(_allocator->shrink_threshold(), 16);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 16);
//    EXPECT_EQ(_allocator->capacity(), 32);
//    EXPECT_EQ(_allocator->growth_increment(), 32);
//    EXPECT_EQ(_allocator->shrink_threshold(), 16);
//    ptrs.pop_back();
//
//    for (auto index = 14; index >= 0; index--) {
//      EXPECT_EQ(_allocator->allocated(), index + 1);
//      EXPECT_EQ(_allocator->capacity(), 32);
//      EXPECT_EQ(_allocator->growth_increment(), 32);
//      EXPECT_EQ(_allocator->shrink_threshold(), 16);
//      ptrs.pop_back();
//    }
//
//    EXPECT_EQ(_allocator->allocated(), 0);
//  }
}