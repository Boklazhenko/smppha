//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_ERRORS_H_
#define SMPPHA_SMPP_ERRORS_H_

#include <string>

namespace smpp {

enum class error {
  no,
  failed_opt_param_deserializing,
  failed_pdu_deserializing,
  already_writing,
  already_reading,
  boost
};

inline std::string to_string(error err) {
  switch (err) {
    case error::no: return "no";
    case error::failed_pdu_deserializing: return "failed_pdu_deserializing";
    case error::failed_opt_param_deserializing: return "failed_opt_param_deserializing";
    case error::already_writing: return "already_writing";
    case error::already_reading: return "already_reading";
    case error::boost: return "boost";
  }

  return "unknown";
}

}

#endif //SMPPHA_SMPP_ERRORS_H_
