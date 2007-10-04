/** 
 * @file sprite_capsule.cc
 * @brief The capsule sprite which contains a bonus or a penalty 
 * @date 2007-09-13
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.15 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_capsule.cc,v 1.15 2007/10/04 05:54:41 gurumeditation Exp $
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
#include "../include/sprite_capsule.h"

/**
 * Create a caspsule sprite
 */
sprite_capsule::sprite_capsule ()
{
  clear_sprite_members ();
  towards = 0;
  capsule_identifier = 0;
  paddle = NULL;
  current_cycling = &sprite_object::cycling_01[0];
}

/**
 * Release a caspsule
 */
sprite_capsule::~sprite_capsule ()
{
}

/**
 * Perform some initializations
 */
void
sprite_capsule::init_members ()
{
  x_maximum = screen_width - ((64 + 16) * resolution);
  x_minimum = 3 * resolution;
  y_maximum = screen_height - 10 * resolution;
  y_minimum = 0 * resolution;
  /* speed of the animation */
  frame_period = 6;
  /* time delay before each image */
  frame_delay = 1;
  set_draw_method (sprite_object::DRAW_CAPSULE);
}

/**
 * Initialize a new indicator capsule (life or reserver)
 * @param brick a pointer to the brick which touched by a ball
 * @param id capsule identifier 
 */
void
sprite_capsule::enable_indicator_capsule (Uint32 id)
{
  set_new_capsule (id, &gagdetBrik[0], 0, 0, NULL);
}

/**
 * Initialize a new bonus or penalty capsule from a brick 
 * @param brick a pointer to the brick which touched by a ball
 * @param id capsule identifier 
 */
void
sprite_capsule::enable_capsule (brick_redraw * brick, Uint32 id)
{
  set_new_capsule (id, &gagdetBrik[0], brick->xcoord_collision, brick->ycoord_collision, brick->paddle);
}

/**
 * Initialize a new bonus or penalty capsule from a ball 
 * @param ball a pointer to the ball sprite which destroyed
 *        the enemy ship
 * @param id capsule identifier 
 */
void
sprite_capsule::enable_capsule (sprite_ball * ball, Uint32 id)
{
  set_new_capsule (id, &gagdetBrik[0], ball->get_x_coord (), ball->get_y_coord (), ball->get_last_paddle_touched ());
}

/**
 * Initialize a new bonus or penalty capsule from a projectile
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 * @param id capsule identifier 
 */

void
sprite_capsule::enable_capsule (sprite_projectile * blast, Uint32 id)
{
  set_new_capsule (id, &gagdetBrik[0], blast->get_x_coord (), blast->get_y_coord (), blast->paddle);
}

/**
 * Initialize a new bonus or penalty capsule from a ball 
 * @param ball a pointer to the ball sprite which touched the guardian
 * @param id capsule identifier 
 */
void
sprite_capsule::enable_guardian_capsule (sprite_ball * ball, Uint32 id)
{
  set_new_capsule (id, &gagdetGuar[0], ball->get_x_coord (), ball->get_y_coord (), ball->get_last_paddle_touched ());
}

/**
 * Set a new bonus or penalty capsule
 * @param id capsule identifier 
 * @param frames frames index list
 * @param xcoord x-coordinate of the capsule
 * @param ycoord y-coordinate of the capsule
 * @param pad a pointer to a sprite capsule
 */
void
sprite_capsule::set_new_capsule (Uint32 id, const Sint16 *frames, Uint32 xcoord, Uint32 ycoord, sprite_paddle *pad)
{
  if (ENABLE_HUGELY_OPTIONS == id || CHANCE == id)
    {
      set_draw_method (sprite_object::DRAW_CAPSULE);
    }
  else
    {
      set_draw_method (sprite_object::DRAW_WITH_TABLES);
    }
  capsule_identifier = id;
  x_coord = xcoord;
  y_coord = ycoord;
  paddle = pad;
  if (NULL != paddle)
    {
       towards = paddle->get_paddle_number ();
    }
  else
    {
      towards = 0;
    }
  id = id >> 1;
  random_counter += id;
  Sint32 index = frames[id]; 
  if (index < 0)
    {
      std::cerr << "sprite_capsule::set_new_capsule() " << "index " <<
	"is not used!" << std::endl;
      return;
    }
  enable_capsule (index);
}

/**
 * Set a new capsule in the shop
 * @param id capsule indentifier
 */
void
sprite_capsule::set_in_shop (Uint32 id)
{
  if (capsule_identifier == id)
    {
      return;
    }
  capsule_identifier = id;
  id = id >> 1;
  random_counter += id;
  Sint32 i = gagdetBrik[id];
  if (i == XXX_VIDE00)          // empty code, no gaget code ?
    {
      is_enabled = false;
    }
  else
    {
      enable_capsule (i);
    }
}

