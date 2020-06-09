#include <hatch/utility/indexed_list.hh>
#include <gtest/gtest.h>

#include <iostream>

namespace hatch {

  class IndexedListTest : public ::testing::Test {
  protected:
    class indexed_list_data : public indexed_list_node<indexed_list_data> {
    public:
      indexed_list_data(unsigned int numerator, unsigned int denominator, double precise) :
          numerator{numerator},
          denominator{denominator},
          precise{precise} {
      }

      unsigned int numerator;
      unsigned int denominator;
      double precise;
    };

    indexed_list_data data[6] = {
        {1, 10, 1.1},
        {2, 20, 2.2},
        {3, 30, 3.3},
        {4, 40, 4.4},
        {5, 50, 5.5},
        {6, 60, 6.6}
    };

  protected:
    void SetUp() override {

    }
  };

  TEST_F(IndexedListTest, SimpleDetachedTest) {
    EXPECT_TRUE(data[0].detached());
    EXPECT_TRUE(data[1].detached());
    EXPECT_TRUE(data[2].detached());
    EXPECT_TRUE(data[3].detached());
    EXPECT_TRUE(data[4].detached());
    EXPECT_TRUE(data[5].detached());

    int count = 0;
    for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
      if (count == 0) {
        EXPECT_EQ(it->numerator, 1);
        EXPECT_EQ(it->denominator, 10);
        EXPECT_EQ(it->precise, 1.1);
      }
      ++count;
    }
    EXPECT_EQ(count, 1);

