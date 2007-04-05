/** 
 * @file controller_spheres.cc 
 * @brief Metallic spheres controller used in congratulations 
 * @created 2004-08-05 
 * @date 2007-04-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_spheres.cc,v 1.5 2007/04/05 16:04:01 gurumeditation Exp $
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
  max_of_sprites = 8;
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

//-----------------------------------------------------------------------------
// runtime
//-----------------------------------------------------------------------------
void
controller_spheres::execution1 ()
{
  const Sint16 *ptSin = handler_resources::zesinus360;
  const Sint16 *ptCos = handler_resources::cosinus360;

  Sint32 r_max = 360;
  Sint32 rad_x = 80 * resolution;
  Sint32 rad_y = 80 * resolution;
  Sint32 centx = (160 * resolution) - (sprites_list[0]->sprite_width / 2);
  Sint32 centy = (120 * resolution) - (sprites_list[0]->sprite_height / 2);

  // varie vitesse des boulles
  speed_rad4 = speed_rad4 + (random_counter & 3);
  if (speed_rad4 >= r_max)
    speed_rad4 -= r_max;
  Sint32 h = (ptSin[speed_rad4] * 2) >> 7;
  Sint32 v = (ptCos[speed_rad4] * 2) >> 7;
  Sint32 sball = 1 + h + v;
  if (!sball)
    {
      sball = 1;
    }

  // varie pointeur
  speed_rad3 = speed_rad3 + (random_counter & 7);
  if (speed_rad3 >= r_max)
    {
      speed_rad3 -= r_max;
    }
  h = (ptSin[speed_rad3] * 3) >> 7;
  v = (ptCos[speed_rad3] * 3) >> 7;
  Sint32 incRd = h + v + 6;
  //printf("** h=%i v=%i speed_rad3=%i\n", h ,v, speed_rad3);

  //VARIE RAYON X
  speed_rad1 = speed_rad1 + incRd;
  if (speed_rad1 >= r_max)
    {
      speed_rad1 -= r_max;
    }
  h = (ptSin[speed_rad1] * 30 * resolution) >> 7;
  v = (ptCos[speed_rad1] * 30 * resolution) >> 7;
  rad_x = rad_x + h + v;
  /*printf("[rad_x  = %i] h=%i v=%i  / incRd=%i speed_rad1=%i \n",
     rad_x, h, v, incRd, speed_rad1); */

  //VARIE RAYON Y
  speed_rad2 = speed_rad2 + incRd;
  if (speed_rad2 >= r_max)
    speed_rad2 -= r_max;
  h = (ptSin[speed_rad2] * 15 * resolution) >> 7;
  v = (ptCos[speed_rad2] * 15 * resolution) >> 7;
  rad_y = rad_y + h + v;


  //Sint32 sball = ptSin[speed_rad0] >> 7 ;
  //spra1 = ptSin[speed_rad0];



  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      zebob->x_maximum += sball;
      //printf("[%i]", zebob->x_maximum);
      if (zebob->x_maximum >= r_max)
        zebob->x_maximum -= r_max;
      if (zebob->x_maximum < 0)
        zebob->x_maximum += r_max;
      Sint32 pos_x =
        (handler_resources::zesinus360[zebob->x_maximum] * rad_x) >> 7;
      Sint32 pos_y =
        (handler_resources::cosinus360[zebob->x_maximum] * rad_y) >> 7;
      pos_x += centx;
      pos_y += centy;
      zebob->x_coord = pos_x;
      zebob->y_coord = pos_y;
      //printf("=> %i %i \n", pos_x , pos_y);
    }
}
