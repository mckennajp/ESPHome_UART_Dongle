#include "midea_mad50_uart_number.h"

namespace esphome {
namespace midea_mad50_uart {

static const char *const TAG = "midea_mad50_uart.number";

void SetPoint::control(float value) {
  uint8_t target = static_cast<uint8_t>(value);
  ESP_LOGI(TAG, "Set target humidity to %u", target);
  this->parent_->UpdateSetpoint(target);
}


}  // namespace midea_mad50_uart
}  // namespace esphome