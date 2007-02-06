/** 
 * @file controller_guardians.h
 * @brief Guardians controller 
 * @created 2003-01-10 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_guardians.h,v 1.3 2007/02/06 20:41:33 gurumeditation Exp $
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
#ifndef __CONTROLLER_GUARDIANS__
#define __CONTROLLER_GUARDIANS__
class controller_guardians;
#include "../include/tecnoballz.h"

/**
 * Structure of a guardan parameters
 */
typedef struct
{
  Sint32 para_power;            // resistance
  Sint32 para_xcent;            // centre X depart tir
  Sint32 para_waitf;            // frequence tir gigaBlitz
  Sint32 para_speed;            // vitesse de deplacement
  Sint32 para_colx1;            // fenetre de collision x-mini
  Sint32 para_coly1;            // fenetre de collision y-mini
  Sint32 para_colx2;            // fenetre de collision x-maxi
  Sint32 para_coly2;            // fenetre de collision y-maxi
  Sint32 para_ycent;            // centre Y départ tir
  Sint32 para_wait2;            // frequence tir
  Sint32 para_nsbob;            // numero du BOB
  Sint32 para_lissa;            // numero de la courbe
  Sint32 para_tfire[16];        // table des pointeurs sur la liste tirs
}
gardlevel;

#include "../include/sprite_guardian.h"
#include "../include/list_sprites.h"
#include "../include/objects_list.h"
#include "../include/controller_bullets.h"
#include "../include/controller_gigablitz.h"
#include "../include/zexplosion.h"

class controller_guardians:public objects_list < sprite_guardian >
{
  friend class sprite_guardian;

private:
  unsigned char *getLissaPt (Sint32 lissa);
  void set_gauge_height ();
public:
    controller_guardians ();
   ~controller_guardians ();
  void create_guardians_list (controller_bullets * pMiss, Sint32 grdPt,
                     controller_gigablitz * pBliz, zexplosion * pExpl);
  void run ();
  void killguards (Sint32 numGa = 0);
  bool is_guardians_destroyed ();
  Sint32 run_scroll (Uint32, Sint32, sprite_ball *, sprite_bullet *);
private:
  Sint32 scrollTemp;          //use for scroll background
  Sint32 offset_ptr;
  unsigned char *displacement_curve;
  controller_gigablitz *ptGigaBlit;
  zexplosion *pexplosion;
  sprite_object **life_gauges_list;

public:
  static Sint32 level2gdpt (Uint32 arean, Uint32 level);

private:
  static gardlevel guard_list[];
  static Sint32 level_list[];
  static const char offzetbyte[256];
  
  typedef enum
    {
      LISSA_NB01,
      LISSA_NB02,
      LISSA_NB03,
      LISSA_NB04,
      LISSA_NB05,
      LISSA_NB07,
      LISSA_NB08,
      LISSA_NB09,
      LISSA_NB11,
      LISSA_NB12,
      LISSA_NB13,
      LISSA_NB15
    }
  CURVE_ID;

/*
  static const Sint32 LISSA_NB01 = 0;
  static const Sint32 LISSA_NB02 = 1;
  static const Sint32 LISSA_NB03 = 2;
  static const Sint32 LISSA_NB04 = 3;
  static const Sint32 LISSA_NB05 = 4;
  static const Sint32 LISSA_NB07 = 5;
  static const Sint32 LISSA_NB08 = 6;
  static const Sint32 LISSA_NB09 = 7;
  static const Sint32 LISSA_NB11 = 8;
  static const Sint32 LISSA_NB12 = 9;
  static const Sint32 LISSA_NB13 = 10;
  static const Sint32 LISSA_NB15 = 11;
 */
};
#endif
