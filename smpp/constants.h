//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SMPP_CONSTANTS_H_
#define SMPPHA_SMPP_CONSTANTS_H_

#include <cstdint>
#include <string>
#include "util.h"

namespace smpp {

enum class command_id : uint32_t {
  generic_nack = 0x80000000,
  bind_receiver = 0x00000001,
  bind_receiver_resp = 0x80000001,
  bind_transmitter = 0x00000002,
  bind_transmitter_resp = 0x80000002,
  query_sm = 0x00000003,
  query_sm_resp = 0x80000003,
  submit_sm = 0x00000004,
  submit_sm_resp = 0x80000004,
  deliver_sm = 0x00000005,
  deliver_sm_resp = 0x80000005,
  unbind = 0x00000006,
  unbind_resp = 0x80000006,
  replace_sm = 0x00000007,
  replace_sm_resp = 0x80000007,
  cancel_sm = 0x00000008,
  cancel_sm_resp = 0x80000008,
  bind_transceiver = 0x00000009,
  bind_transceiver_resp = 0x80000009,
  outbind = 0x0000000B,
  enquire_link = 0x00000015,
  enquire_link_resp = 0x80000015,
  submit_multi = 0x00000021,
  submit_multi_resp = 0x80000021,
  alert_notification = 0x00000102,
  data_sm = 0x00000103,
  data_sm_resp = 0x80000103
};

enum class command_status : uint32_t {
  esme_r_ok = 0x00000000, // No Error
  esme_r_inv_msg_len = 0x00000001,  // Message Length is invalid
  esme_r_inv_cmd_len = 0x00000002,  // Command Length is invalid
  esme_r_inv_cmd_id = 0x00000003,  // Invalid Command ID
  esme_r_inv_bnd_sts = 0x00000004,  // Incorrect BIND Status for given command
  esme_r_aly_bnd = 0x00000005,  // ESME Already in Bound State
  esme_r_inv_prt_flg = 0x00000006,  // Invalid Priority Flag
  esme_r_inv_reg_dlv_flg = 0x00000007,  // Invalid Registered Delivery Flag
  esme_r_sys_err = 0x00000008,  // System Error
  esme_r_inv_src_adr = 0x0000000A,  // Invalid Source Address
  esme_r_inv_dst_adr = 0x0000000B,  // Invalid Dest Addr
  esme_r_inv_msg_id = 0x0000000C,  // Message ID is invalid
  esme_r_bind_fail = 0x0000000D,  // Bind Failed
  esme_r_inv_paswd = 0x0000000E,  // Invalid Password
  esme_r_inv_sys_id = 0x0000000F,  // Invalid System ID
  esme_r_cancel_fail = 0x00000011,  // Cancel SM Failed
  esme_r_replace_fail = 0x00000013,  // Replace SM Failed
  esme_r_msg_q_ful = 0x00000014,  // Message Queue Full
  esme_r_inv_ser_typ = 0x00000015,  // Invalid Service Type
  esme_r_inv_num_dests = 0x00000033,  // Invalid number of destinations
  esme_r_inv_dl_name = 0x00000034,  // Invalid Distribution List name
  esme_r_inv_dest_flag = 0x00000040,  // Destination flag (submit_multi)
  esme_r_inv_sub_rep =
  0x00000042,  // Invalid ‘submit with replace’ request (i.e. submit_sm with replace_if_present_flag set)
  esme_r_inv_esm_submit = 0x00000043,  // Invalid esm_SUBMIT field data
  esme_r_cnt_sub_dl = 0x00000044,  // Cannot Submit to Distribution List
  esme_r_submit_fail = 0x00000045,  // submit_sm or submit_multi failed
  esme_r_inv_src_ton = 0x00000048,  // Invalid Source address TON
  esme_r_inv_src_npi = 0x00000049,  // Invalid Source address NPI
  esme_r_inv_dst_ton = 0x00000050,  // Invalid Destination address TON
  esme_r_inv_dst_npi = 0x00000051,  // Invalid Destination address NPI
  esme_r_inv_sys_typ = 0x00000053,  // Invalid system_type field
  esme_r_inv_rep_flag = 0x00000054,  // Invalid replace_if_present flag
  esme_r_inv_num_msgs = 0x00000055,  // Invalid number of messages
  esme_r_throttled = 0x00000058,  // Throttling error (ESME has exceeded allowed message limits)
  esme_r_inv_sched = 0x00000061,  // Invalid Scheduled Delivery Time
  esme_r_inv_expiry = 0x00000062,  // Invalid message (Expiry time)
  esme_r_inv_dft_msg_id = 0x00000063,  // Predefined Message Invalid or Not Found
  esme_rx_t_appn = 0x00000064,  // ESME Receiver Temporary App Error Code
  esme_rx_p_appn = 0x00000065,  // ESME Receiver Permanent App Error Code
  esme_rx_r_appn = 0x00000066,  // ESME Receiver Reject Message Error Code
  esme_r_query_fail = 0x00000067,  // query_sm request failed
  esme_r_inv_opt_par_stream = 0x000000C0,  // Error in the optional part of the PDU Body.
  esme_r_opt_par_not_allwd = 0x000000C1,  // Optional Parameter not allowed
  esme_r_inv_par_len = 0x000000C2,  // Invalid Parameter Length.
  esme_r_missing_opt_param = 0x000000C3,  // Expected Optional Parameter missing
  esme_r_inv_opt_param_val = 0x000000C4,  // Invalid Optional Parameter Value
  esme_r_delivery_failure = 0x000000FE,  // Delivery Failure (data_sm_resp)
  esme_r_unknown_err = 0x000000FF  // Unknown Error
};

enum class opt_par_tag : uint16_t {
  dest_addr_subunit = 0x0005,
  dest_network_type = 0x0006,
  dest_bearer_type = 0x0007,
  dest_telematics_id = 0x0008,
  source_addr_subunit = 0x000D,
  source_network_type = 0x000E,
  source_bearer_type = 0x000F,
  source_telematics_id = 0x0010,
  qos_time_to_live = 0x0017,
  payload_type = 0x0019,
  additional_status_info_text = 0x001D,
  receipted_message_id = 0x001E,
  ms_msg_wait_facilities = 0x0030,
  privacy_indicator = 0x0201,
  source_subaddress = 0x0202,
  dest_subaddress = 0x0203,
  user_message_reference = 0x0204,
  user_response_code = 0x0205,
  source_port = 0x020A,
  destination_port = 0x020B,
  sar_msg_ref_num = 0x020C,
  language_indicator = 0x020D,
  sar_total_segments = 0x020E,
  sar_segment_seqnum = 0x020F,
  sc_interface_version = 0x0210,
  callback_num_pres_ind = 0x0302,
  callback_num_atag = 0x0303,
  number_of_messages = 0x0304,
  callback_num = 0x0381,
  dpf_result = 0x0420,
  set_dpf = 0x0421,
  ms_availability_status = 0x0422,
  network_error_code = 0x0423,
  message_payload = 0x0424,
  delivery_failure_reason = 0x0425,
  more_messages_to_send = 0x0426,
  message_state = 0x0427,
  ussd_service_op = 0x0501,
  display_time = 0x1201,
  sms_signal = 0x1203,
  ms_validity = 0x1204,
  alert_on_message_delivery = 0x130C,
  its_reply_type = 0x1380,
  its_session_info = 0x1383,
};

enum class man_par_tag {
  system_id,
  password,
  system_type,
  interface_version,
  addr_ton,
  addr_npi,
  address_range,
  source_addr_ton,
  dest_addr_ton,
  esme_addr_ton,
  source_addr_npi,
  dest_addr_npi,
  esme_addr_npi,
  source_addr,
  destination_addr,
  esme_addr,
  service_type,
  esm_class,
  protocol_id,
  priority_flag,
  schedule_delivery_time,
  validity_period,
  registered_delivery,
  replace_if_present_flag,
  data_coding,
  sm_default_msg_id,
  sm_length,
  short_message,
  message_id,
  number_of_dests,
  dest_flag,
  no_unsuccess,
  dl_name,
  message_state_m,
  final_date,
  error_code
};

inline std::string to_string(command_id cmd_id) {
  switch (cmd_id) {
    case command_id::generic_nack : return "generic nack";
    case command_id::bind_receiver: return "bind receiver";
    case command_id::bind_receiver_resp: return "bind receiver resp";
    case command_id::bind_transmitter: return "bind transmitter";
    case command_id::bind_transmitter_resp: return "bind transmitter resp";
    case command_id::query_sm: return "query sm";
    case command_id::query_sm_resp: return "query sm resp";
    case command_id::submit_sm: return "submit sm";
    case command_id::submit_sm_resp: return "submit sm resp";
    case command_id::deliver_sm: return "deliver sm";
    case command_id::deliver_sm_resp: return "deliver sm resp";
    case command_id::unbind: return "unbind";
    case command_id::unbind_resp: return "unbind resp";
    case command_id::replace_sm: return "replace sm";
    case command_id::replace_sm_resp: return "replace sm resp";
    case command_id::cancel_sm: return "cancel sm";
    case command_id::cancel_sm_resp: return "cancel sm resp";
    case command_id::bind_transceiver: return "bind transceiver";
    case command_id::bind_transceiver_resp: return "bind transceiver resp";
    case command_id::outbind: return "outbind";
    case command_id::enquire_link: return "enquire link";
    case command_id::enquire_link_resp: return "enquire link resp";
    case command_id::submit_multi: return "submit multi";
    case command_id::submit_multi_resp: return "submit multi resp";
    case command_id::alert_notification: return "alert notification";
    case command_id::data_sm: return "data sm";
    case command_id::data_sm_resp: return "data sm resp";
  }

  return std::to_string(to_integral(cmd_id));
}

inline std::string to_string(command_status cmd_status) {
  switch (cmd_status) {
    case command_status::esme_r_ok: return "no error";
    case command_status::esme_r_inv_msg_len: return "message length is invalid";
    case command_status::esme_r_inv_cmd_len: return "command length is invalid";
    case command_status::esme_r_inv_cmd_id: return "invalid command id";
    case command_status::esme_r_inv_bnd_sts: return "incorrect bind status for given command";
    case command_status::esme_r_aly_bnd: return "esme already in bound state";
    case command_status::esme_r_inv_prt_flg: return "invalid priority flag";
    case command_status::esme_r_inv_reg_dlv_flg: return "invalid registered delivery flag";
    case command_status::esme_r_sys_err: return "system error";
    case command_status::esme_r_inv_src_adr: return "invalid source address";
    case command_status::esme_r_inv_dst_adr: return "invalid dest addr";
    case command_status::esme_r_inv_msg_id: return "message id is invalid";
    case command_status::esme_r_bind_fail: return "bind failed";
    case command_status::esme_r_inv_paswd: return "invalid password";
    case command_status::esme_r_inv_sys_id: return "invalid system id";
    case command_status::esme_r_cancel_fail: return "cancel sm failed";
    case command_status::esme_r_replace_fail: return "replace sm failed";
    case command_status::esme_r_msg_q_ful: return "message queue full";
    case command_status::esme_r_inv_ser_typ: return "invalid service type";
    case command_status::esme_r_inv_num_dests: return "invalid number of destinations";
    case command_status::esme_r_inv_dl_name: return "invalid distribution list name";
    case command_status::esme_r_inv_dest_flag: return "destination flag (submit_multi)";
    case command_status::esme_r_inv_sub_rep: return "invalid ‘submit with replace’ request (i.e. submit_sm with replace_if_present_flag set)";
    case command_status::esme_r_inv_esm_submit: return "invalid esm_submit field data";
    case command_status::esme_r_cnt_sub_dl: return "cannot submit to distribution list";
    case command_status::esme_r_submit_fail: return "submit_sm or submit_multi failed";
    case command_status::esme_r_inv_src_ton: return "invalid source address TON";
    case command_status::esme_r_inv_src_npi: return "invalid source address NPI";
    case command_status::esme_r_inv_dst_ton: return "invalid destination address TON";
    case command_status::esme_r_inv_dst_npi: return "invalid destination address NPI";
    case command_status::esme_r_inv_sys_typ: return "invalid system_type field";
    case command_status::esme_r_inv_rep_flag: return "invalid replace_if_present flag";
    case command_status::esme_r_inv_num_msgs: return "invalid number of messages";
    case command_status::esme_r_throttled: return "throttling error (ESME has exceeded allowed message limits)";
    case command_status::esme_r_inv_sched: return "invalid scheduled delivery time";
    case command_status::esme_r_inv_expiry: return "invalid message (expiry time)";
    case command_status::esme_r_inv_dft_msg_id: return "predefined message invalid or not found";
    case command_status::esme_rx_t_appn: return "ESME receiver temporary app error code";
    case command_status::esme_rx_p_appn: return "ESME receiver permanent app error code";
    case command_status::esme_rx_r_appn: return "ESME receiver reject message error code";
    case command_status::esme_r_query_fail: return "query_sm request failed";
    case command_status::esme_r_inv_opt_par_stream: return "error in the optional part of the PDU body";
    case command_status::esme_r_opt_par_not_allwd: return "optional parameter not allowed";
    case command_status::esme_r_inv_par_len: return "invalid parameter length";
    case command_status::esme_r_missing_opt_param: return "expected optional parameter missing";
    case command_status::esme_r_inv_opt_param_val: return "invalid optional parameter value";
    case command_status::esme_r_delivery_failure: return "delivery failure (data_sm_resp)";
    case command_status::esme_r_unknown_err: return "unknown error";
  }

  return std::to_string(to_integral(cmd_status));
}

inline std::string to_string(opt_par_tag tag) {
  switch (tag) {
    case opt_par_tag::dest_addr_subunit: return "dest_addr_subunit";
    case opt_par_tag::dest_network_type: return "dest_network_type";
    case opt_par_tag::dest_bearer_type: return "dest_bearer_type";
    case opt_par_tag::dest_telematics_id: return "dest_telematics_id";
    case opt_par_tag::source_addr_subunit: return "source_addr_subunit";
    case opt_par_tag::source_network_type: return "source_network_type";
    case opt_par_tag::source_bearer_type: return "source_bearer_type";
    case opt_par_tag::source_telematics_id: return "source_telematics_id";
    case opt_par_tag::qos_time_to_live: return "qos_time_to_live";
    case opt_par_tag::payload_type: return "payload_type";
    case opt_par_tag::additional_status_info_text: return "additional_status_info_text";
    case opt_par_tag::receipted_message_id: return "receipted_message_id";
    case opt_par_tag::ms_msg_wait_facilities: return "ms_msg_wait_facilities";
    case opt_par_tag::privacy_indicator: return "privacy_indicator";
    case opt_par_tag::source_subaddress: return "source_subaddress";
    case opt_par_tag::dest_subaddress: return "dest_subaddress";
    case opt_par_tag::user_message_reference: return "user_message_reference";
    case opt_par_tag::user_response_code: return "user_response_code";
    case opt_par_tag::source_port: return "source_port";
    case opt_par_tag::destination_port: return "destination_port";
    case opt_par_tag::sar_msg_ref_num: return "sar_msg_ref_num";
    case opt_par_tag::language_indicator: return "language_indicator";
    case opt_par_tag::sar_total_segments: return "sar_total_segments";
    case opt_par_tag::sar_segment_seqnum: return "sar_segment_seqnum";
    case opt_par_tag::sc_interface_version: return "sc_interface_version";
    case opt_par_tag::callback_num_pres_ind: return "callback_num_pres_ind";
    case opt_par_tag::callback_num_atag: return "callback_num_atag";
    case opt_par_tag::number_of_messages: return "number_of_messages";
    case opt_par_tag::callback_num: return "callback_num";
    case opt_par_tag::dpf_result: return "dpf_result";
    case opt_par_tag::set_dpf: return "set_dpf";
    case opt_par_tag::ms_availability_status: return "ms_availability_status";
    case opt_par_tag::network_error_code: return "network_error_code";
    case opt_par_tag::message_payload: return "message_payload";
    case opt_par_tag::delivery_failure_reason: return "delivery_failure_reason";
    case opt_par_tag::more_messages_to_send: return "more_messages_to_send";
    case opt_par_tag::message_state: return "message_state";
    case opt_par_tag::ussd_service_op: return "ussd_service_op";
    case opt_par_tag::display_time: return "display_time";
    case opt_par_tag::sms_signal: return "sms_signal";
    case opt_par_tag::ms_validity: return "ms_validity";
    case opt_par_tag::alert_on_message_delivery: return "alert_on_message_delivery";
    case opt_par_tag::its_reply_type: return "its_reply_type";
    case opt_par_tag::its_session_info: return "its_session_info";
  }

  return std::to_string(to_integral(tag));
}

inline std::string to_string(man_par_tag tag) {
  switch (tag) {
    case man_par_tag::system_id: return "system_id";
    case man_par_tag::password: return "password";
    case man_par_tag::system_type: return "system_type";
    case man_par_tag::interface_version : return "interface_version";
    case man_par_tag::addr_ton: return "addr_ton";
    case man_par_tag::addr_npi: return "addr_npi";
    case man_par_tag::address_range: return "address_range";
    case man_par_tag::source_addr_ton: return "source_addr_ton";
    case man_par_tag::dest_addr_ton: return "dest_addr_ton";
    case man_par_tag::esme_addr_ton: return "esme_addr_ton";
    case man_par_tag::source_addr_npi: return "source_addr_npi";
    case man_par_tag::dest_addr_npi: return "dest_addr_npi";
    case man_par_tag::esme_addr_npi: return "esme_addr_npi";
    case man_par_tag::source_addr: return "source_addr";
    case man_par_tag::destination_addr: return "destination_addr";
    case man_par_tag::esme_addr: return "esme_addr";
    case man_par_tag::service_type: return "service_type";
    case man_par_tag::esm_class: return "esm_class";
    case man_par_tag::protocol_id: return "protocol_id";
    case man_par_tag::priority_flag: return "priority_flag";
    case man_par_tag::schedule_delivery_time: return "schedule_delivery_time";
    case man_par_tag::validity_period: return "validity_period";
    case man_par_tag::registered_delivery: return "registered_delivery";
    case man_par_tag::replace_if_present_flag: return "replace_if_present_flag";
    case man_par_tag::data_coding: return "data_coding";
    case man_par_tag::sm_default_msg_id: return "sm_default_msg_id";
    case man_par_tag::sm_length: return "sm_length";
    case man_par_tag::short_message: return "short_message";
    case man_par_tag::message_id: return "message_id";
    case man_par_tag::number_of_dests: return "number_of_dests";
    case man_par_tag::dest_flag: return "dest_flag";
    case man_par_tag::no_unsuccess: return "no_unsuccess";
    case man_par_tag::dl_name: return "dl_name";
    case man_par_tag::message_state_m: return "message_state_m";
    case man_par_tag::final_date: return "final_date";
    case man_par_tag::error_code: return "error_code";
  }

  return std::to_string(to_integral(tag));
}

}

#endif //SMPPHA_SMPP_CONSTANTS_H_
