#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "include/libartery/Artery.h"

#include <QBitArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewDataReceived(UBYTE slaveAddress,
                           UWORD dataAddress, const QByteArray value);
    void on_pushButtonConnect_clicked();

    void on_pushButtonDisplayText_clicked();

    void on_pushButtonDefaultDisplay_clicked();

    void on_pushButtonZoneOperate_clicked();

    void on_pushButtonRandomZoneOn_clicked();

    void on_pushButtonAvailablePorts_clicked();

    void on_pushButtonDisplayMoveRight_clicked();

    void on_pushButtonDisplayMoveLeft_clicked();

private:
    Ui::MainWindow *ui;
    Artery artery;
        UBYTE slaveAddress;
    int displayMoveIndex;
        bool connected;
};
#endif // MAINWINDOW_H
