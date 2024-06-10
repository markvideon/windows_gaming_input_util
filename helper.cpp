//
// Created by markv on 6/06/2024.
//

#include "helper.h"

void Helper::onGamepadAdded(IInspectable const &_,
                            Gamepad const gamepad) {
    auto parsedRawGC = RawGameController::FromGameController(gamepad);
    auto id = to_string(parsedRawGC.NonRoamableId());
    auto mapEntry = map<string, Gamepad>::value_type(id, gamepad);
    gamepads.insert(mapEntry);
}

void Helper::onGamepadRemoved(IInspectable const &_,
                                        Gamepad const gamepad) {
    auto parsedRawGC = RawGameController::FromGameController(gamepad);
    auto id = to_string(parsedRawGC.NonRoamableId());
    gamepads.erase(id);
}


void Helper::onRawGameControllerAdded(IInspectable const &_,
                                              RawGameController const rawGameController) {

    string id = to_string(rawGameController.NonRoamableId());
    auto mapEntry = map<string, RawGameController>::value_type(id, rawGameController);
    rawGameControllers.insert(mapEntry);
}

void Helper::onRawGameControllerRemoved(IInspectable const &_,
                                                RawGameController const rawGameController) {
    string id = to_string(rawGameController.NonRoamableId());
    rawGameControllers.erase(id);
}
