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
  //virtual void toggle(void) = 0;
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
  //void toggle(void) { Pin::toggle(); }
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
  static const int8_t MAX_COUNT = 4;

  LEDGroupBase(){};

  void init(void)
  {
    for(int8_t i=0; i<MAX_COUNT; i++)
    {
      m_LedArray[i]->init();
    }
  }
   void setColor(bool color)
  {
    for(int8_t i=0; i<MAX_COUNT; i++)
    {
      m_LedArray[i]->setColor(color);
    }
  }
   void setColor(bool color, int8_t index)
  {
    if (index != NIL && index < MAX_COUNT)
      m_LedArray[index]->setColor(color);
  }
   void clear(void)
  {
    for(int8_t i=0; i<MAX_COUNT; i++)
    {
      m_LedArray[i]->clear();
    }
  }
   void set(void)
  {
    for (int8_t i=0; i < MAX_COUNT; i++)
    {
      m_LedArray[i]->set();
    }
  }
  void set(int8_t index)
  {
    clear();
    if(index != NIL && index < MAX_COUNT)
      m_LedArray[index]->set();
  }

  LED_Base* m_LedArray[MAX_COUNT];

private:
  DISALLOW_COPY_AND_ASSIGN(LEDGroupBase);
};

template<typename L1, typename L2, typename L3, typename L4>
class LEDGroup : public LEDGroupBase
{
public:
  static const int8_t MAX_COUNT = 4;
  static L1 led1;
  static L2 led2;
  static L3 led3;
  static L4 led4;

  LEDGroup()
  {
    m_LedArray[0] = &led1;
    m_LedArray[1] = &led2;
    m_LedArray[2] = &led3;
    m_LedArray[3] = &led4;
  }
};

template<typename L1, typename L2, typename L3, typename L4>
L1 LEDGroup<L1, L2, L3, L4>::led1;
template<typename L1, typename L2, typename L3, typename L4>
L2 LEDGroup<L1, L2, L3, L4>::led2;
template<typename L1, typename L2, typename L3, typename L4>
L3 LEDGroup<L1, L2, L3, L4>::led3;
template<typename L1, typename L2, typename L3, typename L4>
L4 LEDGroup<L1, L2, L3, L4>::led4;

/*template<typename L1, typename L2, typename L3, typename L4>
LED_Base* LEDGroup<L1, L2, L3, L4>::m_LedArray[4];*/

#endif /* AVRLIB_DEVICES_BICOLOR_LED_GROUP_H_ */
