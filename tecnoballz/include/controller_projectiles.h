/** 
 * @file controller_projectiles.h
 * @brief Projectiles controller 
 * @date 2007-02-09
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_projectiles.h,v 1.2 2007/02/09 17:05:29 gurumeditation Exp $
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

#ifndef __CONTROLLER_PROJECTILES__
#define __CONTROLLER_PROJECTILES__

class controller_projectiles;

#include "../include/objects_list.h"
#include "../include/handler_display.h"
#include "../include/barreScore.h"
#include "../include/sprite_projectile.h"
#include "../include/sprite_paddle.h"


class controller_projectiles:public objects_list < sprite_projectile >
{
private:
  static const Uint32 MAX_OF_PROJECTILES = 7;
  static const Uint32 SIZE_OF_PROJECTILE = 10;
  static Sint16 bob11_fire[];
  static const Sint16 scieSin640[];
  static const Sint16 scieSin320[];

private:
  sprite_paddle * gun_paddle;
  Sint32 countTempo;
  Uint32 paddle_length;
  const Sint16 *scie_sinus;

public:
    controller_projectiles ();
   ~controller_projectiles ();
  void create_projectiles_list (sprite_paddle * paddle);
  void disponible ();
  void nouveauTir ();
  void init_type1 ();
  void init_type2 ();
  void init_type3 ();
  void init_type4 ();
  void init_type5 ();
  void init_type6 ();
  void init_type7 ();
  void deplaceTir ();
  void move_type1 ();
  void move_type2 ();
  void move_type3 ();
  void move_type4 ();
  void move_type5 ();
  void move_type6 ();
  void move_type7 ();
  void fire1RunOn ();
  void fire2RunOn ();
};
#endif
