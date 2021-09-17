#include "BaseDevice.h"

BaseDevice::BaseDevice(uint8_t address)
    : m_fd(0)
    , m_address(address)
{

}

BaseDevice::~BaseDevice()
{

}

ReturnErrorCode BaseDevice::GetReaderInfomation(GetReaderInformationData& getReaderInformation)
{
    SendMessagePackage sendMessagePackage(m_address, 0x00, 0xf0, nullptr, 0);
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength, 500);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    if (receiveContent.GetState() == ReturnErrorCode::ERROR_CODE_SUCCESS)
    {
        getReaderInformation.SetData(receiveContent.GetData(), receiveContent.GetDataLength());
        m_address = receiveContent.GetAddress();
    }
    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::SetParseMode(const bool speedEnable, const bool baudEnable, const uint8_t baudValue, const PriorMode depthOrWidth)
{
    char data = 0x00;
    if (depthOrWidth == PriorMode::WIDTH_PRIOR)
        data |= 0x01;
    else
        data |= 0x00;

    data |= (baudValue << 1);
//    if (baudValue == 19200)
//        data |= 0x00;
//    else if(baudValue == 38400)
//        data |= 0x02;
//    else if(baudValue == 57600)
//        data |= 0x04;
//    else if(baudValue == 115200)
//        data |= 0x06;
    if(baudEnable == true)
        data |= 0x08;
    if(speedEnable == true)
        data |= 0x20;
    data |= 0xa0;

    SendMessagePackage sendMessagePackage(m_address, 0x70, 0xf0, &data, sizeof(data));
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::GetParseMode(bool& speedEnable, uint8_t& baudValue, PriorMode& depthOrWidth)
{
    SendMessagePackage sendMessagePackage(m_address, 0x71, 0xf0, nullptr, 0);
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    char data = 0x00;
    memcpy(&data, receiveContent.GetData(), 1);
    depthOrWidth = static_cast<PriorMode>(data & 0x01);
    baudValue = (data & 0x06) >> 1;
//    if (baudValue == 0)
//        baudValue = 19200;
//    else if (baudValue == 1)
//        baudValue = 38400;
//    else if (baudValue == 2)
//        baudValue = 57600;
//    else if (baudValue == 3)
//        baudValue = 115200;
    speedEnable = data & 0x20;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::SetPower(uint8_t powerIndex)
{
    SendMessagePackage sendMessagePackage(m_address, 0x20, 0xf0, (char*)&powerIndex, sizeof(powerIndex));
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::GetPower(uint8_t& powerIndex, uint8_t& RFU)
{
    SendMessagePackage sendMessagePackage(m_address, 0x22, 0xf0, nullptr, 0);
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    powerIndex = (*receiveContent.GetData()) & 0x1F;
    RFU = (*receiveContent.GetData()) & 0x80;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::ChooseOrder(int antenna, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate)
{
    ChooseOrderSendData chooseOrderSendData(antenna, selTarget, selAction, maskMem, maskAdr, maskLen, maskData, truncate);
    SendMessagePackage sendMessagePackage(m_address, 0x72, 0x20, (char*)chooseOrderSendData.GetContent(), chooseOrderSendData.GetContentLength());
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength, 5000);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::SetActiveAntenna(uint8_t antennaID)
{
    SendMessagePackage sendMessagePackage(m_address, 0x08, 0xf0, (char*)&antennaID, sizeof(antennaID));
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    ReceiveMessagePackage receiveContent;
    errorCode = Read(receiveContent, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    if(!receiveContent.CheckCRC())
        return ReturnErrorCode::READ_ERROR;

    return receiveContent.GetState();
}

ReturnErrorCode BaseDevice::Inventory(uint8_t flag, uint8_t session, uint8_t Q, std::vector<InventoryData>& inventoryDatas, ISO18000_3_ErrorCode& iso18000_3_errorCode)
{
    inventoryDatas.clear();
    iso18000_3_errorCode = ISO18000_3_ErrorCode::ERROR_CODE_SUCCESS;

    ISO18000_3_SendData iso18000_3(flag, session, Q);
    SendMessagePackage sendMessagePackage(m_address, 0x71, 0x20, (char*)iso18000_3.GetContent(), iso18000_3.GetContentLength());
    int actuallyLength = 0;
    ReturnErrorCode errorCode = Write(sendMessagePackage, actuallyLength);
    if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return errorCode;

    InventoryData inventoryResult;
    while (1)
    {
        ReceiveMessagePackage receiveContent;
        errorCode = Read(receiveContent, actuallyLength, 100);
        if (errorCode != ReturnErrorCode::ERROR_CODE_SUCCESS)
            return errorCode;

        if(receiveContent.GetState() == ReturnErrorCode::ERROR_CODE_SUCCESS)
        {
            inventoryResult.SetData(receiveContent.GetData(), receiveContent.GetDataLength());
            inventoryDatas.push_back(inventoryResult);
        }
        else if (receiveContent.GetState() == ReturnErrorCode::ERROR_CODE_NO_TAG)
            break;
        else
        {
            if (receiveContent.GetState() == ReturnErrorCode::ERROR_CODE_OPERATOR_ERROR || receiveContent.GetState() == ReturnErrorCode::ERROR_CODE_TAG_RETURN_ERROR_CODE)
            {
                uint8_t res =0;
                memcpy(&res, receiveContent.GetData(), 1);
                iso18000_3_errorCode = static_cast<ISO18000_3_ErrorCode>(res);
            }
            return receiveContent.GetState();
        }
    }
    return ReturnErrorCode::ERROR_CODE_NO_TAG;
}
