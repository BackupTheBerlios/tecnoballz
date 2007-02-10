/** 
 * @file controller_moneys.cc 
 * @brief Moneys controller 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_moneys.cc,v 1.2 2007/02/10 13:22:03 gurumeditation Exp $
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

#include "../include/controller_moneys.h"

/**
 * Create the moneys controller 
 */
controller_moneys::controller_moneys ()
{
  littleInit ();
  max_of_sprites = 6;
  sprites_have_shades = true;
  sprite_type_id = BOB_MONEYS;
}

/**
 * Release the moneys controller 
 */
controller_moneys::~controller_moneys ()
{
  release_sprites_list ();
}

/** 
 * Initialize the moneys sprites in the bricks levels
 * @param delay time delay before sending a new money capsule 
 * @param score
 * @param money
 */
void
controller_moneys::initialize (Uint32 delay, barreScore * score,
                               printmoney * money)
{
  send_delay = delay;
  ptbarreScr = score;
  ptPrntmney = money;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->littleInit ();
    }
}

/**
 * Send a money capsule from a brick 
 * @param briPT a pointer to the brick which touched by a ball 
 */
void
controller_moneys::send_money_from_brick (brickClear * briPT)
{
  if (++delay_count <= send_delay)
    {
      return;
    }
  delay_count = 0;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->disponible (briPT))
        {
          return;
        }
    }
}

/**
 * Send a money capsule from a destroyed flying enemy ship
 * @param ball a pointer to the ball sprite which destroyed the enemy ship
 */
void
controller_moneys::send_money (sprite_ball * ball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->disponible (ball))
        {
          return;
        }
    }
}

/**
 * Send a money capsule from a destroyed flying enemy ship
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 */
void
controller_moneys::send_money (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->disponible (blast))
        {
          return;
        }
    }
}

/** 
 * Move money capsules and check collision with the paddles in bricks levels 
 */
void
controller_moneys::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->play_animation_loop ();
      Sint32 j = money->move ();
      if (j)
        {
          current_player->add_scores (20);
          ptPrntmney->creditPlus (j);
        }
    }
}

/** 
 * Initialize the moneys sprites in the guardians levels
 * @param delay time delay before sending a new money capsule 
 */
void
controller_moneys::initialize (Uint32 delay, printmoney * money)
{
  send_delay = delay;
  ptPrntmney = money;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->littleInit ();
    }
}

/**
 * Send a money capsule from a guardian
 * @param ball a pointer to the ball sprite which touched the guardian
 */
void
controller_moneys::send_money_from_guardian (sprite_ball * ball)
{
  if (++delay_count <= send_delay)
    {
      return;
    }
  delay_count = 0;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->disponible (ball))
        {
          return;
        }
    }
}

/** 
 * Move money capsules and check collision with the paddle in guardians levels 
 */
void
controller_moneys::move_bottom ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->play_animation_loop ();
      Sint32 j = money->deplaceMe2 ();
      if (j)
        {
          ptPrntmney->creditPlus (j);
          current_player->add_scores (20);
        }
    }
}
