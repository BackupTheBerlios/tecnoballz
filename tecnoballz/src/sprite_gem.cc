/** 
 * @file sprite_gem.cc 
 * @brief The gem sprite 
 * @created 2004-04-12 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_gem.cc,v 1.7 2007/02/20 20:52:14 gurumeditation Exp $
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
#include "../include/sprite_gem.h"

/**
 * Create the gem sprite
 */
sprite_gem::sprite_gem ()
{
  clear_sprite_members ();
  directionX = 0;
  la_vitesse = 0;
  indicator1 = 0;
  blinkcount = 0;
  rand_count = 0;
  typeof_gem = GREYSQUARE;
  raquettePT = (sprite_paddle *) NULL;
}

/**
 * Release the gem sprite
 */
sprite_gem::~sprite_gem ()
{
}

/**
 * Initialize the coordinates of the max/min
 */
/*
void
sprite_gem::init_members ()
{
  x_maximum = screen_width - ((64 + 16) * resolution);
  x_minimum = 3 * resolution;
  y_maximum = screen_height - 10 * resolution;
  y_minimum = 0 * resolution;
}
*/
//-----------------------------------------------------------------------------
// initialize a new gem
//-----------------------------------------------------------------------------
Sint32
sprite_gem::disponible (sprite_ball * pball)
{
  if (is_enabled)
    {
      return 0;
    }
  initialGem (pball->x_coord, pball->y_coord, pball->paddle_touched);
  return 1;
}

/**
 * Initialize a new gem from a projectile
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 * @return true if the sprite was enabled, otherwise false
 */
Sint32
sprite_gem::disponible (sprite_projectile * blast)
{
  if (is_enabled)
    {
      return 0;
    }
  initialGem (blast->x_coord, blast->y_coord, blast->paddle);
  return 1;
}

/**
 * Initialize the gem
 * @param xcoord
 * @param ycoord
 * @param paddle
 */
void
sprite_gem::initialGem (Sint32 xcoord, Sint32 ycoord, sprite_paddle * paddle)
{
  is_enabled = 1;
  x_coord = xcoord;
  y_coord = ycoord;
  raquettePT = paddle;
  directionX = paddle->get_paddle_number ();
  la_vitesse = resolution;
  Sint32 h = (random_counter >> 4 + rand_count++) & 7;
  random_counter += xcoord;
  h = gem_random[h];
  //h = BOBListNum;               // for tests only !
  typeof_gem = h;
  set_image (h);
  indicator1 = 0;
  sprite_has_shadow = 1;
  blinkcount = 0;
}

//-----------------------------------------------------------------------------
// a new gem was collected
//-----------------------------------------------------------------------------
void
sprite_gem::gemcollect (Sint32 ztype)
{
  typeof_gem = ztype;
  set_image (ztype);
  sprite_has_shadow = 0;
  indicator1 = 1;
  y_coord = screen_height - sprite_height - 2 * resolution;
  x_coord = 270 * resolution + sprite_width * ztype;
  is_enabled = 1;
  blinkcount = 0;
}

//-----------------------------------------------------------------------------
// active blink
//-----------------------------------------------------------------------------
void
sprite_gem::activBlink ()
{
  if (is_enabled && indicator1)
    blinkcount = 30;
}

//-----------------------------------------------------------------------------
// move or blink gem
//-----------------------------------------------------------------------------
Sint32
sprite_gem::move ()
{
  if (is_enabled && !indicator1)
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
                  return typeof_gem;
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
                  return typeof_gem;
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
                  return typeof_gem;
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
                  return typeof_gem;
                }
            }
          else
            is_enabled = 0;
          break;
        }
    }
  else
    //###########################################################
    // blink gem
    //###########################################################
    {
      if (blinkcount > 0 && indicator1)
        {
          if (blinkcount > 20)
            is_enabled = 0;
          else
            is_enabled = 1;
          if (--blinkcount <= 0)
            blinkcount = 30;

        }
    }
  return -1;
}


const Sint32
  sprite_gem::gem_random[8] =
  { GREYSQUARE, GREENSPHER, YELLOWRING, BLUETRIANG,
  RHOMBUGOLD, PENTABRONZ, GREYSQUARE, GREENSPHER
};
