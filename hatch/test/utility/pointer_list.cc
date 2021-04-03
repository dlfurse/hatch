#include <hatch/utility/pointer_list.hh>
#include <gtest/gtest.h>

#include <iostream>

namespace hatch {

  class PointerListTest : public ::testing::Test {
  protected:
    class test_data {
    public:
      test_data() :
          numerator{0}, denominator{0}, precise{0.} {
      }
      test_data(unsigned int numerator, unsigned int denominator, double precise) :
        numerator{numerator}, denominator{denominator}, precise{precise} {
      }

      unsigned int numerator;
      unsigned int denominator;
      double precise;
    };

    class test_node : public test_data, public pointer_list_node<test_node> {
    private:
        using node = pointer_list_node<test_node>;

    public:
      test_node(unsigned int numerator, unsigned int denominator, double precise) :
        test_data{numerator, denominator, precise} {
      }

      pointer_list_node<test_node>& detach() {
        return node::detach();
      }

      void splice_before(test_node& node) {
        return node::splice_before(node);
      }

      void splice_replacing(test_node& node) {
        return node::splice_replacing(node);
      }
      
      void splice_after(test_node& node) {
        return node::splice_after(node);
      }
    };

    using test_root = pointer_list_root<test_node>;

    test_data data[6];
    test_node* node;

    test_node first{1, 10, 1.1};
    test_node second{2, 20, 2.2};
    test_node third{3, 30, 3.3};
    test_node fourth{4, 40, 4.4};
    test_node fifth{5, 50, 5.5};
    test_node sixth{6, 60, 6.6};

    test_root one;
    test_root two;

  protected:
    int dump(test_root& root) {
      int count = 0;
      for (const auto& item : root) {
        data[count].numerator = item.numerator;
        data[count].denominator = item.denominator;
        data[count].precise = item.precise;
        EXPECT_TRUE(++count < 6);
      }
      return count;
    }
  };

  TEST_F(PointerListTest, SimpleEmptyListTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());
    EXPECT_TRUE(fifth.detached());
    EXPECT_TRUE(sixth.detached());

    EXPECT_TRUE(one.empty());
    EXPECT_TRUE(two.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, SingleSpliceBeforeTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());

    second.splice_before(third);

    EXPECT_EQ(second.next(), &third);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(second.prev(), &third);
    EXPECT_EQ(third.next(), &second);

    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    first.splice_before(second);

    EXPECT_EQ(first.next(), &second);
    EXPECT_EQ(second.prev(), &first);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(third.next(), &first);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    second.detach();

    EXPECT_EQ(first.next(), &third);
    EXPECT_EQ(third.prev(), &first);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(third.next(), &first);

