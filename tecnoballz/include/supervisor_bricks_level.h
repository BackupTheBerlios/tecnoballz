/** 
 * @file supervisor_bricks_level.h 
 * @brief Bricks levels supervisor 
 * @date 2007-03-03
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.20 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.h,v 1.20 2007/03/03 20:59:04 gurumeditation Exp $
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
#ifndef __SUPERVISOR_BRICKS_LEVEL__
#define __SUPERVISOR_BRICKS_LEVEL__

class supervisor_bricks_level;

#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"
#include "../include/list_sprites.h"
#include "../include/handler_memory.h"
#include "../include/objects_list.h"
#include "../include/supervisor.h"
#include "../include/controller_ships.h"
#include "../include/sprite_ship.h"
#include "../include/handler_keyboard.h"
#include "../include/tiles_background.h"
#include "../include/controller_paddles.h"
#include "../include/controller_sides_bricks.h"
#include "../include/right_panel_score.h"
#include "../include/handler_display.h"
#include "../include/controller_ejectors.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_gigablitz.h"
#include "../include/handler_players.h"
#include "../include/controller_fontes_game.h"
#include "../include/head_animation.h"
#include "../include/controller_indicators.h"
#include "../include/controller_game_over.h"
#include "../include/short_info_messages.h"
#include "../include/level_data.h"
#include "../include/controller_gems.h"
#include "../include/controller_viewfinders.h"
#include "../include/handler_popup_menu.h"
#include "../include/controller_magnetic_eyes.h"

class supervisor_bricks_level:public virtual supervisor
{
  friend class tiles_background;
  friend class controller_moneys;
  friend class controller_balls;
  friend class controller_paddles;
  friend class controller_capsules;
  friend class controller_fontes_game;
  friend class controller_ships;
  friend class sprite_paddle;

private:
  short_info_messages *info_messages;
  controller_indicators *player_indicators;
  controller_sides_bricks *sides_bricks;
  /** Head animation in the right score panel */
  head_animation *head_anim;
  controller_bricks *bricks;
  tiles_background *tiles_ground;
  controller_ejectors *ejectors_corners;
  controller_balls *balls;
  controller_viewfinders *viewfinders_paddles;
  controller_paddles *paddles;
  right_panel_score *panel_score;
  controller_ships *ships;
  controller_magnetic_eyes *magnetic_eyes;
  controller_moneys *money_capsules;
  controller_capsules *power_up_capsules;
  controller_gems *gem_stones;
  controller_fontes_game *fontes_game;
  controller_gigablitz *gigablitz;
  controller_game_over *game_over;
  sprite_object *bottom_wall;
  sprite_object *money_indicator;
  handler_popup_menu *popup_menu;
  /** Area number from 1 to 2 */
  Sint32 area_number;
  /** Level number from 1 to 12 */
  Sint32 level_number;
  const bricks_levels_desc *level_desc;
  Sint32 next_level;
  /** Time delay before next level */
  Sint32 count_next;
  Sint32 isgameover;

  Sint32 devel_keyx;            //next backgound
  Sint32 devel_keyw;            //previous backgound

  Sint32 indexbgrnd;
public:
    supervisor_bricks_level ();
   ~supervisor_bricks_level ();
  Sint32 first_init ();
  Sint32 main_loop ();

private:
  void init_level ();
  void changebkgd ();
  Sint32 background (Sint32 nbkdg = -1);
};
#endif
