//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_PDU_H_
#define SMPPHA_SMPP_PDU_PDU_H_

#include "../constants.h"
#include "../errors.h"
#include <vector>
#include <map>
#include "optional_params.h"
#include <numeric>
#include <sstream>
#include "mandatory_params.h"
#include "pdu_visitor.h"

namespace smpp {

class i_pdu {
 public:
  virtual ~i_pdu() = default;

  virtual uint32_t cmd_len() const = 0;
  virtual command_id cmd_id() const = 0;
  virtual command_status cmd_status() const = 0;
  virtual uint32_t seq_number() const = 0;

  virtual i_pdu &set_cmd_status(command_status cmd_status) = 0;
  virtual i_pdu &set_seq_number(uint32_t seq_number) = 0;

  virtual error deserialize(const std::vector<uint8_t> &data) = 0;
  virtual std::vector<uint8_t> serialize() const = 0;

  virtual void accept(i_pdu_visitor &visitor) = 0;

  virtual std::string to_string() const = 0;
};

std::shared_ptr<i_pdu> create_default_pdu_by_command_id(command_id);

template<typename... mandatory_param_types>
class pdu : public i_pdu {

  using tuple_type = std::tuple<mandatory_param_types...>;

 public:
  static error tryParse(const std::vector<uint8_t> &data, std::shared_ptr<i_pdu> &p_out_pdu) {
    uint32_t cmd_id;
    binary_reader r(data);
    r >> sink(COMMAND_LENGTH_SIZE) >> cmd_id;

    if (!r.good())
      return error::failed_pdu_deserializing;

    p_out_pdu = create_default_pdu_by_command_id(static_cast<command_id>(cmd_id));

    if (!p_out_pdu)
      return error::failed_pdu_deserializing;

    return p_out_pdu->deserialize(data);
  }

 public:
  uint32_t cmd_len() const override {
    return HEADER_SIZE +
        std::apply([](auto &&... mandatory_params) {
          return (get_param_size(mandatory_params.value) + ... + 0);
        }, _mandatory_params)
        + std::accumulate(_optional_params.begin(), _optional_params.end(), 0, [](size_t size, auto &&opt_param) {
          return size + TAG_TLV_SIZE + LENGTH_TLV_SIZE + opt_param.second->length();
        });
  }

  command_status cmd_status() const override { return _cmd_status; }

  uint32_t seq_number() const override { return _seq_number; }

  pdu<mandatory_param_types...> &set_cmd_status(command_status cmd_status) override {
    _cmd_status = cmd_status;
    return *this;
  }

  pdu<mandatory_param_types...> &set_seq_number(uint32_t seq_number) override {
    _seq_number = seq_number;
    return *this;
  }

  template<typename param_type>
  auto get() const {
    return get<param_type>(typename param_type::param_category_tag());
  }

  template<typename param_type, typename t>
  pdu &set(const t &value) {
    return set<param_type>(value, typename param_type::param_category_tag());
  }

  error deserialize(const std::vector<uint8_t> &data) override {
    binary_reader r(data);

    uint32_t cmd_status;
    r >> sink(COMMAND_LENGTH_SIZE) >> sink(COMMAND_ID_SIZE) >> cmd_status >> _seq_number;
    _cmd_status = static_cast<command_status>(cmd_status);

    if (!r.good())
      return error::failed_pdu_deserializing;

    std::apply([&r, this](auto &&... mandatory_params) {
      auto f = [&r, this](auto &&mandatory_param) {
        r >> mandatory_param.value;

        if constexpr (std::is_same<sm_length, typename std::remove_reference<decltype(mandatory_param)>::type>())
          std::get<short_message>(_mandatory_params).value.data.resize(mandatory_param.value);
      };

      (f(mandatory_params), ...);
    }, _mandatory_params);

    if (!r.good())
      return error::failed_pdu_deserializing;

    while (!r.eof()) {
      uint16_t tag, length;
      r >> tag >> length;
      std::vector<uint8_t> value(length);
      r >> value;

      if (!r.good())
        return error::failed_pdu_deserializing;

      std::shared_ptr<i_optional_param> p_opt_param = create_opt_param_by_tag(static_cast<opt_par_tag>(tag));

      if (!p_opt_param)
        return error::failed_pdu_deserializing;

      if (p_opt_param->deserialize_value(value) != error::no)
        return error::failed_pdu_deserializing;

      _optional_params[tag] = p_opt_param;
    }

    return error::no;
  }

