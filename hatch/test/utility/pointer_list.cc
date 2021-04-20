#include <hatch/utility/pointer_list.hh>
#include <gtest/gtest.h>

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

      void splice(pointer_list_node<test_node>& node) {
        return node::splice(node);
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
        EXPECT_TRUE(++count <= 6);
      }
      return count;
    }
  };

  TEST_F(PointerListTest, SimpleEmptyListTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(fourth.alone());
    EXPECT_TRUE(fifth.alone());
    EXPECT_TRUE(sixth.alone());

    EXPECT_TRUE(one.empty());
    EXPECT_TRUE(two.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, SimpleSpliceTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());

    second.splice(third);

    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &second);
    EXPECT_EQ(&second.prev(), &third);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    first.splice(second);

    EXPECT_EQ(&first.next(), &second);
    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&second.prev(), &first);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    second.splice(second.next());

    EXPECT_EQ(&first.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&third.prev(), &first);

    EXPECT_FALSE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(third.alone());

    first.splice(first);

    EXPECT_EQ(&first.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&third.prev(), &first);

    EXPECT_FALSE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(third.alone());
  }

  TEST_F(PointerListTest, MultipleSpliceTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());

    EXPECT_TRUE(fourth.alone());
    EXPECT_TRUE(fifth.alone());
    EXPECT_TRUE(sixth.alone());

    second.splice(third);
    first.splice(second);

    fifth.splice(sixth);
    fourth.splice(fifth);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    EXPECT_FALSE(fourth.alone());
    EXPECT_FALSE(fifth.alone());
    EXPECT_FALSE(sixth.alone());

    second.splice(fifth);

    EXPECT_EQ(&fourth.next(), &second);
    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.next(), &fifth);
    EXPECT_EQ(&fifth.next(), &sixth);
    EXPECT_EQ(&sixth.next(), &fourth);
    EXPECT_EQ(&fourth.prev(), &sixth);
    EXPECT_EQ(&second.prev(), &fourth);
    EXPECT_EQ(&third.prev(), &second);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&fifth.prev(), &first);
    EXPECT_EQ(&sixth.prev(), &fifth);
  }

  TEST_F(PointerListTest, OneElementFrontTest) {
    one.push_front(first);

    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &first);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);

    node = one.pop_front();
    one.push_front(second);

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.front(), &second);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 2);
    EXPECT_EQ(data[0].denominator, 20);
    EXPECT_EQ(data[0].precise, 2.2);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.alone());
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

    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.back(), &first);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);

    node = one.pop_back();
    one.push_back(second);

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(one.back(), &second);
    EXPECT_EQ(dump(one), 1);
    EXPECT_EQ(data[0].numerator, 2);
    EXPECT_EQ(data[0].denominator, 20);
    EXPECT_EQ(data[0].precise, 2.2);

    node = one.pop_back();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.alone());
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

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
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
    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());
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
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, TwoElementBackTest) {
    one.push_back(first);
    one.push_back(second);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
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
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(third.alone());
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
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_back();

    EXPECT_EQ(node, &first);
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }


  TEST_F(PointerListTest, FourElementFrontBackTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);
    one.push_back(fourth);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());
    EXPECT_FALSE(fourth.alone());
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
    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());
    EXPECT_FALSE(fourth.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);

    node = one.pop_back();

    EXPECT_EQ(node, &fourth);
    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());
    EXPECT_TRUE(fourth.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 2);

    node = one.pop_front();

    EXPECT_EQ(node, &second);
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(fourth.alone());
    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 1);

    node = one.pop_back();

    EXPECT_EQ(node, &third);
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, OneListFrontTest) {
    two.push_front(third);

    EXPECT_TRUE(third.alone());
    EXPECT_FALSE(two.empty());
    EXPECT_EQ(two.front(), &third);
    EXPECT_EQ(dump(two), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    one.push_front(two);

    EXPECT_TRUE(third.alone());
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
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.front(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, OneListBackTest) {
    two.push_back(third);

    EXPECT_TRUE(third.alone());
    EXPECT_FALSE(two.empty());
    EXPECT_EQ(two.back(), &third);
    EXPECT_EQ(dump(two), 1);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);

    one.push_back(two);

    EXPECT_TRUE(third.alone());
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
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(one.empty());
    EXPECT_EQ(one.back(), nullptr);
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, IteratorLoopTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);
    one.push_back(fourth);

    auto index = 0;
    for (auto& item : one) {
      switch (index++) {
        case 0:
          EXPECT_EQ(item.numerator, 1);
          EXPECT_EQ(item.denominator, 10);
          EXPECT_EQ(item.precise, 1.1);
          break;
        case 1:
          EXPECT_EQ(item.numerator, 2);
          EXPECT_EQ(item.denominator, 20);
          EXPECT_EQ(item.precise, 2.2);
          break;
        case 2:
          EXPECT_EQ(item.numerator, 3);
          EXPECT_EQ(item.denominator, 30);
          EXPECT_EQ(item.precise, 3.3);
          break;
        case 3:
          EXPECT_EQ(item.numerator, 4);
          EXPECT_EQ(item.denominator, 40);
          EXPECT_EQ(item.precise, 4.4);
          break;
        default:
          EXPECT_EQ(item.numerator, 0);
          EXPECT_EQ(item.denominator, 0);
          EXPECT_EQ(item.precise, 0.0);
      }
    }
  }

  TEST_F(PointerListTest, IteratorInsertSelfTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    auto begin = one.begin();
    auto iter = begin.insert(one);

    EXPECT_EQ(begin, iter);
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);
  }

  TEST_F(PointerListTest, IteratorInsertBeginTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    two.push_back(fifth);
    two.push_front(fourth);
    two.push_back(sixth);

    auto begin = one.begin();
    auto iter = begin.insert(two);

    EXPECT_NE(begin, one.begin());
    EXPECT_EQ(iter, one.begin());

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 6);
    EXPECT_EQ(data[0].numerator, 4);
    EXPECT_EQ(data[0].denominator, 40);
    EXPECT_EQ(data[0].precise, 4.4);
    EXPECT_EQ(data[1].numerator, 5);
    EXPECT_EQ(data[1].denominator, 50);
    EXPECT_EQ(data[1].precise, 5.5);
    EXPECT_EQ(data[2].numerator, 6);
    EXPECT_EQ(data[2].denominator, 60);
    EXPECT_EQ(data[2].precise, 6.6);
    EXPECT_EQ(data[3].numerator, 1);
    EXPECT_EQ(data[3].denominator, 10);
    EXPECT_EQ(data[3].precise, 1.1);
    EXPECT_EQ(data[4].numerator, 2);
    EXPECT_EQ(data[4].denominator, 20);
    EXPECT_EQ(data[4].precise, 2.2);
    EXPECT_EQ(data[5].numerator, 3);
    EXPECT_EQ(data[5].denominator, 30);
    EXPECT_EQ(data[5].precise, 3.3);

    EXPECT_TRUE(two.empty());
    EXPECT_EQ(dump(two), 0);
  }

  TEST_F(PointerListTest, IteratorInsertEndTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    two.push_back(fifth);
    two.push_front(fourth);
    two.push_back(sixth);

    auto end = two.end();
    auto iter = end.insert(one);

    EXPECT_EQ(end, one.end());
    EXPECT_NE(iter, one.end());

    EXPECT_FALSE(two.empty());
    EXPECT_EQ(dump(two), 6);
    EXPECT_EQ(data[0].numerator, 4);
    EXPECT_EQ(data[0].denominator, 40);
    EXPECT_EQ(data[0].precise, 4.4);
    EXPECT_EQ(data[1].numerator, 5);
    EXPECT_EQ(data[1].denominator, 50);
    EXPECT_EQ(data[1].precise, 5.5);
    EXPECT_EQ(data[2].numerator, 6);
    EXPECT_EQ(data[2].denominator, 60);
    EXPECT_EQ(data[2].precise, 6.6);
    EXPECT_EQ(data[3].numerator, 1);
    EXPECT_EQ(data[3].denominator, 10);
    EXPECT_EQ(data[3].precise, 1.1);
    EXPECT_EQ(data[4].numerator, 2);
    EXPECT_EQ(data[4].denominator, 20);
    EXPECT_EQ(data[4].precise, 2.2);
    EXPECT_EQ(data[5].numerator, 3);
    EXPECT_EQ(data[5].denominator, 30);
    EXPECT_EQ(data[5].precise, 3.3);

    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);
  }

  TEST_F(PointerListTest, IteratorInsertMiddleTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    two.push_back(fifth);
    two.push_front(fourth);
    two.push_back(sixth);

    auto iter = one.begin();
    iter++;
    ++iter;

    iter.insert(two);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 6);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 4);
    EXPECT_EQ(data[2].denominator, 40);
    EXPECT_EQ(data[2].precise, 4.4);
    EXPECT_EQ(data[3].numerator, 5);
    EXPECT_EQ(data[3].denominator, 50);
    EXPECT_EQ(data[3].precise, 5.5);
    EXPECT_EQ(data[4].numerator, 6);
    EXPECT_EQ(data[4].denominator, 60);
    EXPECT_EQ(data[4].precise, 6.6);
    EXPECT_EQ(data[5].numerator, 3);
    EXPECT_EQ(data[5].denominator, 30);
    EXPECT_EQ(data[5].precise, 3.3);

    EXPECT_TRUE(two.empty());
    EXPECT_EQ(dump(two), 0);
  }

  TEST_F(PointerListTest, IteratorRemoveSelfTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    auto start = one.begin();
    ++start;
    start++;

    auto list = start.remove(start);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(dump(list), 0);
  }

  TEST_F(PointerListTest, IteratorRemoveEndTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    auto start = one.end();

    auto end = one.end();

    auto list = start.remove(end);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(dump(list), 0);
  }

  TEST_F(PointerListTest, IteratorRemoveForeignTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    two.push_back(fifth);
    two.push_front(fourth);
    two.push_back(sixth);

    auto start = one.begin();
    ++start;
    start++;

    auto end = two.begin();

    auto list = start.remove(end);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(dump(list), 0);
  }

  TEST_F(PointerListTest, IteratorRemoveMalorderTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    auto start = one.begin();
    ++start;
    start++;

    auto end = one.begin();

    auto list = start.remove(end);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(dump(list), 0);
  }

  TEST_F(PointerListTest, IteratorRemoveEntireTest) {
    one.push_front(second);
    one.push_back(third);
    one.push_front(first);

    auto start = one.begin();

    auto end = one.end();

    auto list = start.remove(end);

    EXPECT_TRUE(one.empty());
    EXPECT_EQ(dump(one), 0);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(dump(list), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 3);
    EXPECT_EQ(data[2].denominator, 30);
    EXPECT_EQ(data[2].precise, 3.3);
  }

  TEST_F(PointerListTest, IteratorRemoveNormalTest) {
    one.push_front(third);
    one.push_back(fourth);
    one.push_front(second);
    one.push_back(fifth);
    one.push_front(first);
    one.push_back(sixth);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 6);
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
    EXPECT_EQ(data[4].numerator, 5);
    EXPECT_EQ(data[4].denominator, 50);
    EXPECT_EQ(data[4].precise, 5.5);
    EXPECT_EQ(data[5].numerator, 6);
    EXPECT_EQ(data[5].denominator, 60);
    EXPECT_EQ(data[5].precise, 6.6);

    auto start = one.begin();
    start++;
    start++;

    auto end = one.begin();
    end++;
    end++;
    end++;
    end++;
    end++;

    auto list = start.remove(end);

    EXPECT_FALSE(one.empty());
    EXPECT_EQ(dump(one), 3);
    EXPECT_EQ(data[0].numerator, 1);
    EXPECT_EQ(data[0].denominator, 10);
    EXPECT_EQ(data[0].precise, 1.1);
    EXPECT_EQ(data[1].numerator, 2);
    EXPECT_EQ(data[1].denominator, 20);
    EXPECT_EQ(data[1].precise, 2.2);
    EXPECT_EQ(data[2].numerator, 6);
    EXPECT_EQ(data[2].denominator, 60);
    EXPECT_EQ(data[2].precise, 6.6);


    EXPECT_FALSE(list.empty());
    EXPECT_EQ(dump(list), 3);
    EXPECT_EQ(data[0].numerator, 3);
    EXPECT_EQ(data[0].denominator, 30);
    EXPECT_EQ(data[0].precise, 3.3);
    EXPECT_EQ(data[1].numerator, 4);
    EXPECT_EQ(data[1].denominator, 40);
    EXPECT_EQ(data[1].precise, 4.4);
    EXPECT_EQ(data[2].numerator, 5);
    EXPECT_EQ(data[2].denominator, 50);
    EXPECT_EQ(data[2].precise, 5.5);
  }

} // namespace hatch

