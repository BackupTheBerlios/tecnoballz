/** 
 * @file supervisor_guards_level.h
 * @brief Guardians level supervisor 
 * @created 2003-01-11
 * @date 2007-02-26
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.17 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_guards_level.h,v 1.17 2007/02/26 21:29:23 gurumeditation Exp $
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
#ifndef __SUPERVISOR_GUARDS_LEVEL__
#define __SUPERVISOR_GUARDS_LEVEL__
class supervisor_guards_level;
#include "../include/tilesmap_scrolling.h"
#include "../include/sprite_bullet.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/supervisor.h"
#include "../include/controller_guardians.h"
#include "../include/controller_bullets.h"
#include "../include/controller_paddles.h"
#include "../include/controller_balls.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_indicators.h"
#include "../include/controller_gigablitz.h"
#include "../include/controller_explosions.h"
#include "../include/controller_viewfinders.h"
#include "../include/controller_game_over.h"
#include "../include/controller_spheres.h"
#include "../include/handler_popup_menu.h"
#include "../include/controller_fontes_game.h"


class supervisor_guards_level:public virtual supervisor
{
  friend class controller_fontes_game;

private:
  controller_gigablitz * gigablitz;
  tilesmap_scrolling *tiles_map;
  controller_guardians *guards;
  controller_bullets *bullets;
  controller_paddles *paddles;
  controller_balls *balls;
  controller_viewfinders *viewfinders_paddles;
  controller_moneys *money_capsules;
  controller_capsules *power_up_capsules;
  controller_indicators *player_indicators;
  controller_explosions *explosions;
  /** Sprite of the money capsule to the left of amount money */
  sprite_object *money_indicator;
  sprite_object *ptBob_name;
  sprite_capsule *ptBobLifes;
  handler_popup_menu *popup_menu;
  /** Area number from 1 to 5 */
  Uint32 area_number;
  /** Current level number 6, 12 or 13 */ 
  Uint32 level_number;
  const atariLevel *levelParam;
  Sint32 scrollTemp;            // time before scroll start
  Sint32 scrollType;            // type of background scrolling
  Sint32 scrolSpeed;            // speed of the scrolling
  controller_game_over *game_over;
  controller_spheres *ptCongBall;
  Uint32 gameover_counter;
  Sint32 count_next;            //temporisation before next level 
  /** If true game is finished, play game over with congratulations */
  bool is_victory; 
  controller_fontes_game *fontes_game;

private:
  void init_level ();
  void run_scroll ();
  void cheat_keys ();

public:
    supervisor_guards_level ();
   ~supervisor_guards_level ();
  Sint32 first_init ();
  Sint32 main_loop ();
};
#endif
