/** 
 * @file controller_bullets.h 
 * @brief Bullets controller 
 $ @date 2007-02-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_bullets.h,v 1.3 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_BULLETS__
#define __CONTROLLER_BULLETS__
class controller_bullets;

#include "../include/list_sprites.h"
#include "../include/sprite_bullet.h"
#include "../include/objects_list.h"
#include "../include/sprite_guardian.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_explosions.h"

class controller_bullets:public objects_list < sprite_bullet, controller_bullets >
{
  friend class controller_guardians;

private:
  static const Sint16 tir01_posi[62];
  static const Sint16 tir02_posi[60];
  static const Sint16 tir04_posi[14];
  static const Sint16 tir10_posi[12];
  static const Sint16 fire_sinus[60];
  sprite_paddle *ptbumper01;
  controller_explosions *explosions;

public:
    controller_bullets (sprite_paddle * pBump, controller_explosions * pexpl);
   ~controller_bullets ();
  void execution1 ();
  void anim_fires ();
  void bumper_col ();
  sprite_bullet *getWeapOne ();
  void newMissile (Sint32 nfire, sprite_guardian * pgard);

private:
  void tir01_init (sprite_guardian * pgard);
  void tir02_init (sprite_guardian * pgard);
  void tir03_init (sprite_guardian * pgard);
  void tir04_init (sprite_guardian * pgard);
  void tir05_init (sprite_guardian * pgard);
  void tir06_init (sprite_guardian * pgard);
  void tir07_init (sprite_guardian * pgard);
  void tir08_init (sprite_guardian * pgard);
  void tir09_init (sprite_guardian * pgard);
  void tir10_init (sprite_guardian * pgard);
  void tir11_init (sprite_guardian * pgard);
};
#endif
