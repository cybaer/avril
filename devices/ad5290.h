/*
 * ad5290.h
 *
 *  Created on: 02.07.2021
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
 *
 * Digital potentiometer
 */

#ifndef AVRLIB_DEVICES_AD5290_H_
#define AVRLIB_DEVICES_AD5290_H_

#include "avrlib/spi.h"
#include "avrlib/base.h"

namespace avrlib 
{

  template<typename SpiMasterBase, typename SlaveSelect, uint8_t Count>
  class AD5290
  {
  public:
    static inline void Init(void)
    {
      SlaveSelect::set_mode(DIGITAL_OUTPUT);
      SlaveSelect::High();
    }

    static inline void write()
    {
      SlaveSelect::Low();
      for(int8_t i=0; i<Count; i++)
      {
        SpiMasterBase::Send(Pot[i]);
      }
      SlaveSelect::High();
    }

    static inline void setValue(uint8_t idx, uint8_t value)
    {
      if(idx < Count)
      {
        Pot[idx] = value;
      }
    }

    static inline void incrementValue(uint8_t idx)
    {
      if(idx < Count)
      {
        uint8_t value = Pot[idx];
        Pot[idx] = value < UINT8_MAX ? ++value : UINT8_MAX;
      }
    }

    static inline void decrementValue(uint8_t idx)
    {
      if(idx < Count)
      {
        uint8_t value = Pot[idx];
        Pot[idx] = value > 0 ? --value : 0;
      }
    }
    static uint8_t Pot[Count];
  };

  template<typename SpiMasterBase, typename SlaveSelect, uint8_t Count>
  uint8_t AD5290<SpiMasterBase, SlaveSelect, Count>::Pot[Count];

}
#endif
