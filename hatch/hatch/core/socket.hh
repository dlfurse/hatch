#ifndef HATCH_SOCKET_HH
#define HATCH_SOCKET_HH

#include <limits>
#include <string>

//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>

#include <hatch/core/async.hh>

namespace hatch {
  enum class socket_domain {
    inet4 = 0,
    inet6 = 1,
    unix = 2,
  };

  template <socket_domain D>
  class socket_domain_traits {
  public:
    static constexpr sa_family_t value();
  };

  template <>
  class socket_domain_traits<socket_domain::inet4> {
  public:
    static constexpr sa_family_t value() { return PF_INET; }
  };

  template <>
  class socket_domain_traits<socket_domain::inet6> {
  public:
    static constexpr sa_family_t value() { return PF_INET6; }
  };

  template <>
  class socket_domain_traits<socket_domain::unix> {
  public:
    static constexpr sa_family_t value() { return PF_UNIX; }
  };

  enum class socket_type {
    tcp = 0,
    udp = 1,
  };

  template <socket_type T>
  class socket_type_traits {
  public:
    static constexpr int value();
  };

  template <>
  class socket_type_traits<socket_type::tcp> {
  public:
    static constexpr int value() { return SOCK_STREAM; }
  };

  template <>
  class socket_type_traits<socket_type::udp> {
  public:
    static constexpr int value() { return SOCK_DGRAM; }
  };


  template <socket_domain D>
  class socket_address;

  template <>
  class socket_address<socket_domain::inet4> {
  public:
    static constexpr sa_family_t family() { return PF_INET; }
    using native = sockaddr_in;

    socket_address(const std::string& address, uint8_t port) :
      _address{address}, _port{port}, _native{0} {
      _native.sin_family = family();
      _native.sin_port = htons(port);
      if (inet_pton(family(), address.c_str(), &_native.sin_addr) == 0) {
        throw std::runtime_error("improperly formatted ipv4 address: " + address);
      }
    }

    const std::string& address() const {
      return _address;
    }

    const uint8_t& port() const {
      return _port;
    }

  private:
    std::string _address;
    uint8_t _port;
    sockaddr_in _native;
  };

  template <>
  class socket_address<socket_domain::inet6> {
  public:
    static constexpr sa_family_t family() { return PF_INET6; }
    using native = sockaddr_in6;

    socket_address(const std::string& address, uint8_t port) :
      _address{address}, _port{port}, _native{0} {
      _native.sin6_family = family();
      _native.sin6_port = htons(port);
      if (inet_pton(family(), address.c_str(), &_native.sin6_addr) == 0) {
        throw std::runtime_error("improperly formatted ipv6 address: " + address);
      }
    }

    const std::string& address() const {
      return _address;
    }

    const uint8_t& port() const {
      return _port;
    }

  private:
    std::string _address;
    uint8_t _port;
    sockaddr_in6 _native;
  };


  template <socket_domain D, socket_type T>
  class socket {
  private:
    int _fd{-1};

  public:
    socket();
  };

  template <socket_domain D, socket_type T>
  socket<D, T>::socket() {
    _fd = ::socket(socket_domain_traits<D>::value(), socket_type_traits<T>::value(), 0);
  }
}

#endif //HATCH_SOCKET_HH
