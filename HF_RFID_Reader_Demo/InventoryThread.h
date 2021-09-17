#ifndef INVENTORYTHREAD_H
#define INVENTORYTHREAD_H

#include <QObject>
#include <QString>
class HF_RFID_Reader_Lib;
class InventoryThread:public QObject
{
    Q_OBJECT
public:
    InventoryThread(HF_RFID_Reader_Lib* reader, std::vector<uint8_t> antennaIDs, uint8_t selTarget, uint8_t selAction, uint8_t maskMem, uint16_t maskAdr, uint8_t maskLen, char* maskData, uint8_t truncate, uint8_t flag, uint8_t session, uint8_t Q);
signals:
    void SendInventroyResult(QString epc, QString RSSI, QString antenna);
    void ThreadEnd();
public slots:
    void StartInventory();
    void StopInventory();
private:
    HF_RFID_Reader_Lib* m_reader;
    std::vector<uint8_t> m_antennaIDs;
    uint8_t m_selTarget;
    uint8_t m_selAction;
    uint8_t m_maskMem;
    uint16_t m_maskAdr;
    uint8_t m_maskLen;
    char* m_maskData;
    uint8_t m_truncate;

    uint8_t m_flag;
    uint8_t m_session;
    uint8_t m_Q;

    bool m_needStop;
};

#endif // INVENTORYTHREAD_H
