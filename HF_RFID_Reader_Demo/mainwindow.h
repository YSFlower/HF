#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "HF_RFID_Reader_Lib.h"

namespace Ui {
class MainWindow;
}

class HF_RFID_Reader_Lib;
class QThread;
class InventoryThread;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void SerialConnectDevice();
    void NetworkConnectDevice();
    void SetBaud();
    void SetPower();
    void Inventory();
    void CloseDevice();
    void UpdateTable(QString epc, QString RSSI, QString antenna);
    void ClearInventoryResult();
    void ThreadEnd();
private:
    Ui::MainWindow *ui;
    HF_RFID_Reader_Lib* m_reader;
    QThread* m_thread;
    InventoryThread* m_inventoryThread;
    QHash<QString, size_t> m_epcHash;
    bool m_speedEnable;
    uint8_t m_baud;
    PriorMode m_priorMode;
};

#endif // MAINWINDOW_H
