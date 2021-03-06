//
// Created by boklazhenko on 11.05.2020.
//

#ifndef SMPPHA_SMPP_PDU_PDU_VISITOR_H_
#define SMPPHA_SMPP_PDU_PDU_VISITOR_H_

#include <memory>

namespace smpp {

class bind_transmitter;
class bind_transmitter_resp;
class bind_receiver;
class bind_receiver_resp;
class bind_transceiver;
class bind_transceiver_resp;
class outbind;
class unbind;
class unbind_resp;
class generic_nack;
class submit_sm;
class submit_sm_resp;
class deliver_sm;
class deliver_sm_resp;
class data_sm;
class data_sm_resp;
class query_sm;
class query_sm_resp;
class cancel_sm;
class cancel_sm_resp;
class replace_sm;
class replace_sm_resp;
class enquire_link;
class enquire_link_resp;
class alert_notification;

class i_pdu_visitor {
 public:
  virtual ~i_pdu_visitor() = default;

  virtual void visit(bind_transmitter &) = 0;

  virtual void visit(bind_transmitter_resp &) = 0;

  virtual void visit(bind_receiver &) = 0;

  virtual void visit(bind_receiver_resp &) = 0;

  virtual void visit(bind_transceiver &) = 0;

  virtual void visit(bind_transceiver_resp &) = 0;

  virtual void visit(outbind &) = 0;

  virtual void visit(unbind &) = 0;

  virtual void visit(unbind_resp &) = 0;

  virtual void visit(generic_nack &) = 0;

  virtual void visit(submit_sm &) = 0;

  virtual void visit(submit_sm_resp &) = 0;

  virtual void visit(deliver_sm &) = 0;

  virtual void visit(deliver_sm_resp &) = 0;

  virtual void visit(data_sm &) = 0;

  virtual void visit(data_sm_resp &) = 0;

  virtual void visit(query_sm &) = 0;

  virtual void visit(query_sm_resp &) = 0;

  virtual void visit(cancel_sm &) = 0;

  virtual void visit(cancel_sm_resp &) = 0;

  virtual void visit(replace_sm &) = 0;

  virtual void visit(replace_sm_resp &) = 0;

  virtual void visit(enquire_link &) = 0;

  virtual void visit(enquire_link_resp &) = 0;

  virtual void visit(alert_notification &) = 0;
};

class i_shared_ptr_pdu_visitor {
 public:
  virtual ~i_shared_ptr_pdu_visitor() = default;

  virtual void visit(const std::shared_ptr<bind_transmitter> &) = 0;

  virtual void visit(const std::shared_ptr<bind_transmitter_resp> &) = 0;

  virtual void visit(const std::shared_ptr<bind_receiver> &) = 0;

  virtual void visit(const std::shared_ptr<bind_receiver_resp> &) = 0;

  virtual void visit(const std::shared_ptr<bind_transceiver> &) = 0;

  virtual void visit(const std::shared_ptr<bind_transceiver_resp> &) = 0;

  virtual void visit(const std::shared_ptr<outbind> &) = 0;

  virtual void visit(const std::shared_ptr<unbind> &) = 0;

  virtual void visit(const std::shared_ptr<unbind_resp> &) = 0;

  virtual void visit(const std::shared_ptr<generic_nack> &) = 0;

  virtual void visit(const std::shared_ptr<submit_sm> &) = 0;

  virtual void visit(const std::shared_ptr<submit_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<deliver_sm> &) = 0;

  virtual void visit(const std::shared_ptr<deliver_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<data_sm> &) = 0;

  virtual void visit(const std::shared_ptr<data_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<query_sm> &) = 0;

  virtual void visit(const std::shared_ptr<query_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<cancel_sm> &) = 0;

  virtual void visit(const std::shared_ptr<cancel_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<replace_sm> &) = 0;

  virtual void visit(const std::shared_ptr<replace_sm_resp> &) = 0;

  virtual void visit(const std::shared_ptr<enquire_link> &) = 0;

  virtual void visit(const std::shared_ptr<enquire_link_resp> &) = 0;

  virtual void visit(const std::shared_ptr<alert_notification> &) = 0;
};

class dummy_pdu_visitor : public i_pdu_visitor {
 public:
  void visit(bind_transmitter &pdu) override {
    //do nothing
  }

  void visit(bind_transmitter_resp &pdu) override {
    //do nothing
  }

  void visit(bind_receiver &pdu) override {
    //do nothing
  }

  void visit(bind_receiver_resp &pdu) override {
    //do nothing
  }

  void visit(bind_transceiver &pdu) override {
    //do nothing
  }

  void visit(bind_transceiver_resp &pdu) override {
    //do nothing
  }

  void visit(outbind &pdu) override {
    //do nothing
  }

  void visit(unbind &pdu) override {
    //do nothing
  }

  void visit(unbind_resp &pdu) override {
    //do nothing
  }

  void visit(generic_nack &pdu) override {
    //do nothing
  }

  void visit(submit_sm &pdu) override {
    //do nothing
  }

  void visit(submit_sm_resp &pdu) override {
    //do nothing
  }

  void visit(deliver_sm &pdu) override {
    //do nothing
  }

  void visit(deliver_sm_resp &pdu) override {
    //do nothing
  }

  void visit(data_sm &pdu) override {
    //do nothing
  }

  void visit(data_sm_resp &pdu) override {
    //do nothing
  }

  void visit(query_sm &pdu) override {
    //do nothing
  }

  void visit(query_sm_resp &pdu) override {
    //do nothing
  }

  void visit(cancel_sm &pdu) override {
    //do nothing
  }

  void visit(cancel_sm_resp &pdu) override {
    //do nothing
  }

  void visit(replace_sm &pdu) override {
    //do nothing
  }

  void visit(replace_sm_resp &pdu) override {
    //do nothing
  }

  void visit(enquire_link &pdu) override {
    //do nothing
  }

  void visit(enquire_link_resp &pdu) override {
    //do nothing
  }

  void visit(alert_notification &pdu) override {
    //do nothing
  }

};

class dummy_shared_ptr_pdu_visitor : public i_shared_ptr_pdu_visitor {
 public:
  void visit(const std::shared_ptr<bind_transmitter> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<bind_transmitter_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<bind_receiver> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<bind_receiver_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<bind_transceiver> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<bind_transceiver_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<outbind> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<unbind> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<unbind_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<generic_nack> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<submit_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<submit_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<deliver_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<deliver_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<data_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<data_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<query_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<query_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<cancel_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<cancel_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<replace_sm> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<replace_sm_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<enquire_link> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<enquire_link_resp> &ptr) override {
    //do nothing
  }

  void visit(const std::shared_ptr<alert_notification> &ptr) override {
    //do nothing
  }
};

}

#endif //SMPPHA_SMPP_PDU_PDU_VISITOR_H_
