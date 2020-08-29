/*
 * Written by Matthew Thomas
 * For ME 382: Introduction to Design
 * Task was to develop a robot to traverse an obstacle course and fire
 * ping pong balls at different targets with a robot. Original design,
 * machined parts, and we created this program instead of using traditional
 * RC design.
 * This program took input from a 3rd party Xbox gamepad to an arduino
 * with an HC-05 bluetooth module.
 */

#include "mainwindow.h"
#include "gamepad.h"
#include "bluetoothmanager.h"

#include <QApplication>
#include <QBluetoothLocalDevice>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Start bluetooth stuff
    QBluetoothLocalDevice localDevice;
    QString localDeviceName;

    if (localDevice.isValid()){

        // Set it up
        localDevice.powerOn();
        localDeviceName = localDevice.name();

        // Configure
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        // Get connected devices
        QList<QBluetoothAddress> remotes;
        remotes = localDevice.connectedDevices();
    }
    else{
        return 1;
    }

    // Connect BluetoothManager and MainWindow
    BluetoothManager *manager;
    manager = new BluetoothManager();
    w.setBluetoothManager(manager);


    // Create the gamepad
    Gamepad *gamepad;
    gamepad = new Gamepad();
    gamepad->setBluetoothManager(manager);

    // Wait for gamepad to be detected before initializing the gamepad class
    do{
        a.processEvents();
    }while(!gamepad->gamepadReady());

    // Initialize gamepad
    gamepad->initializeGamepad();

    return a.exec();
}
