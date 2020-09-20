//
// Created by boklazhenko on 13.05.2020.
//

#include "socket.h"
#include "../errors.h"
#include "../pdu/pdu.h"

smpp::socket::socket(boost::asio::io_context &ioc, boost::asio::ip::tcp::socket &&sock)
    : _ioc(ioc), _sock(std::move(sock)), _writing(false), _reading(false), _read_length_buff(sizeof(uint32_t)) {

}

void smpp::socket::write_async(const std::shared_ptr<i_pdu> &p_pdu, const write_handler &handler) {
  {
    std::lock_guard guard(_cmds_m);
    _write_cmds.push(write_cmd{p_pdu, handler});
  }

  write_next_async();
}

void smpp::socket::read_async(const read_handler &handler) {
  using namespace boost;

  if (_reading.exchange(true)) {
    _ioc.post([s = shared_from_this(), handler]() {
      handler(error::already_reading, nullptr);
    });
    return;
  }

  _read_async(handler);
}

std::shared_ptr<smpp::socket> smpp::socket::create(boost::asio::io_context &ioc,
                                                   boost::asio::ip::tcp::socket &&sock) {
  return std::shared_ptr<socket>(new socket(ioc, std::move(sock)));
}

void smpp::socket::_read_async(const read_handler &handler) {
  using namespace boost;

  std::lock_guard guard(_sock_m);
  _sock.async_receive(asio::buffer(_read_length_buff), asio::ip::tcp::socket::message_peek,
                      [s = shared_from_this(), handler](const system::error_code &ec, size_t size) {
                        if (ec) {
                          s->_reading = false;
                          handler(error::boost, nullptr);
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
                                             handler(error::boost, nullptr);
                                             return;
                                           }

                                           std::shared_ptr<i_pdu> p_pdu;
                                           error e = pdu<>::tryParse(s->_read_buff, p_pdu);
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

  std::unique_lock lock(_cmds_m);
  if (_write_cmds.empty()) {
    _writing = false;
    return;
  }

  write_cmd cmd = _write_cmds.front();
  _write_cmds.pop();

  lock.unlock();

  _write_buff = cmd.p_pdu->serialize();

  std::lock_guard guard(_sock_m);
  asio::async_write(_sock,
                    asio::buffer(_write_buff),
                    [s = shared_from_this(), handler = cmd.handler](const system::error_code &ec, size_t size) {
                      s->_writing = false;
                      s->write_next_async();
                      handler(ec ? error::boost : error::no);
                    });
}

void smpp::socket::close() {
  {
    std::lock_guard guard(_cmds_m);
    while (!_write_cmds.empty()) {
      _ioc.post([handler = _write_cmds.front().handler]() {
        handler(error::aborted);
      });
      _write_cmds.pop();
    }
  }

  {
    std::lock_guard guard(_sock_m);
    _sock.close();
  }
}
