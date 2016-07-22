/*
 * Copyright (C) 2016 Gautier Hattenberger <gautier.hattenberger@enac.fr>
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * Small program sending commands from a classic Game Boy
 * to the Paparazzi UAV system for controlling an aircraft.
 * Connexion with the ground station is done with an Arduino
 * board for converting the GB Link to a standard UART link.
 *
 * In the future is should also be able to display some basic
 * information about the UAV state.
 *
 * See http://paparazziuav.org for more info about Paparazzi
 */

/*
 * Screen size
 * 160 x 144 pixels
 * 4.7 x 4.3 mm
 * 20 x 18 char (19 x 17 effective)
 */

#define DEBUG 0

// number of ticks between sending data, 0 to disable
// mostly usefull if PAD_WAITUP is not set to 1
#define UPDATE_PERIOD 10
// set number of clocks for timeout, 0 to disable
#define TIMEOUT 100
// wait pad up before sending again
#define PAD_WAITUP 0

#include <gb/gb.h>
#include <gb/drawing.h>
#include <stdio.h>
#if TIMEOUT
#include <time.h>
#endif

void main(void)
{
  UBYTE pad = 0;
  UBYTE last_pad = 0;
#if !DEBUG
  char text[20];
  UBYTE idx = 0;
#endif
#if TIMEOUT || UPDATE_PERIOD
  time_t t, dt;
#endif

#if !DEBUG
  gotogxy(0, 1);
  gprint("  === PprzOnGB ===");
  gotogxy(0, 3);
  gprint("Pad: move UAV");
  gotogxy(0, 4);
  gprint("A: down");
  gotogxy(0, 5);
  gprint("B: up");
  gotogxy(0, 6);
  gprint("START: turn right");
  gotogxy(0, 7);
  gprint("SELECT: turn left");
#else
  puts(" DEBUG MODE");
  puts("");
#endif

  while(1) {

    // reset pad and timer
    pad = joypad();
#if TIMEOUT || UPDATE_PERIOD
    t = clock();
    dt = 0;
#endif
#if PAD_WAITUP
    waitpadup();
#endif
    /*
     * If pad is just realeased, send immediately 0, otherwise
     * listen to incoming bytes until a button is pressed
     * or after a timeout to send 0 (no more commands).
     * Timer has 100 clocks per second.
     */
    receive_byte();
    while(_io_status == IO_RECEIVING
#if UPDATE_PERIOD
        && (pad == 0 || dt < UPDATE_PERIOD)
#else
        && pad == 0
#endif
#if TIMEOUT
        && dt < TIMEOUT
#endif
        ) {
      /* Read button status */
      pad = joypad();
      /* pad is just realeased, send data now */
      if (last_pad != 0 && pad == 0) break;
#if TIMEOUT || UPDATE_PERIOD
      /* Read clock */
      dt = clock() - t;
#endif
    }
    last_pad = pad;
#if DEBUG && (TIMEOUT || UPDATE_PERIOD)
    printf("Timer : %d\n", dt);
#endif
    if(_io_status == IO_IDLE) {
      /* New data recived */
#if DEBUG
      printf("Received data : %d\n", _io_in);
#else
      text[idx] = _io_in;
      idx++;
      if (idx == 18 || text[idx-1] == 0) {
        // print a complete text line
        gotogxy(0, 11);
        gprint(text);
        idx = 0;
      }
#endif
    } else {
      /* Interrupted by user, send pad value */
#if DEBUG
      printf("Data to send : %d\n", pad);
#endif
      _io_out = pad;
      send_byte();
      /* Wait for IO completion */
      while(_io_status == IO_SENDING) ;
#if DEBUG
      if (_io_status !=  IO_IDLE)
        printf("IO send error: %d\n", _io_status);
#endif
    }

  }

}

