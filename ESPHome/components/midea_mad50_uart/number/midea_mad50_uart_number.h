#pragma once

#include "esphome/components/number/number.h"
#include "esphome/core/component.h"

#include "../midea_mad50_uart.h"

namespace esphome {
namespace midea_mad50_uart {

class MideaMAD50UARTNumber : public number::Number, public Component, public Parented<MideaMAD50UART> {};

class SetPoint : public MideaMAD50UARTNumber {
 public:
  void control(float value) override;
};


}  // namespace midea_mad50_uart
}  // namespace esphome