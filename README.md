PPRZonGB
========

Tools to control a Paparazzi UAV rotorcraft from a Nintendo Game Boy Classic

- gameboy: run make to build the gameboy binary (PprzOnGB.gb), need to unpuck GBDK
  with `tar -xzf gbdk-2.96a-i586-pc-linux2.2.tar.gz`
- ems-flasher: flash the gameboy binary using ems-flasher
  init and fetch submodule with `git submodule init` and `git submodule update`
- programmable gameboy cards can be found here: http://store.kitsch-bent.com/product/usb-64m-smart-card
- arduino: compile and flash arduino program (converts gamelink to serial) using
  arduino IDE (https://www.arduino.cc/)
- install Paparazzi UAV (http://wiki.paparazziuav.org/wiki/Installation)
  and use `gb2ivy.py` tool (in `sw/ground_segment/joystick` folder)


See video: https://youtu.be/LoLxrchwSpo

