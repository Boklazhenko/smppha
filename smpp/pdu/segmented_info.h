//
// Created by boklazhenko on 15.09.2020.
//

#ifndef SMPPHA_SMPP_PDU_SEGMENTED_INFO_H_
#define SMPPHA_SMPP_PDU_SEGMENTED_INFO_H_

#include <cstdint>

namespace smpp {

class submit_sm;

struct segmented_info {
  int total_parts;
  int part;
  uint16_t  id;

  explicit segmented_info(const submit_sm& pdu);
};

}

#endif //SMPPHA_SMPP_PDU_SEGMENTED_INFO_H_
