#include "display_handler.h"

#include <U8g2lib.h>

#include "global_objects.h"

void prepare_display()
{
  display.setFont(u8g2_font_ncenB14_tr);
  display.setFontRefHeightExtendedText();
  display.setDrawColor(1);
  display.setFontMode(1);
  display.setFontPosTop();
  display.setFontDirection(0);
}

void display_draw_center(const char* text, uint16_t y)
{
  uint16_t text_length = display.getStrWidth(text);
  uint16_t text_offset = display.getDisplayWidth() / 2 - text_length / 2;

  display.drawStr(text_offset, y, text);
}

void display_draw_center(const char* text)
{
  uint16_t text_length = display.getStrWidth(text);
  uint16_t text_x_offset = display.getDisplayWidth() / 2 - text_length / 2;
  uint16_t text_y_offset = display.getDisplayHeight() / 2 - display.getMaxCharHeight() / 2;

  display.drawStr(text_x_offset, text_y_offset, text);
}

void draw_amount_selector(const char* description, const char* display_value_string, const long fillAmount)
{
  display.setFont(u8g2_font_ncenB14_tr);

  uint16_t line_offset = display.getMaxCharHeight() + 6;

  display_draw_center(description, 0);
  display_draw_center(display_value_string, line_offset);
  display.drawRFrame(0, 2 * line_offset, display.getDisplayWidth(), display.getDisplayHeight() - 2 * line_offset - 5, 2);
  
  uint16_t fill = (uint16_t)map(fillAmount, 0, 100, 0, display.getDisplayWidth());
  display.drawRBox(0, 2 * line_offset, fill, display.getDisplayHeight() - 2 * line_offset - 5, 2);
}

void draw_amount_selector(const char* description, int value, const int min_val, const int max_val)
{
  if (value < min_val)
    value = min_val;

  if (value > max_val)
    value = max_val;

  int max_delta = max_val - min_val;
  float coef = 100.0f / max_delta;
  int offsetValue = value - min_val;
  draw_amount_selector(description, String(value).c_str(), (long)offsetValue * coef);
}

void draw_amount_selector(const char* description, float value, const float min_val, const float max_val)
{
  if (value < min_val)
    value = min_val;

  if (value > max_val)
    value = max_val;

  float max_delta = max_val - min_val;
  float coef = 100.0f / max_delta;
  float offsetValue = value - min_val;
  draw_amount_selector(description, String(value, 1).c_str(),(long)offsetValue * coef);
}

void draw_amount_selector(const char* description, double value, const double min_val, const double max_val)
{
  if (value < min_val)
    value = min_val;

  if (value > max_val)
    value = max_val;

  double max_delta = max_val - min_val;
  double coef = 100.0f / max_delta;
  double offsetValue = value - min_val;
  draw_amount_selector(description, String(value, 1).c_str(), (long)offsetValue * coef);
}

void draw_menu(const char* description, const char* item)
{
  display.setFont(u8g2_font_ncenB14_tr);
  display_draw_center(description, 0);
  display.drawLine(0, display.getMaxCharHeight(), display.getDisplayWidth(), display.getMaxCharHeight());

  display.setFont(u8g2_font_ncenB18_tr);
  display_draw_center(item, display.getDisplayHeight() - display.getMaxCharHeight() - 5);
}

void draw_centered_desc_and_val(const char* description, const char* val)
{
  display.setFont(u8g2_font_ncenB14_tr);

  uint16_t line_offset = display.getMaxCharHeight() + 14;

  display_draw_center(description, 0);

  display.setFont(u8g2_font_ncenB18_tr);
  display_draw_center(val, line_offset);
}

void draw_bool_selector(const char* description, bool val)
{
  const char *valString = val ? "On" : "Off";
  draw_centered_desc_and_val(description, valString);
}
