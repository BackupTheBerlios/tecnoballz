/** 
 * @file sprite_bullet.cc 
 * @brief The bullet sprite from a guardian 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_bullet.cc,v 1.1 2007/02/06 16:28:17 gurumeditation Exp $
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
#include "../include/sprite_bullet.h"

/**
 * Create a bullet sprite
 */
sprite_bullet::sprite_bullet ()
{
  clear_sprite_members ();
  type_depla = 0;
  flagDepla1 = 0;
  flagDepla2 = 0;
  flagDepla3 = 0;
  flagDepla4 = 0;
  flagDepla5 = 0;
  flagDepla6 = 0;
  flagDepla7 = 0;
  tir_maxi_x = 303 * resolution;
  tir_maxi_y = 240 * resolution;
  tir_minixy = 2 * resolution;
}

/**
 * Release a bullet sprite
 */
sprite_bullet::~sprite_bullet ()
{
}

//------------------------------------------------------------------------------
// select a trajectory
//------------------------------------------------------------------------------
void
sprite_bullet::execution1 ()
{
  if (is_enabled)
    {
      switch (is_enabled)
        {
        case 1:
          dmissile01 ();
          break;
        case 2:
          dmissile02 ();
          break;
        case 3:
          dmissile03 ();
          break;
        case 4:
          dmissile04 ();
          break;
        case 5:
          dmissile05 ();
          break;
        case 6:
          dmissile06 ();
          break;
        case 7:
          dmissile07 ();
          break;
        case 8:
          dmissile08 ();
          break;
        case 9:
          dmissile09 ();
          break;
        case 10:
          dmissile10 ();
          break;
        case 11:
          dmissile11 ();
          break;
        case 12:
          dmissile12 ();
          break;
        }
    }
}

