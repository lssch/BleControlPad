//
// Created by Lars Schwarz on 18.12.2023.
//

#include "Bluefruit.h"

Bluefruit::Bluefruit(bool log_enabled)
  : _log_enabled(log_enabled),
    _rx_buffer(_interface.buffer.data()),
    _rx_buffer_len(_interface.buffer.size()) {
}

void Bluefruit::parse() {
  if (_interface.header == BLUEFRUIT_HEADER) {
    switch (_interface.cmd) {
      case Interface::Command::CONTROLLER:
          // [‘!’] [‘B’] [‘4’] [‘1’] [CRC]
          _interface.data = std::vector<uint8_t>(3);
        break;
      case Interface::Command::COLOR:
        // [‘!’] [‘C’] [byte red] [byte green] [byte blue] [CRC]
        _interface.data = std::vector<uint8_t>(4);
        break;
    }
    _rx_buffer = _interface.data.data();
    _rx_buffer_len = _interface.data.size();
  } else {
    switch (_interface.cmd) {
      case Interface::Command::CONTROLLER:
        parse_controller();
        break;
      case Interface::Command::COLOR:
        parse_color();
        break;
    }
    _interface.data.clear();
    _rx_buffer = _interface.buffer.data();
    _rx_buffer_len = _interface.buffer.size();
  }

  if (_log_enabled) std::cout << std::endl;
}

uint8_t* Bluefruit::get_rx_buffer() {
  return _rx_buffer;
}

uint8_t Bluefruit::get_rx_buffer_length() {
  return _rx_buffer_len;
}

void Bluefruit::parse_controller() {
  Interface::interface_controller interface_controller{
    .button = static_cast<Interface::Controller::Button>(_interface.data.at(0)),
    .state = static_cast<Interface::Controller::State>(_interface.data.at(1)),
    .crc = _interface.data.at(2)
  };

  switch (interface_controller.button) {
    case Interface::Controller::Button::BUTTON_1:
      controller.button.button1 = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 1: ";
      break;
    case Interface::Controller::Button::BUTTON_2:
      controller.button.button2 = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 2: ";
      break;
    case Interface::Controller::Button::BUTTON_3:
      controller.button.button3 = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 3: ";
      break;
    case Interface::Controller::Button::BUTTON_4:
      controller.button.button4 = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 4: ";
      break;
    case Interface::Controller::Button::DPAD_UP:
      controller.dpad.up = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-up: ";
      break;
    case Interface::Controller::Button::DPAD_DOWN:
      controller.dpad.down = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-down: ";
      break;
    case Interface::Controller::Button::DPAD_LEFT:
      controller.dpad.left = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-left: ";
      break;
    case Interface::Controller::Button::DPAD_RIGHT:
      controller.dpad.right = interface_controller.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-right: ";
      break;
    default:
      return;
  }

  switch (interface_controller.state) {
    case Interface::Controller::State::PRESSED:
      if (_log_enabled) std::cout << "pressed";
      break;
    case Interface::Controller::State::RELEASED:
      if (_log_enabled) std::cout << "released";
      break;
    default:
      return;
  }
}

void Bluefruit::parse_color() {
  Interface::interface_color interface_color{
    .red = _interface.data.at(0),
    .green = _interface.data.at(1),
    .blue = _interface.data.at(2),
    .crc = _interface.data.at(3)
  };

  color.red = interface_color.red;
  color.green = interface_color.green;
  color.blue = interface_color.blue;

}

std::ostream &Controller::operator<<(std::ostream &os, const Controller &controller) {
  os << "Buttons: ("
     << "1: " << controller.button.button1 << "; "
     << "2: " << controller.button.button2 << "; "
     << "3: " << controller.button.button3 << "; "
     << "4: " << controller.button.button4  << ")"
     << std::endl;

  os << "Dpad: ("
     << "up: " << controller.dpad.up << "; "
     << "down: " << controller.dpad.down << "; "
     << "left: " << controller.dpad.left << "; "
     << "right: " << controller.dpad.right << ")";

  return os;
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
  os << "Color: ("
     << "red: " << color.red << "; "
     << "green: " << color.green << "; "
     << "blue: " << color.blue << "; ";

  return os;
}


std::ostream &operator<<(std::ostream &os, const Bluefruit &bluefruit) {
  os << "Controller :" << std::endl;
  os << "\t" << bluefruit.controller;
  os << "\t" << bluefruit.color;

  return os;
}
