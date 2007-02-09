/** 
 * @file controller_game_over.cc 
 * @brief Game Over controller 
 * @created 2002-12-14
 * @date 2007-02-09
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_game_over.cc,v 1.2 2007/02/09 17:05:29 gurumeditation Exp $
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
#include "../include/controller_game_over.h"
#include "../include/handler_audio.h"
#include "../include/handler_resources.h"
#include "../include/scoretable.h"

/**
 * Create the Game Over controller
 */
controller_game_over::controller_game_over ()
{
  littleInit ();
  /* there are 8 letters */
  max_of_sprites = 8;
  sprites_have_shades = true;
  max_of_sprites = 8;
  sprite_type_id = BOB_GAMEOV;
  move_phase = 0;
  ptScorOver = (score_over *) NULL;
}

/**
 * Release the Game Over controller
 */
controller_game_over::~controller_game_over ()
{
  if (NULL != ptScorOver)
    {
      delete ptScorOver;
      ptScorOver = NULL;
    }
  release_sprites_list ();
}

//-----------------------------------------------------------------------------
// rerturn pointer to the "score_over" object
//-----------------------------------------------------------------------------
score_over *
controller_game_over::gtScorOver ()
{
  return ptScorOver;
}

//-----------------------------------------------------------------------------
//      perform some initializations
//-----------------------------------------------------------------------------
Sint32
controller_game_over::first_init (Sint32 offzt)
{
  chrOffsetX = offzt;

  /* initialize score table */ 
  ptScorOver = new score_over ();
  error_init (ptScorOver->first_init (chrOffsetX));
  if (erreur_num)
    return erreur_num;
  return erreur_num;
}

//-----------------------------------------------------------------------------
//      perform some initializations
//-----------------------------------------------------------------------------
void
controller_game_over::initialize (Sint32 iswin)
{
  Sint32 x = 100 * resolution;
  Sint32 y = 200 * resolution;
  const Sint32 *p = zeus_over1;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      zebob->enable ();
      zebob->set_coordinates (x, y);
      zebob->set_image (i);
      Sint32 x2 = *(p++);
      Sint32 y2 = *(p++);
      zebob->x_maximum = x2;
      zebob->y_maximum = y2;
    }
  move_phase = 1;
  go_zetempo = 50 * 10;
#ifndef SOUNDISOFF
  Sint32 iscla = ptScoreTab->test_score ();
  if (iswin)
    audio->play_music (MUSICCONGR);
  else
    {
      if (iscla)
        audio->play_music (MUSICSCORE);
      else
        audio->play_music (MUSICGOVER);
    }
#else
  ptScoreTab->test_score ();
#endif
  ptScorOver->copyToText ();
}

//-----------------------------------------------------------------------------
// runtime
//-----------------------------------------------------------------------------
void
controller_game_over::execution1 (Sint32 iswin)
{
  switch (move_phase)
    {
    case 0:
      initialize (iswin);
      break;

    case 1:
      deplace_01 ();
      break;

    case 2:
      deplace_02 ();
      break;

    case 3:
      deplace_03 ();
      break;

    case 4:
      deplace_04 ();
      break;
    }
  ptScorOver->displayTxt ();
  ptScorOver->draw_copy_from_bitmap ();
}

//-----------------------------------------------------------------------------
// movement 1
//-----------------------------------------------------------------------------
void
controller_game_over::deplace_01 ()
{
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (++zebob->x_maximum >= maxi)
        zebob->x_maximum = 0;
      if (++zebob->y_maximum >= maxi)
        zebob->y_maximum = 0;
      Sint32 x = chrOffsetX + decal + sinus[zebob->x_maximum] * resolution;
      Sint32 y = decal + sinus[zebob->y_maximum] * resolution;
      zebob->set_coordinates (x, y);
      hasard_val += y;
    }
  if (--go_zetempo <= 0)
    {
      move_phase = 2;
    }
}

//-----------------------------------------------------------------------------
// movement 2
//-----------------------------------------------------------------------------
void
controller_game_over::deplace_02 ()
{
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (++zebob->y_maximum >= maxi)
        zebob->y_maximum = 0;
      Sint32 y = decal + sinus[zebob->y_maximum] * resolution;
      zebob->set_y_coord (y);
      hasard_val += y;
    }

  // move the letters "G", "A", "M", "E"
  Sint32 f = 0;
  Sint32 v = 32 * resolution + chrOffsetX;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (zebob->x_coord > v)
        {
          zebob->x_coord -= resolution;
          f = 1;
        }
    }

  // move the letters "O", "V", "E", "R"
  v = 192 * resolution + chrOffsetX;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (zebob->x_coord < v)
        {
          zebob->x_coord += resolution;
          f = 1;
        }
    }

  if (f <= 0)
    move_phase = 3;
}

