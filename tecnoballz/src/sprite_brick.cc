/** 
 * @file sprite_brick.cc
 * @brief The sprite of the brick 
 * @created 2007-09-12
 * @date 2007-09-15
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_brick.cc,v 1.3 2007/09/15 19:20:52 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/sprite_brick.h"

/**
 * Create the Gigablitz sprite
 */
sprite_brick::sprite_brick ()
{
  current_cycling = &sprite_object::cycling_01[0];
  set_draw_method (sprite_object::DRAW_CAPSULE);
  clear_sprite_members ();
}

/**
 * Release the Gigablitz sprite
 */
sprite_brick::~sprite_brick ()
{
}

/**
 *
 * @param h_pos Brick vertical position in the bricks bitmap
 *              0, 1, 2, 3, 4, 5, 6, 7, or 8  
 */

void
sprite_brick::update_image (Uint32 h_pos)
{
   Sint32 index = frame_index - frame_index % 7 + (h_pos >> 1); 
   /* 4 3 => 22
    * 2 3 => 21
    * */
   set_image (index);
    /*
     *  0  1  2  3  4  5  6
     *  7  8  9 10 11 12 13
     * 14 15 16 17 18 19 20   
     * 21 *22 23 24 25 26 27
     * 28 29 30 31 32 33 34
     * 35 36 37 38 39 40 41  
     * 42 43 44 45 46 47 48
     */

}

void
sprite_brick::draw ()
{
  if (!is_enabled || frame_index >= max_of_images)
    {
      return;
    }
  cycling_index &= 7;
  Sint32 color = current_cycling[cycling_index++];
  char *screen = game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = screen;
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  /* pixels data of the sprite image */
  char *pixels = current_drawing_data;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_pixels;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen = screen + k;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          char p = *(pixels++);
          if (p == 29)
            {
              *(screen++) = color;
            }
          else
            {
              *(screen++) = p;
            }
        }
    }
}



