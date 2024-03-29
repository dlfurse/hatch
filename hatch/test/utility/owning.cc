#include <hatch/utility/owning.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <iostream>

#include <cstdint>

namespace hatch {

  class KeepTest : public ::testing::Test {
  protected:
    class test_owner;
    class test_owned;

    class test_owner : public owner<test_owner, test_owned, pointed> {
    public:
      bool has_kept(test_owned* kept) {
        bool result = false;
        foreach([&](auto& node) {
          if (kept == &node) {
            result = true;
          }
        });
        return result;
      }

      uint64_t count_kepts() {
        uint64_t result = 0;
        foreach([&](auto& node) {
          ++result;
        });
        return result;
      }
    };

    class test_owned : public owned<test_owner, test_owned, pointed> {
    public:
      test_owner* keeper() {
        return _owner.pointed<test_owner>::operator->();
      }

      void detach() {
        owned<test_owner, test_owned, pointed>::detach();
      }

      void attach(test_owner* keeper) {
        owned<test_owner, test_owned, pointed>::attach(keeper);
      }

      uint64_t value;
    };

    static constexpr auto keepers = 4;
    test_owner keeper[keepers];

    static constexpr auto kepts = 4;
    test_owned kept[kepts];

  protected:
    void SetUp() override {
      for (auto index = 0u; index < kepts; index++) {
        kept[index].value = index;
      }
    }
  };

  TEST_F(KeepTest, KeepAloneTest) {
    for (auto index = 0u; index < kepts; index++) {
      EXPECT_EQ(&kept[index].prev(), &kept[index]);
      EXPECT_EQ(&kept[index].next(), &kept[index]);
      EXPECT_EQ(kept[index].keeper(), nullptr);
    }
  }

  TEST_F(KeepTest, KeepOneTest) {
    kept[0].attach(&keeper[0]);

    EXPECT_EQ(&kept[0].prev(), &kept[0]);
    EXPECT_EQ(&kept[0].next(), &kept[0]);
    EXPECT_EQ(kept[0].keeper(), &keeper[0]);

    EXPECT_EQ(keeper[0].count_kepts(), 1);
    EXPECT_TRUE(keeper[0].has_kept(&kept[0]));
  }

  TEST_F(KeepTest, KeepOneKeeperMoveConstructorTest) {
    kept[0].attach(&keeper[0]);
    test_owner boss{std::move(keeper[0])};

    EXPECT_EQ(boss.count_kepts(), 1);
    EXPECT_TRUE(boss.has_kept(&kept[0]));

    EXPECT_EQ(keeper[0].count_kepts(), 0);
    EXPECT_FALSE(keeper[0].has_kept(&kept[0]));

    EXPECT_EQ(&kept[0].prev(), &kept[0]);
    EXPECT_EQ(&kept[0].next(), &kept[0]);
    EXPECT_EQ(kept[0].keeper(), &boss);
  }

  TEST_F(KeepTest, KeepOneKeeperMoveAssignmentTest) {
    kept[0].attach(&keeper[0]);
    keeper[1] = std::move(keeper[0]);

    EXPECT_EQ(keeper[1].count_kepts(), 1);
    EXPECT_TRUE(keeper[1].has_kept(&kept[0]));

    EXPECT_EQ(keeper[0].count_kepts(), 0);
    EXPECT_FALSE(keeper[0].has_kept(&kept[0]));

    EXPECT_EQ(&kept[0].prev(), &kept[0]);
    EXPECT_EQ(&kept[0].next(), &kept[0]);
    EXPECT_EQ(kept[0].keeper(), &keeper[1]);
  }

  TEST_F(KeepTest, KeepOneKeptMoveConstructorTest) {
    kept[0].attach(&keeper[0]);
    test_owned node{std::move(kept[0])};

    EXPECT_EQ(&kept[0].prev(), &kept[0]);
    EXPECT_EQ(&kept[0].next(), &kept[0]);
    EXPECT_EQ(kept[0].keeper(), nullptr);

    EXPECT_EQ(&node.prev(), &node);
    EXPECT_EQ(&node.next(), &node);
    EXPECT_EQ(node.keeper(), &keeper[0]);

    EXPECT_EQ(keeper[0].count_kepts(), 1);
    EXPECT_FALSE(keeper[0].has_kept(&kept[0]));
    EXPECT_TRUE(keeper[0].has_kept(&node));
  }

  TEST_F(KeepTest, KeepOneKeptMoveAssignmentTest) {
    kept[0].attach(&keeper[0]);
    kept[1] = std::move(kept[0]);

    EXPECT_EQ(&kept[0].prev(), &kept[0]);
    EXPECT_EQ(&kept[0].next(), &kept[0]);
    EXPECT_EQ(kept[0].keeper(), nullptr);

    EXPECT_EQ(&kept[1].prev(), &kept[1]);
    EXPECT_EQ(&kept[1].next(), &kept[1]);
    EXPECT_EQ(kept[1].keeper(), &keeper[0]);

    EXPECT_EQ(keeper[0].count_kepts(), 1);
    EXPECT_FALSE(keeper[0].has_kept(&kept[0]));
    EXPECT_TRUE(keeper[0].has_kept(&kept[1]));
  }

  TEST_F(KeepTest, KeepOneKeptCopyConstructorTest) {
    kept[0].attach(&keeper[0]);
    test_owned node{kept[0]};

    EXPECT_EQ(&kept[0].prev(), &node);
    EXPECT_EQ(&kept[0].next(), &node);
    EXPECT_EQ(kept[0].keeper(), &keeper[0]);

    EXPECT_EQ(&node.prev(), &kept[0]);
    EXPECT_EQ(&node.next(), &kept[0]);
    EXPECT_EQ(node.keeper(), &keeper[0]);


    EXPECT_EQ(keeper[0].count_kepts(), 2);
    EXPECT_TRUE(keeper[0].has_kept(&kept[0]));
    EXPECT_TRUE(keeper[0].has_kept(&node));
  }

  TEST_F(KeepTest, KeepOneKeptCopyAssignmentTest) {
    kept[0].attach(&keeper[0]);
    kept[1] = kept[0];

    EXPECT_EQ(&kept[0].prev(), &kept[1]);
    EXPECT_EQ(&kept[0].next(), &kept[1]);
    EXPECT_EQ(kept[0].keeper(), &keeper[0]);

    EXPECT_EQ(&kept[1].prev(), &kept[0]);
    EXPECT_EQ(&kept[1].next(), &kept[0]);
    EXPECT_EQ(kept[1].keeper(), &keeper[0]);

    EXPECT_EQ(keeper[0].count_kepts(), 2);
    EXPECT_TRUE(keeper[0].has_kept(&kept[0]));
    EXPECT_TRUE(keeper[0].has_kept(&kept[1]));
  }

}