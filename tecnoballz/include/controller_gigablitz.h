/** 
 * @file controller_gigablitz.h
 * @brief Gigablitz controller 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gigablitz.h,v 1.6 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_GIGABLITZ__
#define __CONTROLLER_GIGABLITZ__
class controller_gigablitz;

#include "../include/sprite_gigablitz.h"
#include "../include/list_sprites.h"
#include "../include/right_panel_score.h"
#include "../include/handler_keyboard.h"
#include "../include/controller_viewfinders.h"
#include "../include/controller_projectiles.h"
#include "../include/handler_players.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/objects_list.h"
#include "../include/head_animation.h"
#include "../include/controller_bricks.h"
#include "../include/controller_explosions.h"

class controller_gigablitz:public objects_list < sprite_gigablitz, controller_gigablitz >
{
  friend class sprite_gigablitz;

private:
  static const Uint32 MAX_OF_GIGABLITZ = 7;
  static Sint32 numeroBOBs[MAX_OF_GIGABLITZ];

  controller_bricks *bricks;
  head_animation *head_anim;
  controller_paddles *paddles;
  controller_explosions *explosions;
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_top;
  sprite_gigablitz *current_gigablitz;
  Sint32 blitz_colx;
  Sint32 blitz_posx;
  Sint32 blitz_posy;
  Sint32 blitz_haut;            //gigablitz height
  Sint32 bitz_ystop;            //"gigablitz" Y max.
  Sint32 bitz_maxiy;            // 
  Sint32 bitz_miniy;            // 
  Sint32 blitz_seta;
  Sint32 blitz_brik;            //width of collison bricks in row
  Uint32 blitz_xsin;

public:
    controller_gigablitz ();
   ~controller_gigablitz ();
  void create_gigablitz_sprites (controller_paddles * paddles, head_animation * gugus,
                     controller_bricks * brick);
  void initDepart ();
  void execution1 ();
  void collision1 ();
  void create_gigablitz_sprites (controller_paddles * paddles, controller_explosions * blast);
  void execution2 ();
  void collision2 ();
  Sint32 guard_shoot (Sint32 value, Sint32 pos_x, Sint32 pos_y,
                      Sint32 large, Sint32 haute);
  Sint32 isactivate ();
};
#endif
