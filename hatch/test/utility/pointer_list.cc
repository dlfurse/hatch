#include <hatch/utility/pointer_list.hh>
#include <gtest/gtest.h>

#include <iostream>

namespace hatch {

  class PointerListTest : public ::testing::Test {
  protected:
    class pointer_list_data : public pointer_list_node<pointer_list_data> {
    public:
      pointer_list_data(unsigned int numerator, unsigned int denominator, double precise) :
          numerator{numerator},
          denominator{denominator},
          precise{precise} {
      }

      unsigned int numerator;
      unsigned int denominator;
      double precise;
    };

    pointer_list_data first{1, 10, 1.1};
    pointer_list_data second{2, 20, 2.2};
    pointer_list_data third{3, 30, 3.3};
    pointer_list_data fourth{4, 40, 4.4};
    pointer_list_data fifth{5, 50, 5.5};
    pointer_list_data sixth{6, 60, 6.6};

  protected:
    void SetUp() override {

    }
  };

  TEST_F(PointerListTest, SimpleDetachedTest) {
    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());
    EXPECT_TRUE(fifth.detached());
    EXPECT_TRUE(sixth.detached());

    int count = 0;
    for (const auto& data : first) {
      if (count == 0) {
        EXPECT_EQ(data.numerator, 1);
        EXPECT_EQ(data.denominator, 10);
        EXPECT_EQ(data.precise, 1.1);
      }
      ++count;
    }
    EXPECT_EQ(count, 1);

    EXPECT_FALSE(first.detach());
    EXPECT_FALSE(second.detach());
    EXPECT_FALSE(third.detach());
    EXPECT_FALSE(fourth.detach());
    EXPECT_FALSE(fifth.detach());
    EXPECT_FALSE(sixth.detach());
  }

  TEST_F(PointerListTest, SimplePairTest) {
    second.insert_after(first);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        }
        ++count;
      }
      EXPECT_EQ(count, 2);
    }

    EXPECT_TRUE(second.detach());
    EXPECT_FALSE(first.detach());
    EXPECT_FALSE(second.detach());

    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());


    second.insert_before(first);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());

    {
      int count = 0;
      for (const auto& data : second) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        }
        ++count;
      }
      EXPECT_EQ(count, 2);
    }

    EXPECT_TRUE(first.detach());
    EXPECT_FALSE(second.detach());
    EXPECT_FALSE(first.detach());

    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
  }

  TEST_F(PointerListTest, SimpleListTest) {
    third.insert_after(first);
    second.insert_before(third);
    fourth.insert_after(third);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(fourth.detached());

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }

    EXPECT_TRUE(second.detach());
    EXPECT_TRUE(first.detach());
    EXPECT_TRUE(fourth.detach());
    EXPECT_FALSE(third.detach());
    EXPECT_FALSE(second.detach());
    EXPECT_FALSE(fourth.detach());
    EXPECT_FALSE(first.detach());

    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());


    third.insert_before(first);
    fourth.insert_after(third);
    second.insert_before(third);

    EXPECT_FALSE(first.detached());
    EXPECT_FALSE(second.detached());
    EXPECT_FALSE(third.detached());
    EXPECT_FALSE(fourth.detached());

    {
      int count = 0;
      for (const auto& data : third) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }

    EXPECT_TRUE(third.detach());
    EXPECT_TRUE(first.detach());
    EXPECT_TRUE(second.detach());
    EXPECT_FALSE(fourth.detach());
    EXPECT_FALSE(third.detach());
    EXPECT_FALSE(first.detach());
    EXPECT_FALSE(second.detach());

    EXPECT_TRUE(first.detached());
    EXPECT_TRUE(second.detached());
    EXPECT_TRUE(third.detached());
    EXPECT_TRUE(fourth.detached());
  }

  TEST_F(PointerListTest, SimpleInsertReplacingTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fourth.insert_replacing(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }
  }

  TEST_F(PointerListTest, SimpleSpliceReplacingTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fifth.insert_after(fourth);
    sixth.insert_after(fifth);

    {
      int count = 0;
      for (const auto& data : fourth) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fifth.splice_replacing(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 4) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 5);
    }
  }

  TEST_F(PointerListTest, SimpleInsertBeforeTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fifth.insert_before(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }
  }

  TEST_F(PointerListTest, SimpleSpliceBeforeTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fifth.insert_after(fourth);
    sixth.insert_after(fifth);

    {
      int count = 0;
      for (const auto& data : fourth) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    sixth.splice_before(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 4) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 5) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 6);
    }
  }

  TEST_F(PointerListTest, SimpleInsertAfterTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fourth.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }
  }

  TEST_F(PointerListTest, SimpleSpliceAfterTest) {
    second.insert_after(first);
    third.insert_after(second);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    fifth.insert_after(fourth);
    sixth.insert_after(fifth);

    {
      int count = 0;
      for (const auto& data : fourth) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    sixth.splice_after(first);

    {
      int count = 0;
      for (const auto& data : first) {
        if (count == 0) {
          EXPECT_EQ(data.numerator, 1);
          EXPECT_EQ(data.denominator, 10);
          EXPECT_EQ(data.precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(data.numerator, 6);
          EXPECT_EQ(data.denominator, 60);
          EXPECT_EQ(data.precise, 6.6);
        } else if (count == 2) {
          EXPECT_EQ(data.numerator, 4);
          EXPECT_EQ(data.denominator, 40);
          EXPECT_EQ(data.precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(data.numerator, 5);
          EXPECT_EQ(data.denominator, 50);
          EXPECT_EQ(data.precise, 5.5);
        } else if (count == 4) {
          EXPECT_EQ(data.numerator, 2);
          EXPECT_EQ(data.denominator, 20);
          EXPECT_EQ(data.precise, 2.2);
        } else if (count == 5) {
          EXPECT_EQ(data.numerator, 3);
          EXPECT_EQ(data.denominator, 30);
          EXPECT_EQ(data.precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 6);
    }
  }

} // namespace hatch

