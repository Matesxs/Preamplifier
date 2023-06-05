#pragma once

#include <Arduino.h>

template<class T>
class TimeLerper
{
public:
  TimeLerper() :
    m_reference(NULL),
    m_start_state(0),
    m_target_state(0),
    m_duration(0),
    m_progress_time(0) {}

  bool setReference(T* reference)
  {
    if (finished())
    {
      m_reference = reference;
      return true;
    }

    return false;
  }

  void set(T target, uint32_t duration, T ref_overwrite)
  {
    (*m_reference) = ref_overwrite;
    set(target, duration);
  }

  void set(T target, uint32_t duration)
  {
    if (m_reference == NULL) return;

    m_start_state = (*m_reference);
    m_target_state = target;
    m_duration = duration;
    m_progress_time = 0;
    m_prev = millis();

    m_minValue = min(m_start_state, m_target_state);
    m_maxValue = max(m_start_state, m_target_state);
  }

  void update()
  {
    uint32_t currTime = millis();
    update(currTime - m_prev);
    m_prev = currTime;
  }

  void update(uint32_t ticks)
  {
    if (m_reference == NULL) return;
    if (finished()) return;
    m_progress_time += ticks;
    float progress = max(min(1.0f, static_cast<float>(m_progress_time) / static_cast<float>(m_duration)), 0.0f);
    (*m_reference) = max(min(lerp(m_start_state, m_target_state, progress), m_maxValue), m_minValue);
  }

  bool finished() 
  {
    if (m_reference == NULL) return true;
    return (*m_reference) == m_target_state; 
  }

  T getTarget()
  {
    return m_target_state;
  }

private:
  T lerp(T a, T b, float p)
	{
		return static_cast<T>(a + p * (b - a));
	}

  uint32_t m_prev;
  T* m_reference;
  T m_start_state;
  T m_target_state;
  uint32_t m_duration;
  uint32_t m_progress_time;

  T m_minValue;
  T m_maxValue;
};