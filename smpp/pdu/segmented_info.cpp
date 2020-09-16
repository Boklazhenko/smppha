//
// Created by boklazhenko on 15.09.2020.
//

#include "segmented_info.h"
#include "pdu.h"

smpp::segmented_info::segmented_info(const smpp::submit_sm &pdu)
    : total_parts(1), part(1), id(0) {
  if (pdu.get<esm_class>() & 0x40) {
    const std::vector<uint8_t> &sm = pdu.get<short_message>();
    if (sm.empty())
      return;

    int udhl = std::min(static_cast<uint8_t>(sm[0] + 1), static_cast<uint8_t>(sm.size()));
    for (const unsigned char *rp = sm.data() + 1; rp < sm.data() + udhl;) {
      int iei = *rp++;
      int iel = *rp++;
      switch (iei) {
        case 0x00: // concat 8 bit
          if (iel == 3) {
            id = *rp++;
            total_parts = *rp++;
            part = *rp++;
            return;
          }
          break;
        case 0x08: // concat 16 bit
          if (iel == 4) {
            id = (((unsigned short) rp[0]) << 8) | ((unsigned short) rp[1]);
            rp += 2;
            total_parts = *rp++;
            part = *rp++;
            return;
          }
          break;
        default:rp += iel;
      }
    }
  } else if (auto sar_total_segments_ = pdu.get<sar_total_segments>(); sar_total_segments_) {
    id = pdu.get<sar_msg_ref_num>().value_or(0);
    part = pdu.get<sar_segment_seqnum>().value_or(1);
    total_parts = sar_total_segments_.value();
  }
}
