//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_
#define SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_

#include <vector>
#include <cstdint>
#include "types.h"
#include "../util.h"
#include "../constants.h"
#include <memory>
#include <boost/system/error_code.hpp>
#include <shared_mutex>

namespace smpp {

struct i_optional_param {
  using param_category_tag = optional_param_category_tag;

  virtual ~i_optional_param() = default;
  virtual boost::system::error_code deserialize_value(const std::vector<uint8_t> &data) = 0;
  virtual std::vector<uint8_t> serialize() const = 0;
  virtual uint16_t length() const = 0;
  virtual std::string to_string() const = 0;
};

struct unknown_optional_param : i_optional_param {
  explicit unknown_optional_param(uint16_t tag)
      : tag_(tag) {}

  boost::system::error_code deserialize_value(const std::vector<uint8_t> &data) override {
    value.data = data;
    return boost::system::error_code();
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

template<uint16_t tag, typename t>
struct optional_param : i_optional_param {

  using underlying_type = t;

  boost::system::error_code deserialize_value(const std::vector<uint8_t> &data) override {
    binary_reader reader(data);

    if constexpr (!std::is_integral<typename std::remove_reference<underlying_type>::type>())
      if constexpr (std::is_same<octet_string_type_category_tag, typename underlying_type::type_category_tag>())
        value.data.resize(data.size());

    reader >> value;
    using namespace boost::system;
    return errc::make_error_code(reader.good() && reader.eof() ? errc::success : errc::protocol_error);
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

  inline static const uint16_t tag_ = tag;
  underlying_type value;
};

template<opt_par_tag tag, typename t>
struct optional_param_helper : optional_param<to_integral(tag), t> {};

struct dest_addr_subunit : optional_param_helper<opt_par_tag::dest_addr_subunit, uint8_t> {};

struct dest_network_type : optional_param_helper<opt_par_tag::dest_network_type, uint8_t> {};

struct source_network_type : optional_param_helper<opt_par_tag::source_network_type, uint8_t> {};

struct dest_bearer_type : optional_param_helper<opt_par_tag::dest_bearer_type, uint8_t> {};

struct source_bearer_type : optional_param_helper<opt_par_tag::source_bearer_type, uint8_t> {};

struct dest_telematics_id : optional_param_helper<opt_par_tag::dest_telematics_id, uint16_t> {};

struct source_addr_subunit : optional_param_helper<opt_par_tag::source_addr_subunit, uint8_t> {};

struct source_telematics_id : optional_param_helper<opt_par_tag::source_telematics_id, uint8_t> {};

struct qos_time_to_live : optional_param_helper<opt_par_tag::qos_time_to_live, uint32_t> {};

struct payload_type : optional_param_helper<opt_par_tag::payload_type, uint8_t> {};

struct additional_status_info_text : optional_param_helper<opt_par_tag::additional_status_info_text,
                                                           c_octet_string < 256>> {
};

struct receipted_message_id : optional_param_helper<opt_par_tag::receipted_message_id, c_octet_string < 65>> {
};

struct ms_msg_wait_facilities : optional_param_helper<opt_par_tag::ms_msg_wait_facilities, uint8_t> {};

struct privacy_indicator : optional_param_helper<opt_par_tag::privacy_indicator, uint8_t> {};

struct source_subaddress : optional_param_helper<opt_par_tag::source_subaddress, octet_string < 23>> {
};

struct dest_subaddress : optional_param_helper<opt_par_tag::dest_subaddress, octet_string < 23>> {
};

struct user_message_reference : optional_param_helper<opt_par_tag::user_message_reference, uint16_t> {};

struct user_response_code : optional_param_helper<opt_par_tag::user_response_code, uint8_t> {};

struct source_port : optional_param_helper<opt_par_tag::source_port, uint16_t> {};

struct language_indicator : optional_param_helper<opt_par_tag::language_indicator, uint8_t> {};

struct destination_port : optional_param_helper<opt_par_tag::destination_port, uint16_t> {};

struct sar_msg_ref_num : optional_param_helper<opt_par_tag::sar_msg_ref_num, uint16_t> {};

struct sar_total_segments : optional_param_helper<opt_par_tag::sar_total_segments, uint8_t> {};

struct sar_segment_seqnum : optional_param_helper<opt_par_tag::sar_segment_seqnum, uint8_t> {};

struct sc_interface_version : optional_param_helper<opt_par_tag::sc_interface_version, uint8_t> {};

struct display_time : optional_param_helper<opt_par_tag::display_time, uint8_t> {};

struct ms_validity : optional_param_helper<opt_par_tag::ms_validity, uint8_t> {};

struct dpf_result : optional_param_helper<opt_par_tag::dpf_result, uint8_t> {};

struct set_dpf : optional_param_helper<opt_par_tag::set_dpf, uint8_t> {};

struct ms_availability_status : optional_param_helper<opt_par_tag::ms_availability_status, uint8_t> {};

struct network_error_code : optional_param_helper<opt_par_tag::network_error_code, octet_string < 3>> {
};

struct message_payload : optional_param_helper<opt_par_tag::message_payload,
                                               octet_string < std::numeric_limits<uint16_t>::max()>> {
};

struct delivery_failure_reason : optional_param_helper<opt_par_tag::delivery_failure_reason, uint8_t> {};

struct more_messages_to_send : optional_param_helper<opt_par_tag::more_messages_to_send, uint8_t> {};

struct message_state : optional_param_helper<opt_par_tag::message_state, uint8_t> {};

struct callback_num : optional_param_helper<opt_par_tag::callback_num, octet_string < 19>> {
};

struct callback_num_pres_ind : optional_param_helper<opt_par_tag::callback_num_pres_ind, uint8_t> {};

struct callback_num_atag : optional_param_helper<opt_par_tag::callback_num_atag, octet_string < 65>> {
};

struct number_of_messages : optional_param_helper<opt_par_tag::number_of_messages, uint8_t> {};

struct sms_signal : optional_param_helper<opt_par_tag::sms_signal, uint16_t> {};

struct alert_on_message_delivery : optional_param_helper<opt_par_tag::alert_on_message_delivery, octet_string < 0>> {
};

struct its_reply_type : optional_param_helper<opt_par_tag::its_reply_type, uint8_t> {};

struct ussd_service_op : optional_param_helper<opt_par_tag::ussd_service_op, uint8_t> {};

struct its_session_info : optional_param_helper<opt_par_tag::its_session_info, uint16_t> {};

class opt_par_factory {
 public:
  opt_par_factory(const opt_par_factory &) = delete;
  opt_par_factory(opt_par_factory &&) = delete;
  opt_par_factory &operator=(const opt_par_factory &) = delete;
  opt_par_factory &operator=(opt_par_factory &&) = delete;

  static opt_par_factory &instance() {
    static opt_par_factory factory;
    return factory;
  }

  template<typename t>
  void add() {
    std::unique_lock lock(_m);
    _creators[t::tag_] = []() {
      return std::make_shared<t>();
    };
  }

  std::shared_ptr<i_optional_param> create(uint16_t tag) {
    std::shared_lock lock(_m);
    if (auto iter = _creators.find(tag); iter != _creators.end()) {
      return iter->second();
    } else {
      return std::make_shared<unknown_optional_param>(tag);
    }
  }

 private:
  opt_par_factory() {
    add<receipted_message_id>();
    add<message_state>();
    add<network_error_code>();
    add<message_payload>();
    add<sar_total_segments>();
    add<sar_msg_ref_num>();
    add<sar_segment_seqnum>();
    add<dest_addr_subunit>();
    add<dest_network_type>();
    add<dest_bearer_type>();
    add<dest_telematics_id>();
    add<source_addr_subunit>();
    add<source_network_type>();
    add<source_bearer_type>();
    add<source_telematics_id>();
    add<qos_time_to_live>();
    add<payload_type>();
    add<additional_status_info_text>();
    add<ms_msg_wait_facilities>();
    add<privacy_indicator>();
    add<source_subaddress>();
    add<dest_subaddress>();
    add<user_message_reference>();
    add<user_response_code>();
    add<source_port>();
    add<destination_port>();
    add<language_indicator>();
    add<sc_interface_version>();
    add<callback_num_pres_ind>();
    add<callback_num_atag>();
    add<number_of_messages>();
    add<callback_num>();
    add<dpf_result>();
    add<set_dpf>();
    add<ms_availability_status>();
    add<delivery_failure_reason>();
    add<more_messages_to_send>();
    add<ussd_service_op>();
    add<display_time>();
    add<sms_signal>();
    add<ms_validity>();
    add<alert_on_message_delivery>();
    add<its_reply_type>();
    add<its_session_info>();
  }
  ~opt_par_factory() = default;

 private:
  std::map<uint16_t, std::function<std::shared_ptr<i_optional_param>()>> _creators;
  std::shared_mutex _m;
};

}

#endif //SMPPHA_SMPP_PDU_OPTIONAL_PARAMS_H_
