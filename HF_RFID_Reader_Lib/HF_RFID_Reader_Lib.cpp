#include "HF_RFID_Reader_Lib.h"
#include "SerialDevice.h"
#include "NetworkDevice.h"
//#include <QtEndian>
#define MASK_BIT 8
#define PARITY 'N'
#define STOP_BIT 1

std::vector<uint8_t> baudValue = {0x00, 0x01, 0x02, 0x03};
int32_t swap_endian(int32_t val)
{
    return ((val >> 24) & 0xFF)|((val >> 8) & 0xFF00) | ((val << 8) & 0xFF0000) | ((val << 24) & 0xFF000000) ;
}


HF_RFID_Reader_Lib::HF_RFID_Reader_Lib()
    : m_baseDevice(nullptr)
{

}

ReturnErrorCode HF_RFID_Reader_Lib::SerialConnect(const char* usbPath, GetReaderInformationData& getReaderInfomationData)
{
    SerialDevice* serialDevice = new SerialDevice;
    ReturnErrorCode errorCode = serialDevice->Open(usbPath);
    if (errorCode!= ReturnErrorCode::ERROR_CODE_SUCCESS)
    {
        delete serialDevice;
        serialDevice = nullptr;
        return errorCode;
    }

    for(size_t i = 0 ; i < baudValue.size(); ++i)
    {
        errorCode = serialDevice->SetConfig(baudValue[i], MASK_BIT, PARITY, STOP_BIT);
        if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
            break;

        errorCode = serialDevice->GetReaderInfomation(getReaderInfomationData);
        if( errorCode == ReturnErrorCode::ERROR_CODE_SUCCESS)
        {
            m_baseDevice = serialDevice;
            break;
        }
    }

    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
    {
        delete serialDevice;
        serialDevice = nullptr;
    }
    return errorCode;
}

ReturnErrorCode HF_RFID_Reader_Lib::NetworkConnect(std::string ip, uint16_t port, GetReaderInformationData& getReaderInfomationData)
{
    NetworkDevice* networkDevice = new NetworkDevice;
    ReturnErrorCode errorCode = networkDevice->Open(ip, port);
    if (errorCode!= ReturnErrorCode::ERROR_CODE_SUCCESS)
    {
        delete networkDevice;
        networkDevice = nullptr;
        return errorCode;
    }

    errorCode = networkDevice->GetReaderInfomation(getReaderInfomationData);
    if( errorCode == ReturnErrorCode::ERROR_CODE_SUCCESS)
        m_baseDevice = networkDevice;
    return errorCode;
}

ReturnErrorCode HF_RFID_Reader_Lib::SetParseMode(const bool speedEnable, const bool baudEnable, const uint8_t baud, const PriorMode depthOrWidth)
{
    ReturnErrorCode errorCode = m_baseDevice->SetParseMode(speedEnable, baudEnable, baud, depthOrWidth);
    if (errorCode!= ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if (strcmp(typeid(m_baseDevice).name(), "SerialDevice") == 0)
        errorCode = static_cast<SerialDevice*>(m_baseDevice)->SetConfig(baud, MASK_BIT, PARITY, STOP_BIT);
    return errorCode;
}

ReturnErrorCode HF_RFID_Reader_Lib::GetParseMode(bool& speedEnable, uint8_t& baud, PriorMode& depthOrWidth)
{
    return m_baseDevice->GetParseMode(speedEnable, baud, depthOrWidth);
}

ReturnErrorCode HF_RFID_Reader_Lib::Close()
{
    ReturnErrorCode errorCode = ReturnErrorCode::ERROR_CODE_SUCCESS;
    if (m_baseDevice != nullptr)
    {
        if (strcmp(typeid(*m_baseDevice).name(), typeid(SerialDevice).name()) == 0)
            errorCode = static_cast<SerialDevice*>(m_baseDevice)->Close();
        else if (strcmp(typeid(*m_baseDevice).name(), typeid(NetworkDevice).name()) == 0)
            errorCode = static_cast<SerialDevice*>(m_baseDevice)->Close();
    }
    return errorCode;
}

ReturnErrorCode HF_RFID_Reader_Lib::SetPower(const uint8_t powerIndex)
{
    if (powerIndex > 13)
        return ReturnErrorCode::POWER_SET_ERROR;
    return m_baseDevice->SetPower(powerIndex);
}

ReturnErrorCode HF_RFID_Reader_Lib::GetPower(uint8_t& powerIndex, uint8_t& RUF)
{
    powerIndex = 0;
    RUF = 0;
    return m_baseDevice->GetPower(powerIndex, RUF);
}

ReturnErrorCode HF_RFID_Reader_Lib::ChooseOrder(std::vector<uint8_t> antennaIDs, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate)
{
    int32_t antennas = 0;
    for(size_t i = 0; i < antennaIDs.size(); ++i)
    {
        int temp = 1;
        temp <<= antennaIDs[i] - 1;
        antennas |= temp;
    }
    //antennas = qToBigEndian(antennas);
    antennas = swap_endian(antennas);
    return m_baseDevice->ChooseOrder(antennas, selTarget, selAction, maskMem, maskAdr, maskLen, maskData, truncate);
}

ReturnErrorCode HF_RFID_Reader_Lib::SetActiveAntenna(uint8_t antennaID)
{
    return m_baseDevice->SetActiveAntenna(antennaID);
}

ReturnErrorCode HF_RFID_Reader_Lib::Inventory(uint8_t flag, uint8_t session, uint8_t Q, std::vector<InventoryData>& inventoryDatas, ISO18000_3_ErrorCode& iso18000_3_errorCode)
{
    ReturnErrorCode re =  m_baseDevice->Inventory(flag, session, Q, inventoryDatas, iso18000_3_errorCode);
    return re;
}
