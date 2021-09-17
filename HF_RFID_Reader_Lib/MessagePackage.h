#ifndef MESSAGEPACKAGE_H
#define MESSAGEPACKAGE_H

#include <stdint.h>
#include <vector>
#include "ErrorCode.h"
class MessagePackage
{
public:
    MessagePackage(unsigned int headLength, unsigned int tailLength);
    bool Empty();
protected:
    /* data：数据内容
     * dataLength：数据内容长度
    */
    void CalculateCRC(unsigned char* data, size_t dataLength);
protected:
    std::vector<unsigned char> m_content;
    unsigned int m_headLength;
    unsigned int m_tailLength;
};

class SendMessagePackage: public MessagePackage
{
public:
    /*com_address：读写器地址
     * cmd：操作命令符
     * state：操作控制符
     * childData：命令操作数
     * childDataLength：命令操作数长度*/
    SendMessagePackage(uint8_t com_address, char cmd, uint8_t state, char* childData, uint8_t childDataLength);
    unsigned char* GetContent();
    uint8_t GetContentLength();
};

class ReceiveMessagePackage: public MessagePackage
{
public:
    ReceiveMessagePackage();
    void SetLength(uint8_t dataLength);
    void SetContent(unsigned char* data);
    ReturnErrorCode GetState();
    unsigned char* GetData();
    uint8_t GetDataLength();
    uint8_t GetAddress();
    uint8_t GetLength();
    uint16_t GetCRC();
    bool CheckCRC();
    void Clear();
};


#endif // MESSAGEPACKAGE_H
