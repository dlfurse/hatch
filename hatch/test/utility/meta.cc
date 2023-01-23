#include <hatch/utility/meta.hh>
#include <gtest/gtest.h>

#include <stdexcept>

namespace hatch {

  class MetaTest : public ::testing::Test {
  public:
    class test_incomplete;

    class test_complete {
    public:
      int value;
    };

  public:
    class test_initial_exception : public std::exception {
    public:
      const char* what() const noexcept override  {
        return "any test_initial was instantiated";
      }
    };

    template <class ...Args>
    class test_initial {
    public:
      test_initial() {
        throw test_initial_exception();
      }
    };

  public:
    class test_final_exception : public std::exception {
    public:
      const char* what() const noexcept override  {
        return "nested test_final was instantiated";
      }
    };

    template <class ...Args>
    class test_final;

    template <class Head, class ...Tail>
    class test_final<Head, Tail...> :
        public test_final<Head>,
        public test_final<Tail...> {
    public:
      test_final() = default;
    };

    template <class Head>
    class test_final<Head> {
    public:
      test_final() = default;
    };

    template <class ...Args>
    class test_final<test_final<Args...>> {
    public:
      test_final() {
        throw test_final_exception();
      }
    };

  public:
    using t1 = typed<1>;
    using t2 = typed<2>;
    using t3 = typed<3>;
    using t4 = typed<4>;
    using t5 = typed<5>;
    using t6 = typed<6>;

    using test_initial_flat = test_initial<t1, t2, t3, t4, t5, t6>;
    using test_initial_nest = test_initial<t1, t2, test_initial<t3, t4>, t5, test_initial<test_initial<t6>>>;

    using test_final_flat = test_final<t1, t2, t3, t4, t5, t6>;
    using test_final_nest = test_final<t1, t2, test_final<t3, t4>, t5, test_final<test_final<t6>>>;
  };

  TEST_F(MetaTest, CompleteWithIncomplete) {
    EXPECT_FALSE(complete_v<test_incomplete>);
  }

  TEST_F(MetaTest, CompleteWithComplete) {
    EXPECT_TRUE(complete_v<test_complete>);
  }

  TEST_F(MetaTest, AllInitialsThrow) {
    using flat = test_initial_flat;
    EXPECT_THROW({
      flat();
    }, test_initial_exception);

    using nest = test_initial_nest;
    EXPECT_THROW({
      nest();
    }, test_initial_exception);
  }

  TEST_F(MetaTest, NestFinalsThrow) {
    using flat = test_final_flat;
    EXPECT_NO_THROW({
      flat();
    });

    using nest = test_final_nest;
    EXPECT_THROW({
      nest();
    }, test_final_exception);
  }

  TEST_F(MetaTest, FlatToFlat) {
    using test = wrapper<test_final<>, test_initial_flat>::type;
    EXPECT_NO_THROW({
      test();
    });

    bool same = std::is_same_v<test, test_final_flat>;
    EXPECT_TRUE(same);
  }

  TEST_F(MetaTest, NestToFlat) {
    using test = wrapper<test_final<>, test_initial_nest>::type;
    EXPECT_NO_THROW({
      test();
    });

    bool same = std::is_same_v<test, test_final_flat>;
    EXPECT_TRUE(same);
  }
}