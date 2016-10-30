/*
 * switch_group.h
 *
 *  Created on: 06.12.2015
 *      Author: cybaer
 */

#ifndef AVRLIB_DEVICES_SWITCH_GROUP_H_
#define AVRLIB_DEVICES_SWITCH_GROUP_H_

using namespace avrlib;

#include "mcp23s17.h"

class SwitchBase
{
public:
  SwitchBase(void)
  : m_State(0xff)
  {}
  virtual void init(void) = 0;
  virtual bool refresh(void) = 0;
  bool released() { return m_State == 0x80; }
  bool pressed()  { return m_State == 0x7f; }
  bool active()   { return m_State == 0xff; }
  bool inactive() { return m_State == 0x00; }
  uint8_t getState() const { return m_State; }

protected:
  uint8_t m_State;
};

/* ein Schalter, debounced */
template<typename Extender, uint8_t Port, uint8_t PinNumber, uint8_t PullUp = 1>
class Switch : public SwitchBase
{
  typedef  PortPin<Extender, Port, PinNumber> Pin;
public:
  Switch()
  {}
  void init(void)
  {
    Pin::set_mode(DIGITAL_INPUT);
    if(PullUp)
      Pin::setPullUp();
  }
  bool refresh()
  {
    m_State = (m_State << 1) | (Pin::value() ? !PullUp : PullUp);
    return active();
  }

private:
  DISALLOW_COPY_AND_ASSIGN(Switch);
};

class SwitchGroupBase
{
public:
  SwitchGroupBase() {};

  static const int8_t MAX_COUNT = 4;
  void init(void)
  {
    for(int8_t i=0; i<MAX_COUNT; i++)
    {
      m_SwArray[i]->init();
    }
  }
  uint8_t refresh(void)
  {
    uint8_t ret = 0;
    // Schleife rückwärts, für korrekte Reihenfolge im Rückgabewert
    for(int8_t i=MAX_COUNT-1; i>=0; i--)
    {
      ret <<= 1;
      ret |= m_SwArray[i]->refresh();
    }
    return ret;
  }
  bool isActive(int8_t& index)
  {
    for (int8_t i = 0; i < MAX_COUNT; i++)
    {
      if (m_SwArray[i]->active())
      {
        index = i;
        return true;
      }
    }
    return false;
  }
  bool getPressed(int8_t& index)
  {
    for(int8_t i=0; i<MAX_COUNT; i++)
    {
      if(m_SwArray[i]->pressed())
      {
        index = i;
        return true;
      }
    }
    return false;
  }
  SwitchBase* m_SwArray[MAX_COUNT];

private:
  DISALLOW_COPY_AND_ASSIGN(SwitchGroupBase);
};

template <typename S1, typename S2, typename S3, typename S4>
class SwitchGroup : public SwitchGroupBase
{
public:

  static S1 sw1;
  static S2 sw2;
  static S3 sw3;
  static S4 sw4;
  SwitchGroup()
    {
      m_SwArray[0] = &sw1;
      m_SwArray[1] = &sw2;
      m_SwArray[2] = &sw3;
      m_SwArray[3] = &sw4;
    }
};
template <typename S1, typename S2, typename S3, typename S4>
S1 SwitchGroup<S1, S2, S3, S4>::sw1;
template <typename S1, typename S2, typename S3, typename S4>
S2 SwitchGroup<S1, S2, S3, S4>::sw2;
template <typename S1, typename S2, typename S3, typename S4>
S3 SwitchGroup<S1, S2, S3, S4>::sw3;
template <typename S1, typename S2, typename S3, typename S4>
S4 SwitchGroup<S1, S2, S3, S4>::sw4;

/*template <typename S1, typename S2, typename S3, typename S4>
SwitchBase* SwitchGroup<S1, S2, S3, S4>::m_SwArray[];*/


#endif /* AVRLIB_DEVICES_SWITCH_GROUP_H_ */
