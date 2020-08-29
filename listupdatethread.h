#ifndef LISTUPDATETHREAD_H
#define LISTUPDATETHREAD_H

#include <QObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QThread>
#include "bluetoothmanager.h"

class UpdateListWorker : public QObject
{
    Q_OBJECT

public:
    UpdateListWorker(QListWidget *deviceList, BluetoothManager *bluetooth);
    void stop();
    bool isRunning();

public slots:
    void run();

private:
    bool runThread;
    QListWidget *list;
    BluetoothManager *bluetoothManager;

};

class UpdateListManager : public QObject
{
    Q_OBJECT

public:
    UpdateListManager(QListWidget *deviceList, BluetoothManager *manager);
    ~UpdateListManager();
    void runThread();
    void stopThread();

private:
    UpdateListWorker *worker;
    QListWidget *list;
    BluetoothManager *bluetoothManager;
    QThread workerThread;

};

#endif // LISTUPDATETHREAD_H
