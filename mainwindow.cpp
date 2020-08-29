#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controls.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    activeBg = Qt::green;
    activeTxt = Qt::black;
    inactiveBg = Qt::white;
    inactiveTxt = Qt::black;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Adds and formats new device names to the device list
void MainWindow::addDeviceName(QString name){
    QListWidgetItem *label;
    label = new QListWidgetItem(name, ui->deviceList);

    label->setForeground(inactiveTxt);
    label->setBackground(inactiveBg);
}

// Starts the device discovery process.
void MainWindow::on_BluetoothSearch_clicked()
{
    if (manager->scanActive()){
        stopBluetoothScan();
    }

    startBluetoothScan();
}

// Manages connecting to a new device
// Triggered when user activates a device name in the deviceList
void MainWindow::on_deviceList_itemActivated(QListWidgetItem *item)
{
    QString currentDeviceName = manager->currentDeviceName();

    // If there is already a device selected then we need to clear out the old one.
    if (!currentDeviceName.isNull()){

        QList<QListWidgetItem*> listRows;
        listRows = ui->deviceList->findItems(currentDeviceName, Qt::MatchExactly);

        QListWidgetItem* lastRow;
        lastRow = listRows.front();
        lastRow->setBackground(inactiveBg);
        lastRow->setForeground(inactiveTxt);
    }

    // Color the new text
    item->setBackground(activeBg);
    item->setForeground(activeTxt);

    // Change the current device name and store it.
    currentDeviceName = item->text();

    manager->changeDevice(currentDeviceName);
}

void MainWindow::setBluetoothManager(BluetoothManager *bluetoothManager){
    manager = bluetoothManager;
};

void MainWindow::on_pushButton_2_clicked()
{
    stopBluetoothScan();
}

void MainWindow::on_clearDeviceList_clicked()
{
    ui->deviceList->clear();
    stopBluetoothScan();
    qDebug() << "Clearing list";
}

void MainWindow::on_sendText_clicked()
{
    // Send a message from the input text line to the connected device
    QString text = ui->textInput->text();
    manager->sendMessage(text);
}

void MainWindow::startBluetoothScan(){
    // Create a new UpdateListManager to manage a thread that adds discovered devices
    // found from the bluetooth device
    listManager = new UpdateListManager(ui->deviceList, manager);
    listManager->runThread();

    // Disable the list so a new device can't be connected while more are still being discovered.
    ui->deviceList->setDisabled(true);
}

void MainWindow::stopBluetoothScan(){
    // Stop the scan and allow the user to connect to a device again
    listManager->stopThread();
    ui->deviceList->setDisabled(false);
}

// The next five events just respond to GUI button clicks and send targetting commands to the bluetooth device

void MainWindow::on_target1_clicked()
{
    manager->sendMessage(Controls::TARGET1);
}

void MainWindow::on_target2_clicked()
{
    manager->sendMessage(Controls::TARGET2);
}

void MainWindow::on_target3_clicked()
{
    manager->sendMessage(Controls::TARGET3);
}

void MainWindow::on_target4_clicked()
{
    manager->sendMessage(Controls::TARGET4);
}

void MainWindow::on_target5_clicked()
{
    manager->sendMessage(Controls::TARGET5);
}
