/** 
 * @file sprite_bullet.h
 * @brief The bullet sprite from a guardian 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_bullet.h,v 1.2 2007/02/06 20:41:33 gurumeditation Exp $
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
#ifndef __SPRITE_BULLET__
#define __SPRITE_BULLET__

class sprite_bullet;

#include "../include/sprite_object.h"
#include "../include/sprite_guardian.h"
#include "../include/sprite_paddle.h"

class sprite_bullet:public sprite_object
{
  friend class controller_bullets;

private:
  Sint32 type_depla;
  Sint32 flagDepla1;
  Sint32 flagDepla2;
  Sint32 flagDepla3;
  Sint32 flagDepla4;
  Sint32 flagDepla5;
  Sint32 flagDepla6;
  Sint32 flagDepla7;
  const short *tablesinus;
  const short *tablecosin;
  sprite_paddle *ptbumper01;
  sprite_guardian *ptguardian;
  Sint32 tir_maxi_x;
  Sint32 tir_maxi_y;
  Sint32 tir_minixy;

public:
    sprite_bullet ();
   ~sprite_bullet ();
  void execution1 ();

private:
  void dmissile01 ();
  void dmissile02 ();
  void dmissile03 ();
  void dmissile04 ();
  void dmissile05 ();
  void dmissile06 ();
  void dmissile07 ();
  void dmissile08 ();
  void dmissile09 ();
  void dmissile10 ();
  void dmissile11 ();
  void dmissile12 ();
  void screenOver (Sint32 xmini);
  void screenStop (Sint32 vmini);
};
#endif