  std::vector<uint8_t> serialize() const override {
    binary_writer w;
    w << cmd_len() << to_integral(cmd_id()) << to_integral(_cmd_status) << _seq_number;

    std::apply([&w](auto &&... args) {
      ((w << args.value), ...);
    }, _mandatory_params);

    for (auto &&p : _optional_params)
      w << p.second->serialize();

    return w.data();
  }

  std::string to_string() const override {
    std::ostringstream sout;
    sout << "HEADER:\n"
         << "\tcommand length: " << cmd_len() << "\n"
         << "\tcommand id: " << smpp::to_string(cmd_id()) << "\n"
         << "\tcommand status: " << smpp::to_string(_cmd_status) << "\n"
         << "\tsequence number: " << _seq_number << "\n";

    sout << "MANDATORY PARAMETERS:\n";
    std::apply([&sout](auto &&... mandatory_params) {
      auto f = [&sout](auto &&mandatory_param) {
        sout << "\t" << smpp::to_string(mandatory_param.tag_) << ": " << std::to_string(mandatory_param.value) << "\n";
      };
      (f(mandatory_params), ...);
    }, _mandatory_params);

    sout << "OPTIONAL PARAMETERS:\n";
    for (auto &&p : _optional_params)
      sout << "\t" << smpp::to_string(static_cast<opt_par_tag>(p.first)) << ": " << p.second->to_string() << "\n";

    return sout.str();
  }

 protected:
  pdu() : _cmd_status(command_status::esme_r_ok), _seq_number(0) {}

  pdu(const pdu &) = default;

  pdu(pdu &&) noexcept = default;

  pdu &operator=(const pdu &) = default;

  pdu &operator=(pdu &&) noexcept = default;

 private:
  template<typename optional_param_type>
  auto get(optional_param_category_tag) const {
    return get<optional_param_type>(optional_param_category_tag(),
                                    std::is_integral<typename optional_param_type::underlying_type>());
  }

  template<typename optional_param_type>
  std::optional<typename optional_param_type::underlying_type> get(optional_param_category_tag, std::true_type) const {
    auto iter = _optional_params.find(optional_param_type::tag_);

    if (iter == _optional_params.end())
      return std::nullopt;

    if (auto p_opt_param = std::dynamic_pointer_cast<optional_param_type>(iter->second))
      return p_opt_param->value;

    return std::nullopt;
  }

  template<typename optional_param_type>
  std::optional<typename optional_param_type::underlying_type::underlying_type> get(optional_param_category_tag,
                                                                                    std::false_type) const {
    auto iter = _optional_params.find(optional_param_type::tag_);

    if (iter == _optional_params.end())
      return std::nullopt;

    if (auto p_opt_param = std::dynamic_pointer_cast<optional_param_type>(iter->second))
      return p_opt_param->value.data;

    return std::nullopt;
  }

  template<typename mandatory_param_type>
  auto get(mandatory_param_category_tag) const {
    return get<mandatory_param_type>(mandatory_param_category_tag(),
                                     std::is_integral<typename mandatory_param_type::underlying_type>());
  }

  template<typename mandatory_param_type>
  auto get(mandatory_param_category_tag, std::true_type) const {
    return std::get<mandatory_param_type>(_mandatory_params).value;
  }

  template<typename mandatory_param_type>
  auto get(mandatory_param_category_tag, std::false_type) const {
    return std::get<mandatory_param_type>(_mandatory_params).value.data;
  }

