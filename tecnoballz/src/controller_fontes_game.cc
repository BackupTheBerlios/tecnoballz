//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "controller_fontes_game.cc"
// created      : ?
// updates      : 2005-01-23
// fonction     : manage mobiles characters ("LEVEL x COMPLETED")
// id           : $Id: controller_fontes_game.cc,v 1.1 2007/02/11 16:04:44 gurumeditation Exp $
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#include "../include/controller_fontes_game.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
controller_fontes_game::controller_fontes_game ()
{
  littleInit ();
  size_line1 = 0;
  size_line2 = 0;
  horz_large = 0;
  max_of_sprites = 20;
  sprites_have_shades = true;
  sprite_type_id = BOB_LETTRE;
  chrOffsetX = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
controller_fontes_game::~controller_fontes_game ()
{
  release_sprites_list ();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void
controller_fontes_game::initialise (Sint32 level, Sint32 offzt)
{
  chrOffsetX = offzt;
  horz_large = 256 * resolution;
  char *monPT = ze_bobText;
  //if(level <= 12)
  {
    Sint32 d = level / 10;
    Sint32 u = level - d * 10;
    ze_bobText[6] = (char) d + '\\';
    ze_bobText[7] = (char) u + '\\';
  }
  /*else
     {    monPT = ze_endText;
     } */

  //###################################################################
  // count the number of characters
  //###################################################################
  size_line1 = 0;
  while (*(monPT++))
    size_line1++;
  size_line2 = 0;
  while (*(monPT++))
    size_line2++;
  size_total = size_line1 + size_line2;

  //###################################################################
  // initialize the images for each character
  //###################################################################
  monPT = ze_bobText;
  for (Sint32 i = 0; i < size_total; i++)
    {
      sprite_fonte_game *chara = sprites_list[i];
      char c = *(monPT++);
      if (c == '\0')
        c = *(monPT++);
      chara->new_offset (c - 'A');
    }

  //###################################################################
  // initialize characters of "LEVEL n"
  //###################################################################
  Sint32 yStrt = 200 * resolution;
  Sint32 a = startValue (size_line1, 0, 0,
                         yStrt, -6 * resolution, 60 * resolution);

  //###################################################################
  // initialize characters of "COMPLETED"
  //###################################################################
  yStrt += 10 * resolution;
  startValue (size_line2, a, size_line1,
              yStrt, -8 * resolution, 80 * resolution);
}

//-----------------------------------------------------------------------------
// initialize start values
// input        => nchar: number of chars peer line
//                      => zerad: radius
//                      => index: 
//                      => yStrt: start y coordinate
//                      => yOffs: Y move offset 
//                      => yStop: stop Y coordinate
//-----------------------------------------------------------------------------
Sint32
controller_fontes_game::startValue (Sint32 nchar, Sint32 zeRad, Sint32 index,
                                    Sint32 yStrt, Sint32 yOffs, Sint32 yStop)
{
  Sint32 width = sprites_list[0]->get_sprite_height ();
  Sint32 e = (horz_large) / nchar;
  Sint32 xStop = (horz_large - (nchar * width)) / 2;
  Sint32 xOffs = 0;             //X move offset (1, -1 or 0)
  Sint32 xStrt = e / 2;         //start X coordinate

  xStop += chrOffsetX;
  xStrt += chrOffsetX;

  for (Sint32 i = index; i < (nchar + index); i++, xStrt += e, xStop += width)
    {
      sprite_fonte_game *chara = sprites_list[i];
      chara->set_coordinates (xStrt, yStrt);
      if (xStrt > xStop)
        xOffs = -1;
      else
        {
          if (xStrt == xStop)
            xOffs = 0;
          else
            xOffs = 1;
        }
      chara->littleInit (zeRad, xStop, yStrt, xOffs, yOffs, yStop);
      zeRad += 8;
      zeRad &= SINUS_MASK;
    }
  return zeRad;
}

//-----------------------------------------------------------------------------
// animation of characters sprites
//-----------------------------------------------------------------------------
void
controller_fontes_game::goMoveText ()
{
  for (Sint32 i = 0; i < size_total; i++)
    {
      sprite_fonte_game *chara = sprites_list[i];
      chara->moveCaract ();
    }
}

//-----------------------------------------------------------------------------
// enable characters sprites
//-----------------------------------------------------------------------------
void
controller_fontes_game::activeText ()
{
  for (Sint32 i = 0; i < size_total; i++)
    {
      sprite_fonte_game *chara = sprites_list[i];
      chara->enable ();
    }
}

//-----------------------------------------------------------------------------
// string to display
//-----------------------------------------------------------------------------
char
  controller_fontes_game::ze_bobText[] = "LEVEL[//\0COMPLETED\0";
char
  controller_fontes_game::ze_endText[] = "[[GAME[[\0FINISHED[\0";