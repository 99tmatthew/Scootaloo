#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QBluetoothDeviceInfo>

#include "bluetoothmanager.h"
#include "listupdatethread.h"

class BluetoothManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setBluetoothManager(BluetoothManager *bluetoothManager);
    void addDeviceName(QString name);

private slots:
    void on_BluetoothSearch_clicked();
    void on_deviceList_itemActivated(QListWidgetItem *item);
    void on_pushButton_2_clicked();
    void on_clearDeviceList_clicked();
    void on_sendText_clicked();

    void on_target1_clicked();

    void on_target2_clicked();

    void on_target3_clicked();

    void on_target4_clicked();

    void on_target5_clicked();

private:

    Ui::MainWindow *ui;

    QColor inactiveTxt;
    QColor inactiveBg;
    QColor activeTxt;
    QColor activeBg;

    BluetoothManager *manager;
    UpdateListManager *listManager;

    void startBluetoothScan();
    void stopBluetoothScan();

};
#endif // MAINWINDOW_H
