/** 
 * @file head_animation.cc 
 * @brief Animate the head in the right score panel 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: head_animation.cc,v 1.2 2007/02/15 17:12:24 gurumeditation Exp $
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
#include "../include/head_animation.h"
#include "../include/handler_resources.h"

/**
 * Create the head animation object
 */
head_animation::head_animation ()
{
  object_init ();
  head_bitmap = (bitmap_data *) NULL;
  frame_delay = 0;
  requested_animation = 0;
  frame_index = 0;
  frame_step = 0;
}

/**
 * Release the head animation object
 */
head_animation::~head_animation ()
{
  object_free ();
  if (head_bitmap)
    {
      delete head_bitmap;
      head_bitmap = (bitmap_data *) NULL;
    }
}

/** 
 * Load the bitmap of the head animation
 */
void
head_animation::load_bitmap ()
{
  head_bitmap = new bitmap_data ();
  head_bitmap->load (handler_resources::RESHEADANI);
  off_source = head_bitmap->get_row_size ();
  off_destin = game_screen->get_width ();
  adr_source = head_bitmap->get_pixel_data ();
  adr_destin =
    game_screen->get_pixel_data (272 * resolution, 106 * resolution);
  head_height = head_bitmap->get_height ();
  head_width = head_bitmap->get_width () / MAX_OF_IMAGES;
  printf("head_width ===============> %i\n",head_width);

}

/**
 * Draw the current image of the hean animation
 */
void
head_animation::draw ()
{
  
  Uint32 xcoord = frame_index * head_width;
  //game_screen->blit_surface (head_bitmap, xcoord, 0, 272 * resolution, 106 * resolution, head_width, head_height);

  return;


  Sint32 *desti = (Sint32 *) adr_destin;
  Sint32 *surce = (Sint32 *) (adr_source + (frame_index * 32 * resolution));
  Sint32 off_s = off_source;
  Sint32 off_d = off_destin;
  Sint32 j = head_height;
  if (resolution == 1)
    {
      for (Sint32 i = 0; i < j; i++)
        {
          desti[0] = surce[0];
          desti[1] = surce[1];
          desti[2] = surce[2];
          desti[3] = surce[3];
          desti[4] = surce[4];
          desti[5] = surce[5];
          desti[6] = surce[6];
          desti[7] = surce[7];
          surce = (Sint32 *) ((char *) surce + off_s);
          desti = (Sint32 *) ((char *) desti + off_d);
        }
    }
  else
    {
      for (Sint32 i = 0; i < j; i++)
        {
          desti[0] = surce[0];
          desti[1] = surce[1];
          desti[2] = surce[2];
          desti[3] = surce[3];
          desti[4] = surce[4];
          desti[5] = surce[5];
          desti[6] = surce[6];
          desti[7] = surce[7];
          desti[8] = surce[8];
          desti[9] = surce[9];
          desti[10] = surce[10];
          desti[11] = surce[11];
          desti[12] = surce[12];
          desti[13] = surce[13];
          desti[14] = surce[14];
          desti[15] = surce[15];
          surce = (Sint32 *) ((char *) surce + off_s);
          desti = (Sint32 *) ((char *) desti + off_d);
        }
    }
}

/**
 * Play and draw the animation of the head
 */
void
head_animation::play ()
{
  draw ();
  /* Counter time delay before next image */
  if (frame_delay > 0)
    {
      frame_delay--;
      return;
    }

  Uint32 frame_num = requested_animation;
  requested_animation = 0;
  /* animation in progress? */
  if (frame_index == 0) 
    {
      frame_index = frame_num;
      frame_step = 1;
      frame_delay = FRAME_PERIOD_1;
      return;
    }

  /* forward animation */
  if (frame_step >= 0)
    {
      switch (frame_index)
        {
        case INTERFERENCE_STOP:
          frame_step = -1;
          frame_index = frame_index + frame_step;
          break;
        case YAWN_STOP:
          frame_step = -1;
          frame_delay = FRAME_PERIOD_2;
          break;
        case LAUGH_STOP:
          frame_step = -1;
          frame_delay = FRAME_PERIOD_2;
          break;
        default:
          frame_delay = FRAME_PERIOD_1;
          frame_index = frame_index + frame_step;
        }
      return;
    }

  /* reverse animation */
  switch (frame_index)
    {
    case INTERFERENCE_START:
      frame_index = 0;
      break;
    case YAWN_START:
      frame_index = 0;
      break;
    case LAUGH_START:
      frame_index = 0;
      break;
    default:
      frame_delay = FRAME_PERIOD_1;
      frame_index = frame_index + frame_step;
    }
}

/**
 * Start laugh head animation
 */
void
head_animation::start_laugh ()
{
  requested_animation = LAUGH_START;
}

/**
 * Start yawn head animation
 */
void
head_animation::start_yawn ()
{
  requested_animation = YAWN_START;
}

/**
 * Start interference animation
 */
void
head_animation::start_interference ()
{
  requested_animation = INTERFERENCE_START;
}
