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
int main() {
    Helper helper;
    RawGameController::RawGameControllerAdded({&helper, &Helper::onRawGameControllerAdded});
    RawGameController::RawGameControllerRemoved({&helper, &Helper::onRawGameControllerRemoved});

    // Give callbacks a moment to invoke, no guarantee they have done so.
    this_thread::sleep_for(chrono::milliseconds(1000));
    auto devices = helper.devices;

    // Prior values
    map<string, array<bool, MAX_ARRAY_SIZE>> oldButtonLookup;
    map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>> oldSwitchLookup;
    map<string, array<double, MAX_ARRAY_SIZE>> oldAxisLookup;

    // Current values
    map<string, array<bool, MAX_ARRAY_SIZE>> buttonLookup;
    map<string, array<GameControllerSwitchPosition, MAX_ARRAY_SIZE>> switchLookup;
    map<string, array<double, MAX_ARRAY_SIZE>> axisLookup;

    cout << "Devices found: " << devices.size() << endl;

    for (auto&& [key, controller] : devices) {
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

        cout << "==== Device (" << key << ") ====" << endl;
        cout << "Buttons (" << controller.ButtonCount() << ")" << endl;
        cout << "Switches (" << controller.SwitchCount() << ")" << endl;
        cout << "Axes (" << controller.AxisCount() << ")" << endl;
    }

    bool runLoop = !devices.empty();
    int32_t stopCount = 0;
    while (runLoop) {
        for (auto&& [key, controller] : devices)
        {
            controller.GetCurrentReading(buttonLookup[key], switchLookup[key], axisLookup[key]);


            for (int32_t i = 0; i < controller.ButtonCount(); i++) {
                if (buttonLookup[key][i] && !oldButtonLookup[key][i]) {
                    cout << "Button " << i << " pressed" << endl;
                } else if (!buttonLookup[key][i] && oldButtonLookup[key][i]) {
                    cout << "Button " << i << " released" << endl;
                }

                oldButtonLookup[key][i] = buttonLookup[key][i];
            }

            int32_t switchCount = controller.SwitchCount();

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

            int32_t axisCount = controller.AxisCount();
            for (int32_t i = 0; i < axisCount; i++) {
                if (abs(axisLookup[key][i] - oldAxisLookup[key][i]) > 0.01) {
                    cout << "Axis " << i << " has value " << axisLookup[key][i] << endl;
                }
                oldAxisLookup[key][i] = axisLookup[key][i];
            }
        }

        this_thread::sleep_for(chrono::milliseconds(17));
    }


    return 0;
}
