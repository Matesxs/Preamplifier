#pragma once

#include "time_lerper.h"

template<class T>
class Fader
{
private:
  double m_progres = 0.0;
  T &from;
  T &to;
  TimeLerper<double> m_lerper;

public:  
  Fader(T &fromState, T &toState)
    :from(fromState)
    ,to(toState)
  {
    m_lerper.setReference(&m_progres);
  }

  bool start(const uint32_t duration)
  {
    m_lerper.set(1.0, duration, 0.0);
    return true;
  }

  bool isActive() { return !m_lerper.finished(); }
  
  bool fade()
  {
    if(m_lerper.finished()) 
      return false;

    m_lerper.update();

    if(m_lerper.finished())
    {
      from.setValues(to);
      return false;
    }

    from.fade(to, m_progres);
    return true;
  }
};