  template<typename optional_param_type, typename t>
  pdu &set(const t &value, optional_param_category_tag) {
    std::shared_ptr<optional_param_type> p_opt_param = std::make_shared<optional_param_type>();
    p_opt_param->value = static_cast<typename optional_param_type::underlying_type>(value);
    _optional_params[optional_param_type::tag_] = p_opt_param;
    return *this;
  }

  template<typename mandatory_param_type, typename t>
  pdu &set(const t &value, mandatory_param_category_tag) {
    std::get<mandatory_param_type>(_mandatory_params).value =
        static_cast<typename mandatory_param_type::underlying_type>(value);
    return *this;
  }

 public:
  static inline uint8_t COMMAND_LENGTH_SIZE = sizeof(uint32_t);
  static inline uint8_t COMMAND_ID_SIZE = sizeof(uint32_t);
  static inline uint8_t COMMAND_STATUS_SIZE = sizeof(uint32_t);
  static inline uint8_t SEQUENCE_NUMBER_SIZE = sizeof(uint32_t);
  static inline uint8_t HEADER_SIZE =
      COMMAND_LENGTH_SIZE + COMMAND_ID_SIZE + COMMAND_STATUS_SIZE + SEQUENCE_NUMBER_SIZE;
  static inline uint8_t TAG_TLV_SIZE = sizeof(uint16_t);
  static inline uint8_t LENGTH_TLV_SIZE = sizeof(uint16_t);

