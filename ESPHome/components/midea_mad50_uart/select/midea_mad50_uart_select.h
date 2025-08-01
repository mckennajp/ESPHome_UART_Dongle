#pragma once

#include "esphome/components/select/select.h"
#include "esphome/core/component.h"

#include "../midea_mad50_uart.h"

namespace esphome {
namespace midea_mad50_uart {

class MideaMAD50UARTSelect : public select::Select, public Component, public Parented<MideaMAD50UART> {};

class Mode : public MideaMAD50UARTSelect {
 public:
  Mode() = default;	
  void control(const std::string &value) override;
};

class Fan : public MideaMAD50UARTSelect {
 public:
  Fan() = default;	
  void control(const std::string &value) override;
};


}  // namespace midea_mad50_uart
}  // namespace esphome