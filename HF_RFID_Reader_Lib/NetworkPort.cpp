#include "NetworkPort.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <poll.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>

NetworkPort::NetworkPort()
{

}

NetworkPort::~NetworkPort()
{}

ReturnErrorCode NetworkPort::NetworkOpen(std::string ip, uint16_t port, int& fd)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
    serverAddress.sin_port = htons(port);
    if (connect(fd, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) != 0)
        return ReturnErrorCode::OPEN_ERROR;
//    int flags = fcntl(fd, F_GETFL, 0);
//    if (flags < 0)
//        return ReturnErrorCode::CONTROL_FILE_ERROR;
//    if (fcntl(fd, F_SETFL, flags | O_RDWR | O_NDELAY | O_NOCTTY) < 0)
//        return ReturnErrorCode::CONTROL_FILE_ERROR;

    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}


ReturnErrorCode NetworkPort::NetworkClose(int& fd)
{
    if (fd != -1)
    {
        if (close(fd) != 0)
            return ReturnErrorCode::CLOSE_ERROR;
    }
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode NetworkPort::NetworkWrite(const int& fd, unsigned char* writeBuffer, size_t writeLength, int& actuallyWriteLength)
{
    actuallyWriteLength = 0;
    int ret = send(fd, writeBuffer, writeLength, 0);
    if (ret < 0)
        return ReturnErrorCode::WRITE_ERROR;

    actuallyWriteLength = ret;
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}

ReturnErrorCode NetworkPort::NetworkRead(const int& fd, unsigned char* readBuffer, int readLength, int& actuallyReadLength)
{
    int ret = recv(fd, readBuffer, readLength, 0);
    if (ret < 0)
        return ReturnErrorCode::READ_ERROR;

    actuallyReadLength = ret;
    return ReturnErrorCode::ERROR_CODE_SUCCESS;
}
