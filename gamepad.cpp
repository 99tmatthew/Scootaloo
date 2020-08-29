#include "gamepad.h"
#include "controls.h"

#include <QDebug>
#include <QLoggingCategory>

Gamepad::Gamepad(QObject *parent)
    : QObject(parent)
    , gamepad(0)
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));

    manager = QGamepadManager::instance();

}

bool Gamepad::gamepadReady(){
    auto gamepads = manager->connectedGamepads();
    return !gamepads.isEmpty();
}

void Gamepad::initializeGamepad(){
    auto gamepads = manager->connectedGamepads();
    gamepad = new QGamepad(*gamepads.begin(), this);
    qDebug() << "Gamepad initialized";

    /*///////////////////////////
     * //////// Buttons /////////
     * /////////////////////////*/
    connect(gamepad, &QGamepad::buttonYChanged, this, [this](double value){
        if (value){
            qDebug() << "Y button";
            bluetoothManager->sendMessage(Controls::B_Y);
        }
    });
    connect(gamepad, &QGamepad::buttonAChanged, this, [this](double value){
        if (value){
            qDebug() << "A button";
            bluetoothManager->sendMessage(Controls::B_A);
        }
    });
    connect(gamepad, &QGamepad::buttonXChanged, this, [this](double value){
        if (value){
            qDebug() << "X button";
            bluetoothManager->sendMessage(Controls::B_X);
        }
    });
    connect(gamepad, &QGamepad::buttonBChanged, this, [this](double value){
        if (value){
            qDebug() << "B button";
            bluetoothManager->sendMessage(Controls::B_B);
        }
    });
    connect(gamepad, &QGamepad::buttonStartChanged, this, [this](double value){
        if (value){
            qDebug() << "Start button";
            bluetoothManager->sendMessage(Controls::START);
        }
    });

    /*///////////////////////////
     * //////// D-PAD ///////////
     * /////////////////////////*/
    connect(gamepad, &QGamepad::buttonUpChanged, this, [this](double value){
        if (value){
            qDebug() << "Up button";
            bluetoothManager->sendMessage(Controls::D_NORTH);
        }
    });
    connect(gamepad, &QGamepad::buttonDownChanged, this, [this](double value){
        if (value){
            qDebug() << "Down button";
            bluetoothManager->sendMessage(Controls::D_SOUTH);
        }
    });
    connect(gamepad, &QGamepad::buttonLeftChanged, this, [this](double value){
        if (value){
            qDebug() << "Left button";
            bluetoothManager->sendMessage(Controls::D_WEST);
        }
    });
    connect(gamepad, &QGamepad::buttonRightChanged, this, [this](double value){
        if (value){
            qDebug() << "Right button";
            bluetoothManager->sendMessage(Controls::D_EAST);
        }
    });

    /*///////////////////////////
     * /// Triggers & Bumpers ///
     * /////////////////////////*/
    connect(gamepad, &QGamepad::buttonL1Changed, this, [this](bool value){ // Left shoulder
        if (value){
            qDebug() << "Left Shoulder";
            bluetoothManager->sendMessage(Controls::LSHOULDER);
        }
    });
    connect(gamepad, &QGamepad::buttonL2Changed, this, [this](double value){ // Left Trigger
        if (value == 1){
            qDebug() << "Left Trigger";
            bluetoothManager->sendMessage(Controls::LTRIGGER);
        }
    });
    connect(gamepad, &QGamepad::buttonL3Changed, this, [this](bool value){ // Left Stick Down
        if (value){
            qDebug() << "Left Stick Down";
            bluetoothManager->sendMessage(Controls::LSTICK_CLICK);
        }
    });
    connect(gamepad, &QGamepad::buttonR1Changed, this, [this](bool value){ // Right shoulder
        if (value){
            qDebug() << "Right Shoulder";
            bluetoothManager->sendMessage(Controls::RSHOULDER);
        }
    });
    connect(gamepad, &QGamepad::buttonR2Changed, this, [this](double value){ // Right Trigger
        if (value == 1){
            qDebug() << "Right Trigger";
            bluetoothManager->sendMessage(Controls::RTRIGGER);
        }
    });
    connect(gamepad, &QGamepad::buttonR3Changed, this, [this](bool value){ // Right Stick Down
        if (value){
            qDebug() << "Right Stick Down";
            bluetoothManager->sendMessage(Controls::RSTICK_CLICK);
        }
    });

    /*//////////////////////////////////*/
    // The following events are toggles //
    /*//////////////////////////////////*/

    // The select button toggles the flywheel to fire the pingpong ball
    connect(gamepad, &QGamepad::buttonSelectChanged, this, [this](double value){
        if (value){
            qDebug() << "Select button";

            if (!flywheelState){
                qDebug() << "Flywheel On";
                flywheelState = true;
                bluetoothManager->sendMessage(Controls::SELECT_ON);
            }
            else {
                qDebug() << "Flywheel Off";
                flywheelState = false;
                bluetoothManager->sendMessage(Controls::SELECT_OFF);
            }
        }
    });

    // Manage the left analog stick
    connect(gamepad, &QGamepad::axisLeftYChanged, this, [this](double value){

        // Create a "zone" where there will be no input
        if (value > axisTolerance || value < -axisTolerance){
            if (value < 0){

                // Only send the signal if this state is not already active
                if (!forwardState){
                    qDebug() << "Move forward";
                    bluetoothManager->sendMessage(Controls::LSTICK_UP);

                    // Set the proper states
                    backwardState = false;
                    forwardState = true;
                }
            }
            else {

                // Only send the signal if this state is not already active
                if (!backwardState){
                    qDebug() << "Move backward";
                    bluetoothManager->sendMessage(Controls::LSTICK_DOWN);

                    // Set the proper states
                    backwardState = true;
                    forwardState = false;
                }
            }
        }
        else{

            // Only send the signal if the stick is within its center tolerance
            // and the bot is either moving forwards or backwards
            if (backwardState || forwardState){
                qDebug() << "Left stick";
                qDebug() << "Stop Motors";
                bluetoothManager->sendMessage(Controls::LSTICK_NEUTRAL);

                // Set both states false
                backwardState = false;
                forwardState = false;
            }
        }
    });

    // Manage the right analog stick
    connect(gamepad, &QGamepad::axisRightXChanged, this, [this](double value){

        // Create a "zone" where there will be no input
        if (value > axisTolerance || value < -axisTolerance){
            if (value > 0){

                // Only send the signal if this state is not already active
                if (!rightState){
                    qDebug() << "Turn right";
                    bluetoothManager->sendMessage(Controls::RSTICK_RIGHT);

                    // Set the proper states
                    leftState = false;
                    rightState = true;
                }
            }
            else {

                // Only send the signal if this state is not already active
                if (!leftState){
                    qDebug() << "Turn left";
                    bluetoothManager->sendMessage(Controls::RSTICK_LEFT);

                    // Set the proper states
                    leftState = true;
                    rightState = false;
                }
            }
        }
        else{

            // Only send the signal if the stick is within its center tolerance
            // and the bot is either moving forwards or backwards
            if (rightState || leftState){
                qDebug() << "Right stick";
                qDebug() << "Stop Motors";
                bluetoothManager->sendMessage(Controls::RSTICK_NEUTRAL);

                // Set both states false
                rightState = false;
                leftState = false;
            }
        }
    });

}

// Passes messages to the bluetoothManager for sending to the connected device
void Gamepad::sendMessage(QString message){
    bluetoothManager->sendMessage(message);
}

void Gamepad::setBluetoothManager(BluetoothManager *blueManager){
    bluetoothManager = blueManager;
}

Gamepad::~Gamepad(){
    delete gamepad;
}
