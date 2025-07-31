#include "midea_mad50_uart_select.h"

namespace esphome {
namespace midea_mad50_uart {

static const char *const TAG = "midea_mad50_uart.select";

void Mode::control(const std::string &value) {
    ESP_LOGI(TAG, "Mode: %s", value.c_str());	
    this->parent_->UpdateMode(value);
}

void Fan::control(const std::string &value) {
    ESP_LOGI(TAG, "Fan: %s", value.c_str());
    this->parent_->UpdateFanSpeed(value);
}


}  // namespace midea_mad50_uart
}  // namespace esphome