/** 
 * @file controller_spheres.cc 
 * @brief Metallic spheres controller used in congratulations 
 * @created 2004-08-05 
 * @date 2007-04-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_spheres.cc,v 1.8 2007/04/07 15:39:10 gurumeditation Exp $
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
#include "../include/controller_spheres.h"
#include "../include/handler_audio.h"
#include "../include/handler_resources.h"
#include "../include/handler_high_score.h"
#include "../include/handler_resources.h"

/**
 * Create the metallic spheres controller
 */
controller_spheres::controller_spheres ()
{
  littleInit ();
  /* 8 metallics spheres */
  max_of_sprites = 16;
  sprites_have_shades = true;
  sprites_have_shades = false;
  sprite_type_id = BOB_ARGENT;
  speed_rad0 = 0;
  speed_rad1 = 0;
  speed_rad2 = 0;
  speed_rad3 = 0;
  speed_rad4 = 0;
}

/**
 * Release the metallic spheres controller
 */
controller_spheres::~controller_spheres ()
{
  release_sprites_list ();
}

/**
 * Perform some initializations
 */
void
controller_spheres::initialize ()
{
  Sint32 offst = 360 / max_of_sprites;
  Sint32 value = 0;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprites_list[i]->enable ();
      sprites_list[i]->x_maximum = value;
      value += offst;
    }
}

/**
 * Animate the metal spheres 
 */
void
controller_spheres::run ()
{
  const Sint16 *ptSin = handler_resources::zesinus360;
  const Sint16 *ptCos = handler_resources::cosinus360;
  Sint32 res = resolution;
  Sint32 angle_max = 360;
  Sint32 horizontal_radius = 80 * res;
  Sint32 vertical_radius = 80 * res;

  /* rotation speed variation */
  speed_rad4 = speed_rad4 + (random_counter & 3);
  if (speed_rad4 >= angle_max)
    {
      speed_rad4 -= angle_max;
    }
  Sint32 h = (ptSin[speed_rad4] * 2) >> 7;
  Sint32 v = (ptCos[speed_rad4] * 2) >> 7;
  Sint32 sphere_speed = 4 + h + v;
  if (0 == sphere_speed)
    {
      sphere_speed = 1;
    }

  // varie pointeur
  speed_rad3 = (speed_rad3 + (random_counter & 7)) % angle_max;
  h = (ptSin[speed_rad3] * 3) >> 7;
  v = (ptCos[speed_rad3] * 3) >> 7;
  Sint32 radius_inc = h + v + 6;

  /* horizontal radius variation */
  speed_rad1 = (speed_rad1 + radius_inc) % angle_max;
  h = (ptSin[speed_rad1] * 30 * res) >> 7;
  v = (ptCos[speed_rad1] * 30 * res) >> 7;
  horizontal_radius = horizontal_radius + h + v;

  /* vertical radius variation */
  speed_rad2 = (speed_rad2 + radius_inc) % angle_max;
  h = (ptSin[speed_rad2] * 15 * res) >> 7;
  v = (ptCos[speed_rad2] * 15 * res) >> 7;
  vertical_radius = vertical_radius + h + v;
  
  Sint32 x_center = (160 * res) - (sprites_list[0]->sprite_width / 2);
  Sint32 y_center = (120 * res) - (sprites_list[0]->sprite_height / 2);
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sphere = sprites_list[i];
      sphere->x_maximum = (sphere->x_maximum + sphere_speed) % angle_max;
      Sint32 xcoord = (ptSin[sphere->x_maximum] * horizontal_radius) >> 7;
      Sint32 ycoord = (ptCos[sphere->x_maximum] * vertical_radius) >> 7;
      xcoord += x_center;
      ycoord += y_center;
      sphere->x_coord = xcoord;
      sphere->y_coord = ycoord;
    }
}
