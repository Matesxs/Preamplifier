#include "input_handlers.h"

#include "global_objects.h"
#include "ui_controller.h"
#include "settings.h"
#include "debug.h"
#include "pins.h"

Button encoder_button(ROT_PUSH, DEBOUNCE_BUTTONS_MS, false, false);
#ifdef CHANNEL_SW_BTN
Button channel_switch_button(CHANNEL_SW_BTN, DEBOUNCE_BUTTONS_MS, true, true);
#endif
RotaryEncoder encoder(ROT_A, ROT_B, RotaryEncoder::LatchMode::FOUR3);

bool shortPush = false;
bool longPush = false;

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

void handle_encoder()
{
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
#ifdef CHANNEL_SW_BTN
  channel_switch_button.read();
#endif

  if (encoder_button.pressedFor(LONG_CLICK_DUR_MS))
    longPush = true;
  else if (encoder_button.pressedFor(SHORT_CLICK_DUR_MS))
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

#ifdef CHANNEL_SW_BTN
  if (channel_switch_button.wasPressed())
    handle_channel_switch_push();
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

void buttons_task(void *)
{
  encoder_button.read();
#ifdef CHANNEL_SW_BTN
  channel_switch_button.read();
#endif

  while (true)
  {
    handle_buttons();

    vTaskDelay(pdMS_TO_TICKS(BUTTONS_PULLING_RATE_MS));
  }

  vTaskDelete(NULL);
}