void sprite_capsule::enable_capsule (Uint32 index)
{
  is_enabled = true;
  frame_index_min = index;
  frame_index = index;
  index += XXX_IMAGES - 1;
  frame_index_max = index;
  set_image (frame_index);
}
 

/**
 * Clone this capsule from another
 * @param capsule capsule source
 */
void
sprite_capsule::clone_from_capsule (sprite_capsule * capsule)
{
  capsule_identifier = capsule->capsule_identifier;
  frame_index_min = capsule->frame_index_min;
  frame_index = capsule->frame_index;
  frame_index_max = capsule->frame_index_max;
  set_image (frame_index);
}

/**
 * Return the identifier of the capsule
 * @return identifier of the capsule
 */
Uint32 sprite_capsule::get_id ()
{
  return capsule_identifier;
}

/**
 * Displacement and collision of the capsule
 * @return the pointer on the paddle object which
 *         was touched by the capsule or NULL if no collision
 */
sprite_paddle *
sprite_capsule::move ()
{
  if (!is_enabled)
    {
      return NULL;
    }
  switch (towards)
    {
    case controller_paddles::BOTTOM_PADDLE:
      y_coord += resolution;
      if (y_coord < y_maximum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
              current_player->add_score (20);
              return paddle;
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    case controller_paddles::RIGHT_PADDLE:
      x_coord += resolution;
      if (x_coord < x_maximum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
              current_player->add_score (20);
              return paddle;
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    case controller_paddles::TOP_PADDLE:
      y_coord -= resolution;
      if (y_coord > y_minimum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
              current_player->add_score (20);
              return paddle;
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

      /* left paddle */
    case controller_paddles::LEFT_PADDLE:
      x_coord -= resolution;
      if (x_coord > x_minimum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
              current_player->add_score (20);
              return paddle;
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    }
  return NULL;
}

//-------------------------------------------------------------------------------
// bricks levels : pointers on the images of the animation of the gadget 
//-------------------------------------------------------------------------------
const Sint16
  sprite_capsule::gagdetBrik[] = { XXX_VIDE00,  // *unused*
  XXX_GLUE00,                   // glue (bricks level only)
  XXX_VIDE00,                   // *unused*
  XXX_FIRE01,                   // fire power 1 (bricks level only)
  XXX_FIRE02,                   // fire power 2 (bricks level only)
  XXX_SIZE_M,                   // shrink bumper (bricks level only)
  XXX_SIZE_P,                   // expand bumper (bricks level only)
  XXX_LIFE_M,                   // lose a life (bricks level only)
  XXX_LIFE_P,                   // extra life
  XXX_BALLE2,                   // extra balls
  XXX_BALLE3,                   // multi balls
  XXX_POWER1,                   // power ball 1
  XXX_POWER2,                   // extra life
  XXX_INVERS,                   // inverse control commands (bricks level only)
  XXX_VIDE00,                   // REBUILD WALL (ONLY SHOP)
  XXX_VIDE00,                   // maxi ball speed (no gadget)
  XXX_VIDE00,                   // bottom bumper[1] enable (no gadget)
  XXX_VIDE00,                   // right bumper[2] enable (no gadget)
  XXX_VIDE00,                   // top bumper[3] enable (no gadget)
  XXX_VIDE00,                   // right bumper[4] enable (no gadget)
  XXX_SIZE01,                   // ball size 2 
  XXX_SIZE02,                   // ball size 3
  XXX_RANDOM,                   // random (bricks level only)
  XXX_MEGA00,                   // all options (bricks level only)
  XXX_VIDE00,                   // Bonus price (shop's price at 1 in the shop) (no gadget)
  XXX_WALL01,                   // bottom wall enable
  XXX_ROBOT1,                   // robot bumper enable
  XXX_CONTRO,                   // balls control 
  XXX_VIDE00,                   // Oeil actif
  XXX_VIDE00,                   // LESS-BRICK (SHOP ONLY)
  XXX_VIDE00,                   // INFOS (SHOP ONLY)
  XXX_VIDE00                    // EXIT (SHOP ONLY)
};

//-------------------------------------------------------------------------------
// guards levels : pointers on the images of the animation of the gadget 
//-------------------------------------------------------------------------------
const
  Sint16
  sprite_capsule::gagdetGuar[] = { XXX_VIDE00,  // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_PROTEC,                   // bumper protect (guards levels only)
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_LIFE_P,                   // extra life
  XXX_VIDE00,                   // *unused*
  XXX_BALLE3,                   // multi balls
  XXX_SIZE01,                   // ball size 2 
  XXX_SIZE02,                   // ball size 3
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00,                   // *unused*
  XXX_VIDE00                    // *unused*
};
