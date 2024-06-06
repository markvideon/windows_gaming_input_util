#include <chrono>
#include <iostream>
#include <thread>
#include <winrt/windows.gaming.input.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "helper.h"

using namespace std;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::Gaming::Input;

int main() {
    Helper helper;
    RawGameController::RawGameControllerAdded({&helper, &Helper::onRawGameControllerAdded});
    RawGameController::RawGameControllerRemoved({&helper, &Helper::onRawGameControllerRemoved});
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto devices = helper.devices;

    cout << "Devices found: " << devices.size() << endl;
    for (auto&& [key, controller] : devices)
    {
        int32_t buttonCount = controller.ButtonCount();
        cout << "== Buttons (" << buttonCount << ") ==" << endl;
        for (int32_t i = 0; i < buttonCount; i++) {

            auto enumValue = static_cast<int>(controller.GetButtonLabel(i));
            cout << "idx " << i << ": "
                << enumValue << " - "
                << "TODO" << endl;
        }
    }

    return 0;
}
