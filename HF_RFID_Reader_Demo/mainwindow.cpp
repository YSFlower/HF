#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include "InventoryThread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_reader(nullptr)
    , m_thread(nullptr)
    , m_inventoryThread(nullptr)

{
    ui->setupUi(this);
    connect(ui->serialStartButton, &QPushButton::clicked, this, &MainWindow::SerialConnectDevice);
    connect(ui->networkStartButton, &QPushButton::clicked, this, &MainWindow::NetworkConnectDevice);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::CloseDevice);
    connect(ui->inventoryButton, &QPushButton::clicked, this, &MainWindow::Inventory);
    connect(ui->setPowerButton, &QPushButton::clicked, this, &MainWindow::SetPower);
    connect(ui->setBaudButton, &QPushButton::clicked, this, &MainWindow::SetBaud);
    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 300);
    ui->tableWidget->setColumnWidth(2, 70);
    ui->tableWidget->setColumnWidth(3, 70);
    ui->tableWidget->setColumnWidth(4, 70);
    ui->setBaudButton->setEnabled(false);
    ui->baudComboBox->setEnabled(false);
    ui->closeButton->setEnabled(false);
    ui->setPowerButton->setEnabled(false);
    ui->inventoryButton->setEnabled(false);
    ui->powerComboBox->setEnabled(false);
    ui->powerComboBox->setEditable(false);
    ui->inventoryCountLabel->setText("0");
}

MainWindow::~MainWindow()
{
    delete m_reader;
    m_reader = nullptr;
    delete ui;
}


