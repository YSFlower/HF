#include "SerialDevice.h"

#include "SerialPort.h"
#include <string.h>
#include <sys/time.h>

SerialDevice::SerialDevice(uint8_t address)
    : BaseDevice(address)
    , m_serialPort(new SerialPort)
{

}

SerialDevice::~SerialDevice()
{

}

ReturnErrorCode SerialDevice::Open(const char* usbPath)
{
    return m_serialPort->SerialOpen(usbPath, m_fd);
}

ReturnErrorCode SerialDevice::SetConfig(const uint8_t nSpeed, const int nBits, const char nEvent, const int nStop)
{
    return m_serialPort->SerialConfig(m_fd, nSpeed, nBits, nEvent, nStop);
}

ReturnErrorCode SerialDevice::Close()
{
    return m_serialPort->SerialClose(m_fd);
}

ReturnErrorCode SerialDevice::Write(SendMessagePackage& sendContent, int& actuallyWriteLength)
{
    return m_serialPort->SerialWrite(m_fd, sendContent.GetContent(), sendContent.GetContentLength(), actuallyWriteLength);
}

ReturnErrorCode SerialDevice::Read(ReceiveMessagePackage& receiveContent, int& actuallyReadLength, unsigned int waitMesc)
{
    ReturnErrorCode errorCode = ReturnErrorCode::ERROR_CODE_SUCCESS;
    int len = 0, messageLength = 0;
    unsigned char* buff = nullptr;
    if (waitMesc == 0)
    {
        uint8_t receiveMessageLength = 0;
        errorCode = m_serialPort->SerialRead(m_fd, &receiveMessageLength, sizeof(receiveMessageLength), len);
        if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
            return errorCode;
        if(len == 0)
            return ReturnErrorCode::READ_ERROR;

        receiveContent.SetLength(receiveMessageLength);

        buff = new unsigned char[receiveMessageLength];
        errorCode = m_serialPort->SerialRead(m_fd, buff, receiveMessageLength, messageLength);
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

            errorCode = m_serialPort->SerialRead(m_fd, &receiveMessageLength, sizeof(receiveMessageLength), len);
            if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
                return errorCode;
            if(len != 0)
            {
                receiveContent.SetLength(receiveMessageLength);
                buff = new unsigned char[receiveMessageLength];
                int buffIndex = 0;
                while(1)
                {
                    gettimeofday(&currentDateTime, NULL);
                    if ((currentDateTime.tv_sec * 1000 + currentDateTime.tv_usec / 1000) - (startDateTime.tv_sec * 1000 + startDateTime.tv_usec / 1000) > waitMesc)
                        break;
                    errorCode = m_serialPort->SerialRead(m_fd, &buff[buffIndex], receiveMessageLength - buffIndex, messageLength);
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
