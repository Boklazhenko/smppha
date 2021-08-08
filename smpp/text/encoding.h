//
// Created by boklazhenko on 08.08.2021.
//

#ifndef SMPPHA_SMPP_TEXT_ENCODING_H_
#define SMPPHA_SMPP_TEXT_ENCODING_H_

#include <vector>
#include <string>

namespace smpp {

class convert_exception : public std::exception {
 public:
  explicit convert_exception(const std::string& message);

  const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

 private:
  std::string _message;
};

class encoding {
 public:
  virtual ~encoding() = default;

  virtual std::string to_string() const = 0;
};

class utf_8 : public encoding {
 public:
  std::string to_string() const override;
};

class ucs : public encoding {
 public:
  std::string to_string() const override;
};

class latin1 : public encoding {
 public:
  std::string to_string() const override;
};

std::vector<uint8_t> convert(const std::vector<uint8_t> &src, const encoding &from, const encoding &to);

}

#endif //SMPPHA_SMPP_TEXT_ENCODING_H_
