#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <stddef.h>
#include <stdint.h>
#include "ErrorCode.h"

/*串口设备操作类*/
class SerialPort
{
public:
    SerialPort();
    ReturnErrorCode SerialOpen(const char* usbPath, int& fd);
    ReturnErrorCode SerialConfig(const int& fd, const uint8_t nSpeed, const int nBits, const char nEvent, const int nStop);
    ReturnErrorCode SerialClose(int& fd);
    ReturnErrorCode SerialWrite(const int& fd, unsigned char* writeBuffer, size_t writeLength, int& actuallyWriteLength);
    ReturnErrorCode SerialRead(const int& fd, unsigned char* readBuffer, int readLength, int& actuallyReadLength);
    ReturnErrorCode SerialFlush(const int& fd);
};

#endif // SERIALPORT_H
