#pragma once

void push_indicator();

void main_screen();
void mute_screen();
void spectrum();

void main_menu_selector(int index);

void input_selector();
void loudness_menu_selector(int index);
void attenuations_menu_selector(int index);
void bass_filter_menu_selector(int index);
void middle_filter_menu_selector(int index);
void treble_filter_menu_selector(int index);
void sub_menu_selector(int index);
void soft_steps_menu_selector(int index);
void led_strip_menu_selector(int index);

void input_gain_settings();
void master_volume_settings();
void treble_gain_settings();
void middle_gain_settings();
void bass_gain_settings();

void loudness_att_settings();
void loudness_center_freq_settings();
void loudness_high_boost_settings();

void attenuation_left_settings();
void attenuation_right_setting();
void attenuation_sub_setting();

void bass_qfactor_settings();
void bass_center_freq_settings();
void bass_dc_settings();

void middle_qfactor_settings();
void middle_center_freq_settings();

void treble_center_freq_settings();

void sub_cutoff_freq_settings();

void soft_steps_time_settings();
void soft_steps_volume_settings();
void soft_steps_loudness_settings();
void soft_steps_middle_settings();
void soft_steps_bass_settings();
void soft_steps_left_att_settings();
void soft_steps_right_att_settings();
void soft_steps_sub_att_settings();

void soft_mute_time_settings();

void led_strip_effect_menu_selector(int index);
void led_strip_color_menu_selector(int index);
void led_strip_color_red_settings();
void led_strip_color_green_settings();
void led_strip_color_blue_settings();
void led_strip_color_brightness_settings();

void settings_menu_selector(int index);
void settings_screensaver_menu_selector(int index);
void settings_screensaver_enable();
void settings_screensaver_delay();
void settings_screensaver_use_spectrum();
void settings_clip_detection();
void settings_overheat_menu_selector(int index);
void settings_overheat_detection_enable();
void settings_overheat_mute();
void settings_overheat_blink();
void settings_brightness();
void factory_reset_confirmation();
