//
// Created by Lars Schwarz on 18.12.2023.
//

#include "BleControlPad.h"
#include <iostream>

BleControlPad::BleControlPad(bool log_enabled)
  : _log_enabled(log_enabled) {
}

void BleControlPad::parse() {
  if (_interface.header != HEADER or _interface.cmd != Interface::Command::Button) return;

  switch (_interface.button) {
    case Interface::Button::BUTTON_1:
      _state.button.button1 = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Button 1: ";
      break;
    case Interface::Button::BUTTON_2:
      _state.button.button2 = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Button 2: ";
      break;
    case Interface::Button::BUTTON_3:
      _state.button.button3 = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Button 3: ";
      break;
    case Interface::Button::BUTTON_4:
      _state.button.button4 = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Button 4: ";
      break;
    case Interface::Button::DPAD_UP:
      _state.dpad.up = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Dpad-up: ";
      break;
    case Interface::Button::DPAD_DOWN:
      _state.dpad.down = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Dpad-down: ";
      break;
    case Interface::Button::DPAD_LEFT:
      _state.dpad.left = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Dpad-left: ";
      break;
    case Interface::Button::DPAD_RIGHT:
      _state.dpad.right = _interface.button_state == Interface::ButtonState::PRESSED;
      if (_log_enabled) std::cout << "Dpad-right: ";
      break;
    default:
      return;
  }

  switch (_interface.button_state) {
    case Interface::ButtonState::PRESSED:
      if (_log_enabled) std::cout << "pressed";
      break;
    case Interface::ButtonState::RELEASED:
      if (_log_enabled) std::cout << "released";
      break;
    default:
      return;
  }

  if (_log_enabled) std::cout << std::endl;
}

uint8_t* BleControlPad::get_rx_buffer() {
  return reinterpret_cast<uint8_t *>(_interface.buffer.data());
}

uint16_t BleControlPad::get_rx_buffer_length() {
  return _interface.buffer.size();
}

std::ostream &operator<<(std::ostream &os, const BleControlPad &controlPad) {
  os << "Button (1: " << controlPad._state.button.button1 << "; 2: "
     << controlPad._state.button.button2 << "; 3: "
     << controlPad._state.button.button3 << "; 4: "
     << controlPad._state.button.button4 << ")" << std::endl;

  os << "Dpad (u: " << controlPad._state.dpad.up << "; d: "
     << controlPad._state.dpad.down << "; l: "
     << controlPad._state.dpad.left << "; r: "
     << controlPad._state.dpad.right << ")" << std::endl;
  return os;
}
