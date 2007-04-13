/**
 * @file controller_gigablitz.cc 
 * @brief Gigablitz controller 
 * @date 2007-04-12
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.11 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gigablitz.cc,v 1.11 2007/04/13 22:15:17 gurumeditation Exp $
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
#include "../include/controller_gigablitz.h"
#include "../include/handler_resources.h"

Uint32 controller_gigablitz::numeroBOBs[MAX_OF_GIGABLITZ] =
  {
    BOB_GIGAB7,
    BOB_GIGAB6, BOB_GIGAB5, BOB_GIGAB4, BOB_GIGAB3, BOB_GIGAB2, BOB_GIGAB1};

/**
 * Create the Gigablitz controller
 */
controller_gigablitz::controller_gigablitz ()
{
  littleInit ();
  /* there are 7 different Gigablitz */
  max_of_sprites = MAX_OF_GIGABLITZ;
  sprites_have_shades = false;
  paddle_bottom = (sprite_paddle *) NULL;
  paddle_top = (sprite_paddle *) NULL;
  gigablitz_height = 0;
  bitz_ystop = 0;
  bitz_maxiy = 0;
  bitz_miniy = 0;
  blitz_seta = 0;
  blitz_brik = 0;
  blitz_xsin = 0;
  blitz_colx = 0;
}

/**
 * Release the Gigablitz controller
 */
controller_gigablitz::~controller_gigablitz ()
{
  release_sprites_list ();
}

/**
 * Create and initialize the sprites of the gigablitz in the bricks levels
 */
void
controller_gigablitz::create_gigablitz_sprites ()
{
  controller_paddles* paddles = controller_paddles::get_instance ();
  paddle_bottom = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  if (BRICKS_LEVEL == super_jump)
    {
      paddle_top = paddles->get_paddle (controller_paddles::TOP_PADDLE);
    }
  alloc_sprites_list ();

  /* load the bitmap of the different Gigablitz,
   * in the 'sprites_bitmap' static member */
  resources->load_sprites_bitmap (handler_resources::BITMAP_GIGABLITZ);

  /* create and initialize the gigablitz sprites */
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gigablitz *gigablitz_sprite = new sprite_gigablitz ();
      gigablitz_sprite->set_object_pos (i);
      gigablitz_sprite->set_draw_method (sprite_object::DRAW_LINE_BY_LINE);
      if (GUARDS_LEVEL == super_jump)
        {
          gigablitz_sprite->mirrorVert = 1;
        }
      Uint32 id = numeroBOBs[i];
      gigablitz_sprite->create_sprite (id, sprites_bitmap, false);
      sprites->add (gigablitz_sprite);
      sprites_list[i] = gigablitz_sprite;
    }

  /* release the bitmap of gigablitz */
  resources->release_sprites_bitmap ();
}

/**
 * Start a new Gigablitz in bricks levels
 */
void
controller_gigablitz::shoot_paddle  ()
{
  if (gigablitz_height > 0)
    {
      return;
    }
      Uint32 length = paddle_bottom->get_length ();
      Uint32 l = length;
      /* smallest bumper is of 16 or 32 pixels width */ 
      l -= paddle_bottom->width_mini;
      /* size of bumper step by 8 or 16 pixels */
      l >>= paddle_bottom->width_deca;
      l = MAX_OF_GIGABLITZ - l - 1;
      current_gigablitz = sprites_list[l];
      gigablitz_height = current_gigablitz->get_sprite_height ();
      Sint32 x = paddle_bottom->get_x_coord ();
      Sint32 y = paddle_bottom->get_y_coord ();
      gigablitz_xcoord = x;
      /* special collision */
      blitz_colx = x;
      current_gigablitz->set_coordinates (x, y);
      Sint32 res = resolution;
      bitz_ystop = 8 * res - gigablitz_height;
      bitz_maxiy = paddle_bottom->get_y_coord ();
      bitz_miniy = 8 * res;
      y = length;
      if (1 == resolution) 
        {
          /* in 320 pixels: width bricks = 16 pixels */
          y = y >> 4;
          x = x & 0x000f;
        }
      else
        {
          /* in 640 pixels: width bricks = 32 pixels */
          y = y >> 5;
          x = x & 0x001f;
        }
      if (x)
        y++;
      blitz_brik = y;
#ifndef SOUNDISOFF
      audio->play_sound (S_TIR_GARD);
#endif
      head_animation *head_anim = head_animation::get_instance ();
      head_anim->start_laugh ();
      controller_ships *ships = controller_ships::get_instance ();
      ships->force_explosion ();
}

