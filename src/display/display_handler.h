#pragma once

#include <cstdint>

void prepare_display(uint8_t brightness);

void display_draw_center(const char* text, uint16_t y);
void display_draw_center(const char* text);

void draw_amount_selector(const char* description, const char* display_value_string, long fillAmount);
void draw_amount_selector(const char* description, int value, int min_val, int max_val);
void draw_amount_selector(const char* description, float value, float min_val, float max_val);
void draw_amount_selector(const char* description, double value, double min_val, double max_val);

void draw_centered_desc_and_val(const char* description, const char* val);
void draw_bool_selector(const char* description, bool val);

void draw_menu(const char* description, const char* item);