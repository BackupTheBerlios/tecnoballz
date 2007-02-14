/** 
 * @file right_panel_score.h
 * @brief The right panel score in the bricks levels 
 * @date 2007-02-14
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: right_panel_score.h,v 1.4 2007/02/14 20:00:08 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef __RIGHT_SCORE_PANEL__
#define __RIGHT_SCORE_PANEL__

class right_panel_score;

#include "../include/bitmap_data.h"
#include "../include/print_text.h"
#include "../include/handler_players.h"
#include "../include/handler_keyboard.h"
#include "../include/controller_gigablitz.h"
#include "../include/controller_balls.h"

class right_panel_score:public print_text
{
private:
  static right_panel_score* panel_score_singleton;

  static const Uint32 DELAY_GIGABLITZ_COUNTDOWN = 10;
  static const Uint32 GAUGE_HEIGHT = 27;
  static const Uint32 GAUGE_XCOORD = 526 / 2;
  static const Uint32 GAUGE_YCOORD = 420 / 2;
  static const Uint32 TEMOINLARG = 10 / 2;
  static const Sint32 POSX_AREAN = 528 / 2;
  static const Sint32 POSY_AREAN = 306 / 2;
  static const Sint32 POSX_LEVEL = 592 / 2;
  static const Sint32 POSY_LEVEL = 306 / 2;
  static const Sint32 POSX_NOMJO = 528 / 2;
  static const Sint32 POSY_NOMJO = 78 / 2;

  static const Sint32 POSX_BESTN = 528 / 2;
  static const Sint32 POSY_BESTN = 344 / 2;

  static const Sint32 POSX_BESTS = 528 / 2;
  static const Sint32 POSY_BESTS = 382 / 2;

  static const Sint32 POSX_SCORE = 528 / 2;
  static const Sint32 POSY_SCORE = 116 / 2;
  static const Sint32 POSX_LIFES = 528 / 2;
  static const Sint32 POSY_LIFES = 152 / 2;
  static const Sint32 POSX_BRICK = 592 / 2;
  static const Sint32 POSY_BRICK = 152 / 2;
  /** The number of bricks which remain */
  Uint32 bricks_counter;
  Sint32 flip_white;
  controller_balls *balls;
  char *scoreAdres;             // buffer address score
  char *lifesAdres;             // buffer address lifes
  char *brickAdres;             // buffer address number bricks
  char *temoinAdrs;             // buffer address indicator blitz
  /** Countdown berfore Gigablitz launch */ 
  Uint32 gigablitz_countdown;
  Uint32 delay_gigablitz_countdown;
  /** Height of the gigablitz gauge in pixels */
  Uint32 gauge_height;
  static unsigned char temoinCol1[GAUGE_HEIGHT];
  static unsigned char temoinCol2[GAUGE_HEIGHT * 2];


private:
    right_panel_score ();
public:
   ~right_panel_score ();
  static right_panel_score *get_instance ();
  void first_init (controller_balls *b);
  void text_refresh ();
  void decrease_bricks_counter (Uint32 dec);
  Uint32 get_bricks_counter ();
  void set_bricks_counter  (Uint32 counter);
  void draw_gigablizt_gauge ();
  void reset_gigablitz_countdown ();
private:
  void draw_background ();

};
#endif
