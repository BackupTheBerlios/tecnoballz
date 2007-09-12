/** 
 * @file sprite_ship.cc 
 * @brief A flying enemy ships sprite 
 * @date 2007-02-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ship.h,v 1.7 2007/09/12 06:32:48 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef __SPRITE_SHIP__
#define __SPRITE_SHIP__

class sprite_ship;

#include "../include/sprite_object.h"
#include "../include/handler_audio.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_gems.h"
#include "../include/sprite_projectile.h"
#include "../include/sprite_ball.h"
#include "../include/controller_bricks.h"

enum
{ CODE_GEMME,
  CODE_MALUS,
  CODE_MONEY
};

class sprite_ship:public sprite_object
{
  friend class controller_ships;
  friend class sprite_projectile;
  friend class controller_balls;

private:
  static const Sint32 ATOM_XMINI = 32;
  static const Sint32 ATOM_XMAXI = 191;
  static const Sint32 ATOM_YMINI = 40;
  static const Sint32 ATOM_YMAXI = 191;
  static const Sint32 ATOM_ANIMA = 8;

private:
  Sint32 atom_power;            // strength
  Sint32 init_power;            // strength
  Sint32 atom_deplX;            // offset X
  Sint32 atom_deplY;            // offset Y
  Sint32 atom_count;            // trajectory change counter
  Sint32 atom_ghost;            // pointer on "tableGhost"
  Sint32 *tableGhost;           // table time keep same trajectory
  Sint32 atom_actif;            // counter activation (0 = active) 
  Sint32 atom_explo;            // 1 = current explosion
  Sint32 atom_oexpl;            // number of the first image's explosion
  Sint32 atom_traje;            // pointeur sur la trajectoir 
  Sint32 apparition;            // time before reactivation
  Sint32 atom_xmini;
  Sint32 atom_xmaxi;
  Sint32 atom_ymini;
  Sint32 atom_ymaxi;
  Sint32 collisionT[8];

  static Sint32 ghostWait1[16];
  static Sint32 ghostWait2[16];
  static Sint32 ghostWait3[16];
  static Sint32 ghostWait4[16];
  static Sint32 *ghost_wait[4];
  static Sint32 ghost_traj[32];
  static Sint32 ghost_bobs[32]; // index animations images of sprites
  static Sint32 codeBounty[16];

public:
  sprite_ship ();
   ~sprite_ship ();
  void gere_atome ();
  void littleInit (Sint32 time0, Sint32 appar, Sint32 index,
                   Sint32 power, Sint32 pos_x, Sint32 pos_y, Sint32 offst);
  void explosion1 (sprite_projectile *);
  void explosion1 (sprite_ball *);
  void explosion2 ();

private:
  bool is_collisions_with_bricks (Uint32 xcoord, Uint32 ycoord);


};
#endif
