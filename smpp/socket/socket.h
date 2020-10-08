//
// Created by boklazhenko on 13.05.2020.
//

#ifndef SMPPHA_SMPP_SESSION_SESSION_H_
#define SMPPHA_SMPP_SESSION_SESSION_H_

#include <vector>
#include <mutex>
#include "boost/asio.hpp"
#include <memory>
#include <queue>

namespace smpp {

class i_pdu;
enum class error;

class socket : public std::enable_shared_from_this<socket> {
  struct write_cmd;

 public:
  using open_handler = std::function<void(error)>;
  using write_handler = std::function<void(error)>;
  using read_handler = std::function<void(error, const std::shared_ptr<i_pdu> &)>;

  void write_async(const std::shared_ptr<i_pdu> &p_pdu, const write_handler &handler);
  void read_async(const read_handler &handler);

  bool is_open() const;

  void open_async(const boost::asio::ip::tcp::endpoint &endpoint, const open_handler &handler);
  void close();

  boost::asio::ip::tcp::socket & handle();

  static std::shared_ptr<socket> create(boost::asio::io_context &ioc);

 private:
  explicit socket(boost::asio::io_context &ioc);

  void write_next_async();

  void _read_async(const read_handler &handler);

 private:
  std::mutex _sock_m;
  boost::asio::io_context &_ioc;
  boost::asio::ip::tcp::socket _sock;

  std::atomic_bool _writing;
  std::vector<uint8_t> _write_buff;

  std::atomic_bool _reading;
  std::vector<uint8_t> _read_length_buff;
  std::vector<uint8_t> _read_buff;

  std::queue<write_cmd> _write_cmds;
  std::mutex _cmds_m;

  struct write_cmd {
    std::shared_ptr<smpp::i_pdu> p_pdu;
    write_handler handler;
  };
};

}

#endif //SMPPHA_SMPP_SESSION_SESSION_H_