void MainWindow::SerialConnectDevice()
{
    if (m_reader != nullptr)
        return;

    m_reader = new HF_RFID_Reader_Lib;
    GetReaderInformationData getReaderInfomationData;
    if (m_reader->SerialConnect("/dev/ttyUSB0", getReaderInfomationData) != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return;
    QListWidgetItem* listWidgetItem = nullptr;
    for (int i = 0; i < getReaderInfomationData.GetAntennaCount(); ++i)
    {
        listWidgetItem = new QListWidgetItem(QString::fromLocal8Bit("天线") + QString::number(i + 1));
        if(i == 0 || i >= 16 && i <= 28)
            listWidgetItem->setCheckState(Qt::CheckState::Checked);
        else
            listWidgetItem->setCheckState(Qt::CheckState::Unchecked);
        ui->antennaItemListWidget->addItem(listWidgetItem);
    }

    uint8_t power = 0;
    uint8_t RUF = 0;
    m_reader->GetPower(power, RUF);
    ui->powerComboBox->setCurrentIndex(power);

    m_reader->GetParseMode(m_speedEnable, m_baud, m_priorMode);
    ui->baudComboBox->setCurrentIndex(m_baud);

    ui->baudComboBox->setEnabled(true);
    ui->setBaudButton->setEnabled(true);
    ui->networkStartButton->setEnabled(false);
    ui->serialStartButton->setEnabled(false);
    ui->closeButton->setEnabled(true);
    ui->setPowerButton->setEnabled(true);
    ui->inventoryButton->setEnabled(true);
    ui->powerComboBox->setEnabled(true);
}

void MainWindow::NetworkConnectDevice()
{
    if (m_reader != nullptr)
        return;

    m_reader = new HF_RFID_Reader_Lib;
    GetReaderInformationData getReaderInfomationData;
    if (m_reader->NetworkConnect("192.168.1.192", 6000, getReaderInfomationData) != ReturnErrorCode::ERROR_CODE_SUCCESS)
        return;
    QListWidgetItem* listWidgetItem = nullptr;
    for (int i = 0; i < getReaderInfomationData.GetAntennaCount(); ++i)
    {
        listWidgetItem = new QListWidgetItem(QString::fromLocal8Bit("天线") + QString::number(i + 1));
        if(i == 0 || i >= 16 && i <= 28)
            listWidgetItem->setCheckState(Qt::CheckState::Checked);
        else
            listWidgetItem->setCheckState(Qt::CheckState::Unchecked);
        ui->antennaItemListWidget->addItem(listWidgetItem);
    }

    uint8_t power = 0;
    uint8_t RUF = 0;
    m_reader->GetPower(power, RUF);
    ui->powerComboBox->setCurrentIndex(power);

    m_reader->GetParseMode(m_speedEnable, m_baud, m_priorMode);
    ui->baudComboBox->setCurrentIndex(m_baud);

    ui->baudComboBox->setEnabled(true);
    ui->setBaudButton->setEnabled(true);
    ui->networkStartButton->setEnabled(false);
    ui->serialStartButton->setEnabled(false);
    ui->closeButton->setEnabled(true);
    ui->powerComboBox->setEnabled(true);
    ui->setPowerButton->setEnabled(true);
    ui->inventoryButton->setEnabled(true);
}

void MainWindow::CloseDevice()
{
    m_reader->Close();
    delete m_reader;
    m_reader = nullptr;
    ui->antennaItemListWidget->clear();

    ui->networkStartButton->setEnabled(true);
    ui->serialStartButton->setEnabled(true);
    ui->baudComboBox->setEnabled(false);
    ui->setBaudButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    ui->powerComboBox->setEnabled(false);
    ui->setPowerButton->setEnabled(false);
    ui->inventoryButton->setEnabled(false);
}

void MainWindow::SetBaud()
{
    m_reader->SetParseMode(m_speedEnable, true, static_cast<uint8_t>(ui->baudComboBox->currentIndex()), m_priorMode);
}

void MainWindow::SetPower()
{
    m_reader->SetPower(ui->powerComboBox->currentIndex());
}

void MainWindow::Inventory()
{
    if (m_reader == nullptr)
        return;

    if (m_thread == nullptr)
    {
        m_thread = new QThread;
        std::vector<uint8_t> antennaIDs;
        for(int i = 0; i < ui->antennaItemListWidget->count(); ++i)
        {
            if (ui->antennaItemListWidget->item(i)->checkState() == Qt::CheckState::Checked)
                antennaIDs.push_back(ui->antennaItemListWidget->item(i)->text().mid(2).toUInt());
        }
        if (antennaIDs.size() == 0)
            return;
        m_inventoryThread = new InventoryThread(m_reader, antennaIDs, 0x02, 0x00, 0x01, 0x2000, 0, nullptr, 0x00, 0x00, 0x02, 0x04);
        m_inventoryThread->moveToThread(m_thread);
        connect(m_thread, &QThread::started, m_inventoryThread, &InventoryThread::StartInventory);
        connect(m_inventoryThread, &InventoryThread::ThreadEnd, this, &MainWindow::ThreadEnd);
        connect(ui->inventoryButton, &QPushButton::clicked, m_inventoryThread, &InventoryThread::StopInventory);
        connect(m_inventoryThread, &InventoryThread::SendInventroyResult, this, &MainWindow::UpdateTable);
        connect(ui->clearInventoryButton, & QPushButton::clicked, this, &MainWindow::ClearInventoryResult);
        ui->inventoryButton->setText(QString::fromLocal8Bit("停止盘点"));
        ui->label->setText("start");
        m_thread->start();
    }
    else
    {
        m_inventoryThread->StopInventory();
        m_thread->quit();
        m_thread->wait();
        disconnect(m_thread, &QThread::started, m_inventoryThread, &InventoryThread::StartInventory);
        disconnect(m_inventoryThread, &InventoryThread::SendInventroyResult, this, &MainWindow::UpdateTable);
        delete m_inventoryThread;
        m_inventoryThread = nullptr;
        delete m_thread;
        m_thread = nullptr;
        ui->inventoryButton->setText(QString::fromLocal8Bit("开始盘点"));
        ui->label->setText("end");
    }
}

void MainWindow::ThreadEnd()
{
    ui->label->setText("end");
}



void MainWindow::ClearInventoryResult()
{
    ui->tableWidget->setRowCount(0);
    m_epcHash.clear();
    ui->inventoryCountLabel->setText("0");
}

void MainWindow::UpdateTable(QString epc, QString RSSI, QString antennaNumber)
{
    QHash<QString, size_t>::iterator itr = m_epcHash.find(epc);
    if (itr != m_epcHash.end())
    {
        QString w =ui->tableWidget->item(itr.value(), 3)->text();
        if (RSSI.toUInt() != ui->tableWidget->item(itr.value(), 3)->text().toUInt())
            ui->tableWidget->item(itr.value(), 3)->setText(RSSI);
        ui->tableWidget->item(itr.value(), 4)->setText(antennaNumber);
        return;
    }
    int rowCount = ui->tableWidget->rowCount();
    m_epcHash.insert(epc, rowCount);

    int t = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    /*创建序号*/
    QTableWidgetItem* indexItem = new QTableWidgetItem(QString::number(rowCount + 1));
    indexItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(rowCount, 0, indexItem);

    /*创建epc*/
    QTableWidgetItem* epcItem = new QTableWidgetItem(epc);
    epcItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(rowCount, 1, epcItem);

    /*创建epc长度*/
    QTableWidgetItem* epcLength = new QTableWidgetItem(QString::number(epc.size()));
    epcLength->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(rowCount, 2, epcLength);

    /*创建RSSI*/
    QTableWidgetItem* RSSIItem = new QTableWidgetItem(RSSI);
    RSSIItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(rowCount, 3, RSSIItem);

    /*创建天线号*/
    QTableWidgetItem* antennaNumberItem = new QTableWidgetItem(antennaNumber);
    antennaNumberItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(rowCount, 4, antennaNumberItem);

    ui->inventoryCountLabel->setText(QString::number(ui->tableWidget->rowCount()));
}
