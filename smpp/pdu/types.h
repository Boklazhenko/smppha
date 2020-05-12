//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_TYPES_H_
#define SMPPHA_SMPP_PDU_TYPES_H_

#include <string>
#include <vector>
#include "spdlog/fmt/fmt.h"

namespace smpp {

struct mandatory_param_category_tag {};

struct optional_param_category_tag {};

struct c_string_type_category_tag {};

struct fixed_c_string_type_category_tag : c_string_type_category_tag {};

struct octet_string_type_category_tag {};

template<int max_length>
struct c_octet_string {

  using underlying_type = std::string;

  c_octet_string() = default;
  template<typename t>
  explicit c_octet_string(t v)
      : data(std::move(v)) {}

  using type_category_tag = c_string_type_category_tag;
  int max_length_ = max_length;
  std::string data;
};

template<int length>
struct fixed_c_octet_string {

  using underlying_type = std::string;

  fixed_c_octet_string() = default;
  template<typename t>
  explicit fixed_c_octet_string(t v)
      : data(std::move(v)) {}

  using type_category_tag = fixed_c_string_type_category_tag;
  int length_ = length;
  std::string data;
};

template<int max_length>
struct octet_string {

  using underlying_type = std::vector<uint8_t>;

  octet_string() = default;

  explicit octet_string(const std::string &v)
      : data(v.begin(), v.end()) {}

  template<typename t>
  explicit octet_string(const std::vector<t> &v)
      : data(v.begin(), v.end()) {}

  using type_category_tag = octet_string_type_category_tag;
  int max_length_ = max_length;
  std::vector<uint8_t> data;
};

template<typename t>
size_t get_param_size(const t &value, c_string_type_category_tag) {
  return value.data.size() + 1;
}

template<typename t>
size_t get_param_size(const t &value, octet_string_type_category_tag) {
  return value.data.size();
}

template<typename t>
size_t get_param_size(const t &value, std::true_type) {
  return sizeof(t);
}

template<typename t>
size_t get_param_size(const t &value, std::false_type) {
  return get_param_size(value, typename t::type_category_tag());
}

template<typename t>
size_t get_param_size(const t &value) {
  return get_param_size(value, std::is_integral<t>());
}

}

namespace std {

template<int max_length>
std::string to_string(const smpp::c_octet_string<max_length> &v) {
  return v.data;
}

template<int length>
std::string to_string(const smpp::fixed_c_octet_string<length> &v) {
  return v.data;
}

template<int max_length>
std::string to_string(const smpp::octet_string<max_length> &v) {
  return fmt::format("{}", string(v.data.begin(), v.data.end()));
}

}

#endif //SMPPHA_SMPP_PDU_TYPES_H_
