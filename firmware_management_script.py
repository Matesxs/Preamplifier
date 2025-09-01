#!/usr/bin/python3

# Adds PlatformIO post-processing to merge all the ESP flash images into a single image.

import os
from shutil import copyfile

Import("env", "projenv")

board_config = env.BoardConfig()
firmware_build_bin = "${BUILD_DIR}/${PROGNAME}.bin"
standard_bin = "firmware.bin"
merged_bin = "firmware-merged.bin"


def merge_bin_action(source, target, env):
  flash_images = [
    *env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])),
    "$ESP32_APP_OFFSET",
    source[0].get_abspath(),
  ]
  merge_cmd = " ".join(
    [
      '"$PYTHONEXE"',
      '"$OBJCOPY"',
      "--chip",
      board_config.get("build.mcu", "esp32"),
      "merge_bin",
      "-o",
      merged_bin,
      "--flash_mode",
      board_config.get("build.flash_mode", "dio"),
      "--flash_freq",
      "${__get_board_f_flash(__env__)}",
      "--flash_size",
      board_config.get("upload.flash_size", "4MB"),
      *flash_images,
    ]
  )
  env.Execute(merge_cmd)

def copy_standard_firmware(source, target, env):
  copyfile(source[0].get_abspath(), standard_bin)


env.AddCustomTarget(
  name="mergebin",
  dependencies=firmware_build_bin,
  actions=merge_bin_action,
  title="Merge binary",
  description="Build combined image",
  always_build=True,
)

env.AddCustomTarget(
  name="copybin",
  dependencies=firmware_build_bin,
  actions=copy_standard_firmware,
  title="Copy firmware",
  description="Copy firmware to root directory",
  always_build=True,
)