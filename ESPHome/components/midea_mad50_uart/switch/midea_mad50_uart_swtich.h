#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

#include "../midea_mad50_uart.h"

namespace esphome {
namespace midea_mad50_uart {

class MideaMAD50UARTSwitch : public switch_::Switch, public Component, public Parented<MideaMAD50UART> {};

class PowerSwitch : public MideaMAD50UARTSwitch {
 public:
  void write_state(bool state) override;
};

class PumpSwitch : public MideaMAD50UARTSwitch {
 public:
  void write_state(bool state) override;
};

}  // namespace midea_mad50_uart
}  // namespace esphome