    EXPECT_FALSE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(third.detached());
  }

  TEST_F(PointerListTest, MultipleSpliceBeforeTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());

    EXPECT_TRUE(fourth.detached());
    EXPECT_TRUE(fifth.detached());
    EXPECT_TRUE(sixth.detached());

    second.splice_before(third);
    first.splice_before(second);

    fifth.splice_before(sixth);
    fourth.splice_before(fifth);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    EXPECT_FALSE(fourth.detached());
    EXPECT_FALSE(fifth.detached());
    EXPECT_FALSE(sixth.detached());

    second.splice_before(fifth);

    EXPECT_EQ(fourth.next(), &second);
    EXPECT_EQ(second.next(), &third);
    EXPECT_EQ(third.next(), &first);
    EXPECT_EQ(first.next(), &fifth);
    EXPECT_EQ(fifth.next(), &sixth);
    EXPECT_EQ(sixth.next(), &fourth);
    EXPECT_EQ(fourth.prev(), &sixth);
    EXPECT_EQ(second.prev(), &fourth);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(fifth.prev(), &first);
    EXPECT_EQ(sixth.prev(), &fifth);
  }

  TEST_F(PointerListTest, SingleSpliceReplacingTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());

    second.splice_after(first);
    third.splice_before(first);

    EXPECT_EQ(first.next(), &second);
    EXPECT_EQ(second.next(), &third);
    EXPECT_EQ(third.next(), &first);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(second.prev(), &first);
    EXPECT_EQ(first.prev(), &third);

    fourth.splice_replacing(first);

    EXPECT_EQ(fourth.next(), &second);
    EXPECT_EQ(second.next(), &third);
    EXPECT_EQ(third.next(), &fourth);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(second.prev(), &fourth);
    EXPECT_EQ(fourth.prev(), &third);
  }

  TEST_F(PointerListTest, MultipleSpliceReplacingTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());

    EXPECT_TRUE(fourth.detached());
    EXPECT_TRUE(fifth.detached());
    EXPECT_TRUE(sixth.detached());

    second.splice_before(third);
    first.splice_after(third);

    fifth.splice_before(sixth);
    fourth.splice_after(sixth);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    EXPECT_FALSE(fourth.detached());
    EXPECT_FALSE(fifth.detached());
    EXPECT_FALSE(sixth.detached());

    second.splice_replacing(sixth);

    EXPECT_EQ(fourth.next(), &fifth);
    EXPECT_EQ(fifth.next(), &second);
    EXPECT_EQ(second.next(), &third);
    EXPECT_EQ(third.next(), &first);
    EXPECT_EQ(first.next(), &fourth);
    EXPECT_EQ(fourth.prev(), &first);
    EXPECT_EQ(fifth.prev(), &fourth);
    EXPECT_EQ(second.prev(), &fifth);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(first.prev(), &third);
  }

  TEST_F(PointerListTest, SingleInsertAfterTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());

    second.splice_after(first);

    EXPECT_EQ(first.next(), &second);
    EXPECT_EQ(second.prev(), &first);
    EXPECT_EQ(first.prev(), &second);
    EXPECT_EQ(second.next(), &first);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_TRUE(third.detached());

    third.splice_after(second);

    EXPECT_EQ(third.next(), &first);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(third.prev(), &second);
    EXPECT_EQ(second.next(), &third);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    second.detach();

    EXPECT_EQ(first.next(), &third);
    EXPECT_EQ(third.prev(), &first);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(third.next(), &first);

    EXPECT_FALSE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(third.detached());
  }

  TEST_F(PointerListTest, MultipleSpliceAfterTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());

    EXPECT_TRUE(fourth.detached());
    EXPECT_TRUE(fifth.detached());
    EXPECT_TRUE(sixth.detached());

    second.splice_after(first);
    third.splice_after(second);

    fifth.splice_after(fourth);
    sixth.splice_after(fifth);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());

    EXPECT_FALSE(fourth.detached());
    EXPECT_FALSE(fifth.detached());
    EXPECT_FALSE(sixth.detached());

    third.splice_after(fourth);

    EXPECT_EQ(fourth.next(), &third);
    EXPECT_EQ(third.next(), &first);
    EXPECT_EQ(first.next(), &second);
    EXPECT_EQ(second.next(), &fifth);
    EXPECT_EQ(fifth.next(), &sixth);
    EXPECT_EQ(sixth.next(), &fourth);
    EXPECT_EQ(fourth.prev(), &sixth);
    EXPECT_EQ(third.prev(), &fourth);
    EXPECT_EQ(first.prev(), &third);
    EXPECT_EQ(second.prev(), &first);
    EXPECT_EQ(fifth.prev(), &second);
    EXPECT_EQ(sixth.prev(), &fifth);
  }

  TEST_F(PointerListTest, OneElementFrontTest) {
    one.push_front(first);

    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &first);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);

    node = one.pop_front();
    one.push_front(second);

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &second);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 2);
    EXPECT_EQ(data[0].denominator, 20);
    EXPECT_EQ(data[0].precise, 2.2);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.front(), nullptr);
    EXPECT_EQ(dump(one), 0);

    node = one.pop_front();

    EXPECT_EQ(node, nullptr);
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.front(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, OneElementBackTest) {
    one.push_back(first);

    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.back(), &first);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);

    node = one.pop_back();
    one.push_back(second);

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.back(), &second);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 2);
    EXPECT_EQ(data[0].denominator, 20);
    EXPECT_EQ(data[0].precise, 2.2);

    node = one.pop_back();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.back(), nullptr);
    EXPECT_EQ(dump(one), 0);

    node = one.pop_back();

    EXPECT_EQ(node, nullptr);
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.back(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, TwoElementFrontTest) {
    one.push_front(second);
    one.push_front(first);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &first);
    EXPECT_EQ(dump(one), 2);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);

    node = one.pop_front();
    one.push_front(third);

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &third);
    EXPECT_EQ(dump(one), 2);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);

    node = one.pop_front();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, TwoElementBackTest) {
    one.push_back(first);
    one.push_back(second);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 2);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);

    node = one.pop_back();
    one.push_back(third);

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.detached());
    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 2);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 3);
    EXPECT_EQ(data[1].denominator, 30);
    EXPECT_EQ(data[1].precise, 3.3);

    node = one.pop_back();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_back();

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }


  TEST_F(PointerListTest, FourElementFrontBackTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);
    one.push_back(fourth);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(fourth.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 4);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);
    EXPECT_EQ(data[3].numerator, 4);
    EXPECT_EQ(data[3].denominator, 40);
    EXPECT_EQ(data[3].precise, 4.4);

    node = one.pop_front();

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(fourth.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);

    node = one.pop_back();

    EXPECT_EQ(node, &fourth);
    EXPECT_TRUE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_TRUE(fourth.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 2);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_back();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, OneListFrontTest) {
    two.push_front(third);

    EXPECT_TRUE(third.detached());
    EXPECT_FALSE(two.empty());
    EXPECT_EQ(two.front(), &third);
    EXPECT_EQ(dump(two), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    one.push_front(two);

    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(two.empty());
    EXPECT_EQ(two.front(), nullptr);
    EXPECT_EQ(dump(two), 0);
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &third);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    node = one.pop_front();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.front(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, OneListBackTest) {
    two.push_back(third);

    EXPECT_TRUE(third.detached());
    EXPECT_FALSE(two.empty());
    EXPECT_EQ(two.back(), &third);
    EXPECT_EQ(dump(two), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    one.push_back(two);

    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(two.empty());
    EXPECT_EQ(two.back(), nullptr);
    EXPECT_EQ(dump(two), 0);
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.back(), &third);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    node = one.pop_back();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.back(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }


//  TEST_F(PointerListTest, SimpleInsertReplacingTest) {
//    second.splice_after(first);
//    third.splice_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fourth.insert_replacing(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleInsertReplacingTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fourth.insert_replacing(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleSpliceReplacingTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fifth.insert_after(fourth);
//    sixth.insert_after(fifth);
//
//    {
//      int count = 0;
//      for (const auto& data : fourth) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fifth.splice_replacing(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        } else if (count == 3) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 4) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 5);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleInsertBeforeTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fifth.insert_before(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 3) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 4);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleSpliceBeforeTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fifth.insert_after(fourth);
//    sixth.insert_after(fifth);
//
//    {
//      int count = 0;
//      for (const auto& data : fourth) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    sixth.splice_before(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 3) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 4) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 5) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 6);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleInsertAfterTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fourth.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 3) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 4);
//    }
//  }
//
//  TEST_F(PointerListTest, SimpleSpliceAfterTest) {
//    second.insert_after(first);
//    third.insert_after(second);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    fifth.insert_after(fourth);
//    sixth.insert_after(fifth);
//
//    {
//      int count = 0;
//      for (const auto& data : fourth) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 3);
//    }
//
//    sixth.splice_after(first);
//
//    {
//      int count = 0;
//      for (const auto& data : first) {
//        if (count == 0) {
//          EXPECT_EQ(data.numerator, 1);
//          EXPECT_EQ(data.denominator, 10);
//          EXPECT_EQ(data.precise, 1.1);
//        } else if (count == 1) {
//          EXPECT_EQ(data.numerator, 6);
//          EXPECT_EQ(data.denominator, 60);
//          EXPECT_EQ(data.precise, 6.6);
//        } else if (count == 2) {
//          EXPECT_EQ(data.numerator, 4);
//          EXPECT_EQ(data.denominator, 40);
//          EXPECT_EQ(data.precise, 4.4);
//        } else if (count == 3) {
//          EXPECT_EQ(data.numerator, 5);
//          EXPECT_EQ(data.denominator, 50);
//          EXPECT_EQ(data.precise, 5.5);
//        } else if (count == 4) {
//          EXPECT_EQ(data.numerator, 2);
//          EXPECT_EQ(data.denominator, 20);
//          EXPECT_EQ(data.precise, 2.2);
//        } else if (count == 5) {
//          EXPECT_EQ(data.numerator, 3);
//          EXPECT_EQ(data.denominator, 30);
//          EXPECT_EQ(data.precise, 3.3);
//        }
//        ++count;
//      }
//      EXPECT_EQ(count, 6);
//    }
//  }

} // namespace hatch

