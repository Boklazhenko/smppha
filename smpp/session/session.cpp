//
// Created by boklazhenko on 13.05.2020.
//

#include "session.h"
#include "../errors.h"
#include "../pdu/pdu.h"

smpp::session::session(boost::asio::io_context &ioc, boost::asio::ip::tcp::socket &&sock)
    : _ioc(ioc), _sock(std::move(sock)), _writing(false), _reading(false), _read_length_buff(sizeof(uint32_t)) {

}
void smpp::session::write_async(const std::shared_ptr<i_pdu> &p_pdu, const write_handler &handler) {
  using namespace boost;

  if (_writing) {
    _ioc.post([handler]() {
      handler(error::already_writing);
    });
    return;
  }

  _writing = true;
  _write_buff = p_pdu->serialize();

  std::lock_guard guard(_mutex);
  asio::async_write(_sock,
                    asio::buffer(_write_buff),
                    [s = shared_from_this(), handler](system::error_code ec, size_t size) {
                      s->_writing = false;
                      handler(ec ? error::boost : error::no);
                    });
}

void smpp::session::read_async(const read_handler &handler) {
  using namespace boost;

  if (_reading) {
    _ioc.post([handler]() {
      handler(error::already_reading, nullptr);
    });
    return;
  }

  _reading = true;
  _read_async(handler);
}

std::shared_ptr<smpp::session> smpp::session::create(boost::asio::io_context &ioc,
                                                     boost::asio::ip::tcp::socket &&sock) {
  return std::shared_ptr<session>(new session(ioc, std::move(sock)));
}

void smpp::session::_read_async(const read_handler &handler) {
  using namespace boost;

  std::lock_guard guard(_mutex);
  _sock.async_receive(asio::buffer(_read_length_buff), asio::ip::tcp::socket::message_peek,
                      [s = shared_from_this(), handler](system::error_code ec, size_t size) {
                        if (ec) {
                          handler(error::boost, nullptr);
                          return;
                        }

                        if (size < s->_read_length_buff.size()) {
                          s->_ioc.post([s, handler]() {
                            s->_read_async(handler);
                          });
                          return;
                        }

                        s->_read_buff.resize(to_uint32(s->_read_length_buff));
                        std::lock_guard guard(s->_mutex);
                        asio::async_read(s->_sock,
                                         asio::buffer(s->_read_buff),
                                         [s, handler](system::error_code ec, size_t size) {
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
