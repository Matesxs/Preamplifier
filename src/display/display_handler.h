#pragma once

#include <Arduino.h>

void prepare_display();

void display_draw_center(const char* text, uint16_t y);
void display_draw_center(const char* text);

void draw_amount_selector(const char* description, const char* display_value_string, long fillAmount);
void draw_amount_selector(const char* description, const int value, const int min_val, const int max_val);
void draw_amount_selector(const char* description, const float value, const float min_val, const float max_val);
void draw_amount_selector(const char* description, const double value, const double min_val, const double max_val);

void draw_centered_desc_and_val(const char* description, const char* val);
void draw_bool_selector(const char* description, bool val);

void draw_menu(const char* description, const char* item);