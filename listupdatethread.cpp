#include "listupdatethread.h"


UpdateListWorker::UpdateListWorker(QListWidget *deviceList, BluetoothManager *bluetooth){
    list = deviceList;
    bluetoothManager = bluetooth;
    runThread = false;
}

void UpdateListWorker::run(){

    runThread = true;
    bluetoothManager->startScan();

    while (runThread){

        list->clear();

        QList<QBluetoothDeviceInfo> discoveredDevices;
        discoveredDevices = bluetoothManager->getDiscoveredDevices();
        QList<QBluetoothDeviceInfo>::iterator deviceIterator;

        for(deviceIterator = discoveredDevices.begin(); deviceIterator!= discoveredDevices.end(); deviceIterator++){

            QListWidgetItem *label;
            label = new QListWidgetItem(deviceIterator->name(), list);
            list->addItem(label);
            qDebug() << "Device:" << deviceIterator->name() << "added";

        };

        QThread::sleep(1);
    }
}

void UpdateListWorker::stop(){
    runThread = false;
    bluetoothManager->stopScan();
}

bool UpdateListWorker::isRunning(){
    return runThread;
}

UpdateListManager::UpdateListManager(QListWidget *deviceList, BluetoothManager *manager){
    list = deviceList;
    worker = new UpdateListWorker(deviceList, manager);
    bluetoothManager = manager;
}

void UpdateListManager::runThread(){
    if (worker->isRunning()){
        worker->stop();
    }

    qDebug() << bluetoothManager->scanActive();

    if (bluetoothManager->scanActive()){
        bluetoothManager->stopScan();
    }
    bluetoothManager->startScan();

    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::started, worker, &UpdateListWorker::run);
    workerThread.start();

}

void UpdateListManager::stopThread(){
    worker->stop();
    workerThread.quit();
}

UpdateListManager::~UpdateListManager(){
    worker->stop();
}
