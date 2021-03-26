//
// Created by boklazhenko on 13.05.2020.
//

#include "socket.h"
#include "../pdu/pdu.h"

smpp::socket::socket(boost::asio::io_context &ioc)
    : _ioc(ioc),
      _sock(boost::asio::ip::tcp::socket(_ioc)),
      _writing(false),
      _reading(false),
      _read_length_buff(sizeof(uint32_t)) {

}

void smpp::socket::write_async(const std::shared_ptr<i_pdu> &p_pdu, const write_handler &handler) {
  _write_cmds.enqueue(write_cmd{p_pdu, handler});

  write_next_async();
}

void smpp::socket::read_async(const read_handler &handler) {
  using namespace boost;

  if (_reading.exchange(true)) {
    _ioc.post([s = shared_from_this(), handler]() {
      handler(boost::system::errc::make_error_code(boost::system::errc::operation_in_progress), nullptr);
    });
    return;
  }

  _read_async(handler);
}

std::shared_ptr<smpp::socket> smpp::socket::create(boost::asio::io_context &ioc) {
  return std::shared_ptr<socket>(new socket(ioc));
}

void smpp::socket::_read_async(const read_handler &handler) {
  using namespace boost;

  std::lock_guard guard(_sock_m);
  _sock.async_receive(asio::buffer(_read_length_buff), asio::ip::tcp::socket::message_peek,
                      [s = shared_from_this(), handler](const system::error_code &ec, size_t size) {
                        if (ec) {
                          s->_reading = false;
                          handler(ec, nullptr);
                          return;
                        }

                        if (size < s->_read_length_buff.size()) {
                          s->_ioc.post([s, handler]() {
                            s->_read_async(handler);
                          });
                          return;
                        }

                        uint32_t length;
                        binary_reader(s->_read_length_buff) >> length;
                        s->_read_buff.resize(length);
                        std::lock_guard guard(s->_sock_m);
                        asio::async_read(s->_sock,
                                         asio::buffer(s->_read_buff),
                                         [s, handler](const system::error_code &ec, size_t size) {
                                           s->_reading = false;
                                           if (ec) {
                                             handler(ec, nullptr);
                                             return;
                                           }

                                           std::shared_ptr<i_pdu> p_pdu;
                                           boost::system::error_code e = pdu<>::tryParse(s->_read_buff, p_pdu);
                                           handler(e, p_pdu);
                                         });
                      });
}

bool smpp::socket::is_open() const {
  return _sock.is_open();
}

void smpp::socket::write_next_async() {
  using namespace boost;

  if (_writing.exchange(true))
    return;

  write_cmd cmd;
  if(!_write_cmds.try_dequeue(cmd)) {
    _writing = false;
    return;
  }

  _write_buff = cmd.p_pdu->serialize();

  std::lock_guard guard(_sock_m);
  asio::async_write(_sock,
                    asio::buffer(_write_buff),
                    [s = shared_from_this(), handler = cmd.handler](const system::error_code &ec, size_t size) {
                      s->_writing = false;
                      s->write_next_async();
                      handler(ec);
                    });
}

void smpp::socket::close() {
  write_cmd cmd;
  while (_write_cmds.try_dequeue(cmd)) {
    _ioc.post([handler = cmd.handler]() {
      handler(boost::system::errc::make_error_code(boost::system::errc::operation_canceled));
    });
  }

  {
    std::lock_guard guard(_sock_m);
    boost::system::error_code ec;
    _sock.cancel(ec);
    _sock.close(ec);
  }
}

void smpp::socket::open_async(const boost::asio::ip::tcp::endpoint &endpoint,
                              const smpp::socket::open_handler &handler) {
  _sock.async_connect(endpoint, [handler](boost::system::error_code ec) {
    handler(ec);
  });
}

boost::asio::ip::tcp::socket &smpp::socket::handle() {
  return _sock;
}
