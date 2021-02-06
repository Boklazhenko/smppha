//
// Created by boklazhenko on 06.02.2021.
//

#ifndef SMPPHA_SMPP_PDU_DELIVERY_RECEIPT_INFO_H_
#define SMPPHA_SMPP_PDU_DELIVERY_RECEIPT_INFO_H_

#include <string>

namespace smpp {

enum class delivery_receipt_state : uint8_t;

struct delivery_receipt_info {
  std::string msg_id;
  delivery_receipt_state state;
  uint16_t err;
  std::string text;

  delivery_receipt_info(const std::string &msg_id_,
                        time_t submit_time,
                        time_t done_time,
                        delivery_receipt_state state_,
                        uint16_t err_);
};

}

#endif //SMPPHA_SMPP_PDU_DELIVERY_RECEIPT_INFO_H_
