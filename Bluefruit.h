//
// Created by Lars Schwarz on 18.12.2023.
//

#ifndef STM32_BLUEFRUIT_BLUEFRUIT_H
#define STM32_BLUEFRUIT_BLUEFRUIT_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <array>


#define BLUEFRUIT_HEADER '!'

namespace Interface {
  enum class Command : uint8_t {
    CONTROLLER = 'B',
    COLOR = 'C'
  };

  class Interface {
  public:
    uint8_t header;
    Command cmd;
    uint8_t crc;
  };

  namespace Controller {
    enum class Button : uint8_t {
      BUTTON_1 = '1',
      BUTTON_2 = '2',
      BUTTON_3 = '3',
      BUTTON_4 = '4',
      DPAD_UP = '5',
      DPAD_DOWN = '6',
      DPAD_LEFT = '7',
      DPAD_RIGHT = '8'
    };

    enum class State : uint8_t {
      PRESSED = '1',
      RELEASED = '0'
    };

    class Interface {
    public:
      Controller::Button button;
      Controller::State state;
    };
  }

  namespace Color {
    class Interface {
    public:
      uint8_t red;
      uint8_t green;
      uint8_t blue;
    };
  }
}

namespace Controller {
  class Dpad {
  public:
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
  };

  class Buttons {
  public:
    bool button1{false};
    bool button2{false};
    bool button3{false};
    bool button4{false};
  };

  class Controller {
  public:
    Buttons button;
    Dpad dpad;

    friend std::ostream& operator<<(std::ostream& os, const Controller& controller);
  };
}

class Color {
public:
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  friend std::ostream& operator<<(std::ostream& os, const Color& color);
};

class Bluefruit {
public:
  explicit Bluefruit(bool log_enabled);
  void parse(uint16_t received_bytes);
  uint8_t* get_rx_buffer();
  uint8_t get_rx_buffer_length();

  friend std::ostream& operator<<(std::ostream& os, const Bluefruit& bluefruit);

  Controller::Controller controller;
  Color color;
private:
  void parse_controller(Interface::Controller::Interface interface);
  void parse_color(Interface::Color::Interface interface);

  bool _log_enabled;
  std::array<uint8_t, sizeof(Interface::Interface) + sizeof(Interface::Color::Interface)> _rx_buffer;
};


#endif //STM32_BLUEFRUIT_BLUEFRUIT_H
