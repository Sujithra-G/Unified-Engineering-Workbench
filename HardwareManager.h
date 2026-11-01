#pragma once
#include <string>
struct DeviceStatus { std::string type,name,state,message; };
DeviceStatus mockDevice(const std::string& type, bool simulateTimeout=false);
