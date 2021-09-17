#include "NetworkDevice.h"
#include "NetworkPort.h"
#include <sys/time.h>
NetworkDevice::NetworkDevice(uint8_t address)
    : m_networkPort(new NetworkPort)
    , BaseDevice(address)
{

}


NetworkDevice::~NetworkDevice()
{

}

ReturnErrorCode NetworkDevice::Open(std::string ip, uint16_t port)
{
    return m_networkPort->NetworkOpen(ip, port, m_fd);
}

ReturnErrorCode NetworkDevice::Close()
{
    return m_networkPort->NetworkClose(m_fd);
}

ReturnErrorCode NetworkDevice::Write(SendMessagePackage& sendContent, int& actuallyWriteLength)
{
    return m_networkPort->NetworkWrite(m_fd, sendContent.GetContent(), sendContent.GetContentLength(), actuallyWriteLength);
}

ReturnErrorCode NetworkDevice::Read(ReceiveMessagePackage& receiveContent, int& actuallyReadLength, unsigned int waitMesc)
{
    ReturnErrorCode errorCode = ReturnErrorCode::ERROR_CODE_SUCCESS;
    int len = 0, messageLength = 0;
    unsigned char* buff = nullptr;
    if (waitMesc == 0)
    {
        uint8_t receiveMessageLength = 0;
        errorCode = m_networkPort->NetworkRead(m_fd, &receiveMessageLength, sizeof(receiveMessageLength), len);
        if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
            return errorCode;
        if(len == 0)
            return ReturnErrorCode::READ_ERROR;

        receiveContent.SetLength(receiveMessageLength);

        buff = new unsigned char[receiveMessageLength];
        errorCode = m_networkPort->NetworkRead(m_fd, buff, receiveMessageLength, messageLength);
        if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        {
            receiveContent.Clear();
            return errorCode;
        }

        if (messageLength == receiveMessageLength)
        {
            actuallyReadLength = 1 + receiveMessageLength;
            receiveContent.SetContent(buff);
        }
        else
            receiveContent.Clear();
    }
    else
    {
        timeval startDateTime, currentDateTime;
        gettimeofday(&startDateTime, NULL);
        uint8_t receiveMessageLength = 0;
        bool receiveFinish = false;
        while (!receiveFinish)
        {
            gettimeofday(&currentDateTime, NULL);
            if ((currentDateTime.tv_sec * 1000 + currentDateTime.tv_usec / 1000) - (startDateTime.tv_sec * 1000 + startDateTime.tv_usec / 1000) > waitMesc)
                break;

            errorCode = m_networkPort->NetworkRead(m_fd, &receiveMessageLength, sizeof(receiveMessageLength), len);
            if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
                return errorCode;
            if(len != 0)
            {
                receiveContent.SetLength(receiveMessageLength);
                buff = new unsigned char[receiveMessageLength];
                int buffIndex = 0;
                while(1)
                {
                    if ((currentDateTime.tv_sec * 1000 + currentDateTime.tv_usec / 1000) - (startDateTime.tv_sec * 1000 + startDateTime.tv_usec / 1000) > waitMesc)
                        break;
                    errorCode = m_networkPort->NetworkRead(m_fd, &buff[buffIndex], receiveMessageLength - buffIndex, messageLength);
                    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
                        return errorCode;
                    if (actuallyReadLength > 0)
                        buffIndex += messageLength;

                    if (buffIndex == receiveMessageLength)
                    {
                        receiveContent.SetContent(buff);
                        actuallyReadLength = receiveMessageLength + 1;
                        receiveFinish = true;
                        break;
                    }
                }
            }
        }
    }
    if (buff != nullptr)
    {
        delete []buff;
        buff = nullptr;
    }

    if (receiveContent.Empty())
        return ReturnErrorCode::READ_ERROR;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;
    return errorCode;
}

