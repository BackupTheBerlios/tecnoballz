/** 
 * @file controller_explosions.cc 
 * @brief Explosions controller  
 * @created 2003-03-02 
 * @date 2007-02-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_explosions.cc,v 1.2 2007/02/09 17:05:29 gurumeditation Exp $
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
#include "../include/controller_explosions.h"
#include "../include/handler_audio.h"

/**
 * Create the explosions controller
 */
controller_explosions::controller_explosions ()
{
  littleInit ();
  max_of_sprites = 28;
  sprites_have_shades = false;
  sound_delay = 0;
  sprite_type_id = BOB_EXPLO1;
}

/**
 * Release the explosions controller
 */
controller_explosions::~controller_explosions ()
{
  release_sprites_list ();
}

/**
 * Create the list of explosions sprites
 */
void
controller_explosions::create_explosions_list ()
{

  alloc_sprites_list ();
  Sint32 bobn1 = BOB_EXPLO1;
  Sint32 bobn2 = BOB_EXPLO2;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *explosion = new sprite_object ();
      explosion->set_object_pos (i);
      explosion->create_sprite (bobn1, sprites_bitmap, 0);
      Sint32 bobnu = bobn2;
      bobn2 = bobn1;
      bobn1 = bobnu;
      sprites_list[i] = explosion;
      sprites->add (explosion);
    }
}

/**
 * Add a new explosion
 * @param xcoord x coordinate of the explosion
 * @param ycoord y coordinate of the explosion
 */
void
controller_explosions::add (Uint32 xcoord, Uint32 ycoord)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *explosion = sprites_list[i];
      if (explosion->is_enabled)
        {
          continue;
        }
          if (++sound_delay > 4)
            {
              sound_delay = 0;
#ifndef SOUNDISOFF
              audio->play_sound (S_BIGEXPLO);
#endif
            }
          explosion->is_enabled = true;
          explosion->x_coord = xcoord - (explosion->sprite_width / 2);
          explosion->y_coord = ycoord - (explosion->sprite_height / 2);
          explosion->frame_index = explosion->frame_index_min;
          explosion->frame_period = 4 + (hasard_val & 7);
          explosion->frame_delay = explosion->frame_period;
          explosion->set_image ();
          return;
    }
}

/**
 * Animation of all explisions
 */ 
void
controller_explosions::play_animation ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *explosion = sprites_list[i];
      if (explosion->is_enabled)
        explosion->play_animation_once ();
    }
}
