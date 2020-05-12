//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SRC_UTIL_H_
#define SMPPHA_SRC_UTIL_H_

#include <type_traits>
#include "pdu/types.h"
#include <cmath>

namespace smpp {

template<typename e>
constexpr auto to_integral(e e_) -> typename std::underlying_type<e>::type {
  return static_cast<typename std::underlying_type<e>::type>(e_);
}

class sink {
 public:
  explicit sink(uint32_t bytes_count)
      : _bytes_count(bytes_count) {}

  uint32_t bytes_count() const { return _bytes_count; }

 private:
  uint32_t _bytes_count;
};

class binary_reader {
 public:
  explicit binary_reader(const std::vector<uint8_t> &data);

  binary_reader &operator>>(uint32_t &v);

  binary_reader &operator>>(uint16_t &v);

  binary_reader &operator>>(uint8_t &v);

  binary_reader &operator>>(std::vector<uint8_t> &v);

  template<typename t>
  binary_reader &operator>>(t &v) {
    return read(v, typename t::type_category_tag());
  }

  template<typename c_octet_string_type>
  binary_reader &read(c_octet_string_type &v, c_string_type_category_tag) {
    if (_good) {
      if (std::distance(_curr_pos, _data.cend()) >= 1) {
        auto begin = _curr_pos;
        bool endFound = false;
        while (!endFound && _curr_pos != _data.end())
          endFound = *_curr_pos++ == 0;

        if (endFound && std::distance(begin, _curr_pos) <= v.max_length_)
          v.data.assign(begin, _curr_pos - 1);
        else
          _good = false;
      } else
        _good = false;
    }

    return *this;
  }

  template<typename fixed_c_octet_string_type>
  binary_reader &read(fixed_c_octet_string_type &v, fixed_c_string_type_category_tag) {
    if (_good) {
      if (std::distance(_curr_pos, _data.cend()) >= 1) {
        auto begin = _curr_pos;
        bool endFound = false;
        while (!endFound && _curr_pos != _data.end())
          endFound = *_curr_pos++ == 0;

        if (endFound && (std::distance(begin, _curr_pos) == 1 || std::distance(begin, _curr_pos) == v.length_))
          v.data.assign(begin, _curr_pos - 1);
        else
          _good = false;
      } else
        _good = false;
    }

    return *this;
  }

  template<typename octet_string_type>
  binary_reader &read(octet_string_type &v, octet_string_type_category_tag) {
    v.data.resize(std::min(static_cast<int>(v.data.size()), v.max_length_));
    return *this >> v.data;
  }

  binary_reader &operator>>(const sink &s);

  bool good() const;

  bool eof() const;

  size_t available() const;

 private:
  const std::vector<uint8_t> &_data;
  bool _good;
  std::remove_reference<decltype(_data)>::type::const_iterator _curr_pos;
};

class binary_writer {
 public:
  binary_writer &operator<<(uint8_t v);

  binary_writer &operator<<(uint32_t v);

  binary_writer &operator<<(uint16_t v);

  binary_writer &operator<<(const std::vector<uint8_t> &v);

  template<typename t>
  binary_writer &operator<<(t &v) {
    return write(v, typename t::type_category_tag());
  }

  template<typename octet_string_type>
  binary_writer &write(const octet_string_type &v, octet_string_type_category_tag) {
    int length = std::min(v.max_length_, static_cast<int>(v.data.size()));
    _data.reserve(_data.size() + length);
    _data.insert(_data.end(), v.data.begin(), v.data.begin() + length);
    return *this;
  }

  template<typename c_octet_string_type>
  binary_writer &write(const c_octet_string_type &v, c_string_type_category_tag) {
    int length = std::min(v.max_length_, static_cast<int>(v.data.size()) + 1);
    _data.reserve(_data.size() + length);
    _data.insert(_data.end(), v.data.begin(), v.data.begin() + length - 1);
    _data.push_back(0);
    return *this;
  }

  template<typename fixed_c_octet_string_type>
  binary_writer &write(const fixed_c_octet_string_type &v, fixed_c_string_type_category_tag) {
    if (v.data.size() == v.length_ - 1) {
      _data.reserve(_data.size() + v.length_);
      _data.insert(_data.end(), v.data.begin(), v.data.end());
    }
    _data.push_back(0);
    return *this;
  }

  const std::vector<uint8_t> &data() const;

 private:
  std::vector<uint8_t> _data;
};

inline uint32_t to_uint32(const std::vector<uint8_t> &bytes) {
  uint32_t result = 0;
  unsigned int k = (unsigned int) std::pow(256, (double) bytes.size() - 1);
  for (unsigned int i = 0; i < bytes.size(); ++i) {
    result += bytes[i] * k;
    k /= 256;
  }
  return result;
}

inline std::vector<uint8_t> to_byte_vector(uint64_t number, uint8_t size) {
  std::vector<uint8_t> result;
  result.reserve(size);

  for (int16_t i = size - 1; i >= 0; --i)
    result.push_back((number >> i * 7 % (sizeof(uint64_t) * 8) >> i % (sizeof(uint64_t) * 8)) & 0xFF);

  return result;
}

}

#endif //SMPPHA_SRC_UTIL_H_
