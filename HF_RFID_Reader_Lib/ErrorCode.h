#ifndef ERRORCODE_H
#define ERRORCODE_H
enum class ReturnErrorCode
{
    /*打开错误*/
    OPEN_ERROR = -1,
    /*tcgetattr　错误*/
    GET_TERMINAL_PARAMETER_ERROR = -2,
    /*fcntl 错误*/
    CONTROL_FILE_ERROR = -3,
    /*波特率选择错误*/
    CHOOSE_BAUD_RATE_ERROR = -4,
    /*数据位错误*/
    CHOOSE_MASK_BIT_ERROR = -5,
    /*奇偶校验位错误*/
    CHOOSE_PARITY_ERROR = -6,
    /*停止位错误*/
    CHOOSE_STOP_BIT_ERROR = -7,
    /*tcsetattr 错误*/
    SET_TERMINAL_PARAMETER_ERROR = -8,
    /*弃用*/
    RESTORE_TERMINAL_PARAMETER_ERROR = -9,
    /*刷新缓冲区错误*/
    FLUSH_ERROR = -10,
    /*写错误*/
    WRITE_ERROR = -11,
    /*读错误*/
    READ_ERROR = -12,
    /*select 错误*/
    FILE_DESCRIPTOR_ERROR = -13,
    /*关闭错误*/
    CLOSE_ERROR = -14,
    /*功率设置错误*/
    POWER_SET_ERROR = -15,
    /*无错误，API调用成功*/
    ERROR_CODE_SUCCESS = 0x00,
    /*命令操作数长度错误*/
    ERROR_CODE_ORDER_OPERATOR_COUNT_LENGTH_ERROR = 0x01,
    /*操作命令不支持*/
    ERROR_CODE_OPERAOTR_ORDER_NO_SUPPORT = 0x02,
    /*感应场处于关闭状态*/
    ERROR_CODE_RF_CLOSE = 0x05,
    /*EEPROM操作出错*/
    ERROR_CODE_EEPROM_ERROR = 0x06,
    /*指定的最大询查响应时间溢出*/
    RERROR_CODE_INVENTORY_SCAN_TIME_OVERFLOW = 0x0A,
    /*在最大询查响应时间内无得到所有电子标签的UID*/
    ERROR_CODE_INVENTORY_SCAN_NOT_GET_TAG_UID = 0x0B,
    /*ISO error*/
    ERROR_CODE_ISO_ERROR = 0x0C,
    /*无电子标签*/
    ERROR_CODE_NO_TAG = 0x0E,
    /*操作出错（出现此错误，查看ErrorCode错误代码表）*/
    ERROR_CODE_OPERATOR_ERROR = 0x0F,
    /*后续还有响应数据块*/
    ERROR_CODE_HAVE_OTHER_RESPONSE_DATA_BLOCK = 0x10,
    /*天线异常进入保护状态*/
    ERROR_CODE_ANTENNA_UNUSUAL_INTO_PROTECT_STATS = 0x11,
    /*温度过高进入保护状态*/
    ERROR_CODE_TEMPERATURE_TOO_HIGH_INTO_PROTECT_STATS = 0x12,
    /*电流过大进入保护状态*/
    ERROR_CODE_ELECTRICITY_TOO_BIG_INTO_PROTECT_STATS = 0x13,
    /*读写器出现严重错误*/
    ERROR_CODE_READER_HAPPEND_SERIOUS_ERROR = 0x14,
    /*网络心跳包*/
    ERROR_CODE_NETWORK_HEARTBEAT = 0x20,
    /*标签操作失败*/
    ERROR_CODE_TAG_OPERATOR_FAIL = 0x21,
    /*销毁密码不能为0*/
    ERROR_CODE_DESTORY_PASSWORD_IS_NOT_ZERO = 0x22,
    /*访问密码不正确*/
    ERROR_CODE_INTERVIEW_PASSWORD_NOT_CORRECT = 0x23,
    /*电子标签返回错误代码*/
    ERROR_CODE_TAG_RETURN_ERROR_CODE = 0x2F,
    /*通讯错误*/
    ERROR_CODE_COMMUNICATION_ERROR = 0x30,
    /*CRC校验错误*/
    ERROR_CODE_CRC_CHECKOUT_ERROR = 0x31,
    /*保留1*/
    ERROR_CODE_RESERVE1 = 0x32,
    /*保留2*/
    ERROR_CODE_RESERVE2 = 0x33,
    /*保留3*/
    ERROR_CODE_RESERVE3 = 0x34,
    /*读写器已被打开*/
    ERROR_CODE_ALREADY_OPEN = 0x35,
    /*保留4*/
    ERROR_CODE_RESERVE4 = 0x36,
    /*无效句柄*/
    ERROR_CODE_INVALID_HANDLE = 0x37,
};

//ISO18000-3-ErrorCode 错误代码表
enum class ISO18000_3_ErrorCode
{
    ERROR_CODE_SUCCESS = -1,
    /*其它错误，全部捕捉未被其它代码覆盖的错误*/
    ERROR_CODE_OTHER_ERROR = 0x00,
    /*存储器超限或不被支持的 PC 值，规定存储位置不存在或标签不支持 PC 值*/
    ERROR_CODE_BEYOND_STORAGE_LIMIT_OR_NOT_SUPPORT_PC_VALUE = 0x03,
    /*存储器锁定，存储位置锁定永久锁定，且不可写入*/
    ERROR_CODE_STORAGE_LOCK = 0x04,
    /*电源不足，标签电源不足，无法执行存储写入操作*/
    ERROR_CODE_POWER_SUPPLY_LACK = 0x0B,
    /*非特定错误，标签不支持特定错误代码*/
    ERROR_CODE_NOT_SPECIFIC_ERROR = 0x0F,
};

enum class PriorMode
{
    /*宽度优先*/
    WIDTH_PRIOR = 0,
    /*深度优先*/
    DEEP_PRIOR = 1,
};
#endif // ERRORCODE_H
