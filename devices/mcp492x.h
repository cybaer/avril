// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Driver for a MCP492x DAC (SPI single/dual 12-bits DAC).

#ifndef AVRLIB_DEVICES_MCP492X_H_
#define AVRLIB_DEVICES_MCP492X_H_

#include "avrlib/spi.h"
#include "avrlib/op.h"

using namespace avrlib;

namespace avrlib {

static const bool LowGain = false;
static const bool HighGain = true;

enum DacVoltageReference {
  BUFFERED_REFERENCE,
  UNBUFFERED_REFERENCE
};

template<typename Interface,
         DacVoltageReference voltage_reference = UNBUFFERED_REFERENCE>
class Dac {
 public:
  /*enum {
    buffer_size = 0,
    data_size = 8,
  };*/
  Dac() { }

  static void Init() {
    Interface::Init();
  }
  static inline void setHighGain(bool high) { m_HighGain = high; }

  static inline void Write(uint16_t value) {
    Write(value, 0);
  }

  static inline void Write(uint16_t value, uint8_t channel, bool highGain = false) {
    //value = U8Swap4(value);

    uint8_t command;
    command = ( 0x10 | (value >> 8));
    if (channel) {
      command |= 0x80;
    }
    if (voltage_reference == BUFFERED_REFERENCE) {
      command |= 0x40;
    }
    if (!highGain) {
      command |= 0x20;
    }
    Interface::WriteWord(command, value & 0xff);
  }
 private:
  static bool m_HighGain;
};

template<typename Interface, DacVoltageReference voltage_reference>
bool Dac<Interface, voltage_reference>::m_HighGain;

}  // namespace avrlib

#endif  // AVRLIB_DEVICES_MCP492X_H_
