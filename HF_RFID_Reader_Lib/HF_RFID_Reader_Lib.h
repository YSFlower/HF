#ifndef HF_RFID_READER_LIB_H
#define HF_RFID_READER_LIB_H

//#include "hf_rfid_reader_lib_global.h"
#include "SerialDevice.h"
#include <string>



class /*HF_RFID_READER_LIBSHARED_EXPORT*/ HF_RFID_Reader_Lib
{
public:
    HF_RFID_Reader_Lib();
    /*串口连接
     * usbPath:设备名称　例如/dev/ttyUSB0
     * getReaderInfomationData：读写器信息*/
    ReturnErrorCode SerialConnect(const char* usbPath, GetReaderInformationData& getReaderInfomationData);

    /*网口连接
     * ip：ip地址
     * port：端口号
     * getReaderInfomationData：读写器信息 */
    ReturnErrorCode NetworkConnect(std::string ip, uint16_t port, GetReaderInformationData& getReaderInfomationData);

    /*设置读写器解析模式
    speedEnable：设置询查加速功能
    baudEnable：波特率设置使能位
    baud：波特率设置　19200=00;38400=01;57600=10;115200=11;
    depthOrWidth：深度优先DEEP_PRIOR　宽度优先WIDTH_PRIOR*/
    ReturnErrorCode SetParseMode(const bool speedEnable, const bool baudEnable, const uint8_t baud, const PriorMode depthOrWidth);

    /*获取解析模式
    speedEnable：设置询查加速功能
    baud：波特率设置　19200=00;38400=01;57600=10;115200=11;
    depthOrWidth：深度优先DEEP_PRIOR　宽度优先WIDTH_PRIOR*/
    ReturnErrorCode GetParseMode(bool& speedEnable, uint8_t& baud, PriorMode& depthOrWidth);

    /*断开连接*/
    ReturnErrorCode Close();

    /*设置功率　powerIndex: 0对应0.5W 1对应1.0W 以此类推 最大到13对应7.0W*/
    ReturnErrorCode SetPower(const uint8_t powerIndex);

    /*获取功率
     * powerIndex：0对应0.5W 1对应1.0W 以此类推 最大到13对应7.0W
     * RUF：不知道是什么。。*/
    ReturnErrorCode GetPower(uint8_t& powerIndex, uint8_t& RUF);

    /*选择命令　每次盘点之前都要调用　本命令可从多个指定天线口发送选择命令。
     * antennaIDs：为具体的天线号数组　天线号从1开始
     * 其他参数不懂　目前采用demo中的默认值 */
    ReturnErrorCode ChooseOrder(std::vector<uint8_t> antennaIDs, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate);

    /*设置当前有效天线　每次盘点需要选择一个天线
    *antennaID：天线号 天线号从1开始*/
    ReturnErrorCode SetActiveAntenna(uint8_t antennaID);

    /*盘点　　目前采用demo中的默认值*/
    ReturnErrorCode Inventory(uint8_t flag, uint8_t session, uint8_t Q, std::vector<InventoryData>& inventoryDatas, ISO18000_3_ErrorCode& iso18000_3_errorCode);

private:    
    /*设备对象*/
    BaseDevice* m_baseDevice;
};

#endif // HF_RFID_READER_LIB_H
