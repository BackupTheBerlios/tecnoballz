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
 * $Id: head_animation.h,v 1.2 2007/02/15 17:12:24 gurumeditation Exp $
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
#ifndef __HEAD_ANIMATION__
#define __HEAD_ANIMATION__
class head_animation;

#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/tecnoballz.h"

class head_animation:public virtual tecnoballz
{
private:
  Sint32 off_source;            //modulo source
  Sint32 off_destin;            //modulo destination
  char *adr_source;             //graphic page memory address
  char *adr_destin;             //buffer memory address
  /** Height of the head in pixels */
  Uint32 head_height;
  /** Width of the head in pixels */
  Uint32 head_width;
  /** Bitmap which contains all head images */
  bitmap_data *head_bitmap;
  /** Time delay before next image of the animation */
  Uint32 frame_delay;
  /** Identifier of the requested animation */
  Uint32 requested_animation;
  /** Index of the current image */
  Uint32 frame_index;
  /** Offset 1 or -1 */
  Sint32 frame_step;

  typedef enum
    {
      INTERFERENCE_START = 1,
      INTERFERENCE_STOP = 3,
      YAWN_START = 4,
      YAWN_STOP = 8,
      LAUGH_START = 9,
      LAUGH_STOP = 13,
      MAX_OF_IMAGES
    }
    HEAD_FRAMES;

  /** Speed of animation */
  static const Uint32 FRAME_PERIOD_1 = 5;
  static const Uint32 FRAME_PERIOD_2 = 20;

public:
    head_animation ();
   ~head_animation ();
  void load_bitmap ();
  void play ();
  void start_yawn ();
  void start_laugh ();
  void start_interference ();
private:
  void draw ();
};
#endif
