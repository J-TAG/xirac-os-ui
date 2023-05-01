#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QRandomGenerator>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), displayMoveIndex(0), connected(false)
{
    ui->setupUi(this);

    connect(&artery, &Artery::newDataReceived, this, &MainWindow::onNewDataReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewDataReceived(UBYTE slaveAddress,
                                   UWORD dataAddress, const QByteArray value)
{
    ui->plainTextEditLog->appendPlainText(QString("onNewDataReceived: %1 %2 ")
                                              .arg(slaveAddress)
                                              .arg(dataAddress)
                                              .append(value.toHex()));
}

void MainWindow::on_pushButtonConnect_clicked()
{
    const QString portToConnect = ui->lineEditPortName->text();
    // Update member slave address
    this->slaveAddress = ui->lineEditSlaveAddress->text().toUInt(nullptr, 16);

    ui->plainTextEditLog->appendPlainText(QString("Connecting to: %1").arg(portToConnect));

    // Connect to device
    if (artery.connect(portToConnect)) {
        ui->plainTextEditLog->appendPlainText("Connected to serial port.");
        connected = true;
    } else {
        ui->plainTextEditLog->appendPlainText("Can't connect to serial port.");
        connected = false;
    }
}


void MainWindow::on_pushButtonDisplayText_clicked()
{
    artery.displayText(slaveAddress, ui->lineEditDisplayText->text());
}


void MainWindow::on_pushButtonDefaultDisplay_clicked()
{
    artery.displayDefault(slaveAddress);
}


void MainWindow::on_pushButtonZoneOperate_clicked()
{
    if (this->ui->checkBoxToggle->isChecked()) {
        // On
        artery.presetSingleRegister(this->slaveAddress, 0x36c, 1);
    } else {
        // Off
        artery.presetSingleRegister(this->slaveAddress, 0x36c, 0);
    }
}


void MainWindow::on_pushButtonRandomZoneOn_clicked()
{
    // Generate a random number
    int val = QRandomGenerator::global()->bounded(0x1, 0xff);

    // Set a register
    artery.presetSingleRegister(slaveAddress, 0x036c, val);
}

void MainWindow::on_pushButtonAvailablePorts_clicked()
{
    // Show available serial ports
    QSerialPortInfo portInfo;
    ui->plainTextEditLog->appendPlainText("Available ports:");
    const auto ports = portInfo.availablePorts();
    for (const auto& port : ports) {
        ui->plainTextEditLog->appendPlainText(port.portName());
    }
}

void MainWindow::on_pushButtonDisplayMoveRight_clicked()
{
    // Move Xirac display to right
    if(this->displayMoveIndex>128) {
        this->displayMoveIndex=128;
    }
    ui->plainTextEditLog->appendPlainText(QString("Display index: %1").arg(this->displayMoveIndex));
    artery.presetSingleRegister(slaveAddress, xirac::variables::Display::DisplayBufferPointer,
                                ++this->displayMoveIndex);
}


void MainWindow::on_pushButtonDisplayMoveLeft_clicked()
{
    // Move Xirac display to left
    if(this->displayMoveIndex<1) {
        this->displayMoveIndex = 1;
    }
    ui->plainTextEditLog->appendPlainText(QString("Display index: %1").arg(this->displayMoveIndex));
    artery.presetSingleRegister(slaveAddress, xirac::variables::Display::DisplayBufferPointer,
                                --this->displayMoveIndex);
}

