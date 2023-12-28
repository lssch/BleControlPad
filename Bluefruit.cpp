//
// Created by Lars Schwarz on 18.12.2023.
//

#include "Bluefruit.h"

Bluefruit::Bluefruit(bool log_enabled)
  : _log_enabled(log_enabled) {
}

void Bluefruit::parse(uint16_t received_bytes) {
  Interface::Interface interface{
    .header = _rx_buffer.at(0),
    .cmd = static_cast<Interface::Command>(_rx_buffer.at(1)),
    .crc = _rx_buffer.at(received_bytes)
  };

  if (interface.header != BLUEFRUIT_HEADER) return;

  if (interface.cmd == Interface::Command::CONTROLLER and received_bytes == sizeof(Interface::Interface) + sizeof(Interface::Controller::Interface))
    // [‘!’] [‘B’] [‘4’] [‘1’] [CRC]
    parse_controller({
      .button = static_cast<Interface::Controller::Button>(_rx_buffer.at(2)),
      .state = static_cast<Interface::Controller::State>(_rx_buffer.at(3)),
    });
  else if ((interface.cmd == Interface::Command::COLOR and received_bytes == sizeof(Interface::Interface) + sizeof(Interface::Color::Interface)))
    // [‘!’] [‘C’] [byte red] [byte green] [byte blue] [CRC]
    parse_color({
      .red = _rx_buffer.at(2),
      .green = _rx_buffer.at(3),
      .blue = _rx_buffer.at(4)
    });
}

uint8_t* Bluefruit::get_rx_buffer() {
  return _rx_buffer.data();
}

uint8_t Bluefruit::get_rx_buffer_length() {
  return _rx_buffer.size();
}

void Bluefruit::parse_controller(Interface::Controller::Interface interface) {
  if (_log_enabled) std::cout << "Received a Controller command" << std::endl;

  switch (interface.button) {
    case Interface::Controller::Button::BUTTON_1:
      controller.button.button1 = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 1: ";
      break;
    case Interface::Controller::Button::BUTTON_2:
      controller.button.button2 = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 2: ";
      break;
    case Interface::Controller::Button::BUTTON_3:
      controller.button.button3 = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 3: ";
      break;
    case Interface::Controller::Button::BUTTON_4:
      controller.button.button4 = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Buttons 4: ";
      break;
    case Interface::Controller::Button::DPAD_UP:
      controller.dpad.up = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-up: ";
      break;
    case Interface::Controller::Button::DPAD_DOWN:
      controller.dpad.down = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-down: ";
      break;
    case Interface::Controller::Button::DPAD_LEFT:
      controller.dpad.left = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-left: ";
      break;
    case Interface::Controller::Button::DPAD_RIGHT:
      controller.dpad.right = interface.state == Interface::Controller::State::PRESSED;
      if (_log_enabled) std::cout << "Dpad-right: ";
      break;
    default:
      return;
  }

  switch (interface.state) {
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

void Bluefruit::parse_color(Interface::Color::Interface interface) {
  if (_log_enabled) std::cout << "Received a Controller command" << std::endl;

  color.red = interface.red;
  color.green = interface.green;
  color.blue = interface.blue;

  if (_log_enabled) std:: cout << color << std::endl;
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
