/** 
 * @file sprite_money.cc 
 * @brief The money sprite 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_money.cc,v 1.3 2007/02/06 16:28:17 gurumeditation Exp $
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
#include "../include/sprite_money.h"

/**
 * Create the money sprite
 */
sprite_money::sprite_money ()
{
  clear_sprite_members ();
  directionX = 0;
  valeurFric = 0;
  la_vitesse = 0;
  raquettePT = (sprite_paddle *) NULL;
}

/**
 * Release the money sprite
 */
sprite_money::~sprite_money ()
{
}

//-----------------------------------------------------------------------------
// perform some initialization
//-----------------------------------------------------------------------------
void
sprite_money::littleInit ()
{
  x_maximum = screen_width - ((64 + 16) * resolution);
  x_minimum = 3 * resolution;
  y_maximum = screen_height - 10 * resolution;
  y_minimum = 0 * resolution;
  frame_period = 5;             //animation delay 
  frame_delay = 1;              //animation delay counter

  //###################################################################
  //initialize multiplier of the value of the capsules of money 
  //###################################################################
  switch (joueurGere->getAreaNum ())
    {
    default:
      area_multi = 1;
      break;
    case 3:
    case 4:
      area_multi = 2;
      break;
    case 5:
      area_multi = 4;
      break;
    }
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a bricks
//-----------------------------------------------------------------------------
Sint32
sprite_money::disponible (brickClear * briPT)
{
  if (is_enabled)
    return 0;
  init_money (briPT->balle_posX, briPT->balle_posY, briPT->raquettePT);
  return 1;

}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui or Gards
//-----------------------------------------------------------------------------
Sint32
sprite_money::disponible (sprite_ball * pball)
{
  if (is_enabled)
    return 0;
  init_money (pball->x_coord, pball->y_coord, pball->raket_ball);
  return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money from a Bouiboui 
//-----------------------------------------------------------------------------
Sint32
sprite_money::disponible (sprite_projectile * pfire)
{
  if (is_enabled)
    return 0;
  init_money (pfire->x_coord, pfire->y_coord, pfire->raquettePT);
  return 1;
}

//-----------------------------------------------------------------------------
// initialize a new capsule of money
//-----------------------------------------------------------------------------
void
sprite_money::init_money (Sint32 pos_x, Sint32 pos_y, sprite_paddle * raket)
{
  is_enabled = 1;
  x_coord = pos_x;
  y_coord = pos_y;
  raquettePT = raket;
  Sint32 i = hasard_val;
  i = i & 0x003;
  hasard_val += i;
  directionX = raket->get_paddle_number ();
  switch (i)
    {
    case 0:
      //printf("sprite_money::init_money(): 10 * %i\n", area_multi);
      valeurFric = 10 * area_multi;
      la_vitesse = resolution;
      break;
    case 1:
      //printf("sprite_money::init_money(): 20 * %i\n", area_multi);
      valeurFric = 20 * area_multi;
      la_vitesse = resolution * 2;
      break;
    case 2:
      //printf("sprite_money::init_money(): 30 * %i\n", area_multi);
      valeurFric = 30 * area_multi;
      la_vitesse = resolution * 3;
      break;
    default:
      //printf("sprite_money::init_money(): 20 * %i\n", area_multi);
      valeurFric = 20 * area_multi;
      la_vitesse = resolution * 2;
      break;
    }
}

//-----------------------------------------------------------------------------
// bricks level: displacement and collision of the money
//-----------------------------------------------------------------------------
Sint32
sprite_money::move ()
{
  if (is_enabled)
    {
      Sint32 i = la_vitesse;
      sprite_paddle *raket = raquettePT;
      switch (directionX)
        {
          //###########################################################
          // bottom bumper
          //###########################################################
        case 1:
          y_coord += i;
          if (y_coord < y_maximum)
            {
              if (collision1 (raket))
                {
                  is_enabled = 0;
#ifndef SOUNDISOFF
                  audio->play_sound (S_MONNAIES);
#endif
                  return (valeurFric);
                }
            }
          else
            is_enabled = 0;
          break;

          //###########################################################
          // right bumper          
          //###########################################################
        case 2:
          x_coord += i;
          if (x_coord < x_maximum)
            {
              if (collision1 (raket))
                {
                  is_enabled = 0;
#ifndef SOUNDISOFF
                  audio->play_sound (S_MONNAIES);
#endif
                  return (valeurFric);
                }
            }
          else
            is_enabled = 0;
          break;

          //###########################################################
          // top bumper
          //###########################################################
        case 3:
          y_coord -= i;
          if (y_coord > y_minimum)
            {
              if (collision1 (raket))
                {
                  is_enabled = 0;
#ifndef SOUNDISOFF
                  audio->play_sound (S_MONNAIES);
#endif
                  return (valeurFric);
                }
            }
          else
            is_enabled = 0;
          break;

          //###########################################################
          // left bumper
          //###########################################################
        case 4:
          x_coord -= i;
          if (x_coord > x_minimum)
            {
              if (collision1 (raket))
                {
                  is_enabled = 0;
#ifndef SOUNDISOFF
                  audio->play_sound (S_MONNAIES);
#endif
                  return (valeurFric);
                }
            }
          else
            is_enabled = 0;
          break;
        }
    }
  return (0);
}

//-----------------------------------------------------------------------------
// guards level: displacement and collision of the money
//-----------------------------------------------------------------------------
Sint32
sprite_money::deplaceMe2 ()
{
  if (is_enabled)
    {
      Sint32 i = la_vitesse;
      sprite_paddle *raket = raquettePT;
      y_coord += i;
      if (y_coord < y_maximum)
        {
          if (collision1 (raket))
            {
              is_enabled = 0;
#ifndef SOUNDISOFF
              audio->play_sound (S_MONNAIES);
#endif
              return valeurFric;
            }
        }
      else
        is_enabled = 0;
    }
  return 0;
}