/**
 * Move the Gigablitz in bricks level
 */
void
controller_gigablitz::run_in_bricks_levels ()
{
  if (0 == gigablitz_height)
    {
      return;
    }
 
  /* vertical moving */
  Sint32 y = current_gigablitz->get_y_coord ();
  Sint32 res = resolution;
  y = y - 8 * res;
  if (y <= bitz_ystop)
    {
      current_gigablitz->disable ();
      gigablitz_height = 0;
    }
  else
    {
      if (y >= bitz_maxiy)
        {
          current_gigablitz->disable ();
        }
      else
        {
          current_gigablitz->enable ();
        }
    }

  /* determine last line of the gigablitz sprite */
  Sint32 h = current_gigablitz->get_sprite_height ();
  Sint32 l = bitz_maxiy - y;
  if (l > h)
    {
      l = h;
    }
  if (l < 1)
    {
      l = 1;
    }
  current_gigablitz->affligLast = l;
  l = bitz_miniy - y;
  if (l >= h)
    {
      l = h - 1;
    }
  if (l < 0)
    {
      l = 0;
    }
  current_gigablitz->affligFrst = l;

  /* horizontal move */
  blitz_xsin = (blitz_xsin + 50) & SINUS_MASK;
  Sint32 x = (table_cosL[blitz_xsin] * 5 * res) >> SINUS_DECA;
  x = gigablitz_xcoord + x;
  current_gigablitz->set_coordinates (x, y);
  if (y >= 0)
    {
      collision1 ();
    }
}

//-------------------------------------------------------------------------------
// bricks levels: collision with the gigablitz and briks
//-------------------------------------------------------------------------------
void
controller_gigablitz::collision1 ()
{
  controller_bricks *bricks = controller_bricks::get_instance ();
  /* brick's width in pixels */
  Sint32 bwght = bricks->get_brick_width ();
  /* y-offset between 2 bricks */
  Sint32 byoff = bricks->getYOffset ();
  /* first indestructible brick */
  Sint32 indus = bricks->getBkIndus ();
  if (blitz_brik > 0)
    {
      Sint32 x = blitz_colx;
      Sint32 y = current_gigablitz->get_y_coord ();
      x /= bwght;               // x = x / 32 (width of a brick)
      y /= byoff;               // y = y / 16 (space between two bricks in height)
      y *= controller_bricks::NB_BRICKSH;       // y = y * 16 (number of bricks on the same line)
      x += y;
      brick_info *tMega = bricks->get_bricks_map ();
      for (Sint32 i = 0; i < blitz_brik; i++)
        {
          brick_info *megaT = (tMega + x);
          Sint32 v = megaT->brique_rel;
          if (v != 0)
            {
	      /* list of bricks to clear or redraw */
	      brick_redraw *briP2 = bricks->get_bricks_redraw_next (); 
              if (v < indus)
		{
                  briP2->balle_posX = 512;        // flag brick blitz destroy
		}
              else
		{
                  briP2->balle_posX = -1;
		}
              briP2->pixel_offset = megaT->pixel_offset;
              briP2->brick_map = megaT;
              megaT->h_pos = -1;
              megaT->brique_rel = 0;    // RAZ brick code
              briP2->number = megaT->number;    // brick number
	      /* restore background under brick */
              briP2->is_background = true;
            }
          x++;
        }
    }
}

/**
 * Create and initialize the sprites of the gigablitz in the guardians levels
 */
/*
void
controller_gigablitz::create_gigablitz_sprites ()
{
  controller_paddles* paddles = controller_paddles::get_instance ();
  paddle_bottom = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);

  alloc_sprites_list ();
  resources->load_sprites_bitmap (handler_resources::BITMAP_GIGABLITZ);

  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gigablitz *gigablitz_sprite = new sprite_gigablitz ();
      gigablitz_sprite->set_object_pos (i);
      gigablitz_sprite->mirrorVert = 1;
      gigablitz_sprite->set_draw_method (sprite_object::DRAW_LINE_BY_LINE);
      Sint32 n = numeroBOBs[i];
      gigablitz_sprite->create_sprite (n, sprites_bitmap, 0);
      sprites->add (gigablitz_sprite);
      sprites_list[i] = gigablitz_sprite;
    }
  resources->release_sprites_bitmap ();
}
*/

