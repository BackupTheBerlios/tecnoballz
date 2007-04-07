/** 
 * @file controller_spheres.cc 
 * @brief Metallic spheres controller used in congratulations 
 * @created 2004-08-05 
 * @date 2007-04-07
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_spheres.cc,v 1.9 2007/04/07 20:24:57 gurumeditation Exp $
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
  max_of_sprites = 12;
  sprites_have_shades = true;
  sprites_have_shades = false;
  sprite_type_id = BOB_ARGENT;
  radius_horizontal_variation = 0;
  radius_vertical_variation = 0;
  radius_inc_variation = 0;
  radius_vinc_variation = 0;
  radius_sphere_speed = 0;
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
  const Sint16 *sin = handler_resources::zesinus360;
  const Sint16 *cos = handler_resources::cosinus360;
  Sint32 res = resolution;
  Sint32 angle_max = 360;
  Sint32 horizontal_radius = 80 * res;
  Sint32 vertical_radius = 80 * res;

  /* rotation speed variation */
  radius_sphere_speed = (radius_sphere_speed + (random_counter & 3)) % angle_max;
  Sint32 h = (sin[radius_sphere_speed] * 2) >> 7;
  Sint32 v = (cos[radius_sphere_speed] * 2) >> 7;
  Sint32 sphere_speed = 4 + h + v;
  if (0 == sphere_speed)
    {
      sphere_speed = 1;
    }

  /* radius increment variation */
  radius_inc_variation = (radius_inc_variation + (random_counter & 7)) % angle_max;
  h = (sin[radius_inc_variation] * 3) >> 7;
  v = (cos[radius_inc_variation] * 3) >> 7;
  Sint32 radius_inc = h + v + 6;

  /* horizontal radius variation */
  radius_horizontal_variation = (radius_horizontal_variation + radius_inc) % angle_max;
  h = (sin[radius_horizontal_variation] * 30 * res) >> 7;
  v = (cos[radius_horizontal_variation] * 30 * res) >> 7;
  horizontal_radius = horizontal_radius + h + v;

 /* radius increment variation */
  radius_vinc_variation = (radius_vinc_variation + (random_counter & 3)) % angle_max;
  h = (sin[radius_vinc_variation] * 6) >> 7;
  v = (cos[radius_vinc_variation] * 5) >> 7;
  Sint32 radius_vinc = h + v + 7;

  /* vertical radius variation */
  radius_vertical_variation = (radius_vertical_variation + radius_vinc) % angle_max;
  h = (sin[radius_vertical_variation] * 15 * res) >> 7;
  v = (cos[radius_vertical_variation] * 15 * res) >> 7;
  vertical_radius = vertical_radius + h + v;
  
  Sint32 x_center = (160 * res) - (sprites_list[0]->sprite_width / 2);
  Sint32 y_center = (120 * res) - (sprites_list[0]->sprite_height / 2);
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sphere = sprites_list[i];
      sphere->x_maximum = (sphere->x_maximum + sphere_speed) % angle_max;
      Sint32 xcoord = (sin[sphere->x_maximum] * horizontal_radius) >> 7;
      Sint32 ycoord = (cos[sphere->x_maximum] * vertical_radius) >> 7;
      xcoord += x_center;
      ycoord += y_center;
      sphere->x_coord = xcoord;
      sphere->y_coord = ycoord;
    }
}
