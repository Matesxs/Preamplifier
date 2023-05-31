#include "input_handlers.h"

#include <JC_Button.h>
#include <RotaryEncoder.h>

#include "global_objects.h"
#include "display/ui_controller.h"
#include "settings.h"
#include "debug.h"
#include "pins.h"
#include "preamp.h"

namespace InputHandling
{
  Button encoder_button(ENCODER_BUTTON, DEBOUNCE_BUTTONS_MS, false, false);
#ifdef CHANNEL_SWITCH_BUTTON
  Button channel_switch_button(INPUT_SWITCH_BUTTON, DEBOUNCE_BUTTONS_MS, true, true);
#endif
#ifdef CHANNEL_ROTARY_SWITCH
  Button channel1_switch(ROTARY_CHANNEL1_PIN, DEBOUNCE_ROTARY_SWITCH, true, true);
  Button channel2_switch(ROTARY_CHANNEL2_PIN, DEBOUNCE_ROTARY_SWITCH, true, true);
  Button channel3_switch(ROTARY_CHANNEL3_PIN, DEBOUNCE_ROTARY_SWITCH, true, true);
  Button channel4_switch(ROTARY_CHANNEL4_PIN, DEBOUNCE_ROTARY_SWITCH, true, true);
#endif
  RotaryEncoder encoder(ROT_A, ROT_B, RotaryEncoder::LatchMode::FOUR3);

  bool shortPush = false;
  bool longPush = false;

  void encoder_task(void*);
  void buttons_task(void*);
  void IRAM_ATTR update_encoder();

  void init()
  {
    encoder_button.begin();
#ifdef CHANNEL_SWITCH_BUTTON
    channel_switch_button.begin();
#endif
#ifdef CHANNEL_ROTARY_SWITCH
    channel1_switch.begin();
    channel2_switch.begin();
    channel3_switch.begin();
    channel4_switch.begin();
#endif

    attachInterrupt(ROT_A, update_encoder, CHANGE);
    attachInterrupt(ROT_B, update_encoder, CHANGE);

    xTaskCreateUniversal(encoder_task, "enc", 2048, NULL, 1, NULL, 1);
    xTaskCreateUniversal(buttons_task, "btns", 2048, NULL, 1, NULL, 1);
  }

  void handle_encoder_cw()
  {
    // DEBUG("CW\n");
    handle_controll(InputType::ENC_CW);
  }

  void handle_encoder_ccw()
  {
    // DEBUG("CCW\n");
    handle_controll(InputType::ENC_CCW);
  }

  void handle_encoder_push()
  {
    // DEBUG("Push\n");
    handle_controll(InputType::ENC_PUSH);
  }

  void handle_encoder_hold()
  {
    // DEBUG("Long Push\n");
    handle_controll(InputType::ENC_LPUSH);
  }

  void handle_channel_switch_push()
  {
    DEBUG("Channel switch Push\n");
    handle_controll(InputType::CH_SW_PUSH);
  }

  void handle_switch_to_chan_1()
  {
    DEBUG("Channel1 switch Push\n");
    handle_controll(InputType::SW_CH1);
  }

  void handle_switch_to_chan_2()
  {
    DEBUG("Channel2 switch Push\n");
    handle_controll(InputType::SW_CH2);
  }

  void handle_switch_to_chan_3()
  {
    DEBUG("Channel3 switch Push\n");
    handle_controll(InputType::SW_CH3);
  }

  void handle_switch_to_chan_4()
  {
    DEBUG("Channel4 switch Push\n");
    handle_controll(InputType::SW_CH4);
  }

  void handle_encoder()
  {
    encoder.tick();

    static long pos = 0;
    long newPos = encoder.getPosition();
    if (newPos != pos)
    {
      if (newPos < pos)
        handle_encoder_cw();
      else
        handle_encoder_ccw();

      pos = newPos;
    }
  }

  void IRAM_ATTR update_encoder()
  {
    encoder.tick();
  }

  void handle_buttons()
  {
    encoder_button.read();
#ifdef CHANNEL_SWITCH_BUTTON
    channel_switch_button.read();
#endif

#ifdef CHANNEL_ROTARY_SWITCH
    channel1_switch.read();
    channel2_switch.read();
    channel3_switch.read();
    channel4_switch.read();
#endif

    if (encoder_button.pressedFor(ENCODER_LONG_CLICK_DUR_MS))
      longPush = true;
    else if (encoder_button.pressedFor(ENCODER_SHORT_CLICK_DUR_MS))
      shortPush = true;

    if (encoder_button.wasReleased())
    {
      if (longPush)
        handle_encoder_hold();
      else if (shortPush)
        handle_encoder_push();

      longPush = false;
      shortPush = false;
    }

#ifdef CHANNEL_SWITCH_BUTTON
    if (channel_switch_button.wasPressed())
      handle_channel_switch_push();
#endif

#ifdef CHANNEL_ROTARY_SWITCH
    if (channel1_switch.wasPressed())
      handle_switch_to_chan_1();

    if (channel2_switch.wasPressed())
      handle_switch_to_chan_2();

    if (channel3_switch.wasPressed())
      handle_switch_to_chan_3();

    if (channel4_switch.wasPressed())
      handle_switch_to_chan_4();
#endif
  }

  void encoder_task(void*)
  {
    while (true)
    {
      handle_encoder();
      vTaskDelay(pdMS_TO_TICKS(ENCODER_PULLING_RATE_MS));
    }

    vTaskDelete(NULL);
  }

  void buttons_task(void*)
  {
    encoder_button.read();
#ifdef CHANNEL_SWITCH_BUTTON
    channel_switch_button.read();
#endif
#ifdef CHANNEL_ROTARY_SWITCH
    channel1_switch.read();
    channel2_switch.read();
    channel3_switch.read();
    channel4_switch.read();

    switch (number_of_channels)
    {
    case 4:
      if (!digitalRead(ROTARY_CHANNEL4_PIN))
        Preamp::setInput(3);

    case 3:
      if (!digitalRead(ROTARY_CHANNEL3_PIN))
        Preamp::setInput(2);

    case 2:
      if (!digitalRead(ROTARY_CHANNEL2_PIN))
        Preamp::setInput(1);

    case 1:
      if (!digitalRead(ROTARY_CHANNEL1_PIN))
        Preamp::setInput(0);
    
    default:
      break;
    }
#endif

    while (true)
    {
      handle_buttons();

      vTaskDelay(pdMS_TO_TICKS(BUTTONS_PULLING_RATE_MS));
    }

    vTaskDelete(NULL);
  }
};
