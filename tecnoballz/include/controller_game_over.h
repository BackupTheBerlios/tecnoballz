/** 
 * @file controller_game_over.h
 * @brief Game Over controller 
 * @created 2002-12-14
 * @date 2007-02-09
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_game_over.h,v 1.3 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_GAME_OVER__
#define __CONTROLLER_GAME_OVER__

class controller_game_over;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
#include "../include/score_over.h"

class controller_game_over:public objects_list < sprite_object, controller_game_over >
{
private:
  Uint32 move_phase;            // step number: 0, 1, 2 or 3
  Uint32 go_zetempo;            // tempo before next step
  score_over *ptScorOver;
  Sint32 chrOffsetX;

  static const Sint32 SIZETSINUS = 200;
  static const Sint32 sinus_over[SIZETSINUS];
  static const Sint32 zeus_over1[];

private:
  void deplace_01 ();
  void deplace_02 ();
  void deplace_03 ();
  void deplace_04 ();

public:
    controller_game_over ();
   ~controller_game_over ();
  score_over *gtScorOver ();
  Sint32 first_init (Sint32 offzt = 0);
  void initialize (Sint32 iswin);
  void execution1 (Sint32 iswin = 0);
};
#endif
