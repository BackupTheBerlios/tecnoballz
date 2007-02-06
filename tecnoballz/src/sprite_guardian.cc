/** 
 * @file sprite_guardian.cc 
 * @brief The guardian sprite 
 * @created 2003-01-09 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_guardian.cc,v 1.1 2007/02/06 12:26:01 gurumeditation Exp $
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
#include "../include/sprite_guardian.h"

/**
 * Create a guardian sprite
 */
sprite_guardian::sprite_guardian ()
{
  gard_power = 0;
  gard_xcent = 0;
  gardwaitf1 = 0;
  gardwaitf2 = 0;
  gard_speed = 0;
  gard_colx1 = 0;
  gard_coly1 = 0;
  gard_colx2 = 0;
  gard_coly2 = 0;
  gard_ycent = 0;
  gard_wait1 = 0;
  gard_wait2 = 0;
  gard_lissa = 0;
  gardptfire = 0;
  gard_touch = 0;
  gard_clign = 0;
  hasardval2 = hasard_val;
  if (hasardval2 < 0)
    hasardval2 = -hasardval2;
}

/**
 * Release a guardian sprite
 */
sprite_guardian::~sprite_guardian ()
{
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32
sprite_guardian::init_guard (gardlevel * guard, unsigned char *ptLis,
                             zeMissiles * pMiss, zeGigaBlit * pBliz,
                             zexplosion * pexpl)
{
  ptGigaBlit = pBliz;
  pExplosion = pexpl;
  gard_power = guard->para_power * hardChoice;  //strength 
  gard_xcent = (guard->para_xcent * resolution) - (11 * resolution / 2);        //middle x from where weapons starts 
  gardwaitf1 = guard->para_waitf / hardChoice;  //shoot frequency of gigaBlitz
  gardwaitf2 = gardwaitf1 / hardChoice; //shoot frequency of gigaBlitz
  gard_speed = guard->para_speed;       //speed of moving
  if (hardChoice == 4)
    gard_speed *= 2;
  gard_colx1 = guard->para_colx1 * resolution;
  gard_coly1 = guard->para_coly1 * resolution;
  gard_colx2 = guard->para_colx2 * resolution;
  gard_coly2 = guard->para_coly2 * resolution;
  gard_ycent = (guard->para_ycent * resolution) - (11 * resolution / 2);
  gard_wait1 = guard->para_wait2 / hardChoice;
  gard_wait2 = guard->para_wait2 / hardChoice;
  gard_tfire = &guard->para_tfire[0];
  ptr_lissa1 = ptLis;
  ptr_lissa2 = ptLis;
  x_minimum = 6 * resolution;
  y_minimum = 2 * resolution;
  x_maximum = screen_width - sprite_width;
  y_maximum = screen_height - sprite_height;
  y_maximum = (232 - 8 - 1 - 16) * resolution;
  ptMissiles = pMiss;
  if (sprite_width == resolution * 32)
    explotempo = 7;
  else
    explotempo = 3;
  return erreur_num;
}

/**
 * Move the guardian, fire the bullets and gigablitz
 * @param voffset vertical offset
 */
void
sprite_guardian::run (Uint32 voffset)
{
  /**
   * guards is alive
   */
  if (gard_power > 0)
    {
      Sint32 x = (ptr_lissa1[0]);
      x = x * (screen_width - sprite_width) / 192;
      Sint32 y = ptr_lissa1[1] * resolution;
      x_coord = x;
      y_coord = y;
      //y_coord = y + voffset;
      ptr_lissa1 = ptr_lissa1 + gard_speed;
      Sint16 *ptest = (Sint16 *) ptr_lissa1;
      if (ptest[0] == -1)
        ptr_lissa1 = ptr_lissa2;
      gard_clign = !gard_clign;
      if (gard_touch > 0 && gard_clign > 0)
        {
          is_enabled = 0;
          gard_touch--;
        }
      else
        is_enabled = 1;
      start_fire ();
      startBlitz ();
    }
  else
    {
      //###############################################################
      //      the guard explodes 
      //###############################################################
      if (explo_time > 0)
        {
          explo_time--;
          if (!(explo_time & explotempo))
            {
              Sint32 pos_x = x_coord;
              Sint32 pos_y = y_coord;
              hasard_val += pos_x;
              Sint32 vrand = hasard_val;
              if (vrand < 0)
                vrand = -vrand;
              Sint32 val_1 = (vrand + explo_time - y_coord) % sprite_width;
              Sint32 val_2 =
                (hasardval2 - explo_time + y_coord) % sprite_height;
              pos_x += val_1;
              pos_y += val_2;
              pExplosion->add_explos (pos_x, pos_y);
              hasardval2 = vrand;
            }
          gard_clign = !gard_clign;
          if (gard_clign > 0)
            is_enabled = 0;
          else
            is_enabled = 1;
        }
      //###############################################################
      // the guard is dead 
      //###############################################################
      else
        is_enabled = 0;
    }
}

//-----------------------------------------------------------------------------
// start a new weapon (composed of several objects "tecno_miss")
//-----------------------------------------------------------------------------
void
sprite_guardian::start_fire ()
{
  if (gard_wait1-- <= 0)
    {
      if (y_coord <= (80 * resolution))
        {
          gard_wait1 = gard_wait2;
#ifndef SOUNDISOFF
          audio->play_sound (S_TIR_GARD);
#endif
          Sint32 nfire = gard_tfire[gardptfire];
          if (nfire < 0)
            {
              nfire = gard_tfire[0];
              gardptfire = 0;
            }
          gardptfire++;         // pt/next weapons
          ptMissiles->newMissile (nfire, this);
        }
    }
}

//-----------------------------------------------------------------------------
// start a new GigaBlitz
//-----------------------------------------------------------------------------
void
sprite_guardian::startBlitz ()
{
  if (gardwaitf1-- <= 0)
    {
      Sint32 v = hasard_val & 7;
      //v = 7;        //test only
      if (sprite_width > (32 * resolution))
        v = table_gga1[v];
      else
        v = table_gga2[v];
      //v = 0;        //test only 
      if (ptGigaBlit->guard_shoot (v, x_coord, y_coord,
                                   sprite_width, sprite_height))
        gardwaitf1 = gardwaitf2;
    }
}

//-----------------------------------------------------------------------------
// list of differents gigablitz
//-----------------------------------------------------------------------------
// Guard of 64/128 pixels
const Sint32
sprite_guardian::table_gga1[8] = { 0, 1, 2, 3, 4, 5, 6, 5
};

// Guard of 32/64 pixels
const Sint32
sprite_guardian::table_gga2[8] = { 6, 5, 4, 6, 5, 4, 6, 5
};
