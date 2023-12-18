//
// Created by Lars Schwarz on 18.12.2023.
//

#ifndef STM32_BLUEFRUIT_BLECONTROLPAD_H
#define STM32_BLUEFRUIT_BLECONTROLPAD_H

#include <array>
#include <cstdint>
#include <ostream>


#define HEADER '!'

namespace Interface {
  enum class Command: char {
    Button = 'B'
  };

  enum class Button: char {
    BUTTON_1 = '1',
    BUTTON_2 = '2',
    BUTTON_3 = '3',
    BUTTON_4 = '4',
    DPAD_UP = '5',
    DPAD_DOWN = '6',
    DPAD_LEFT = '7' ,
    DPAD_RIGHT = '8'
  };

  enum class ButtonState: char {
    PRESSED = '1',
    RELEASED = '0'
  };

  typedef union {
    struct {
      char header;
      Command cmd;
      Button button;
      ButtonState button_state;
    };
    std::array<char, 5> buffer;
  } interface;
}

namespace State {
  class Dpad {
  public:
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
  };

  class Button {
  public:
    bool button1{false};
    bool button2{false};
    bool button3{false};
    bool button4{false};
  };

  class ButtonState {
  public:
    Button button;
    Dpad dpad;
  };
}

class BleControlPad {
public:
  BleControlPad(bool log_enabled);
  void parse();
  uint8_t* get_rx_buffer();
  uint16_t get_rx_buffer_length();

  friend std::ostream& operator<<(std::ostream& os, const BleControlPad& controlPad);

  State::ButtonState _state;
private:
  bool _log_enabled;
  Interface::interface _interface;
};


#endif //STM32_BLUEFRUIT_BLECONTROLPAD_H
