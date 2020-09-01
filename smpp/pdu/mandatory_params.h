//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_MANDATORY_PARAMS_H_
#define SMPPHA_SMPP_PDU_MANDATORY_PARAMS_H_

#include "types.h"
#include "../constants.h"

namespace smpp {

template<typename t, man_par_tag tag>
struct mandatory_param {

  using param_category_tag = mandatory_param_category_tag;
  using underlying_type = t;

  mandatory_param() = default;

  explicit mandatory_param(const underlying_type &defaultValue)
      : value(defaultValue) {}

  virtual ~mandatory_param() = default;

  const man_par_tag tag_ = tag;
  underlying_type value;
};

struct system_id : mandatory_param<c_octet_string<16>, man_par_tag::system_id> {};

struct password : mandatory_param<c_octet_string<9>, man_par_tag::password> {};

struct system_type : mandatory_param<c_octet_string<13>, man_par_tag::system_type> {};

struct interface_version : mandatory_param<uint8_t, man_par_tag::interface_version> {};

struct addr_ton : mandatory_param<uint8_t, man_par_tag::addr_ton> {};

struct addr_npi : mandatory_param<uint8_t, man_par_tag::addr_npi> {};

struct address_range : mandatory_param<c_octet_string<41>, man_par_tag::address_range> {};

struct source_addr_ton : mandatory_param<uint8_t, man_par_tag::source_addr_ton> {};

struct dest_addr_ton : mandatory_param<uint8_t, man_par_tag::dest_addr_ton> {};

struct esme_addr_ton : mandatory_param<uint8_t, man_par_tag::esme_addr_ton> {};

struct source_addr_npi : mandatory_param<uint8_t, man_par_tag::source_addr_npi> {};

struct dest_addr_npi : mandatory_param<uint8_t, man_par_tag::dest_addr_npi> {};

struct esme_addr_npi : mandatory_param<uint8_t, man_par_tag::esme_addr_npi> {};

struct source_addr : mandatory_param<c_octet_string<21>, man_par_tag::source_addr> {};

struct destination_addr : mandatory_param<c_octet_string<21>, man_par_tag::destination_addr> {};

struct esme_addr : mandatory_param<c_octet_string<65>, man_par_tag::esme_addr> {};

struct service_type : mandatory_param<c_octet_string<6>, man_par_tag::service_type> {};

struct esm_class : mandatory_param<uint8_t, man_par_tag::esm_class> {};

struct protocol_id : mandatory_param<uint8_t, man_par_tag::protocol_id> {};

struct priority_flag : mandatory_param<uint8_t, man_par_tag::priority_flag> {};

struct schedule_delivery_time : mandatory_param<fixed_c_octet_string<17>, man_par_tag::schedule_delivery_time> {};

struct validity_period : mandatory_param<fixed_c_octet_string<17>, man_par_tag::validity_period> {};

struct registered_delivery : mandatory_param<uint8_t, man_par_tag::registered_delivery> {};

struct replace_if_present_flag : mandatory_param<uint8_t, man_par_tag::replace_if_present_flag> {};

struct data_coding : mandatory_param<uint8_t, man_par_tag::data_coding> {};

struct sm_default_msg_id : mandatory_param<uint8_t, man_par_tag::sm_default_msg_id> {};

struct sm_length : mandatory_param<uint8_t, man_par_tag::sm_length> {};

struct short_message : mandatory_param<octet_string<254>, man_par_tag::short_message> {};

struct message_id : mandatory_param<c_octet_string<65>, man_par_tag::message_id> {};

struct number_of_dests : mandatory_param<uint8_t, man_par_tag::number_of_dests> {};

struct dest_flag : mandatory_param<uint8_t, man_par_tag::dest_flag> {};

struct no_unsuccess : mandatory_param<uint8_t, man_par_tag::no_unsuccess> {};

struct dl_name : mandatory_param<c_octet_string<21>, man_par_tag::dl_name> {};

struct message_state_m : mandatory_param<uint8_t, man_par_tag::message_state_m> {};

struct final_date : mandatory_param<fixed_c_octet_string<17>, man_par_tag::final_date> {};

struct error_code : mandatory_param<uint8_t, man_par_tag::error_code> {};

}

#endif //SMPPHA_SMPP_PDU_MANDATORY_PARAMS_H_
