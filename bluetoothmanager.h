#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

class BluetoothManager : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothManager(QObject *parent = 0);
    ~BluetoothManager();

    void changeDevice(QBluetoothAddress address);
    void changeDevice(QString name);
    void sendMessage(QString message);
    void sendMessage(const char* character);
    void startScan();
    void stopScan();

    QList<QBluetoothDeviceInfo> getDiscoveredDevices();
    QString currentDeviceName();
    bool scanActive();

public slots:
    void deviceDiscovered(QBluetoothDeviceInfo deviceInfo);

private:
    QBluetoothLocalDevice localDevice;
    QBluetoothUuid *uuid;
    QString uuidString;
    QBluetoothServiceInfo *serviceInfo;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothAddress currentDevice;
    QBluetoothDeviceInfo currentInfo;
    QBluetoothSocket *socket;

    QString deviceName;

};

#endif // BLUETOOTHMANAGER_H
