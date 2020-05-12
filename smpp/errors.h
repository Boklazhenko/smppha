//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_ERRORS_H_
#define SMPPHA_SMPP_ERRORS_H_

namespace smpp {

enum class error {
  no,
  failed_opt_param_deserializing,
  failed_pdu_deserializing
};

}

#endif //SMPPHA_SMPP_ERRORS_H_
