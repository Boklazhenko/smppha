//
// Created by boklazhenko on 10.05.2020.
//

#include "util.h"
#include <arpa/inet.h>

smpp::binary_reader::binary_reader(const std::vector<uint8_t> &data)
    : _data(data), _good(true), _curr_pos(_data.data()), _end(_data.data() + _data.size()) {}

smpp::binary_reader &smpp::binary_reader::operator>>(uint32_t &v) {
  if (_good) {
    if (available() >= sizeof(uint32_t)) {
      v = ntohl(*reinterpret_cast<const uint32_t *>(_curr_pos));
      std::advance(_curr_pos, sizeof(uint32_t));
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(uint16_t &v) {
  if (_good) {
    if (available() >= sizeof(uint16_t)) {
      v = ntohs(*reinterpret_cast<const uint16_t *>(_curr_pos));
      std::advance(_curr_pos, sizeof(uint16_t));
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(uint8_t &v) {
  if (_good) {
    if (available() >= sizeof(uint8_t))
      v = *_curr_pos++;
    else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(std::vector<uint8_t> &v) {
  if (_good) {
    if (available() >= v.size()) {
      std::copy(_curr_pos, _curr_pos + v.size(), v.begin());
      std::advance(_curr_pos, v.size());
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(const smpp::sink &s) {
  if (_good) {
    if (available() >= s.bytes_count())
      std::advance(_curr_pos, s.bytes_count());
    else
      _good = false;
  }

  return *this;
}

bool smpp::binary_reader::good() const {
  return _good;
}

bool smpp::binary_reader::eof() const {
  return _curr_pos >= _end;
}

size_t smpp::binary_reader::available() const {
  return std::distance(_curr_pos, _end);
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint8_t v) {
  _data.push_back(v);
  return *this;
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint32_t v) {
  v = htonl(v);
  _data.insert(_data.end(), reinterpret_cast<char *>(&v), reinterpret_cast<char *>(&v) + sizeof(v));
  return *this;
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint16_t v) {
  v = htons(v);
  _data.insert(_data.end(), reinterpret_cast<char *>(&v), reinterpret_cast<char *>(&v) + sizeof(v));
  return *this;
}

smpp::binary_writer &smpp::binary_writer::operator<<(const std::vector<uint8_t> &v) {
  _data.reserve(_data.size() + v.size());
  _data.insert(_data.end(), v.begin(), v.end());
  return *this;
}

const std::vector<uint8_t> &smpp::binary_writer::data() const {
  return _data;
}