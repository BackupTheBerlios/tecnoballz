/** 
 * @file controller_viewfinders.cc 
 * @brief Paddles viewfinders controller 
 * @date 2007-02-12
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_viewfinders.cc,v 1.2 2007/02/12 16:28:19 gurumeditation Exp $
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
#include "../include/controller_viewfinders.h"
#include "../include/sprite_ball.h"

/*
 * Create the viewfinders controller
 */
controller_viewfinders::controller_viewfinders ()
{
  littleInit ();
  max_of_sprites = 4;
  sprites_have_shades = false;
  sprite_type_id = BOB_DIRECT;
  num_of_paddles = 0;
}

/*
 * Release the viewfinders controller
 */
controller_viewfinders::~controller_viewfinders ()
{
  if (NULL != paddles_list)
    {
      delete[]paddles_list;
      paddles_list = (sprite_paddle **) NULL;
    }
  release_sprites_list ();
}

/**
 * Perform some initializations
 * @param paddles pointer to the balls controller object
 * @param numof_paddles number of paddles managed
 */
void
controller_viewfinders::initialize (controller_paddles * paddles, Uint32 numof_paddles)
{
  if (numof_paddles == 0)
    {
      throw std::runtime_error ("(!)controller_viewfinders::initialize() "
          "failed! Number of paddles must be higher than 0!");
    }
  num_of_paddles = numof_paddles;

  try 
  {
    paddles_list = new sprite_paddle *[num_of_paddles];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)controller_viewfinders::initialize() "
      "not enough memory to allocate " <<
      num_of_paddles << " list elements!" << std::endl;
    throw;
  }
  for (Uint32 i = 0; i < numof_paddles; i++)
    {
      paddles_list[i] = paddles->get_paddle (i + 1);
    }
}

/**
 * Enable, move, anim and disable the wiewfinders
 */
void
controller_viewfinders::run ()
{
  for (Uint32 i = 0; i < num_of_paddles; i++)
    {
      sprite_ball *ball = paddles_list[i]->ball_glued;
      if (NULL != ball && ball->is_enabled)
        {
          sprites_list[i]->aspireBOB2 (ball);
          sprites_list[i]->enable ();
          sprites_list[i]->set_image (ball->directBall / 4);
        }
      else
        {
          sprites_list[i]->disable ();
        }
    }
}