//-----------------------------------------------------------------------------
// movement 3
//-----------------------------------------------------------------------------
void
controller_game_over::deplace_03 ()
{
  // move the letters "G", "A", "M", "E"
  Sint32 f = 0;
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (++zebob->y_maximum >= maxi)
        zebob->y_maximum = 0;
      if (zebob->y_coord <= decal)
        {
          zebob->y_coord = decal;
          f++;
        }
      else
        {
          Sint32 y = decal + sinus[zebob->y_maximum] * resolution;
          zebob->set_y_coord (y);
        }
      hasard_val += zebob->y_maximum;
    }

  // move the letters "O", "V", "E", "R"
  Sint32 v = 191 * resolution;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      if (++zebob->y_maximum >= maxi)
        zebob->y_maximum = 0;
      if (zebob->y_coord >= v)
        {
          zebob->y_coord = v;
          f++;
        }
      else
        {
          Sint32 y = decal + sinus[zebob->y_maximum] * resolution;
          zebob->set_y_coord (y);
        }
      hasard_val += zebob->y_maximum;
    }
  if (f == 8)
    move_phase = 4;
}

//-----------------------------------------------------------------------------
// movement 4 : the letters are moving towards their final abscissas
//-----------------------------------------------------------------------------
void
controller_game_over::deplace_04 ()
{
  // move the letters "G", "A", "M", "E"
  Sint32 offst = 2 * resolution;
  Sint32 final = 35 * resolution + chrOffsetX;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *zebob = sprites_list[i];
      Sint32 o = final - zebob->x_coord;
      if (o > offst)
        o = offst;
      else
        {
          if (o < (-offst))
            o = -offst;
        }
      zebob->x_coord += o;
      final = final + 54 * resolution;
      hasard_val += zebob->x_coord;
    }

  // move the letters "O", "V", "E", "R"
  final = 32 * resolution + chrOffsetX;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *zebob = sprites_list[i];
      Sint32 o = final - zebob->x_coord;
      if (o > offst)
        o = offst;
      else
        {
          if (o < (-offst))
            o = -offst;
        }
      zebob->x_coord += o;
      final = final + 54 * resolution;
      hasard_val += zebob->x_coord;
    }
}

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
const Sint32
  controller_game_over::sinus_over[SIZETSINUS] =
  { 159, 159, 159, 159, 158, 158, 158, 157, 156, 156,
  155, 154, 153, 152, 151, 150, 149, 148, 147, 145,
  144, 142, 141, 139, 137, 135, 134, 132, 130, 128,
  126, 124, 122, 120, 117, 115, 113, 111, 108, 106,
  104, 101, 99, 96, 94, 91, 89, 86, 84, 81,
  79, 76, 74, 71, 69, 66, 64, 61, 59, 57,
  54, 52, 50, 47, 45, 43, 40, 38, 36, 34,
  32, 30, 28, 26, 24, 23, 21, 19, 18, 16,
  15, 13, 12, 11, 9, 8, 7, 6, 5, 4,
  4, 3, 2, 2, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 2, 2, 3, 4,
  4, 5, 6, 7, 8, 9, 11, 12, 13, 15,
  16, 18, 19, 21, 23, 24, 26, 28, 30, 32,
  34, 36, 38, 40, 43, 45, 47, 50, 52, 54,
  57, 59, 61, 64, 66, 69, 71, 74, 76, 79,
  81, 84, 86, 89, 91, 94, 96, 99, 101, 104,
  106, 108, 111, 113, 115, 117, 120, 122, 124, 126,
  128, 130, 132, 134, 135, 137, 139, 141, 142, 144,
  145, 147, 148, 149, 150, 151, 152, 153, 154, 155,
  156, 156, 157, 158, 158, 158, 159, 159, 159, 159,
};

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
const Sint32
  controller_game_over::zeus_over1[] = { 0, 175,
  25, 150,
  50, 125,
  75, 100,
  100, 75,
  125, 50,
  150, 25,
  175, 0
};
