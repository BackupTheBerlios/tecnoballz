/** 
 * @file sprite_guardian.h
 * @brief The guardian sprite 
 * @created 2003-01-09 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_guardian.h,v 1.1 2007/02/06 12:26:01 gurumeditation Exp $
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
#ifndef __SPRITE_GUARDIAN__
#define __SPRITE_GUARDIAN__
class sprite_guardian;
#include "../include/sprite_object.h"
#include "../include/handler_display.h"
#include "../include/controller_guardians.h"
#include "../include/zeMissiles.h"
#include "../include/controller_balls.h"
#include "../include/zeGigaBlit.h"
#include "../include/zexplosion.h"

class sprite_guardian:public sprite_object
{
  friend class controller_guardians;
  friend class zeMissiles;
  friend class tecno_miss;
  friend class controller_balls;

private:
  const static Sint32 table_gga1[8];    //table different size of gigablitz
  const static Sint32 table_gga2[8];    //table different size of gigablitz

  Sint32 gard_power;            //strength
  Sint32 explo_time;            //duration of the explosions
  Sint32 explotempo;
  Sint32 gard_xcent;            //middle x from where weapons starts
  Sint32 gardwaitf1;            //shoot frequency of gigaBlitz
  Sint32 gardwaitf2;            //shoot frequency of gigaBlitz
  Sint32 gard_speed;            //speed of moving
  Sint32 gard_colx1;            // fenetre de collision x-mini
  Sint32 gard_coly1;            // fenetre de collision y-mini
  Sint32 gard_colx2;            // fenetre de collision x-maxi
  Sint32 gard_coly2;            // fenetre de collision y-maxi
  Sint32 gard_ycent;            //middle y from where weapons starts
  Sint32 gard_wait1;            //shoot frequency of weapons
  Sint32 gard_wait2;            //shoot frequency of weapons
  Sint32 gard_nsbob;            //sprite's number
  Sint32 gard_lissa;            //curve's number
  Sint32 *gard_tfire;           //table des pointeurs sur la liste tirs
  Sint32 gardptfire;            //
  Sint32 gard_touch;
  Sint32 gard_clign;
  unsigned char *ptr_lissa1;
  unsigned char *ptr_lissa2;
  zeMissiles *ptMissiles;
  zeGigaBlit *ptGigaBlit;
  zexplosion *pExplosion;
  Sint32 hasardval2;

public:
    sprite_guardian ();
   ~sprite_guardian ();
  Sint32 init_guard (gardlevel * guard, unsigned char *ptLis,
                     zeMissiles * pMiss, zeGigaBlit * pBliz,
                     zexplosion * pexpl);
  void run (Uint32 voffset);

private:
  void start_fire ();
  void startBlitz ();
};

#endif
