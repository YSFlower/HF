#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <mutex>
#include "BaseDevice.h"
#include "ErrorCode.h"
#include "DataStruct.h"
class SendMessagePackage;
class ReceiveMessagePackage;
class SerialPort;

class SerialDevice :public BaseDevice
{
public:
    SerialDevice(uint8_t address = 0xFF);
    virtual ~SerialDevice();
    ReturnErrorCode Open(const char* usbPath);
    ReturnErrorCode SetConfig(const uint8_t nSpeed, const int nBits, const char nEvent, const int nStop);
    ReturnErrorCode Close();
    ReturnErrorCode Write(SendMessagePackage& sendContent, int& actuallyWriteLength);
    ReturnErrorCode Read(ReceiveMessagePackage& receiveContent, int& actuallyReadLength, unsigned int waitMesc = 0);
private:
    SerialPort* m_serialPort;
};

#endif // SERIALDEVICE_H
