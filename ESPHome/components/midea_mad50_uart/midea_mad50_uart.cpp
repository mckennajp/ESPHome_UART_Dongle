#include "esphome/core/log.h"
#include "midea_mad50_uart.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace midea_mad50_uart {

static const char *TAG = "midea_dehumidifier_uart.component";

static const uint8_t crc_table[] = {
    0x00,0x5e,0xbc,0xE2,0x61,0x3F,0xDD,0x83,
    0xC2,0x9C,0x7E,0x20,0xA3,0xFD,0x1F,0x41,
    0x9D,0xC3,0x21,0x7F,0xFC,0xA2,0x40,0x1E,
    0x5F,0x01,0xE3,0xBD,0x3E,0x60,0x82,0xDC,
    0x23,0x7D,0x9F,0xC1,0x42,0x1C,0xFE,0xA0,
    0xE1,0xBF,0x5D,0x03,0x80,0xDE,0x3C,0x62,
    0xBE,0xE0,0x02,0x5C,0xDF,0x81,0x63,0x3D,
    0x7C,0x22,0xC0,0x9E,0x1D,0x43,0xA1,0xFF,
    0x46,0x18,0xFA,0xA4,0x27,0x79,0x9B,0xC5,
    0x84,0xDA,0x38,0x66,0xE5,0xBB,0x59,0x07,
    0xDB,0x85,0x67,0x39,0xBA,0xE4,0x06,0x58,
    0x19,0x47,0xA5,0xFB,0x78,0x26,0xC4,0x9A,
    0x65,0x3B,0xD9,0x87,0x04,0x5A,0xB8,0xE6,
    0xA7,0xF9,0x1B,0x45,0xC6,0x98,0x7A,0x24,
    0xF8,0xA6,0x44,0x1A,0x99,0xC7,0x25,0x7B,
    0x3A,0x64,0x86,0xD8,0x5B,0x05,0xE7,0xB9,
    0x8C,0xD2,0x30,0x6E,0xED,0xB3,0x51,0x0F,
    0x4E,0x10,0xF2,0xAC,0x2F,0x71,0x93,0xCD,
    0x11,0x4F,0xAD,0xF3,0x70,0x2E,0xCC,0x92,
    0xD3,0x8D,0x6F,0x31,0xB2,0xEC,0x0E,0x50,
    0xAF,0xF1,0x13,0x4D,0xCE,0x90,0x72,0x2C,
    0x6D,0x33,0xD1,0x8F,0x0C,0x52,0xB0,0xEE,
    0x32,0x6C,0x8E,0xD0,0x53,0x0D,0xEF,0xB1,
    0xF0,0xAE,0x4C,0x12,0x91,0xCF,0x2D,0x73,
    0xCA,0x94,0x76,0x28,0xAB,0xF5,0x17,0x49,
    0x08,0x56,0xB4,0xEA,0x69,0x37,0xD5,0x8B,
    0x57,0x09,0xEB,0xB5,0x36,0x68,0x8A,0xD4,
    0x95,0xCB,0x29,0x77,0xF4,0xAA,0x48,0x16,
    0xE9,0xB7,0x55,0x0B,0x88,0xD6,0x34,0x6A,
    0x2B,0x75,0x97,0xC9,0x4A,0x14,0xF6,0xA8,
    0x74,0x2A,0xC8,0x96,0x15,0x4B,0xA9,0xF7,
    0xB6,0xE8,0x0A,0x54,0xD7,0x89,0x6B,0x35
};

uint8_t MideaMAD50UART::crc8(uint8_t* addr, uint8_t len) {
    uint8_t crc = 0;
    while (len--) {
        crc = crc_table[*addr++ ^ crc];
    }
    return crc;
}

uint8_t MideaMAD50UART::checksum(uint8_t* addr, uint8_t len) {
    uint8_t sum = 0;
    *addr++;

    while (len--) {
        sum = sum + *addr++;
    }

    return 256 - sum;
}




void MideaMAD50UART::GenerateHeader(uint8_t* header, uint8_t msgType, uint8_t agreementVersion, uint8_t frameSyncCheck, uint8_t packetLength) {
    header[0] = 0xAA; //Sync Header
    header[1] = 10 + packetLength + 1;
    header[2] = 0xA1; //ApplianceType
    header[3] = frameSyncCheck; //Frame sync check (not used)
    header[4] = 0x00; //Reserved
    header[5] = 0x00; //Reserved
    header[6] = 0x00; //MsgId
    header[7] = 0x00; //ProtocolVersion
    header[8] = agreementVersion;
    header[9] = msgType;
}