//------------------------------------------------------------------------------
// weapons: trajectory number 1
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile01 ()
{
  y_coord = y_coord + (4 * resolution);
  Sint32 v = tablesinus[++flagDepla1];
  if (v == 99)
    {
      flagDepla1 = 0;
      v = 32;
    }
  x_coord = flagDepla2 + (v * resolution);
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 2
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile02 ()
{
  flagDepla3 = flagDepla3 + 2;
  if (tablesinus[flagDepla3] == 99)     //end of the table?
    flagDepla3 = 0;
  flagDepla2 = flagDepla2 + (4 * resolution);   //new position y 
  y_coord = flagDepla2 + (tablesinus[flagDepla3 + 1] * resolution);
  x_coord = flagDepla1 + (tablesinus[flagDepla3] * resolution);
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 3
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile03 ()
{
  y_coord = y_coord + (4 * resolution);
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 4
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile04 ()
{
  if (flagDepla5)
    {
      if (--flagDepla4 <= 0)
        {
          flagDepla4 = flagDepla5;
          flagDepla5--;
          flagDepla3 = flagDepla3 + 2;
          if (tablesinus[flagDepla3] == 99)     //end of the table?
            flagDepla3 = 0;
          y_coord = flagDepla2 + (tablesinus[flagDepla3 + 1] * resolution);
          x_coord = flagDepla1 + (tablesinus[flagDepla3] * resolution);
        }
    }
  else
    {
      x_coord -= flagDepla6;
      y_coord += flagDepla7;
    }
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 5
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile05 ()
{
  if (flagDepla1)
    {
      flagDepla1--;
      y_coord += resolution;
    }
  else
    {
      x_coord -= flagDepla2;
      y_coord += flagDepla3;
    }
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 6
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile06 ()
{
  flagDepla3 = flagDepla3 + 2;
  if (tablesinus[flagDepla3] == 99)     //end of the table?
    flagDepla3 = 0;
  flagDepla2 = flagDepla2 + (3 * resolution);   //new position y 
  y_coord = flagDepla2 + (tablesinus[flagDepla3 + 1] * resolution);
  if (flagDepla2 < (150 * resolution))
    {
      flagDepla1 = flagDepla1 + 2 * resolution;
      if (ptbumper01->x_coord < flagDepla1)
        flagDepla1 = flagDepla1 - 4 * resolution;
    }
  x_coord = flagDepla1 + (tablesinus[flagDepla3] * resolution);
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 7
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile07 ()
{
  y_coord = y_coord + (3 * resolution); //new position y 
  if (y_coord < (150 * resolution))
    {
      x_coord = x_coord + 2 * resolution;
      if (ptbumper01->x_coord < x_coord)
        x_coord = x_coord - 4 * resolution;
    }
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 8 - infernal circle (circle which grows)
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile08 ()
{
  flagDepla3 = flagDepla3 + 2;
  if (flagDepla3 >= 360)
    flagDepla3 = 0;
  Sint32 x = tablesinus[flagDepla3] * flagDepla4;
  Sint32 y = tablecosin[flagDepla3] * flagDepla4;
  x = x >> 7;
  y = y >> 7;
  x_coord = ptguardian->x_coord + ptguardian->gard_xcent + x;
  y_coord = ptguardian->y_coord + ptguardian->gard_ycent + y;
  flagDepla4 += resolution;
  screenOver (tir_minixy);
}


//------------------------------------------------------------------------------
// weapons: trajectory number 9 - sucker circle (circle which rebounds )
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile09 ()
{
  flagDepla3 = flagDepla3 + 4;
  if (flagDepla3 >= 360)
    flagDepla3 = 0;
  Sint32 r = (16 * resolution);
  Sint32 x = tablesinus[flagDepla3] * r;
  Sint32 y = tablecosin[flagDepla3] * r;
  x = x >> 7;
  y = y >> 7;
  x_coord = flagDepla1 + x;
  y_coord = flagDepla2 + y;
  flagDepla2 += resolution;
  if (flagDepla4 > 0)
    {
      flagDepla1 += (5 * resolution);
      if (flagDepla1 > (284 * resolution))
        flagDepla4 = 0;
    }
  else
    {
      flagDepla1 -= (5 * resolution);
      if (flagDepla1 < (-80 * resolution))
        flagDepla4 = 1;
    }
  screenStop (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 10 - elipse
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile10 ()
{
  flagDepla3 = flagDepla3 + 6;
  if (flagDepla3 >= 360)
    flagDepla3 -= 360;
  Sint32 ray_x = flagDepla4;
  Sint32 ray_y = ray_x - (3 * resolution);
  if (--flagDepla5 <= 0)
    {
      flagDepla5 = 3;
      flagDepla4 += resolution;
    }
  Sint32 x = tablesinus[flagDepla3] * ray_x;
  Sint32 y = tablecosin[flagDepla3] * ray_y;
  x = x >> 7;
  y = y >> 7;
  x_coord = flagDepla1 + x;
  y_coord = flagDepla2 + y;
  flagDepla2 += 3 * resolution;
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 11 - triangle I
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile11 ()
{
  flagDepla3 = flagDepla3 + 4;
  if (flagDepla3 >= 360)
    flagDepla3 -= 360;
  Sint32 r = (11 * resolution);
  Sint32 x = tablesinus[flagDepla3] * r;
  Sint32 y = tablecosin[flagDepla3] * r;
  x = x >> 7;
  y = y >> 7;
  x_coord = flagDepla1 + x;
  y_coord = flagDepla2 + y;
  flagDepla2 += (resolution * 3);
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: trajectory number 12 - triangle II
//------------------------------------------------------------------------------
void
sprite_bullet::dmissile12 ()
{
  flagDepla3 = flagDepla3 + 4;
  if (flagDepla3 >= 360)
    flagDepla3 -= 360;
  Sint32 r = (sprite_width * resolution);
  Sint32 x = tablesinus[flagDepla3] * r;
  Sint32 y = tablecosin[flagDepla3] * r;
  x = x >> 7;
  y = y >> 7;
  x_coord = flagDepla1 + x;
  y_coord = flagDepla2 + y;
  flagDepla1 += flagDepla4;
  flagDepla2 += flagDepla5;
  screenOver (tir_minixy);
}

//------------------------------------------------------------------------------
// weapons: overflow test
//------------------------------------------------------------------------------
void
sprite_bullet::screenOver (Sint32 vmini)
{
  if (x_coord > tir_maxi_x || x_coord < vmini ||
      y_coord < vmini || y_coord > tir_maxi_y)
    is_enabled = 0;
}

//------------------------------------------------------------------------------
// weapons: overflow test
//------------------------------------------------------------------------------
void
sprite_bullet::screenStop (Sint32 vmini)
{
  if (y_coord > tir_maxi_y)
    is_enabled = 0;
  else
    {
      if (y_coord < vmini)
        y_coord = vmini;
      if (x_coord > tir_maxi_x)
        x_coord = tir_maxi_x;
      else
        {
          if (x_coord < vmini)
            x_coord = vmini;
        }
    }
}
