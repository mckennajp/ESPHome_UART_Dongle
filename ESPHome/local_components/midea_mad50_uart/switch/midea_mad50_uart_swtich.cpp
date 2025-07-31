#include "midea_mad50_uart_swtich.h"

namespace esphome {
namespace midea_mad50_uart {

static const char *const TAG = "midea_mad50_uart.switch";

void PowerSwitch::write_state(bool state) {
    ESP_LOGI(TAG, "Power %s", state ? "ON" : "OFF");
    this->parent_->UpdatePowerState(state);
}

void PumpSwitch::write_state(bool state) {
    ESP_LOGI(TAG, "Pump %s", state ? "ON" : "OFF");
    this->parent_->UpdatePumpState(state);
}


}  // namespace midea_mad50_uart
}  // namespace esphome