#include "DataStruct.h"

ChildReceiveData::~ChildReceiveData()
{}

GetReaderInformationData::GetReaderInformationData()
    : m_version(0)
    , m_antennaCount(0)
    , m_RFU(0)
    , m_readerType(0)
    , m_treatyType(0)
    , m_inventoryScanTime(0)
{

}

GetReaderInformationData::~GetReaderInformationData()
{

}

void GetReaderInformationData::SetData(unsigned char* data, size_t dataLength)
{
    if (dataLength == 0)
        return;

    unsigned char* p = data;
    memcpy(&m_version, p, sizeof(m_version));
    p += sizeof(m_version);
    memcpy(&m_antennaCount, p, sizeof(m_antennaCount));
    p += sizeof(m_antennaCount);
    memcpy(&m_RFU, p, sizeof(m_RFU));
    p += sizeof(m_RFU);
    memcpy(&m_readerType, p, sizeof(m_readerType));
    p += sizeof(m_readerType);
    memcpy(&m_treatyType, p, sizeof(m_treatyType));
    p += sizeof(m_treatyType);
    memcpy(&m_inventoryScanTime, p, sizeof(m_inventoryScanTime));

    m_antennaCount = m_antennaCount & 0x1F;
}

uint16_t GetReaderInformationData::GetVersion() const
{
    return m_version;
}

uint8_t GetReaderInformationData::GetAntennaCount() const
{
    return m_antennaCount;
}

uint8_t GetReaderInformationData::GetRFU() const
{
    return m_RFU;
}
uint8_t GetReaderInformationData::GetReaderType() const
{
    return m_readerType;
}

uint16_t GetReaderInformationData::GetTreatyType() const
{
    return m_treatyType;
}
uint8_t GetReaderInformationData::GetInventoryScanTime() const
{
    return m_inventoryScanTime;
}

InventoryData::InventoryData()
{}

void InventoryData::SetData(unsigned char* data, size_t dataLength)
{
    epc = std::string((char*)data, dataLength - 1);
    RSSI =std::string((char*)(data + dataLength - 1), 1);
}

InventoryData::~InventoryData()
{

}

std::string& InventoryData::GetEPC()
{
    return epc;
}

std::string& InventoryData::GetRSSI()
{
    return RSSI;
}

ChildSendData::ChildSendData()
{}

unsigned char* ChildSendData::GetContent()
{
    return m_content.data();
}

size_t ChildSendData::GetContentLength()
{
    return m_content.size();
}

ChooseOrderSendData::ChooseOrderSendData(int ant, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate)
{
    m_content.resize(11 + maskLen);
    memcpy(&m_content[0], &ant, sizeof(int));
    memcpy(&m_content[4], &selTarget, sizeof(uint8_t));
    memcpy(&m_content[5], &selAction, sizeof(uint8_t));
    memcpy(&m_content[6], &maskMem, sizeof(uint8_t));
    memcpy(&m_content[7], &maskAdr, sizeof(uint16_t));
    memcpy(&m_content[9], &maskLen, sizeof(uint8_t));
    memcpy(&m_content[10], &maskData, sizeof(char) * maskLen);
    memcpy(&m_content[10 + maskLen], &truncate, sizeof(uint8_t));
}

ISO18000_3_SendData::ISO18000_3_SendData(uint8_t flag, uint8_t session, uint8_t Q)
{
    m_content.resize(3);
    memcpy(&m_content[0], &flag, sizeof(uint8_t));
    memcpy(&m_content[1], &session, sizeof(uint8_t));
    memcpy(&m_content[2], &Q, sizeof(uint8_t));
}
