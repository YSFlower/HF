#ifndef BASEDEVICE_H
#define BASEDEVICE_H
#include <stddef.h>
#include "DataStruct.h"
#include "ErrorCode.h"
#include "MessagePackage.h"
#include <vector>

/*设备连接基类*/
class BaseDevice
{
public:
    /*address：写入地址　默认0xFF*/
    BaseDevice(uint8_t address = 0xFF);
    virtual ~BaseDevice();
    /*获取读写器信息*/
    ReturnErrorCode GetReaderInfomation(GetReaderInformationData& antennaCount);

    /*设置读写器解析模式*/
    ReturnErrorCode SetParseMode(const bool speedEnable, const bool baudEnable, const uint8_t baudValue, const PriorMode depthOrWidth);

    /*获取解析模式*/
    ReturnErrorCode GetParseMode(bool& speedEnable, uint8_t& baudValue, PriorMode& depthOrWidth);

    /*设置功率*/
    ReturnErrorCode SetPower(uint8_t powerIndex);

    /*获取功率*/
    ReturnErrorCode GetPower(uint8_t& powerIndex, uint8_t& RFU);

    /*选择命令*/
    ReturnErrorCode ChooseOrder(int antenna, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate);

    /*设置当前有效天线*/
    ReturnErrorCode SetActiveAntenna(uint8_t antennaID);

    /*盘点*/
    ReturnErrorCode Inventory(uint8_t flag, uint8_t session, uint8_t Q, std::vector<InventoryData>& inventoryDatas, ISO18000_3_ErrorCode& iso18000_3_errorCode);

    /*写消息*/
    virtual ReturnErrorCode Write(SendMessagePackage& sendContent, int& actuallyWriteLength) = 0;

    /*读消息*/
    virtual ReturnErrorCode Read(ReceiveMessagePackage& receiveContent, int& actuallyReadLength, unsigned int waitMesc = 0) = 0;

    /*关闭*/
    virtual ReturnErrorCode Close() = 0;

protected:    
    /*设备句柄*/
    int m_fd;

    /*写入地址*/
    uint8_t m_address;
};

#endif // BASEDEVICE_H
