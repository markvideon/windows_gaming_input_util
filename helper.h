//
// Created by markv on 6/06/2024.
//

#ifndef WINDOWS_GAMING_INPUT_UTIL_HELPER_H
#define WINDOWS_GAMING_INPUT_UTIL_HELPER_H

#include <iostream>
#include <winrt/windows.gaming.input.h>
#include <winrt/windows.foundation.collections.h>

using namespace std;
using namespace winrt::Windows;
using namespace Gaming::Input;
using namespace Foundation;

class Helper {
public:
    void onRawGameControllerAdded(IInspectable const &_, RawGameController const rawGameController);
    void onGamepadAdded(IInspectable const &_, Gamepad const rawGameController);
    void onRawGameControllerRemoved(IInspectable const &_, RawGameController const gamepad);
    void onGamepadRemoved(IInspectable const &_, Gamepad const gamepad);

    map<string, Gamepad> gamepads;
    map<string, RawGameController> rawGameControllers;
};

#endif //WINDOWS_GAMING_INPUT_UTIL_HELPER_H
