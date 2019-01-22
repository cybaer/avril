/*
 * mcp23s08.h
 *
 *  Created on: 20.01.2019
 *      Author: cybaer
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MCP23S08_H_
#define MCP23S08_H_

#include "avrlib/spi.h"
#include "avrlib/gpio.h"

namespace avrlib
{
  static const uint8_t PORT_A = 0;
  static const uint8_t PORT_B = 1;

  static const uint8_t MCP23S08_WRITE = 0x00;
  static const uint8_t MCP23S08_READ = 0x01;

  static const uint8_t MCP23S08_IODIR = 0x00;
  static const uint8_t MCP23S08_IPOL = 0x01;
  static const uint8_t MCP23S08_GPINTEN = 0x02;
  static const uint8_t MCP23S08_DEFVAL = 0x03;
  static const uint8_t MCP23S08_INTCON = 0x04;
  static const uint8_t MCP23S08_IOCON = 0x05;
  static const uint8_t MCP23S08_GPPU = 0x06;
  static const uint8_t MCP23S08_INTF = 0x07;
  static const uint8_t MCP23S08_INTCAP = 0x08;
  static const uint8_t MCP23S08_GPIO = 0x09;
  static const uint8_t MCP23S08_OLAT = 0x0A;


  struct Registers
  {
    uint8_t Gpio;
    uint8_t IoDir;
    uint8_t Gppu;

  };

  template<typename SpiMasterBase, typename SlaveSelect, uint8_t addr>
  class MCP23S08
  {
  public:

    static inline void Write(uint8_t address, uint8_t data)
    {
      SlaveSelect::Low();
      SpiMasterBase::Send(slaveAddress | MCP23S08_WRITE);
      SpiMasterBase::Send(address);
      SpiMasterBase::Send(data);
      SlaveSelect::High();
    }

    static inline uint8_t Read(uint8_t address)
    {
      SlaveSelect::Low();
      SpiMasterBase::Send(slaveAddress | MCP23S08_READ);
      SpiMasterBase::Send(address);
      uint8_t data = SpiMasterBase::Receive();
      SlaveSelect::High();
      return data;
    }

    static inline void ReadIO(void)
    {
      // store values of inputs only
      Ports.Gpio &= ~Ports.IoDir;
      Ports.Gpio |= (Ports.IoDir & Read(MCP23S08_GPIO));
    }
    static inline void WriteIO(void)
    {
      Write(MCP23S08_GPIO, Ports.Gpio);
    }

    static inline void Init(void)
    {
      SlaveSelect::set_mode(DIGITAL_OUTPUT);

      SlaveSelect::Low();
      //SpiMasterBase::Send(0x40);  //0x48 für hohe Adressen mit A2 = 1
      SpiMasterBase::Send(MCP23S08_IOCON);
      //SpiMasterBase::Send(0x08);  // HAEN=1 (Enable Addressing)
      SlaveSelect::High();

      Write(MCP23S08_IODIR, Ports.IoDir);

      Write(MCP23S08_GPPU, Ports.Gppu);
    }

    static Registers Ports;

  private:
    static const uint8_t slaveAddress = 0x40 | (addr << 1);

  };

  // funktioniert, aber warum?
  template<typename SpiMasterBase, typename SlaveSelect, uint8_t addr>
  Registers MCP23S08<SpiMasterBase, SlaveSelect, addr>::Ports;

  template<typename Extender, uint8_t Pin>
  class PortPin
  {
  public:
    static inline void clear(){ Extender::Ports.Gpio &= ~_BV(Pin); }
    static inline void set(){ Extender::Ports.Gpio |= _BV(Pin); }
    static inline void set(bool val)
    {
      if(val)
        set();
      else
        clear();
    }
    static inline void High() {setPullUp();};
    static inline void toggle(){ Extender::Ports.Gpio ^= _BV(Pin); }
    static inline bool value(){ return Extender::Ports.Gpio & _BV(Pin) ? true : false; }
    static inline void set_mode(uint8_t mode)
    {
      if(mode == DIGITAL_INPUT)
      {
        Extender::Ports.IoDir |= _BV(Pin);
      }
      else
      {
        Extender::Ports.IoDir &= ~_BV(Pin);
      }
    }
    static inline void setPullUp(){ Extender::Ports.Gppu |= _BV(Pin); }
  };
}
#endif /* MCP23S08_H_ */
