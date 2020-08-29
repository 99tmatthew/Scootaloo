#include "bluetoothmanager.h"
#include <QDebug>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    // Sets up bluetooth for QT
    // The uuidString is necessary, but I'm not sure why this value is the right one, it just works
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    uuidString = "00001101-0000-1000-8000-00805F9B34FB";
    serviceInfo = new QBluetoothServiceInfo();
    serviceInfo->registerService(localDevice.address());
}

void BluetoothManager::deviceDiscovered(QBluetoothDeviceInfo deviceInfo){
    qDebug() << "Found new device: " << deviceInfo.name() <<"(" << deviceInfo.address().toString() << ")";
}

// Changes devices when given a bluetooth address
void BluetoothManager::changeDevice(QBluetoothAddress address){
    socket->connectToService(address, QBluetoothUuid(uuidString), QIODevice::ReadWrite);
}

// Changes devices when given a device name, like from the deviceList
// User needs to make sure there aren't duplicates though
void BluetoothManager::changeDevice(QString name){

    QList<QBluetoothDeviceInfo> foundDevices = getDiscoveredDevices();

    // Iterate through the list to find the name selected in deviceList
    QList<QBluetoothDeviceInfo>::iterator deviceIterator;
    QBluetoothAddress newDevice;
    for(deviceIterator = foundDevices.begin(); deviceIterator!= foundDevices.end(); deviceIterator++){

        if (deviceIterator->name() == name){
            newDevice = deviceIterator->address();
            qDebug() << "Found" << deviceIterator->name() << "address" << deviceIterator->address().toString();
        }
    };

    deviceName = name;
    changeDevice(newDevice);
}

// Sends a message over bluetooth to the connected device
void BluetoothManager::sendMessage(QString message){
    socket->write(message.toStdString().c_str());
    qDebug() << "Sending" << message << "to device.";
}

// Sends a message over bluetooth to the connected device
void BluetoothManager::sendMessage(const char* character)
{
    socket->write(character);
    qDebug() << "Sending" << QString::fromStdString(character) << "to device.";
}

// Stop searching for new devices
void BluetoothManager::stopScan(){
    if (discoveryAgent->isActive()){
        discoveryAgent->stop();
        qDebug() << "Stopping scan";
    }
}

// Start searching for new devices
void BluetoothManager::startScan(){
    discoveryAgent->start();
    qDebug() << "Starting scan";
}

QList<QBluetoothDeviceInfo> BluetoothManager::getDiscoveredDevices(){
    return discoveryAgent->discoveredDevices();
}

QString BluetoothManager::currentDeviceName(){
    return deviceName;
}

bool BluetoothManager::scanActive(){

    return discoveryAgent->isActive();
}

BluetoothManager::~BluetoothManager(){
    delete discoveryAgent;
    delete socket;
}
