/** 
 * @file controller_gems.cc 
 * @brief The gems controller 
 * @created 2004-04-12 
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gems.cc,v 1.6 2007/02/11 21:03:24 gurumeditation Exp $
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

#include "../include/controller_gems.h"

/**
 * Create the gems controller
 */
controller_gems::controller_gems ()
{
  littleInit ();
  max_of_sprites = MAX_OF_GEMS;
  sprites_have_shades = true;
  sprite_type_id = BOB_GEMSTO;
}

/**
 * Release the gems controller
 */
controller_gems::~controller_gems ()
{
  release_sprites_list ();
}

/**
 * Initialize the gemstones sprites
 * @param score
 * @param coins
 * @param pad pointer to the paddles controller 
 */
void
controller_gems::initialize (right_panel_score * score,
                             controller_indicators * coins,
                             controller_paddles * pad)
{
  ptBarScore = score;
  ptPrintmon = coins;
  paddles = pad;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (current_player->gem_is_set (i))
        gem->gemcollect (i);
    }
}

/**
 * Send a gem stone
 * @param ball a pointer to the ball sprite which destroyed the enemy ship
 */
void
controller_gems::send (sprite_ball * ball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (gem->disponible (ball))
        {
          return;
        }
    }
}

/**
 * Send a gem stone
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 */
void
controller_gems::send (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (gem->disponible (blast))
        {
          return;
        }
    }
}

/**
 * Move gems and collision with the paddles
 */
void
controller_gems::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      Sint32 ztype = gem->move ();
      if (ztype < 0)
        {
          /* this gem is disabled or was not collected by a paddle  */
          continue;
        }
      if (current_player->gem_is_set (ztype))
        {
          /* this gem was already collected */
          continue;
        }
      current_player->add_score (500);
      gem->gemcollect (ztype);
      if (!current_player->gem_enable (ztype))
        {
          /* the six gems were not collected yet */
          continue;
        }
      /*
       * the six gems were collected!
       */
      for (Uint32 k = 0; k < max_of_sprites; k++)
        {
          sprite_gem *zegem = sprites_list[k];
          zegem->activBlink ();
        }
      current_player->add_score (2500);
      ptPrintmon->increase_money_amount (500);
      current_player->add_life (3);
      /* jump to the next level */
      ptBarScore->scoreBrick (0);
      /* enable right paddle */
      current_player->set_bumpOn (controller_paddles::RIGHT_PADDLE, 3);
      /* enable top paddle */
      current_player->set_bumpOn (controller_paddles::TOP_PADDLE, 3);
      /* enable left paddle */
      current_player->set_bumpOn (controller_paddles::LEFT_PADDLE, 3);
      sprite_paddle *paddle;
      paddle = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle->enable ();
      paddle = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle->enable ();
      paddle = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle->enable ();
      return;
    }
}
