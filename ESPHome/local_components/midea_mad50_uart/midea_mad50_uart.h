#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif 
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
namespace esphome {
namespace midea_mad50_uart {

enum fanSpeed_t {
    low = 40,
    high = 80
};
enum mode_t {
    setpoint = 1,
    continuous = 2,
    smart = 3,
    clothesDrying = 4
};

struct dehumidifierState_t { 
  bool powerOn;
  bool pumpOn;
  mode_t mode;
  fanSpeed_t fanSpeed;
  uint8_t humiditySetpoint;
  uint8_t currentHumidity;
  uint8_t errorCode;
  bool filterCleaningRequest;
};
struct rxFrame_t{
	uint8_t rxBuffer[255];
	uint8_t rxBufferIndex;
	uint8_t rxRemainingByte;
};

class MideaMAD50UART : public uart::UARTDevice, public PollingComponent {
#ifdef USE_SWITCH
  SUB_SWITCH(power_switch)
  SUB_SWITCH(pump_switch)
#endif
#ifdef USE_NUMBER
  SUB_NUMBER(set_point)
#endif
#ifdef USE_SENSOR
  protected:
    sensor::Sensor *humidity_sensor_{nullptr}; 
  public:
    void set_humidity_sensor(sensor::Sensor *sensor) { this->humidity_sensor_ = sensor; }
#endif
#ifdef USE_TEXT_SENSOR
  protected:
    text_sensor::TextSensor *status_text_sensor_{nullptr}; 
  public:
    void set_status_text_sensor(text_sensor::TextSensor *text_sensor) { this->status_text_sensor_ = text_sensor; }
#endif
#ifdef USE_SELECT
	SUB_SELECT(mode)
	SUB_SELECT(fan)
#endif

  public:
    void setup() override;
	void update() override;
    void loop() override;
    void dump_config() override;	 
	
	
 
  private:
    uint8_t checksum(uint8_t* addr, uint8_t len);
    uint8_t crc8(uint8_t* addr, uint8_t len);
    void GenerateHeader(uint8_t* header, uint8_t msgType, uint8_t agreementVersion, uint8_t frameSyncCheck, uint8_t packetLength);
    void GeneratePowerSwitchCommand(uint8_t* cmd, uint8_t powerswitch);
    uint8_t GenerateStatusCommand(uint8_t* cmd); // return number of characters
    uint8_t GenerateNetworkUpdateCommand(uint8_t* cmd);
    uint8_t serialTxBuf[255];
    uint8_t StatusCommand[33];
    uint8_t NetworkCommand[31];
    uint8_t StateChangeCommand[34];
    uint8_t GenerateCommand(uint8_t* cmd, bool powerOn, bool pumpOn, mode_t mode, fanSpeed_t fanSpeed, uint8_t humiditySetpoint, bool powerCmdFlag);
    void SendCommand(uint8_t* cmd);
    void HandShaking(uint8_t step);
    rxFrame_t rxFrame;
    
  private:
    dehumidifierState_t state;
    void UpdateMAD50State(uint8_t* rxbuffer);
    void PublishState();
    uint8_t handShakingStepIndex;
    void Delayms(uint32_t ms);
  
     
  public:
    void UpdatePowerState(bool powerstate);
    void UpdatePumpState(bool pumpstate);
    void UpdateSetpoint(uint8_t target);
    void UpdateHumidity(uint8_t humitidy);
    //void UpdateStatus(uint8_t error);
    void UpdateMode(const std::string &value);
    void UpdateFanSpeed(const std::string &value);
    void set_handShaking_step(int val);
    

};


}  // namespace midea_mad50_uart
}  // namespace esphome