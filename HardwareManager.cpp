#include "HardwareManager.h"
#include <stdexcept>
DeviceStatus mockDevice(const std::string& type, bool timeout) {
    if(type!="Microcontroller" && type!="FPGA" && type!="JTAG")
        throw std::invalid_argument("Unknown device type.");
    if(timeout) return {type,type+" mock","Disconnected","Recoverable timeout; retry connection."};
    return {type,type+" mock","Connected","Telemetry: temperature=24.5 C; link=OK (SIMULATED)."};
}
