#ifndef NETWORKPORT_H
#define NETWORKPORT_H

#include <stddef.h>
#include <stdint.h>
#include <string>
#include "ErrorCode.h"

/*网口设备操作类*/
class NetworkPort
{
public:
    NetworkPort();
    ~NetworkPort();
    ReturnErrorCode NetworkOpen(std::string ip, uint16_t port, int& fd);
    ReturnErrorCode NetworkClose(int& fd);
    ReturnErrorCode NetworkWrite(const int& fd, unsigned char* writeBuffer, size_t writeLength, int& actuallyWriteLength);
    ReturnErrorCode NetworkRead(const int& fd, unsigned char* readBuffer, int readLength, int& actuallyReadLength);
};

#endif // NETWORKPORT_H