    EXPECT_FALSE(data[0].detach(data));
    EXPECT_FALSE(data[1].detach(data));
    EXPECT_FALSE(data[2].detach(data));
    EXPECT_FALSE(data[3].detach(data));
    EXPECT_FALSE(data[4].detach(data));
    EXPECT_FALSE(data[5].detach(data));
  }

  TEST_F(IndexedListTest, SimplePairTest) {
    data[1].insert_after(data, data[0]);

    EXPECT_FALSE(data[0].detached());
    EXPECT_FALSE(data[1].detached());

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        }
        ++count;
      }
      EXPECT_EQ(count, 2);
    }

    EXPECT_TRUE(data[1].detach(data));
    EXPECT_FALSE(data[0].detach(data));
    EXPECT_FALSE(data[1].detach(data));

    EXPECT_TRUE(data[0].detached());
    EXPECT_TRUE(data[1].detached());


    data[1].insert_before(data, data[0]);

    EXPECT_FALSE(data[0].detached());
    EXPECT_FALSE(data[1].detached());

    {
      int count = 0;
      for (auto it = data[1].begin(data); it != data[1].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        }
        ++count;
      }
      EXPECT_EQ(count, 2);
    }

    EXPECT_TRUE(data[0].detach(data));
    EXPECT_FALSE(data[1].detach(data));
    EXPECT_FALSE(data[0].detach(data));

    EXPECT_TRUE(data[0].detached());
    EXPECT_TRUE(data[1].detached());
  }

  TEST_F(IndexedListTest, SimpleListTest) {
    data[2].insert_after(data, data[0]);
    data[1].insert_before(data, data[2]);
    data[3].insert_after(data, data[2]);

    EXPECT_FALSE(data[0].detached());
    EXPECT_FALSE(data[1].detached());
    EXPECT_FALSE(data[2].detached());
    EXPECT_FALSE(data[3].detached());

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }

    EXPECT_TRUE(data[1].detach(data));
    EXPECT_TRUE(data[0].detach(data));
    EXPECT_TRUE(data[3].detach(data));
    EXPECT_FALSE(data[2].detach(data));
    EXPECT_FALSE(data[1].detach(data));
    EXPECT_FALSE(data[3].detach(data));
    EXPECT_FALSE(data[0].detach(data));

    EXPECT_TRUE(data[0].detached());
    EXPECT_TRUE(data[1].detached());
    EXPECT_TRUE(data[2].detached());
    EXPECT_TRUE(data[3].detached());


    data[2].insert_before(data, data[0]);
    data[3].insert_after(data, data[2]);
    data[1].insert_before(data, data[2]);

    EXPECT_FALSE(data[0].detached());
    EXPECT_FALSE(data[1].detached());
    EXPECT_FALSE(data[2].detached());
    EXPECT_FALSE(data[3].detached());

    {
      int count = 0;
      for (auto it = data[2].begin(data); it != data[2].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }

    EXPECT_TRUE(data[2].detach(data));
    EXPECT_TRUE(data[0].detach(data));
    EXPECT_TRUE(data[1].detach(data));
    EXPECT_FALSE(data[3].detach(data));
    EXPECT_FALSE(data[2].detach(data));
    EXPECT_FALSE(data[0].detach(data));
    EXPECT_FALSE(data[1].detach(data));

    EXPECT_TRUE(data[0].detached());
    EXPECT_TRUE(data[1].detached());
    EXPECT_TRUE(data[2].detached());
    EXPECT_TRUE(data[3].detached());
  }

  TEST_F(IndexedListTest, SimpleInsertReplacingTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[3].insert_replacing(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }
  }

  TEST_F(IndexedListTest, SimpleSpliceReplacingTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[4].insert_after(data, data[3]);
    data[5].insert_after(data, data[4]);

    {
      int count = 0;
      for (auto it = data[3].begin(data); it != data[3].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[4].splice_replacing(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 4) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 5);
    }
  }

  TEST_F(IndexedListTest, SimpleInsertBeforeTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[4].insert_before(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }
  }

  TEST_F(IndexedListTest, SimpleSpliceBeforeTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[4].insert_after(data, data[3]);
    data[5].insert_after(data, data[4]);

    {
      int count = 0;
      for (auto it = data[3].begin(data); it != data[3].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[5].splice_before(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 4) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 5) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 6);
    }
  }

  TEST_F(IndexedListTest, SimpleInsertAfterTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[3].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 4);
    }
  }

  TEST_F(IndexedListTest, SimpleSpliceAfterTest) {
    data[1].insert_after(data, data[0]);
    data[2].insert_after(data, data[1]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[4].insert_after(data, data[3]);
    data[5].insert_after(data, data[4]);

    {
      int count = 0;
      for (auto it = data[3].begin(data); it != data[3].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        }
        ++count;
      }
      EXPECT_EQ(count, 3);
    }

    data[5].splice_after(data, data[0]);

    {
      int count = 0;
      for (auto it = data[0].begin(data); it != data[0].end(data); ++it) {
        if (count == 0) {
          EXPECT_EQ(it->numerator, 1);
          EXPECT_EQ(it->denominator, 10);
          EXPECT_EQ(it->precise, 1.1);
        } else if (count == 1) {
          EXPECT_EQ(it->numerator, 6);
          EXPECT_EQ(it->denominator, 60);
          EXPECT_EQ(it->precise, 6.6);
        } else if (count == 2) {
          EXPECT_EQ(it->numerator, 4);
          EXPECT_EQ(it->denominator, 40);
          EXPECT_EQ(it->precise, 4.4);
        } else if (count == 3) {
          EXPECT_EQ(it->numerator, 5);
          EXPECT_EQ(it->denominator, 50);
          EXPECT_EQ(it->precise, 5.5);
        } else if (count == 4) {
          EXPECT_EQ(it->numerator, 2);
          EXPECT_EQ(it->denominator, 20);
          EXPECT_EQ(it->precise, 2.2);
        } else if (count == 5) {
          EXPECT_EQ(it->numerator, 3);
          EXPECT_EQ(it->denominator, 30);
          EXPECT_EQ(it->precise, 3.3);
        }
        ++count;
      }
      EXPECT_EQ(count, 6);
    }
  }

} // namespace hatch

