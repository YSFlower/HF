#ifndef NETWORKDEVICE_H
#define NETWORKDEVICE_H

#include "BaseDevice.h"
class NetworkPort;
class NetworkDevice:public BaseDevice
{
public:
    NetworkDevice(uint8_t address = 0xFF);
    ~NetworkDevice();
    ReturnErrorCode Open(std::string ip = "192.168.1.192", uint16_t port = 6000);
    ReturnErrorCode Close();
    ReturnErrorCode Write(SendMessagePackage& sendContent, int& actuallyWriteLength);
    ReturnErrorCode Read(ReceiveMessagePackage& receiveContent, int& actuallyReadLength, unsigned int waitMesc = 0);
private:
    NetworkPort* m_networkPort;
};

#endif // NETWORKDEVICE_H
