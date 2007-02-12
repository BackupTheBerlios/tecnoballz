/** 
 * @file controller_indicators.cc 
 * @brief Controller of money amount, reverse penalty   
 * @created 2002-11-28 
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_indicators.cc,v 1.3 2007/02/12 16:28:19 gurumeditation Exp $
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
#include "../include/controller_indicators.h"

/**
 * Create the indicators controller
 */
controller_indicators::controller_indicators ()
{
  littleInit ();
  max_of_sprites = 8;
  sprites_have_shades = false;
  y_position = YINVERSEUR * resolution;
  sprite_type_id = BOB_MONEY0;
  money_posy = y_position;
  life_sprite = NULL;
  reverse_sprite = NULL;
  money_sprite = NULL;
}

/**
 * Release the indicators controller
 */
controller_indicators::~controller_indicators ()
{
  if (NULL != life_sprite)
    {
      delete life_sprite;
      life_sprite = NULL;
    }
  if (NULL != reverse_sprite)
    {
      delete reverse_sprite;
      reverse_sprite = NULL;
    }
  if (NULL != money_sprite)
    {
      delete money_sprite;
      money_sprite = NULL;
    }
  release_sprites_list ();
}

void
controller_indicators::create_indicators_sprites (controller_paddles * pads, sprite_money *money, sprite_capsule *reverse, sprite_capsule *life)
{
  ptZraquett = pads;
  create_sprites_list ();

  money_sprite = new sprite_money ();
  money->duplicate_to (money_sprite);
  sprites->add (money_sprite);
  init_money ();
  
  /*
   * reverse sprite is only enable in the bricks levels
   */
  if (NULL != reverse)
    {
      reverse_sprite = new sprite_capsule ();
      reverse->duplicate_to (reverse_sprite);
      sprites->add (reverse_sprite);

      Sint32 x = 215 * resolution;
      reverse_sprite->new_gadget (GAD_INVERS);
      reverse_sprite->set_coordinates (x, money_posy);
      reverse_sprite->set_frame_delay (5);
      x += reverse_sprite->get_sprite_width ();
      sprite_object **liste = sprites_list + 6;
      for (Uint32 i = 0; i < 2; i++)
        {
          sprite_object *x_bob = *(liste++);
          x_bob->set_coordinates (x, money_posy);
          x += 8 * resolution;
        }
    }
  /*
   * life sprite is only enable in the guards levels
   */
  if (NULL != life)
    {
      life_sprite = new sprite_capsule ();
      life->duplicate_to (life_sprite);
      sprites->add (life_sprite);
      Sint32 x = 264 * resolution;
      life_sprite->new_gadget (GAD_LIFE_P);
      life_sprite->set_coordinates (x, money_posy);
      life_sprite->set_frame_delay (5);
      x += life_sprite->get_sprite_width ();
      sprite_object **liste = sprites_list + 6;
      for (Uint32 i = 0; i < 2; i++)
        {
          sprite_object *x_bob = *(liste++);
          x_bob->set_coordinates (x, money_posy);
          x += 8 * resolution;
          x_bob->enable ();
        }
    }
}