uint8_t MideaMAD50UART::GenerateStatusCommand(uint8_t* cmd)
{
	cmd[0] = 0x41; cmd[1] = 0x81; cmd[2] = 0x00; cmd[3] = 0xFF; cmd[4] = 0x00;
	cmd[5] = 0x00; cmd[6] = 0x00; cmd[7] = 0x00; cmd[8] = 0x00; cmd[9] = 0x00; 
	cmd[10] = 0x00; cmd[11] = 0x00; cmd[12] = 0x00; cmd[13] = 0x00; cmd[14] = 0x00; 
	cmd[15] = 0x00; cmd[16] = 0x00; cmd[17] = 0x00; cmd[18] = 0x00; cmd[19] = 0x00; 
	cmd[20] = 0x06; 
	return 21;
} 
uint8_t MideaMAD50UART::GenerateNetworkUpdateCommand(uint8_t* cmd){
	cmd[0] = 0x01; // Byte 0: Module type: - 0x00 RF module - 0x01 WiFi module
	cmd[1] = 0x01; // Byte 1: WiFi module working mode: - 0x01 Client mode  - 0x02 Configuration mode - 0x03 AP mode
	cmd[2] = 0x04; // Byte 2: WiFi signal strength - 0x00 No signal - 0x01 Weak - 0x02 Low - 0x03 Medium - 0x04 Strong - 0xFF WiFi is not supported
	cmd[3] = 0x01; // Byte 3-6: IPv4 address of client in reverse order
	cmd[4] = 0x00;
	cmd[5] = 0x00;
	cmd[6] = 0x7F; // 127.0.0.1
	cmd[7] = 0xFF; // Byte 7: RF Signal strength - 0x00, no signal - 0x01, weak - 0x02, low - 0x03, medium - 0x04, strong - 0xFF, RF is not supported
	cmd[8] = 0x00; // Byte 8: Router status - 0x00, wireless router is connected - 0x01, wireless router not connected - 0x02, connecting to a wireless router - 0x03, password verification error - 0x04, no wireless router found - 0x05, IP cannot be obtained - 0x06, wireless unstable - 0xFF, WI-FI failure
	cmd[9] = 0x00; // Byte 9: Cloud service connection status: - 0x00, connected to the cloud service center - 0x01, not connected to the cloud service center - 0x02, unstable internet connection - 0x03, domain name resolution error - 0x04, cloud service connection refused - 0x05, cloud service maintenance - 0xFF, cloud service failure
	cmd[10] = 0x00; // Byte 10: Direct LAN connection status - 0x00: No connection/connection has been disconnected - 0x01: connected/connected with mobile terminal
	cmd[11] = 0x00; // Byte 11 Number of TCP connections between the module and the mobile terminal
	cmd[12] = 0x01; // Byte 12 - 19 Reserved / known
	cmd[13] = 0x00;
	cmd[14] = 0x09;
	cmd[15] = 0x00;
	cmd[16] = 0x03;
	cmd[17] = 0x00;
	cmd[18] = 0x00;
	return 19;
}

