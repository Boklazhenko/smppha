//
// Created by boklazhenko on 06.02.2021.
//

#include "delivery_receipt_info.h"
#include "spdlog/fmt/fmt.h"
#include "../constants.h"

long long YYMMDDhhmm(time_t t) {
  struct tm ttt;
  localtime_r(&t, &ttt);
  return (ttt.tm_year % 100) * 100000000LL + (1 + ttt.tm_mon) * 1000000LL + ttt.tm_mday * 10000LL +
      ttt.tm_hour * 100LL + ttt.tm_min;
}

smpp::delivery_receipt_info::delivery_receipt_info(const std::string &msg_id_,
                                                   time_t submit_time,
                                                   time_t done_time,
                                                   delivery_receipt_state state_,
                                                   uint16_t err_)
    : msg_id(msg_id_),
      state(state_),
      err(err_),
      text(fmt::format("id:{} sub:001 dlvrd:00{} submit date:{:0>10} done date:{:0>10} stat:{} err:{:0>3}",
                       msg_id,
                       static_cast<int>((state == delivery_receipt_state::delivered)),
                       YYMMDDhhmm(submit_time),
                       YYMMDDhhmm(done_time),
                       to_string(state),
                       err % 1000)) {

}
