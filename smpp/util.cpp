//
// Created by boklazhenko on 10.05.2020.
//

#include "util.h"

smpp::binary_reader::binary_reader(const std::vector<uint8_t> &data)
    : _data(data), _good(true), _curr_pos(_data.begin()) {}

smpp::binary_reader &smpp::binary_reader::operator>>(uint32_t &v) {
  if (_good) {
    if (std::distance(_curr_pos, _data.cend()) >= sizeof(uint32_t)) {
      v = *_curr_pos++ * std::pow(2, 24);
      v += *_curr_pos++ * std::pow(2, 16);
      v += *_curr_pos++ * std::pow(2, 8);
      v += *_curr_pos++;
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(uint16_t &v) {
  if (_good) {
    if (std::distance(_curr_pos, _data.cend()) >= sizeof(uint16_t)) {
      v = *_curr_pos++ * std::pow(2, 8);
      v += *_curr_pos++;
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(uint8_t &v) {
  if (_good) {
    if (std::distance(_curr_pos, _data.cend()) >= sizeof(uint8_t))
      v = *_curr_pos++;
    else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(std::vector<uint8_t> &v) {
  if (_good) {
    if (std::distance(_curr_pos, _data.cend()) >= v.size()) {
      std::copy(_curr_pos, _curr_pos + v.size(), v.begin());
      std::advance(_curr_pos, v.size());
    } else
      _good = false;
  }

  return *this;
}

smpp::binary_reader &smpp::binary_reader::operator>>(const smpp::sink &s) {
  if (_good) {
    if (std::distance(_curr_pos, _data.cend()) >= s.bytes_count())
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
  return _curr_pos >= _data.end();
}

size_t smpp::binary_reader::available() const {
  return std::distance(_curr_pos, _data.end());
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint8_t v) {
  _data.push_back(v);
  return *this;
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint32_t v) {
  *this << to_byte_vector(v, sizeof(uint32_t));
  return *this;
}

smpp::binary_writer &smpp::binary_writer::operator<<(uint16_t v) {
  *this << to_byte_vector(v, sizeof(uint16_t));
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