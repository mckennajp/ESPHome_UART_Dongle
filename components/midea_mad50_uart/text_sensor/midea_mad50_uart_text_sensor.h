#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

#include "../midea_mad50_uart.h"

namespace esphome {
namespace midea_mad50_uart {

class MideaMAD50UARTTextSensor : public text_sensor::TextSensor, public Component, public Parented<MideaMAD50UART> {};

class Status : public MideaMAD50UARTTextSensor {

};


}  // namespace midea_mad50_uart
}  // namespace esphome