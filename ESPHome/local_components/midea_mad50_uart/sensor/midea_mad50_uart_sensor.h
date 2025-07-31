#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

#include "../midea_mad50_uart.h"

namespace esphome {
namespace midea_mad50_uart {

class MideaMAD50UARTSensor : public sensor::Sensor, public Component, public Parented<MideaMAD50UART> {};

class Humidity : public MideaMAD50UARTSensor {

};


}  // namespace midea_mad50_uart
}  // namespace esphome