/**
 * Move the Gigablitz in the guardians level
 */
void
controller_gigablitz::run_in_guardians_level ()
{
  if (0 == gigablitz_height)
    {
      return;
    }
  Sint32 y = current_gigablitz->get_y_coord ();
  Sint32 res = resolution;
  y = y + (6 * res);
  blitz_xsin = (blitz_xsin + 50) & SINUS_MASK;
  Sint32 x = (table_cosL[blitz_xsin] * 5 * res) >> SINUS_DECA;
  x = gigablitz_xcoord + x;
  current_gigablitz->set_coordinates (x, y);

  /*
   * determine last line of the gigablitz sprite
   */
  Sint32 h = current_gigablitz->get_sprite_height ();
  Sint32 l = display->get_height () - y;
  if (l > h)
    {
      l = h;
    }
  if (l < 1)
    {
      l = 1;
    }
  current_gigablitz->affligLast = l;
  l = bitz_miniy - y;
  if (l >= h)
    l = h - 1;
  if (l < 0)
    l = 0;
  current_gigablitz->affligFrst = l;
  if (y >= 240 * res)
    {
      current_gigablitz->disable ();
      gigablitz_height = 0;
    }

  collision_with_paddle ();
}

/**
 * Collision between paddle and gigablitz in a guardians level
 */
void
controller_gigablitz::collision_with_paddle ()
{
  if (0 == gigablitz_height || paddle_bottom->is_invincible ())
    {
      return;
    }
  Sint32 gx = current_gigablitz->get_x_coord ();
  Sint32 gy = current_gigablitz->get_y_coord ();
  Sint32 gw = current_gigablitz->get_collision_width ();
  Sint32 x = paddle_bottom->get_x_coord ();
  Sint32 y = paddle_bottom->get_y_coord ();
  Sint32 w = paddle_bottom->get_length ();
  Sint32 h = paddle_bottom->get_sprite_height ();
  if (gy + (Sint32) gigablitz_height < y || gx + gw < x || gx > x + w
      || gy > y + h)
    {
      return;
    }
#ifndef SOUNDISOFF
  audio->play_sound (S_RAKEXPLO);
  audio->play_sound (S_ENLEVVIE);
#endif

  controller_explosions *explosions = controller_explosions::get_instance ();
  explosions->add
  (x + paddle_bottom->get_length () / 2,
   y + paddle_bottom->get_sprite_height () / 2);
  current_player->remove_life (1);
  paddle_bottom->set_invincibility (100);
}

/**
 * Guardian shoot a gigablitz in a guardians level
 * @param id gigablitz identifier from 0 to 7
 * @param xcoord x coordinate of the guardian
 * @param ycoord y coordinate of the guardian
 * @param width width of the guardian in pixels 
 * @param height height of the guardian in pixels 
 * @return true if the gigablitz was well fired, otherwise false
 */
bool
controller_gigablitz::shoot_guardian (Uint32 id, Sint32 xcoord,
                                      Sint32 ycoord, Uint32 width,
                                      Uint32 height)
{
  if (gigablitz_height > 0)
    {
      /* a gigablitz is already current */
      return false;
    }
  current_gigablitz = sprites_list[id];
  gigablitz_height = current_gigablitz->get_sprite_height ();
  Uint32 w = current_gigablitz->get_sprite_width ();
  current_gigablitz->set_coordinates (xcoord, ycoord);
  xcoord = xcoord + (width - w) / 2;
  if (xcoord < 0)
    {
      xcoord = 0;
    }
  current_gigablitz->set_coordinates (xcoord, ycoord);
#ifndef SOUNDISOFF

  audio->play_sound (S_TIR_GARD);
#endif

  current_gigablitz->enable ();
  gigablitz_xcoord = xcoord;
  return true;
}

/**
 * Check if the gigablitz is enabled
 * @return true if gigablitz is enabled, otherwise false
 */
bool
controller_gigablitz::is_enable ()
{
  return gigablitz_height > 0 ? true : false;
}
