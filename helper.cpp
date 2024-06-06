//
// Created by markv on 6/06/2024.
//

#include "helper.h"

void Helper::onRawGameControllerAdded(IInspectable const &_,
                                              RawGameController const gamepad) {
    string id = to_string(gamepad.NonRoamableId());
    auto mapEntry = map<string, RawGameController>::value_type(id, gamepad);
    devices.insert(mapEntry);
}

void Helper::onRawGameControllerRemoved(IInspectable const &_,
                                                RawGameController const gamepad) {
    string id = to_string(gamepad.NonRoamableId());
    devices.erase(id);
}
