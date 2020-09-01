//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_
#define SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_

#include "../errors.h"
#include <vector>
#include <cstdint>
#include "types.h"
#include "../util.h"
#include "../constants.h"
#include <memory>

namespace smpp {

struct i_optional_param {
  using param_category_tag = optional_param_category_tag;

  virtual ~i_optional_param() = default;
  virtual error deserialize_value(const std::vector<uint8_t> &data) = 0;
  virtual std::vector<uint8_t> serialize() const = 0;
  virtual uint16_t length() const = 0;
  virtual std::string to_string() const = 0;
};

struct unknown_optional_param : i_optional_param {
  explicit unknown_optional_param(uint16_t tag)
      : tag_(tag) {}

  error deserialize_value(const std::vector<uint8_t> &data) override {
    value.data = data;
    return error::no;
  }

  std::vector<uint8_t> serialize() const override {
    binary_writer writer;
    writer << tag_ << static_cast<uint16_t >(get_param_size(value)) << value;
    return writer.data();
  }

  uint16_t length() const override {
    return static_cast<uint16_t>(get_param_size(value));
  }

  std::string to_string() const override {
    return std::to_string(value);
  }

  uint16_t tag_;
  octet_string<std::numeric_limits<uint16_t>::max()> value;
};

template<opt_par_tag tag, typename t>
struct optional_param : i_optional_param {

  using underlying_type = t;

  error deserialize_value(const std::vector<uint8_t> &data) override {
    binary_reader reader(data);
    reader >> value;
    return reader.good() && reader.eof() ? error::no : error::failed_opt_param_deserializing;
  }

  std::vector<uint8_t> serialize() const override {
    binary_writer writer;
    writer << tag_ << static_cast<uint16_t >(get_param_size(value)) << value;
    return writer.data();
  }

  uint16_t length() const override {
    return static_cast<uint16_t>(get_param_size(value));
  }

  std::string to_string() const override {
    return std::to_string(value);
  }

