#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QObject>
#include <QGamepad>
#include <QGamepadManager>

#include "bluetoothmanager.h"

class Gamepad : public QObject
{
    Q_OBJECT
public:
    explicit Gamepad(QObject *parent = 0);
    ~Gamepad();
    void initializeGamepad();
    bool gamepadReady();

    void setBluetoothManager(BluetoothManager *blueManager);


private:
    QGamepadManager *manager;
    QGamepad *gamepad;
    BluetoothManager *bluetoothManager;

    void sendMessage(QString message);

    bool flywheelState = false;
    bool forwardState = false;
    bool backwardState = false;
    bool rightState = false;
    bool leftState = false;

    double axisTolerance = 0.01;
};

#endif // GAMEPAD_H
