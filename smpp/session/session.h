//
// Created by boklazhenko on 13.05.2020.
//

#ifndef SMPPHA_SMPP_SESSION_SESSION_H_
#define SMPPHA_SMPP_SESSION_SESSION_H_

#include <vector>
#include <mutex>
#include "boost/asio.hpp"
#include <memory>

namespace smpp {

class i_pdu;
enum class error;

class session : public std::enable_shared_from_this<session> {
 public:
  using write_handler = std::function<void(error)>;
  using read_handler = std::function<void(error, const std::shared_ptr<i_pdu> &)>;

  void write_async(const std::shared_ptr<i_pdu> &p_pdu, const write_handler &handler);
  void read_async(const read_handler &handler);

  static std::shared_ptr<session> create(boost::asio::io_context &ioc, boost::asio::ip::tcp::socket &&sock);

 private:
  session(boost::asio::io_context &ioc, boost::asio::ip::tcp::socket &&sock);

  void _read_async(const read_handler &handler);

 private:
  std::mutex _mutex;
  boost::asio::io_context &_ioc;
  boost::asio::ip::tcp::socket _sock;

  std::atomic_bool _writing;
  std::vector<uint8_t> _write_buff;

  std::atomic_bool _reading;
  std::vector<uint8_t> _read_length_buff;
  std::vector<uint8_t> _read_buff;
};

}

#endif //SMPPHA_SMPP_SESSION_SESSION_H_