  inline static const uint16_t tag_ = to_integral(tag);
  underlying_type value;
};

struct dest_addr_subunit : optional_param<opt_par_tag::dest_addr_subunit, uint8_t> {};

struct dest_network_type : optional_param<opt_par_tag::dest_network_type, uint8_t> {};

struct source_network_type : optional_param<opt_par_tag::source_network_type, uint8_t> {};

struct dest_bearer_type : optional_param<opt_par_tag::dest_bearer_type, uint8_t> {};

struct source_bearer_type : optional_param<opt_par_tag::source_bearer_type, uint8_t> {};

struct dest_telematics_id : optional_param<opt_par_tag::dest_telematics_id, uint16_t> {};

struct source_addr_subunit : optional_param<opt_par_tag::source_addr_subunit, uint8_t> {};

struct source_telematics_id : optional_param<opt_par_tag::source_telematics_id, uint8_t> {};

struct qos_time_to_live : optional_param<opt_par_tag::qos_time_to_live, uint32_t> {};

struct payload_type : optional_param<opt_par_tag::payload_type, uint8_t> {};

struct additional_status_info_text : optional_param<opt_par_tag::additional_status_info_text, c_octet_string<256>> {};

struct receipted_message_id : optional_param<opt_par_tag::receipted_message_id, c_octet_string<65>> {};

struct ms_msg_wait_facilities : optional_param<opt_par_tag::ms_msg_wait_facilities, uint8_t> {};

struct privacy_indicator : optional_param<opt_par_tag::privacy_indicator, uint8_t> {};

struct source_subaddress : optional_param<opt_par_tag::source_subaddress, octet_string<23>> {};

struct dest_subaddress : optional_param<opt_par_tag::dest_subaddress, octet_string<23>> {};

struct user_message_reference : optional_param<opt_par_tag::user_message_reference, uint16_t> {};

struct user_response_code : optional_param<opt_par_tag::user_response_code, uint8_t> {};

struct source_port : optional_param<opt_par_tag::source_port, uint16_t> {};

struct language_indicator : optional_param<opt_par_tag::language_indicator, uint8_t> {};

struct destination_port : optional_param<opt_par_tag::destination_port, uint16_t> {};

struct sar_msg_ref_num : optional_param<opt_par_tag::sar_msg_ref_num, uint16_t> {};

struct sar_total_segments : optional_param<opt_par_tag::sar_total_segments, uint8_t> {};

struct sar_segment_seqnum : optional_param<opt_par_tag::sar_segment_seqnum, uint8_t> {};

struct sc_interface_version : optional_param<opt_par_tag::sc_interface_version, uint8_t> {};

struct display_time : optional_param<opt_par_tag::display_time, uint8_t> {};

struct ms_validity : optional_param<opt_par_tag::ms_validity, uint8_t> {};

struct dpf_result : optional_param<opt_par_tag::dpf_result, uint8_t> {};

struct set_dpf : optional_param<opt_par_tag::set_dpf, uint8_t> {};

struct ms_availability_status : optional_param<opt_par_tag::ms_availability_status, uint8_t> {};

struct network_error_code : optional_param<opt_par_tag::network_error_code, octet_string<3>> {};

struct message_payload : optional_param<opt_par_tag::message_payload,
                                        octet_string<std::numeric_limits<uint16_t>::max()>> {
};

struct delivery_failure_reason : optional_param<opt_par_tag::delivery_failure_reason, uint8_t> {};

struct more_messages_to_send : optional_param<opt_par_tag::more_messages_to_send, uint8_t> {};

struct message_state : optional_param<opt_par_tag::message_state, uint8_t> {};

struct callback_num : optional_param<opt_par_tag::callback_num, octet_string<19>> {};

struct callback_num_pres_ind : optional_param<opt_par_tag::callback_num_pres_ind, uint8_t> {};

struct callback_num_atag : optional_param<opt_par_tag::callback_num_atag, octet_string<65>> {};

struct number_of_messages : optional_param<opt_par_tag::number_of_messages, uint8_t> {};

struct sms_signal : optional_param<opt_par_tag::sms_signal, uint16_t> {};

struct alert_on_message_delivery : optional_param<opt_par_tag::alert_on_message_delivery, octet_string<0>> {};

struct its_reply_type : optional_param<opt_par_tag::its_reply_type, uint8_t> {};

struct ussd_service_op : optional_param<opt_par_tag::ussd_service_op, uint8_t> {};

struct its_session_info : optional_param<opt_par_tag::its_session_info, uint16_t> {};

inline std::shared_ptr<i_optional_param> create_opt_param_by_tag(opt_par_tag tag) {
  using namespace std;
  switch (tag) {
    case opt_par_tag::receipted_message_id:return make_shared<receipted_message_id>();
    case opt_par_tag::message_state:return make_shared<message_state>();
    case opt_par_tag::network_error_code:return make_shared<network_error_code>();
    case opt_par_tag::message_payload:return make_shared<message_payload>();
    case opt_par_tag::sar_total_segments:return make_shared<sar_total_segments>();
    case opt_par_tag::sar_msg_ref_num:return make_shared<sar_msg_ref_num>();
    case opt_par_tag::sar_segment_seqnum:return make_shared<sar_segment_seqnum>();
    case opt_par_tag::dest_addr_subunit:return make_shared<dest_addr_subunit>();
    case opt_par_tag::dest_network_type:return make_shared<dest_network_type>();
    case opt_par_tag::dest_bearer_type:return make_shared<dest_bearer_type>();
    case opt_par_tag::dest_telematics_id:return make_shared<dest_telematics_id>();
    case opt_par_tag::source_addr_subunit:return make_shared<source_addr_subunit>();
    case opt_par_tag::source_network_type:return make_shared<source_network_type>();
    case opt_par_tag::source_bearer_type:return make_shared<source_bearer_type>();
    case opt_par_tag::source_telematics_id:return make_shared<source_telematics_id>();
    case opt_par_tag::qos_time_to_live:return make_shared<qos_time_to_live>();
    case opt_par_tag::payload_type:return make_shared<payload_type>();
    case opt_par_tag::additional_status_info_text:return make_shared<additional_status_info_text>();
    case opt_par_tag::ms_msg_wait_facilities:return make_shared<ms_msg_wait_facilities>();
    case opt_par_tag::privacy_indicator:return make_shared<privacy_indicator>();
    case opt_par_tag::source_subaddress:return make_shared<source_subaddress>();
    case opt_par_tag::dest_subaddress:return make_shared<dest_subaddress>();
    case opt_par_tag::user_message_reference:return make_shared<user_message_reference>();
    case opt_par_tag::user_response_code:return make_shared<user_response_code>();
    case opt_par_tag::source_port:return make_shared<source_port>();
    case opt_par_tag::destination_port:return make_shared<destination_port>();
    case opt_par_tag::language_indicator:return make_shared<language_indicator>();
    case opt_par_tag::sc_interface_version:return make_shared<sc_interface_version>();
    case opt_par_tag::callback_num_pres_ind:return make_shared<callback_num_pres_ind>();
    case opt_par_tag::callback_num_atag:return make_shared<callback_num_atag>();
    case opt_par_tag::number_of_messages:return make_shared<number_of_messages>();
    case opt_par_tag::callback_num:return make_shared<callback_num>();
    case opt_par_tag::dpf_result:return make_shared<dpf_result>();
    case opt_par_tag::set_dpf:return make_shared<set_dpf>();
    case opt_par_tag::ms_availability_status:return make_shared<ms_availability_status>();
    case opt_par_tag::delivery_failure_reason:return make_shared<delivery_failure_reason>();
    case opt_par_tag::more_messages_to_send:return make_shared<more_messages_to_send>();
    case opt_par_tag::ussd_service_op:return make_shared<ussd_service_op>();
    case opt_par_tag::display_time:return make_shared<display_time>();
    case opt_par_tag::sms_signal:return make_shared<sms_signal>();
    case opt_par_tag::ms_validity:return make_shared<ms_validity>();
    case opt_par_tag::alert_on_message_delivery:return make_shared<alert_on_message_delivery>();
    case opt_par_tag::its_reply_type:return make_shared<its_reply_type>();
    case opt_par_tag::its_session_info:return make_shared<its_session_info>();
  }

  return make_shared<unknown_optional_param>(to_integral(tag));
}

}

#endif //SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_
