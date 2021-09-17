#ifndef COMMANDSTRUCT_H
#define COMMANDSTRUCT_H

#include <stdint.h>
#include <vector>
#include <iostream>
#include <string.h>
#pragma pack(push, 1)

class ChildReceiveData
{
public:
    virtual ~ChildReceiveData();
    virtual void SetData(unsigned char* data, size_t dataLength) = 0;
};

class GetReaderInformationData:public ChildReceiveData
{
public:
    GetReaderInformationData();
    virtual ~GetReaderInformationData();
    void SetData(unsigned char* data, size_t dataLength);
    uint16_t GetVersion() const;
    uint8_t GetAntennaCount() const;
    uint8_t GetRFU() const;
    uint8_t GetReaderType() const;
    uint16_t GetTreatyType() const;
    uint8_t GetInventoryScanTime() const;
private:
    uint16_t m_version;
    uint8_t m_antennaCount;
    uint8_t m_RFU;
    uint8_t m_readerType;
    uint16_t m_treatyType;
    uint8_t m_inventoryScanTime;
};

class InventoryData:public ChildReceiveData
{
public:
    InventoryData();
    virtual ~InventoryData();
    void SetData(unsigned char* data, size_t dataLength);
    std::string& GetEPC();
    std::string& GetRSSI();
private:
    std::string epc;
    std::string RSSI;
};

class ChildSendData
{
public:
    ChildSendData();
    unsigned char* GetContent();
    size_t GetContentLength();
protected:
    std::vector<unsigned char> m_content;
};

class ChooseOrderSendData:public ChildSendData
{
public:
    ChooseOrderSendData(int ant, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate);
};


class ISO18000_3_SendData:public ChildSendData
{
public:
    ISO18000_3_SendData(uint8_t flag, uint8_t session, uint8_t Q);
};

#pragma pack(pop)

#endif // COMMANDSTRUCT_H
