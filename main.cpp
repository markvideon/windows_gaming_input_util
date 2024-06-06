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

int main() {
    Helper helper;
    RawGameController::RawGameControllerAdded({&helper, &Helper::onRawGameControllerAdded});
    RawGameController::RawGameControllerRemoved({&helper, &Helper::onRawGameControllerRemoved});
    this_thread::sleep_for(chrono::milliseconds(1000));
    auto devices = helper.devices;

    cout << "Devices found: " << devices.size() << endl;

    bool runLoop = true;
    int32_t stopCount = 0;
    while (runLoop) {
        for (auto&& [key, controller] : devices)
        {
            int32_t buttonCount = controller.ButtonCount();
            cout << "== Buttons (" << buttonCount << ") ==" << endl;
            for (int32_t i = 0; i < buttonCount; i++) {

                auto enumValue = static_cast<int>(controller.GetButtonLabel(i));
                cout << "idx " << i << ": "
                     << enumValue << " - "
                     << "TODO" << endl;

                // ToDo: Get reading
                // Todo: Increment count based on press of STOP_BUTTON

                if (stopCount == PRESSES_TO_STOP) {
                    runLoop = false;
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(17));
    }


    return 0;
}