//-----------------------------------------------------------------------------
// bricks levels: perform some initializations
// input        => gamer:
//                      => raket:
//                      => money:
//                      => rever:
//-----------------------------------------------------------------------------
/*
void
controller_indicators::initialise (controller_paddles * raket,
                                   sprite_object * money,
                                   sprite_capsule * rever)
{
  ptZraquett = raket;
  //money_sprite = money;
  reverse_sprite = rever;

  // credit of money sprites
  init_money ();

  // initialize "reverser malus" sprites
  Sint32 x = 215 * resolution;
  reverse_sprite->new_gadget (GAD_INVERS);
  reverse_sprite->set_coordinates (x, money_posy);
  reverse_sprite->set_frame_delay (5);
  x += reverse_sprite->get_sprite_width ();
  sprite_object **liste = sprites_list + 6;
  for (Sint32 i = 0; i < 2; i++)
    {
      sprite_object *x_bob = *(liste++);
      x_bob->set_coordinates (x, money_posy);
      x += 8 * resolution;
    }
}

//-----------------------------------------------------------------------------
// guard levels: perform some initializations
// input        => gamer:
//                      => raket:
//                      => money:
//                      => lifes:
//-----------------------------------------------------------------------------
void
controller_indicators::init_guard (
                                   controller_paddles * raket,
                                   sprite_object * money,
                                   sprite_capsule * lifes)
{
  ptZraquett = raket;
  //money_sprite = money;
  life_sprite = lifes;

  // credit of money sprites
  init_money ();

  // extra life sprites
  Sint32 x = 264 * resolution;
  life_sprite->new_gadget (GAD_LIFE_P);
  life_sprite->set_coordinates (x, money_posy);
  life_sprite->set_frame_delay (5);
  x += life_sprite->get_sprite_width ();
  sprite_object **liste = sprites_list + 6;
  for (Sint32 i = 0; i < 2; i++)
    {
      sprite_object *x_bob = *(liste++);
      x_bob->set_coordinates (x, money_posy);
      x += 8 * resolution;
      x_bob->enable ();
    }
}
*/
//-----------------------------------------------------------------------------
// initialize credit of money sprites
//-----------------------------------------------------------------------------
void
controller_indicators::init_money ()
{
  // gadget sprite
  Sint32 x = 24 * resolution;
  money_sprite->set_coordinates (x, money_posy);
  money_sprite->set_frame_delay (5);
  money_sprite->enable ();
  x += money_sprite->get_sprite_width ();

  // characters sprites
  sprite_object **liste = sprites_list;
  for (Sint32 i = 0; i < 6; i++)
    {
      sprite_object *x_bob = *(liste++);
      x_bob->set_coordinates (x, money_posy);
      x_bob->enable ();
      x += 8 * resolution;
    }
}

//-----------------------------------------------------------------------------
// display credit of money and "reverser malus"
//-----------------------------------------------------------------------------
void
controller_indicators::execution1 (Sint32 value)
{
  exec_money (value);

  //###################################################################
  // display "reverser malus" if enable
  //###################################################################
  sprite_object **liste = sprites_list + 6;
  Sint32 inves = ptZraquett->get_reverse_counter ();
  if (inves > 0)
    {
      inves--;
      ptZraquett->set_reverse_counter (inves);
      inves = inves / 2;
      Sint32 baseN = 10;
      while (baseN > 0)
        {
          Sint32 i = 0;
          while (inves >= baseN)
            {
              inves -= baseN;
              i++;
            }
          baseN /= 10;
          sprite_object *x_bob = *(liste++);
          x_bob->set_image (i);
          x_bob->enable ();
        }
      reverse_sprite->enable ();
      reverse_sprite->play_animation_loop ();
    }
  else
    {
      for (Sint32 i = 0; i < 2; i++)
        {
          sprite_object *x_bob = *(liste++);
          x_bob->disable ();
        }
      reverse_sprite->disable ();
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void
controller_indicators::execution2 (Sint32 value, Sint32 lifes)
{
  exec_money (value);
  Sint32 baseN = 10;
  sprite_object **liste = sprites_list + 6;
  while (baseN > 0)
    {
      Sint32 i = 0;
      while (lifes >= baseN)
        {
          lifes -= baseN;
          i++;
        }
      baseN /= 10;
      sprite_object *x_bob = *(liste++);
      x_bob->set_image (i);
    }
  life_sprite->play_animation_loop ();
}

//-----------------------------------------------------------------------------
// display credit of money
//-----------------------------------------------------------------------------
void
controller_indicators::exec_money (Sint32 value)
{
  Sint32 baseN = 100000;
  sprite_object **liste = sprites_list;
  while (baseN > 0)
    {
      Sint32 i = 0;
      while (value >= baseN)
        {
          value -= baseN;
          i++;
        }
      baseN /= 10;
      sprite_object *x_bob = *(liste++);
      x_bob->set_y_coord (money_posy);
      x_bob->set_image (i);
    }
  money_sprite->set_y_coord (money_posy);
  money_sprite->play_animation_loop ();
  if (money_posy < y_position)
    money_posy += resolution;
}

/**
 * Increase the amount of money
 * @param value money amount, 10, 20, 30, ...
 */
void
controller_indicators::increase_money_amount (Uint32 value)
{
  money_posy = y_position - 5 * resolution;
  current_player->amount_of_money += value;
  if (current_player->area_number >= 3)
    {
      current_player->amount_of_money += value;
    }
  if (current_player->area_number >= 5)
    {
      current_player->amount_of_money += value;
    }
}
