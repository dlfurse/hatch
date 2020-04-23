#include <hatch/core/socket.hh>
#include <gtest/gtest.h>

namespace hatch {
  class SocketTest : public ::testing::Test {
  protected:
    //socket<socket_domain::inet4, socket_type::tcp> _inet4_tcp_socket;

  protected:
    void SetUp() override {

    }
  };

  TEST_F(SocketTest, SimpleSocketTest) {
    //_inet4_tcp_socket._fd;
  }
}