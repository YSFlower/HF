#include "MessagePackage.h"
#include <string.h>
#include <iostream>

MessagePackage::MessagePackage(unsigned int headLength, unsigned int tailLength)
    : m_headLength(headLength)
    , m_tailLength(tailLength)
{

}

bool MessagePackage::Empty()
{
    return m_content.size() == 0? true: false;
}

void MessagePackage::CalculateCRC(unsigned char* pData, size_t dataLength)
{
    unsigned int POLYNOMIAL = 0x8408;
    unsigned int PRESET_VALUE = 0xffff;

    size_t i, j;
    unsigned  int  current_crc_value = PRESET_VALUE;
    for (i = 0; i < dataLength; i++)
    {
        current_crc_value = current_crc_value ^ static_cast<unsigned int>(pData[i]);
        for (j = 0; j<8; j++)
        {
            if (current_crc_value & 0x0001)
            {
                current_crc_value = (current_crc_value >> 1) ^ POLYNOMIAL;
            }
            else
            {
                current_crc_value = (current_crc_value >> 1);
            }
        }
    }
    pData[i++] = static_cast<unsigned char>(current_crc_value & 0x00ff);
    pData[i] = static_cast<unsigned char>((current_crc_value >> 8) & 0x00ff);
}

SendMessagePackage::SendMessagePackage(uint8_t com_address, char cmd, uint8_t state, char* childData, uint8_t childDataLength)
    : MessagePackage(4, 2)
{
    m_content.resize(m_headLength + childDataLength + m_tailLength);
    uint8_t length = static_cast<uint8_t>(m_content.size() - 1);
    memcpy(&m_content[0], &length, sizeof(length));
    memcpy(&m_content[1], &com_address, sizeof(com_address));
    memcpy(&m_content[2], &cmd, sizeof(cmd));
    memcpy(&m_content[3], &state, sizeof(state));
    memcpy(&m_content[4], childData, childDataLength);
    CalculateCRC(&m_content[0], m_headLength + childDataLength);
}
unsigned char* SendMessagePackage::GetContent()
{
    return m_content.data();
}
uint8_t SendMessagePackage::GetContentLength()
{
    return static_cast<uint8_t>(m_content.size());
}

ReceiveMessagePackage::ReceiveMessagePackage()
    : MessagePackage(3, 2)
{}

void ReceiveMessagePackage::SetLength(uint8_t dataLength)
{
    m_content.resize(1 + dataLength);
    m_content[0] = dataLength;
}

void ReceiveMessagePackage::SetContent(unsigned char* data)
{
    memcpy(&m_content[1], data, m_content.size() - 1);
}

ReturnErrorCode ReceiveMessagePackage::GetState()
{
    return static_cast<ReturnErrorCode>(m_content[2]);
}

unsigned char* ReceiveMessagePackage::GetData()
{
    return &m_content[m_headLength];
}

uint8_t ReceiveMessagePackage::GetDataLength()
{
    return static_cast<uint8_t>(m_content.size() - m_headLength - m_tailLength);
}

uint8_t ReceiveMessagePackage::GetAddress()
{
    return m_content[1];
}

uint8_t ReceiveMessagePackage::GetLength()
{
    return m_content[0];
}

uint16_t ReceiveMessagePackage::GetCRC()
{
    uint16_t crc = 0;
    memcpy(&crc, &(m_content[m_content.size() - m_tailLength]), sizeof(crc));
    return crc;
}

bool ReceiveMessagePackage::CheckCRC()
{
    uint16_t resultCRC = GetCRC();
    CalculateCRC(&m_content[0], m_content.size() - m_tailLength);
    uint16_t calculateCRC = GetCRC();
    return resultCRC == calculateCRC;
}

void ReceiveMessagePackage::Clear()
{
    m_content.clear();
}
