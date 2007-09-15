/** 
 * @file sprite_brick.cc
 * @brief The sprite of the brick 
 * @created 2007-09-12
 * @date 2007-09-15
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_brick.cc,v 1.2 2007/09/15 08:45:16 gurumeditation Exp $
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
  clear_sprite_members ();
}

/**
 * Release the Gigablitz sprite
 */
sprite_brick::~sprite_brick ()
{
}

void
sprite_brick::draw ()
{
  cycling_index &= 7;
  Sint32 color = current_cycling[cycling_index++];
  printf("color: %i\n", color);
  char *screen = game_screen->get_pixel_data (x_coord, y_coord);
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
          if (p == 0)
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



