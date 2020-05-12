//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_MANDATORYPARAMS_H_
#define SMPPHA_SMPP_PDU_MANDATORYPARAMS_H_

#include "types.h"

namespace smpp {

template<typename t>
struct mandatory_param {

  using param_category_tag = mandatory_param_category_tag;
  using underlying_type = t;

  mandatory_param() = default;

  explicit mandatory_param(const underlying_type &defaultValue)
      : value(defaultValue) {}

  virtual ~mandatory_param() = default;

  underlying_type value;
};

struct system_id : mandatory_param<c_octet_string<16>> {};

struct password : mandatory_param<c_octet_string<9>> {};

struct system_type : mandatory_param<c_octet_string<13>> {};

struct interface_version : mandatory_param<uint8_t> {};

struct addr_ton : mandatory_param<uint8_t> {};

struct addr_npi : mandatory_param<uint8_t> {};

struct address_range : mandatory_param<c_octet_string<41>> {};

struct source_addr_ton : mandatory_param<uint8_t> {};

struct dest_addr_ton : mandatory_param<uint8_t> {};

struct esme_addr_ton : mandatory_param<uint8_t> {};

struct source_addr_npi : mandatory_param<uint8_t> {};

struct dest_addr_npi : mandatory_param<uint8_t> {};

struct esme_addr_npi : mandatory_param<uint8_t> {};

struct source_addr : mandatory_param<c_octet_string<21>> {};

struct destination_addr : mandatory_param<c_octet_string<21>> {};

struct esme_addr : mandatory_param<c_octet_string<65>> {};

struct service_type : mandatory_param<c_octet_string<6>> {};

struct esm_class : mandatory_param<uint8_t> {};

struct protocol_id : mandatory_param<uint8_t> {};

struct priority_flag : mandatory_param<uint8_t> {};

struct schedule_delivery_time : mandatory_param<fixed_c_octet_string<17>> {};

struct validity_period : mandatory_param<fixed_c_octet_string<17>> {};

struct registered_delivery : mandatory_param<uint8_t> {};

struct replace_if_present_flag : mandatory_param<uint8_t> {};

struct data_coding : mandatory_param<uint8_t> {};

struct sm_default_msg_id : mandatory_param<uint8_t> {};

struct sm_length : mandatory_param<uint8_t> {};

struct short_message : mandatory_param<octet_string<254>> {};

struct message_id : mandatory_param<c_octet_string<65>> {};

struct number_of_dests : mandatory_param<uint8_t> {};

struct dest_flag : mandatory_param<uint8_t> {};

struct no_unsuccess : mandatory_param<uint8_t> {};

struct dl_name : mandatory_param<c_octet_string<21>> {};

struct message_state_m : mandatory_param<uint8_t> {};

struct final_date : mandatory_param<fixed_c_octet_string<17>> {};

struct error_code : mandatory_param<uint8_t> {};

}

#endif //SMPPHA_SMPP_PDU_MANDATORYPARAMS_H_
