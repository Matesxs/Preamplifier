#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <Preferences.h>

#include "led_state.h"
#include "fader.h"
#include "effects/solid_color.h"
#include "effects_getter.h"

class StripHandler
{
public:
  StripHandler(uint16_t countPixels, uint8_t pin) :
    m_length(countPixels),
    m_strip(countPixels, pin),
    m_currentLedStates(m_strip),
    m_targetLedStates(m_strip),
    m_ledFader(m_currentLedStates, m_targetLedStates)
  {
  }

  void begin()
  {
    m_ledState.begin("led_state", false);

    m_effectIdx = m_ledState.getULong("eff_id", 0);
    m_currentColor = { m_ledState.getUChar("cr", 0), m_ledState.getUChar("cb", 0), m_ledState.getUChar("cg", 0) };
    m_brightness = m_ledState.getUChar("br", 255);

    m_strip.Begin();
    applyEffect();
  }

  void render()
  {
    m_currentLedStates.render();
    if(m_ledFader.isActive())
      m_targetLedStates.render();
    if(!m_ledFader.fade())
      m_currentLedStates.commit();

    m_strip.Show();
  }

  void applyEffect()
  {
    BaseEffect* effect = effectFromData(m_effectIdx, m_currentColor.Dim(m_brightness), m_brightness);
    if (effect == NULL)
    {
      effect = new SolidColor({0, 0, 0});
      m_currentColor = {0, 0, 0};
      m_brightness = 255;
      m_effectIdx = 0;
    }

    m_targetLedStates.setFunction(effect);
    m_ledFader.start(2000);

    m_ledState.putULong("eff_id", m_effectIdx);
    m_ledState.putUChar("cr", m_currentColor.R);
    m_ledState.putUChar("cg", m_currentColor.G);
    m_ledState.putUChar("cb", m_currentColor.B);
    m_ledState.putUChar("br", m_brightness);
  }

  RgbColor getColor()
  {
    return m_currentColor;
  }

  void setColor(RgbColor color)
  {
    m_currentColor = color;
  }

  uint8_t getBrightness()
  {
    return m_brightness;
  }

  void setBrighness(uint8_t brightness)
  {
    m_brightness = brightness;
  }

  void incBrightness()
  {
    if (m_brightness >= 255) return;
    m_brightness++;
  }

  void decBrightness()
  {
    if (m_brightness <= 0) return;
    m_brightness--;
  }

  uint32_t getEffectIdx()
  {
    return m_effectIdx;
  }

  void setEffectIdx(uint32_t effectIdx)
  {
    m_effectIdx = effectIdx;
  }

  void factoryReset()
  {
    m_ledState.clear();
  }

private:
  uint32_t m_effectIdx = 0;
  RgbColor m_currentColor;
  uint8_t m_brightness = 255;

  uint16_t m_length;
  NeoPixelBus<NeoBrgFeature, NeoWs2811Method> m_strip;
  LedState m_currentLedStates;
  LedState m_targetLedStates;
  Fader<LedState> m_ledFader;
  Preferences m_ledState;
};