uint8_t MideaMAD50UART::GenerateCommand(uint8_t* cmd,bool powerOn, bool pumpOn,mode_t mode, fanSpeed_t fanSpeed, uint8_t humiditySetpoint, bool powerCmdFlag){
	cmd[0] = 0x48; // magic
	cmd[1] = powerOn? 0x43 : 0x42; // power on: 0x43, power off: 0x42
	cmd[2] = (uint8_t) mode; // mode: setpoint: 0x01, continuous: 0x02, confort: 0x03, clothdrying: 0x04
	cmd[3] = (uint8_t) fanSpeed | 0x80; // fan speed: low: 0xA8, high: 0xD0
	cmd[4] = 0x7F;
	cmd[5] = 0x7F;
	cmd[6] = 0x00;
	cmd[7] = humiditySetpoint; // humidity setpoint
	cmd[8] = 0x00;
	cmd[9] = powerCmdFlag? 0x00: ( pumpOn? 0x18 : 0x10 ); // pump: on: 0x18, off: 0x10
	cmd[10] = 0x00;
	cmd[11] = 0x00;
	cmd[12] = 0x00;
	cmd[13] = 0x00;
	cmd[14] = 0x00; // some unkonwn ramdon set flag
	cmd[15] = 0x00;
	cmd[16] = 0x00;
	cmd[17] = 0x00;
	cmd[18] = 0x00;
	cmd[19] = 0x00;
	cmd[20] = 0x00;
	cmd[21] = 0x00;
	return 22;
}
void MideaMAD50UART::setup() {


	// generate the status request command
	uint8_t payloadLength = GenerateStatusCommand( &StatusCommand[10]);
	uint8_t frameSyncCheck = 0x00;
	uint8_t agreementVersion = 0x00;
	uint8_t msgType = 0x03;
	GenerateHeader( &StatusCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
	StatusCommand[10 + payloadLength] = crc8(StatusCommand + 10, payloadLength);
	StatusCommand[10 + payloadLength + 1] = 0x00;
	StatusCommand[10 + payloadLength + 1] = checksum(StatusCommand, 10 + payloadLength + 1);
	// generate network update command
	payloadLength = GenerateNetworkUpdateCommand(&NetworkCommand[10]);
	frameSyncCheck = 0xBF;
	agreementVersion = 0x08;
	msgType = 0x63;
	GenerateHeader( &NetworkCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
	NetworkCommand[10 + payloadLength] = 0x00; // for netowrk frame, no need for crc8
	NetworkCommand[10 + payloadLength + 1] = 0x00;
    NetworkCommand[10 + payloadLength + 1] = checksum(NetworkCommand, 10 + payloadLength + 1);
	// send the status update request for initializing the dongle's internal states
	//this-> write_array ( (const uint8_t*) StatusCommand, 33);
	// initialize the hand shaking process
	Delayms(180);
	//	handShakingStepIndex = 0;

	// initialize the rxPacket_t structure
	rxFrame.rxBufferIndex = 255;
	rxFrame.rxRemainingByte = 0;

}

void MideaMAD50UART::update() {
	// check the appliance for status update
	switch (handShakingStepIndex)
	{
		case 0:
			HandShaking(0);
			break;
		case 1:
			HandShaking(1);
			break;
		case 2:
			HandShaking(2);
			break;
		case 3:
			// hand shaking is completed, send the status request command
			this-> write_array ( (const uint8_t*) StatusCommand, 33);
			break;
	}
	ESP_LOGI(TAG, "handShakingStepIndex: %d", handShakingStepIndex);
}
void MideaMAD50UART::loop() {
	// receive data
	uint8_t rxByte;
	while( this->available() )
	{
		this->read_byte(&rxByte);
		if (rxByte == 0xAA) {
			// starting of a frame
			rxFrame.rxBufferIndex = 0;
			rxFrame.rxBuffer[rxFrame.rxBufferIndex] = rxByte;
			rxFrame.rxBufferIndex++;
		}
		else if( rxFrame.rxBufferIndex != 255){
			// in the middle of the receiving the frame
			if( rxFrame.rxBufferIndex == 1) {
				// this is the length of the remaining bytes
				rxFrame.rxRemainingByte = rxByte - 1;
				rxFrame.rxBuffer[rxFrame.rxBufferIndex] = rxByte;
				rxFrame.rxBufferIndex ++;
			}
			else{
				// continue to get the remaining bytes
				if ( rxFrame.rxRemainingByte > 1 ) {
					//continue to the end of frame
					rxFrame.rxBuffer[rxFrame.rxBufferIndex] = rxByte;
					rxFrame.rxBufferIndex ++;
					rxFrame.rxRemainingByte -- ;
				}
				else if ( rxFrame.rxRemainingByte == 1 ){
					// last byte if the incoming frame 
					rxFrame.rxBuffer[rxFrame.rxBufferIndex] = rxByte;
					// decode and response to the receiverd frame
					//
					if (rxFrame.rxBuffer[9] == 0x63 ) {
						// to update the applicance for dongle's network status
						ESP_LOGI(TAG, "Response to network update request");
						this-> write_array ( (const uint8_t*) NetworkCommand, 31);
					}
					else if (rxFrame.rxBuffer[10] == 0xC8 ) {
						// to update the dongle internal states based on the received information
						ESP_LOGI(TAG, "Update appliance status");
						UpdateMAD50State(rxFrame.rxBuffer);
						PublishState();
					}else if(rxFrame.rxBuffer[9] == 0x07) {
						// this is the 2nd send in the handshake process
						HandShaking(1);
						handShakingStepIndex = 1;
					}
					else if(rxFrame.rxBuffer[9] == 0xA0) {
						// this is the 2nd send in the handshake process
						HandShaking(2);
						handShakingStepIndex = 2;
					}else if(rxFrame.rxBuffer[9] == 0x05) {
						// this is the 2nd send in the handshake process
						HandShaking(3);
						handShakingStepIndex = 3;
					}
					// reset the rxBuffer_t values
					rxFrame.rxBufferIndex =255;
					rxFrame.rxRemainingByte = 0;
				}
			}
		}
		else{
			// ignore random bytes floating around
			
		}
	}
	
}
void MideaMAD50UART::HandShaking(uint8_t step){
	uint8_t payloadLength = 0;
	uint8_t msgType = 0;
	uint8_t agreementVersion = 0;
	uint8_t frameSyncCheck = 0;
	switch (step)
	{
		case 0:
			// preparing the initial query to the appliance
			payloadLength = 0;
			msgType = 0x07;
			agreementVersion = 0x00;
			frameSyncCheck = 0xF4;
			GenerateHeader( &serialTxBuf[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
			serialTxBuf[2] = 0xFF;
			serialTxBuf[6] = 0x01;
			// send the initial query to the application
			SendCommand(&serialTxBuf[0]);
			break;
		case 1:
			// preparing the 2nd hand shaking command
			memset(&serialTxBuf[10], 0, 19);
			payloadLength = 19;
			msgType = 0xA0;
			agreementVersion = 0x08;
			frameSyncCheck = 0xBF;
			GenerateHeader( &serialTxBuf[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
			SendCommand(&serialTxBuf[0]);
			break;
		case 2:
			payloadLength = GenerateNetworkUpdateCommand(&serialTxBuf[10]);
			frameSyncCheck = 0xBF;
			agreementVersion = 0x08;
			msgType = 0x0D;
			GenerateHeader( &serialTxBuf[0], msgType, agreementVersion, frameSyncCheck, payloadLength);	
			SendCommand(&serialTxBuf[0]);		
			break;
		case 3:
			this-> write_array ( (const uint8_t*) rxFrame.rxBuffer, rxFrame.rxBuffer[1] + 1 );
			break;


	}
}
void MideaMAD50UART::UpdateMAD50State(uint8_t* rxbuffer){
	// based on the received status frame from the appliance, update the internal states
	state.powerOn = rxbuffer[11] & 0x01; // appliance power state: -On: 0x01 -Off: 0x00
	state.pumpOn = (rxbuffer[19] & 0x08) >> 3; // pump state: -On: 0x01 -Off: 0x00 
	state.mode = (mode_t) ((rxbuffer[12] & 0x0f)); // mode: -Setpoint: 0x01 -Continuous: 0x02 -Smart: 0x03 -Clothes Drying: 0x04
	state.fanSpeed = (fanSpeed_t) ((rxbuffer[13] & 0x7F) ); // fan speed: -Low: 0x40 -High: 0x80
	state.humiditySetpoint = (rxbuffer[17] > 99)? 99: rxbuffer[17]; //humidity setpoint
	state.currentHumidity = rxbuffer[26]; //current humidity
	state.errorCode = rxbuffer[31]; //error code
	state.filterCleaningRequest = (rxbuffer[19] * 0x80 ) >>7; // filter cleaning request
}
void MideaMAD50UART::PublishState(){
	// publish the internal states to the control and sensor components
	#ifdef USE_SWITCH
	if (this->power_switch_switch_ != nullptr)
		this->power_switch_switch_->publish_state(state.powerOn);
	if (this->pump_switch_switch_ != nullptr)
		this->pump_switch_switch_->publish_state(state.pumpOn);
	#endif

 	#ifdef USE_NUMBER
	if (this->set_point_number_ != nullptr)
		this->set_point_number_->publish_state(state.humiditySetpoint);
 	#endif

	#ifdef USE_SENSOR
	if (this->humidity_sensor_ != nullptr)
		this->humidity_sensor_->publish_state((float)state.currentHumidity);
   	#endif

	#ifdef USE_TEXT_SENSOR
	   std::string msg;
	   if (this->status_text_sensor_ != nullptr)
		 switch (state.errorCode )
		 {
			 case 0x00:
				 msg = "Normal";
				 break;
			 case 0x25:
				 msg = "Tank Out";
				 break;
			 case 0x26:
				 msg = "Tank Full";
				 break;
			 default:
				 if (state.filterCleaningRequest)
				 {
					 msg = "Filter Full";
				 }
				 else
				 {
					 msg = "0x" + std::to_string(state.errorCode >> 4) + std::to_string(state.errorCode & 0xF ) ;
				 }
				 break;
		 };
		 this->status_text_sensor_->publish_state(msg);	
	 #endif


	#ifdef USE_SELECT
	switch(state.mode)
	{
		case setpoint:
			this->mode_select_->publish_state( (const std::string) "SetPoint");
			break;
		case continuous:
			this->mode_select_->publish_state( (const std::string) "Continuous");
			break;
		case smart:
			this->mode_select_->publish_state( (const std::string) "Smart");
			break;
		case clothesDrying:
			this->mode_select_->publish_state( (const std::string) "Drying");
			break;
		default:
			break;
	}
	switch(state.fanSpeed)
	{
		case low:
			this->fan_select_->publish_state( (const std::string) "Low");
			break;
		case high:
			this->fan_select_->publish_state( (const std::string) "High");
			break;
		default:
			break;
	}
 	#endif

}
void MideaMAD50UART::SendCommand(uint8_t* cmd){
	// send the command to the appliance
	uint8_t payloadLength = cmd[1] - 10 - 1;
	cmd[10 + payloadLength] = crc8(cmd + 10, payloadLength);
	cmd[10 + payloadLength + 1] = 0x00;
	cmd[10 + payloadLength + 1] = checksum(cmd, 10 + payloadLength + 1);
	// send the state change command to the appliance
	this-> write_array ( (const uint8_t*) cmd, 10 + payloadLength + 1 + 1);

}
void MideaMAD50UART::UpdatePowerState(bool powerstate){
	// generate the power on/off command
	uint8_t payloadLength = GenerateCommand( &StateChangeCommand[10], powerstate, state.pumpOn, state.mode, state.fanSpeed, state.humiditySetpoint, true);
	uint8_t frameSyncCheck = 0x00;
	uint8_t agreementVersion = 0x00;
	uint8_t msgType = 0x02;
	GenerateHeader( &StateChangeCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
	StateChangeCommand[10 + payloadLength] = crc8(StateChangeCommand + 10, payloadLength);
    StateChangeCommand[10 + payloadLength + 1] = 0x00;
	StateChangeCommand[10 + payloadLength + 1] = checksum(StateChangeCommand, 10 + payloadLength + 1);
	// send the state change command to the appliance
	this-> write_array ( (const uint8_t*) StateChangeCommand, 34);


}
void MideaMAD50UART::UpdatePumpState(bool pumpstate){
	if( state.powerOn) {
		// only be able to tun on/off the pump when the appliance is powered on
		uint8_t payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, pumpstate, state.mode, state.fanSpeed, state.humiditySetpoint, false);
		uint8_t frameSyncCheck = 0x00;
		uint8_t agreementVersion = 0x00;
		uint8_t msgType = 0x02;
		GenerateHeader( &StateChangeCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
		StateChangeCommand[10 + payloadLength] = crc8(StateChangeCommand + 10, payloadLength);
		StateChangeCommand[10 + payloadLength + 1] = 0x00;
		StateChangeCommand[10 + payloadLength + 1] = checksum(StateChangeCommand, 10 + payloadLength + 1);
		// send the state change command to the appliance
		this-> write_array ( (const uint8_t*) StateChangeCommand, 34);
	}
	else{
		// appliance is powered off, no need to turn on/off the pump
		// restore the pump switch UI
		#ifdef USE_SWITCH
		if (this->pump_switch_switch_ != nullptr)
			this->pump_switch_switch_->publish_state(state.pumpOn);
		#endif
		return;
	}

}

void MideaMAD50UART::UpdateSetpoint(uint8_t target){
	if( state.powerOn) {
		// only be able to change humidity setpoint when the appliance is powered on
		uint8_t payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, state.mode, state.fanSpeed, target, true);
		uint8_t frameSyncCheck = 0x00;
		uint8_t agreementVersion = 0x00;
		uint8_t msgType = 0x02;
		GenerateHeader( &StateChangeCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
		SendCommand(&StateChangeCommand[0]);
	}
	else{
		// appliance is powered off, no need to change the setpoint
		// restore the setpoint UI
		#ifdef USE_NUMBER
		if (this->set_point_number_ != nullptr)
			this->set_point_number_->publish_state(state.humiditySetpoint);
		#endif
		return;
	}

}

void MideaMAD50UART::UpdateHumidity(uint8_t humitidy){

}

//void MideaMAD50UART::UpdateStatus(uint8_t error){
	
//}
void MideaMAD50UART::UpdateMode(const std::string &value){
	//ESP_LOGI(TAG, "Update Mode status: %s", value.c_str());
	if( state.powerOn) {
		// only be able to change mode when the appliance is powered on
		uint8_t payloadLength = 0;
		if (value == "SetPoint"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, (mode_t) setpoint, state.fanSpeed, state.humiditySetpoint, true);
		}else if (value == "Continuous"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, (mode_t) continuous, state.fanSpeed, state.humiditySetpoint, true);
		}else if (value == "Smart"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, (mode_t) smart, state.fanSpeed, state.humiditySetpoint, true);
		}else if (value == "Drying"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, (mode_t) clothesDrying, state.fanSpeed, state.humiditySetpoint, true);
		}
		uint8_t frameSyncCheck = 0x00;
		uint8_t agreementVersion = 0x00;
		uint8_t msgType = 0x02;
		GenerateHeader( &StateChangeCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
		SendCommand(&StateChangeCommand[0]);
	}
	else{
		// appliance is powered off, no need to change the mode
		// restore the mode switch UI
		#ifdef USE_SELECT
		switch(state.mode)
		{
			case setpoint:
				this->mode_select_->publish_state( (const std::string) "SetPoint");
				break;
			case continuous:
				this->mode_select_->publish_state( (const std::string) "Continuous");
				break;
			case smart:
				this->mode_select_->publish_state( (const std::string) "Smart");
				break;
			case clothesDrying:
				this->mode_select_->publish_state( (const std::string) "Drying");
				break;
			default:
				break;
		}
		#endif
		return;
	}

}

void MideaMAD50UART::UpdateFanSpeed(const std::string &value){
	if( state.powerOn) {
		// only be able to change fan speed when the appliance is powered on
		uint8_t payloadLength = 0;
		if (value == "Low"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, state.mode, (fanSpeed_t) low, state.humiditySetpoint, true);
		}
		else if (value == "High"){
			payloadLength = GenerateCommand( &StateChangeCommand[10], state.powerOn, state.pumpOn, state.mode, (fanSpeed_t) high, state.humiditySetpoint, true);
		}
		uint8_t frameSyncCheck = 0x00;
		uint8_t agreementVersion = 0x00;
		uint8_t msgType = 0x02;
		GenerateHeader( &StateChangeCommand[0], msgType, agreementVersion, frameSyncCheck, payloadLength);
		SendCommand(&StateChangeCommand[0]);
		
	}
	else{
		// appliance is powered off, no need to change the fan speed
		// restore the pump switch UI
		#ifdef USE_SELECT
		switch(state.fanSpeed)
		{
			case low:
				this->fan_select_->publish_state( (const std::string) "Low");
				break;
			case high:
				this->fan_select_->publish_state( (const std::string) "High");
				break;
			default:
				break;
		}
		#endif
		return;
	}
}
void MideaMAD50UART::Delayms(uint32_t ms){
	// delay for the given time
	delay(ms);
}
void MideaMAD50UART::dump_config(){
    ESP_LOGCONFIG(TAG, "Midea Dehumidifier UART");
}

void MideaMAD50UART::set_handShaking_step(int val)
{
	handShakingStepIndex = val;
	// val = 3: skip the hand shaking process, just send the status request command
	// val = 0: start the hand shaking process
}

}  // namespace midea_mad50_uart
}  // namespace esphome