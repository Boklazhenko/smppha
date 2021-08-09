//
// Created by boklazhenko on 08.08.2021.
//

#include "encoding.h"
#include <iconv.h>
#include <memory>

smpp::convert_exception::convert_exception(const std::string &message)
    : _message(message) {}

const char *smpp::convert_exception::what() const noexcept {
  return _message.c_str();
}

std::vector<uint8_t> smpp::convert(const std::vector<uint8_t> &src,
                                   const smpp::encoding &from,
                                   const smpp::encoding &to) {
  iconv_t cd = iconv_open(to.to_string().c_str(), from.to_string().c_str());

  std::shared_ptr<void *> defer = std::shared_ptr<void *>(nullptr, [cd](void *) {
    iconv_close(cd);
  });

  if (cd == (iconv_t) -1) {
    throw convert_exception("iconv_open failed");
  }

  size_t lsrc = src.size();
  size_t ldst = 2 * lsrc;
  std::vector<uint8_t> dst;
  dst.resize(ldst);
  char *psrc = (char *) &src[0];
  char *pdst = (char *) &dst[0];
  iconv(cd, &psrc, &lsrc, &pdst, &ldst);

  if (lsrc) {
    throw convert_exception("iconv failed");
  }

  dst.resize(dst.size() - ldst);
  return dst;
}

std::string smpp::utf_8::to_string() const {
  return "UTF-8";
}

uint8_t smpp::utf_8::to_uint8() const {
  return 0x00;
}

smpp::utf_8::operator uint8_t() const {
  return this->to_uint8();
}

std::string smpp::ucs::to_string() const {
  return "UCS-2BE";
}

uint8_t smpp::ucs::to_uint8() const {
  return 0x08;
}

smpp::ucs::operator uint8_t() const {
  return this->to_uint8();
}

std::string smpp::latin1::to_string() const {
  return "ISO8859-1";
}

uint8_t smpp::latin1::to_uint8() const {
  return 0x03;
}

smpp::latin1::operator uint8_t() const {
  return this->to_uint8();
}
