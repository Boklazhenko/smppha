//
// Created by boklazhenko on 10.05.2020.
//

#ifndef SMPPHA_SMPP_CONSTANTS_H_
#define SMPPHA_SMPP_CONSTANTS_H_

#include <cstdint>

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

}

#endif //SMPPHA_SMPP_CONSTANTS_H_
