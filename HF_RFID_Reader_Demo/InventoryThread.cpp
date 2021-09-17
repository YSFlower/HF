#include "InventoryThread.h"
#include "HF_RFID_Reader_Lib.h"
#include "DataStruct.h"
InventoryThread::InventoryThread(HF_RFID_Reader_Lib* reader, std::vector<uint8_t> antennaIDs, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate, uint8_t flag, uint8_t session, uint8_t Q)
    : m_reader(reader)
    , m_antennaIDs(antennaIDs)
    , m_selTarget(selTarget)
    , m_selAction(selAction)
    , m_maskMem(maskMem)
    , m_maskAdr(maskAdr)
    , m_maskLen(maskLen)
    , m_maskData(new char[maskLen])
    , m_truncate(truncate)
    , m_flag(flag)
    , m_session(session)
    , m_Q(Q)
    , m_needStop(false)

{
    if (maskData != nullptr)
        memcpy(m_maskData, maskData, maskLen);
}

void InventoryThread::StartInventory()
{
    std::vector<InventoryData> inventoryResults;
    ISO18000_3_ErrorCode iso18000_3_errorCode;
    int count = 1;
    //while (!m_needStop)
    while (count--)
    {
        if (m_reader->ChooseOrder(m_antennaIDs, m_selTarget, m_selAction, m_maskMem, m_maskAdr, m_maskLen, m_maskData, m_truncate) != ReturnErrorCode::ERROR_CODE_SUCCESS)
            return;
        for(size_t i = 0; i < m_antennaIDs.size(); ++i)
        {
            if (m_reader->SetActiveAntenna(m_antennaIDs[i] - 1) != ReturnErrorCode::ERROR_CODE_SUCCESS)
                return;
            if (m_reader->Inventory(m_flag, m_session, m_Q, inventoryResults, iso18000_3_errorCode) != ReturnErrorCode::ERROR_CODE_NO_TAG)
                return;
            if (iso18000_3_errorCode != ISO18000_3_ErrorCode::ERROR_CODE_SUCCESS)
                return;
            for (size_t j = 0; j < inventoryResults.size(); ++j)
            {
                uint8_t rssi = 0;
                memcpy(&rssi, inventoryResults[j].GetRSSI().data(), 1);
                emit SendInventroyResult(QString(QByteArray::fromStdString(inventoryResults[j].GetEPC()).toHex()), QString::number(rssi), QString::fromLocal8Bit("天线") + QString::number(m_antennaIDs[i]));
            }
        }
    }
    emit ThreadEnd();
}

void InventoryThread::StopInventory()
{
    m_needStop = true;
}
