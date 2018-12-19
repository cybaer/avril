/*
 * devices.h
 *
 *  Created on: 05.12.2015
 *      Author: cybaer
 */

#ifndef AVRLIB_DEVICES_BICOLOR_LED_GROUP_H_
#define AVRLIB_DEVICES_BICOLOR_LED_GROUP_H_

using namespace avrlib;
static const int8_t NIL = 0xff;

#include "mcp23s17.h"

class LED_Base
{
public:
  virtual void init(void) = 0;
  virtual void setColor(bool) = 0;
  virtual void set(void) = 0;
  virtual void clear(void) = 0;
  virtual void toggle(void) = 0;
  virtual void set(bool) = 0;
};

//template<typename Extender, uint8_t Port, uint8_t PinNumber>
template<typename Pin, typename Color>
class LED : public LED_Base
{
public:
  LED()
  {}
  void init(void)
  {
    Pin::set_mode(DIGITAL_OUTPUT);
  }
  inline void setColor(bool color) { bool val = get(); Color::set(color); set(val); }
  inline void set(void) { Pin::set(!Color::value()); }
  inline void clear(void) { Pin::set(Color::value()); }
  void toggle(void) { Pin::toggle(); }
  inline void set(bool val)
  {
    if(val)
      set();
    else
      clear();
  }
  inline bool get(void){ return Pin::value() != Color::value(); }
};


class LEDGroupBase
{
public:

  LEDGroupBase(LED_Base** leds, int8_t count)
  : m_Leds(leds)
  , m_Count(count)
  {};

  void init(void)
  {
    for(int8_t i=0; i<m_Count; i++)
    {
      m_Leds[i]->init();
    }
  }
   void setColor(bool color)
  {
    for(int8_t i=0; i<m_Count; i++)
    {
      m_Leds[i]->setColor(color);
    }
  }
   void setColor(bool color, int8_t index)
  {
    if (index != NIL && index < m_Count)
      m_Leds[index]->setColor(color);
  }
   void clear(void)
  {
    for(int8_t i=0; i<m_Count; i++)
    {
      m_Leds[i]->clear();
    }
  }
   void set(void)
  {
    for (int8_t i=0; i < m_Count; i++)
    {
      m_Leds[i]->set();
    }
  }
  void set(int8_t index)
  {
    clear();
    if(index != NIL && index < m_Count)
      m_Leds[index]->set();
  }
  void setWithMask(uint8_t mask)
  {
    for (int8_t i=0; i < m_Count; i++)
    {
      m_Leds[i]->set(mask & 1<<i);
    }
  }
  void toggle(int8_t index)
  {
    if (index != NIL && index < m_Count)
      m_Leds[index]->toggle();
  }

  LED_Base** m_Leds;
  int8_t m_Count;


private:
  DISALLOW_COPY_AND_ASSIGN(LEDGroupBase);
};

template<typename L1, typename L2, typename L3, typename L4>
class LEDGroup : public LEDGroupBase
{
public:
  static const int8_t LED_COUNT = 4;
  static L1 led1;
  static L2 led2;
  static L3 led3;
  static L4 led4;

  LEDGroup()
  : LEDGroupBase(m_LedArray, LED_COUNT)
  {
    m_LedArray[0] = &led1;
    m_LedArray[1] = &led2;
    m_LedArray[2] = &led3;
    m_LedArray[3] = &led4;
  }
  static LED_Base* m_LedArray[LED_COUNT];
};

template<typename L1, typename L2, typename L3, typename L4>
L1 LEDGroup<L1, L2, L3, L4>::led1;
template<typename L1, typename L2, typename L3, typename L4>
L2 LEDGroup<L1, L2, L3, L4>::led2;
template<typename L1, typename L2, typename L3, typename L4>
L3 LEDGroup<L1, L2, L3, L4>::led3;
template<typename L1, typename L2, typename L3, typename L4>
L4 LEDGroup<L1, L2, L3, L4>::led4;

template<typename L1, typename L2, typename L3, typename L4>
LED_Base* LEDGroup<L1, L2, L3, L4>::m_LedArray[4];


template<typename L1, typename L2, typename L3, typename L4, typename L5>
class LEDGroup5 : public LEDGroupBase
{
public:
  static const int8_t LED_COUNT = 5;
  static L1 led1;
  static L2 led2;
  static L3 led3;
  static L4 led4;
  static L5 led5;

  LEDGroup5()
  : LEDGroupBase(m_LedArray, LED_COUNT)
  {
    m_LedArray[0] = &led1;
    m_LedArray[1] = &led2;
    m_LedArray[2] = &led3;
    m_LedArray[3] = &led4;
    m_LedArray[4] = &led5;
  }
  static LED_Base* m_LedArray[LED_COUNT];
};

template<typename L1, typename L2, typename L3, typename L4, typename L5>
L1 LEDGroup5<L1, L2, L3, L4, L5>::led1;
template<typename L1, typename L2, typename L3, typename L4, typename L5>
L2 LEDGroup5<L1, L2, L3, L4, L5>::led2;
template<typename L1, typename L2, typename L3, typename L4, typename L5>
L3 LEDGroup5<L1, L2, L3, L4, L5>::led3;
template<typename L1, typename L2, typename L3, typename L4, typename L5>
L4 LEDGroup5<L1, L2, L3, L4, L5>::led4;
template<typename L1, typename L2, typename L3, typename L4, typename L5>
L5 LEDGroup5<L1, L2, L3, L4, L5>::led5;

template<typename L1, typename L2, typename L3, typename L4, typename L5>
LED_Base* LEDGroup5<L1, L2, L3, L4, L5>::m_LedArray[5];

#endif /* AVRLIB_DEVICES_BICOLOR_LED_GROUP_H_ */
