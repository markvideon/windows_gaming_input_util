#include <chrono>
#include <iostream>
#include <thread>
#include <winrt/windows.gaming.input.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "helper.h"

using namespace std;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Gaming::Input;

const int32_t PRESSES_TO_STOP = 5;
const GamepadButtons STOP_BUTTON = GamepadButtons::Menu;

const int32_t MAX_ARRAY_SIZE = 40;
const double AXIS_THRESHOLD = 0.01;
const double TRIGGER_THRESHOLD = 0.01;

int main() {
    Helper helper;
    RawGameController::RawGameControllerAdded({&helper, &Helper::onRawGameControllerAdded});
    RawGameController::RawGameControllerRemoved({&helper, &Helper::onRawGameControllerRemoved});

    Gamepad::GamepadAdded({&helper, &Helper::onGamepadAdded});
    Gamepad::GamepadRemoved({&helper, &Helper::onGamepadRemoved});

    // Give callbacks a moment to invoke, no guarantee they have done so.
    this_thread::sleep_for(chrono::seconds(1));
    auto gamepads = helper.gamepads;
    auto rawGameControllers = helper.rawGameControllers;

    // Prior values
    map<string, array<bool, MAX_ARRAY_SIZE>> oldButtonLookup;
    map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>> oldSwitchLookup;
    map<string, array<double, MAX_ARRAY_SIZE>> oldAxisLookup;
    map<string, GamepadReading> oldGamepadReading;

    // Current values
    map<string, array<bool, MAX_ARRAY_SIZE>> buttonLookup;
    map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>> switchLookup;
    map<string, array<double, MAX_ARRAY_SIZE>> axisLookup;

    cout << "Devices found: " << rawGameControllers.size() << endl;
    cout << "Warning: Devices may be reported more than once if they are connected in "
            "multiple ways. E.g. connected via USB while paired by Bluetooth." << endl;

    // Prepare lists (non-gamepads) and show info for each RawGameController.
    for (auto&& [key, controller] : rawGameControllers) {
        cout << "Display Name (" << to_string(controller.DisplayName()) << ")" << endl;
        cout << "Product ID (" << to_string(controller.HardwareProductId()) << ")" << endl;
        cout << "Vendor ID (" << to_string(controller.HardwareVendorId()) << ")" << endl;
        cout << "NonRoamableId (" << key << ")" << endl;
        cout << "Buttons (" << controller.ButtonCount() << ")" << endl;
        cout << "Switches (" << controller.SwitchCount() << ")" << endl;
        cout << "Axes (" << controller.AxisCount() << ")" << endl;

        bool controllerIsGamepad = gamepads.count(key) > 0;
        if (controllerIsGamepad > 0) {
            auto readingEntry1 = map<string, GamepadReading>::value_type(key, GamepadReading());
            oldGamepadReading.insert(readingEntry1);
        } else {
            auto buttonEntry1 = map<string, array<bool, MAX_ARRAY_SIZE>>::value_type(key, {});
            auto buttonEntry2 = map<string, array<bool, MAX_ARRAY_SIZE>>::value_type(buttonEntry1);
            oldButtonLookup.insert(buttonEntry1);
            buttonLookup.insert(buttonEntry2);

            auto switchEntry1 = map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>>::value_type(key, {});;
            auto switchEntry2 = map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>>::value_type(switchEntry1);
            oldSwitchLookup.insert(switchEntry1);
            switchLookup.insert(switchEntry2);

            auto axisEntry1 = map<string, array<double, MAX_ARRAY_SIZE>>::value_type(key, {});;
            auto axisEntry2 = map<string, array<double, MAX_ARRAY_SIZE>>::value_type(axisEntry1);
            oldAxisLookup.insert(axisEntry1);
            axisLookup.insert(axisEntry2);
        }
    }

    bool runLoop = !rawGameControllers.empty();
    int32_t stopCount = 0;
    while (runLoop) {
        for (auto&& [key, rawGameController] : rawGameControllers)
        {
            if (gamepads.count(key) > 0) {
                auto gamepad = gamepads.at(key);
                auto oldReading = oldGamepadReading[key];
                auto reading = gamepad.GetCurrentReading();

                if (abs(reading.LeftThumbstickX - oldReading.LeftThumbstickX) > AXIS_THRESHOLD) {
                    cout << "LeftThumbstickX: " << reading.LeftThumbstickX << endl;
                }

                if (abs(reading.LeftThumbstickY - oldReading.LeftThumbstickY) > AXIS_THRESHOLD) {
                    cout << "LeftThumbstickY: " << reading.LeftThumbstickY << endl;
                }

                if (abs(reading.RightThumbstickX - oldReading.RightThumbstickX) > AXIS_THRESHOLD) {
                    cout << "RightThumbstickX: " << reading.RightThumbstickX << endl;
                }

                if (abs(reading.RightThumbstickY - oldReading.RightThumbstickY) > AXIS_THRESHOLD) {
                    cout << "RightThumbstickY: " << reading.RightThumbstickY << endl;
                }

                if (abs(reading.LeftTrigger - oldReading.LeftTrigger) > TRIGGER_THRESHOLD) {
                    cout << "LeftTrigger: " << reading.LeftTrigger << endl;
                }

                if (abs(reading.RightTrigger - oldReading.RightTrigger) > TRIGGER_THRESHOLD) {
                    cout << "RightTrigger: " << reading.RightTrigger << endl;
                }

                if (GamepadButtons::Menu == (reading.Buttons & GamepadButtons::Menu) &&
                    GamepadButtons::Menu != (oldReading.Buttons & GamepadButtons::Menu))
                {
                    cout << "Menu is pressed" << endl;
                }

                if (GamepadButtons::View == (reading.Buttons & GamepadButtons::View) &&
                    GamepadButtons::View != (oldReading.Buttons & GamepadButtons::View))
                {
                    cout << "View is pressed" << endl;
                }

                if (GamepadButtons::A == (reading.Buttons & GamepadButtons::A) &&
                    GamepadButtons::A != (oldReading.Buttons & GamepadButtons::A))
                {
                    cout << "A is pressed" << endl;
                }

                if (GamepadButtons::B == (reading.Buttons & GamepadButtons::B) &&
                    GamepadButtons::B != (oldReading.Buttons & GamepadButtons::B))
                {
                    cout << "B is pressed" << endl;
                }

                if (GamepadButtons::X == (reading.Buttons & GamepadButtons::X) &&
                    GamepadButtons::X != (oldReading.Buttons & GamepadButtons::X))
                {
                    cout << "X is pressed" << endl;
                }

                if (GamepadButtons::Y == (reading.Buttons & GamepadButtons::Y) &&
                    GamepadButtons::Y != (oldReading.Buttons & GamepadButtons::Y))
                {
                    cout << "Y is pressed" << endl;
                }

                oldGamepadReading[key] = reading;
            } else {
                // Read as RawGameController
                rawGameController.GetCurrentReading(buttonLookup[key], switchLookup[key], axisLookup[key]);

                for (int32_t i = 0; i < rawGameController.ButtonCount(); i++) {
                    if (buttonLookup[key][i] && !oldButtonLookup[key][i]) {
                        cout << "Button " << i << " pressed" << endl;
                    } else if (!buttonLookup[key][i] && oldButtonLookup[key][i]) {
                        // cout << "Button " << i << " released" << endl;
                    }

                    oldButtonLookup[key][i] = buttonLookup[key][i];
                }

                int32_t switchCount = rawGameController.SwitchCount();

                for (int32_t i = 0; i < switchCount; i++) {
                    if (switchLookup[key][i] != oldSwitchLookup[key][i]) {
                        if (switchLookup[key][i] == GameControllerSwitchPosition::Center) {
                            cout << "Switch " << i << " center" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::Up) {
                            cout << "Switch " << i << " up" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::UpRight) {
                            cout << "Switch " << i << " up right" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::Right) {
                            cout << "Switch " << i << " right" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::DownRight) {
                            cout << "Switch " << i << " down right" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::Down) {
                            cout << "Switch " << i << " down" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::DownLeft) {
                            cout << "Switch " << i << " down left" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::Left) {
                            cout << "Switch " << i << " left" << endl;
                        } else if (switchLookup[key][i] == GameControllerSwitchPosition::UpLeft) {
                            cout << "Switch " << i << " up left" << endl;
                        } else {
                            cout << "No / unexpected value for switch. (" << static_cast<int>(switchLookup[key][i]) << ")" << endl;
                        }
                    }
                    oldSwitchLookup[key][i] = switchLookup[key][i];
                }

                int32_t axisCount = rawGameController.AxisCount();
                for (int32_t i = 0; i < axisCount; i++) {
                    if (abs(axisLookup[key][i] - oldAxisLookup[key][i]) > AXIS_THRESHOLD) {
                        cout << "Axis " << i << " has value " << axisLookup[key][i] << endl;
                    }
                    oldAxisLookup[key][i] = axisLookup[key][i];
                }
            }

        }

        this_thread::sleep_for(chrono::milliseconds(17));
    }


    return 0;
}