 private:
  command_status _cmd_status;
  uint32_t _seq_number;
  tuple_type _mandatory_params;
  std::map<uint16_t, std::shared_ptr<i_optional_param>> _optional_params;
};

class bind_transmitter
    : public pdu<system_id,
                 password,
                 system_type,
                 interface_version,
                 addr_ton, addr_npi,
                 address_range> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_transmitter;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class bind_transmitter_resp
    : public pdu<system_id> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_transmitter_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class bind_receiver
    : public pdu<system_id,
                 password,
                 system_type,
                 interface_version,
                 addr_ton,
                 addr_npi,
                 address_range> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_receiver;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class bind_receiver_resp
    : public pdu<system_id> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_receiver_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class bind_transceiver
    : public pdu<system_id,
                 password,
                 system_type,
                 interface_version,
                 addr_ton,
                 addr_npi,
                 address_range> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_transceiver;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class bind_transceiver_resp
    : public pdu<system_id> {
 public:
  command_id cmd_id() const override {
    return command_id::bind_transceiver_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class outbind
    : public pdu<system_id,
                 password> {
 public:
  command_id cmd_id() const override {
    return command_id::outbind;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class unbind
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::unbind;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class unbind_resp
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::unbind_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class generic_nack
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::generic_nack;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class submit_sm
    : public pdu<service_type,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr,
                 dest_addr_ton,
                 dest_addr_npi,
                 destination_addr,
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
                 short_message> {
 public:
  command_id cmd_id() const override {
    return command_id::submit_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class submit_sm_resp
    : public pdu<message_id> {
 public:
  command_id cmd_id() const override {
    return command_id::submit_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class deliver_sm
    : public pdu<service_type,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr,
                 dest_addr_ton,
                 dest_addr_npi,
                 destination_addr,
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
                 short_message> {
 public:
  command_id cmd_id() const override {
    return command_id::deliver_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class deliver_sm_resp
    : public pdu<message_id> {
 public:
  command_id cmd_id() const override {
    return command_id::deliver_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class data_sm
    : public pdu<service_type,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr,
                 dest_addr_ton,
                 dest_addr_npi,
                 destination_addr,
                 esm_class,
                 registered_delivery,
                 data_coding> {
 public:
  command_id cmd_id() const override {
    return command_id::data_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class data_sm_resp
    : public pdu<message_id> {
 public:
  command_id cmd_id() const override {
    return command_id::data_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class query_sm
    : public pdu<message_id,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr> {
 public:
  command_id cmd_id() const override {
    return command_id::query_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class query_sm_resp
    : public pdu<message_id,
                 final_date,
                 message_state_m,
                 error_code> {
 public:
  command_id cmd_id() const override {
    return command_id::query_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class cancel_sm
    : public pdu<service_type,
                 message_id,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr,
                 dest_addr_ton,
                 dest_addr_npi,
                 destination_addr> {
 public:
  command_id cmd_id() const override {
    return command_id::cancel_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class cancel_sm_resp
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::cancel_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class replace_sm
    : public pdu<message_id,
                 source_addr_ton,
                 source_addr_npi,
                 source_addr,
                 schedule_delivery_time,
                 validity_period,
                 registered_delivery,
                 sm_default_msg_id,
                 sm_length,
                 short_message> {
 public:
  command_id cmd_id() const override {
    return command_id::replace_sm;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class replace_sm_resp
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::replace_sm_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class enquire_link
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::enquire_link;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class enquire_link_resp
    : public pdu<> {
 public:
  command_id cmd_id() const override {
    return command_id::enquire_link_resp;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

class alert_notification
    : public pdu<source_addr_ton, source_addr_npi, source_addr, esme_addr_ton, esme_addr_npi, esme_addr> {
 public:
  command_id cmd_id() const override {
    return command_id::alert_notification;
  }

  void accept(i_pdu_visitor &visitor) override {
    visitor.visit(*this);
  }
};

inline std::shared_ptr<i_pdu> create_default_pdu_by_command_id(command_id cmd_id) {
  switch (cmd_id) {
    case command_id::submit_sm:return std::make_shared<submit_sm>();
    case command_id::submit_sm_resp:return std::make_shared<submit_sm_resp>();
    case command_id::deliver_sm:return std::make_shared<deliver_sm>();
    case command_id::deliver_sm_resp:return std::make_shared<deliver_sm_resp>();
    case command_id::generic_nack:return std::make_shared<generic_nack>();
    case command_id::bind_receiver:return std::make_shared<bind_receiver>();
    case command_id::bind_receiver_resp:return std::make_shared<bind_receiver_resp>();
    case command_id::bind_transmitter:return std::make_shared<bind_transmitter>();
    case command_id::bind_transmitter_resp:return std::make_shared<bind_transmitter_resp>();
    case command_id::query_sm:return std::make_shared<query_sm>();
    case command_id::query_sm_resp:return std::make_shared<query_sm_resp>();
    case command_id::unbind:return std::make_shared<unbind>();
    case command_id::unbind_resp:return std::make_shared<unbind_resp>();
    case command_id::replace_sm:return std::make_shared<replace_sm>();
    case command_id::replace_sm_resp:return std::make_shared<replace_sm_resp>();
    case command_id::cancel_sm:return std::make_shared<cancel_sm>();
    case command_id::cancel_sm_resp:return std::make_shared<cancel_sm_resp>();
    case command_id::bind_transceiver:return std::make_shared<bind_transceiver>();
    case command_id::bind_transceiver_resp:return std::make_shared<bind_transceiver_resp>();
    case command_id::outbind:return std::make_shared<outbind>();
    case command_id::enquire_link:return std::make_shared<enquire_link>();
    case command_id::enquire_link_resp:return std::make_shared<enquire_link_resp>();
    case command_id::alert_notification:return std::make_shared<alert_notification>();
    case command_id::data_sm:return std::make_shared<data_sm>();
    case command_id::data_sm_resp:return std::make_shared<data_sm_resp>();
  }

  return nullptr;
}

inline std::string to_string(const i_pdu &pdu) {
  return pdu.to_string();
}

inline std::string to_string(const std::shared_ptr<i_pdu> &pdu) {
  return pdu->to_string();
}

}

#endif //SMPPHA_SMPP_PDU_PDU_H_
