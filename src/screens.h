#pragma once

void push_indicator();

void main_screen();
void mute_screen();

void main_menu_selector(int index);

void input_selector();
void loudness_menu_selector(int index);
void attenuations_menu_selector(int index);
void bass_filter_menu_selector(int index);
void middle_filter_menu_selector(int index);
void treble_filter_menu_selector(int index);
void sub_menu_selector(int index);
void soft_steps_menu_selector(int index);
void factory_reset_configmation();

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
