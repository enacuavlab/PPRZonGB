gb2serial
=========

Communication between a gameboy (DMG/GBC) and a serial interface (e.g. PC)
through an arduino using the LINK cable.

Only passing data from one side to the other.

Pinout for an Arduino Pro Mini:

Link Cable     Arduino      Description (GB side)
6              GND          GND
5              3            SC
2              4            SI
3              5            SO



Based on the examples of Tom Van den Bon (gameboy-arduino)

