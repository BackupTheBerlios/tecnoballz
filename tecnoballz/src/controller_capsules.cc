/** 
 * @file controller_capsules.cc 
 * @brief Capsules controller 
 * @date 2007-02-25
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.15 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_capsules.cc,v 1.15 2007/02/26 09:01:04 gurumeditation Exp $
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
#include "../include/controller_capsules.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_players.h"
#include "../include/sprite_projectile.h"
#include "../include/handler_players.h"

/**
 * Create the capsules controller
 */
controller_capsules::controller_capsules ()
{
  littleInit ();
  frame_delay = 0;
  frame_period = 5;
  frame_index = 0;
  malus_step = 0;
  malus_frek = 0;
  bricks_breaked_count = 0;
  num_of_caspules_bought = 0;
  capsules_released_count = 0;
  bonus_step = 0;
  shopping_cart_index = 0;
  max_of_sprites = 0;
  sprites_have_shades = false;
  /* additional table to drawing pixel by pixel is required
   * for color cycling of the chance capsule */
  is_draw_pixel_by_pixel = true;
  sprite_type_id = BOB_GADGET;
  shopping_cart = NULL;
}

/**
 *  Release capsules controller
 */
controller_capsules::~controller_capsules ()
{
  release_sprites_list ();
}

/**
 * Perform some initializations
 * @param mStep time interval between two capsules 
 * @param table pointer to the list of capsules
 */
void
controller_capsules::initialize (Sint32 mStep,
                                 const Sint16 * table,
                                 short_info_messages * ptMes,
                                 controller_balls * pBall,
                                 sprite_object * pWall)
{
  ptNewBalls = pBall;
  ptBob_wall = pWall;
  controller_paddles* paddles = controller_paddles::get_instance ();
  paddle_selected = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  malus_frek = mStep;
  bonus_step = 0;
  random_list = table;
  malus_step = 0;               //"malus" drop frequency 
  bricks_breaked_count = 0;               //number of bricks current destroyed
  capsules_released_count = 0;               //number of bonuses dropped 
  ptMiniMess = ptMes;
  sprite_capsule *capsule = sprites_list[0];
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      capsule = sprites_list[i];
      capsule->init_members ();
    }
  frame_period = capsule->frame_period;

  if (super_jump == BRICKS_LEVEL)
    {
      controller_bricks *bricks = controller_bricks::get_instance ();
      Uint32 numof_bricks = bricks->get_num_of_bricks ();
      shopping_cart = current_player->get_shopping_cart ();
      num_of_caspules_bought = current_player->get_cou_nb ();
      /* at least a bought bonus? */
      if (num_of_caspules_bought > 0)
	{
	  bonus_step = (numof_bricks - numof_bricks / 2) / num_of_caspules_bought;
	}
      else
	{
          /* no bonus bought in the shop,
	   * initialize with the maximum value */
	  bonus_step  = numof_bricks + 1;
	}
    }
}

/**
 * Check if send a bonus or penalty capsule
 * @param briPT
 */
void
controller_capsules::send_capsule_from_bricks (brickClear * briPT)
{
  bricks_breaked_count++;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *caspule = sprites_list[i];
      if (caspule->is_enabled)
        { 
	  continue;
	}

	  /*
	   * Send a random penalty or bonus capsule
	   */
          malus_step++;
          if (malus_step > malus_frek)
            {
              malus_step = 0;
              caspule->enable_capsule (briPT, random_list[random_counter & 0x3F]);
            }
	  /*
	   * Send a bonus capsule bought in th shop
	   */
          else
            {
              if (num_of_caspules_bought > capsules_released_count)
                {
                  if (bricks_breaked_count > bonus_step)
                    {
                      bricks_breaked_count = 0;
                      capsules_released_count++;
                      Sint32 j = shopping_cart[shopping_cart_index];
                      if (!j)
                        {
                          j = shopping_cart[0];
                          shopping_cart_index = 0;
                        }
                      shopping_cart_index++;
                      caspule->enable_capsule (briPT, j);
                    }
                }
            }
          return;
        }
}

//-------------------------------------------------------------------------------
// bricks levels: send a malus (from BouiBoui)
//-------------------------------------------------------------------------------
void
controller_capsules::send_malus (sprite_ball * pball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *gadg = sprites_list[i];
      if (!gadg->is_enabled)
        {
          Sint16 j = random_counter & 0x1F; //value 0 to 31 
          j = *(random_list + j);
          gadg->enable_capsule (pball, j);
          return;
        }
    }
}

//-------------------------------------------------------------------------------
// bricks levels: send a malus (from BouiBoui)
//-------------------------------------------------------------------------------
void
controller_capsules::send_malus (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      if (capsule->is_enabled)
        {
	  continue;
        }
          Sint16 j = random_counter & 0x1F; //value 0 to 31 
          j = *(random_list + j);
          capsule->enable_capsule (blast, j);
          return;
    }
}


//-------------------------------------------------------------------------------
// guards level : test if send a gadget (malus or bonus)
//-------------------------------------------------------------------------------
void
controller_capsules::check_if_send_capsule (sprite_ball * ball)
{
  malus_step++;
  if (malus_step <= malus_frek)
    {
      return;
    }
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      if (capsule->is_enabled)
        {
	  continue;
        }
          Sint16 j = random_counter & 0x1F; //value 0 to 31 
          j = *(random_list + j);
          malus_step = 0;
          capsule->enable_guardian_capsule (ball, j);
          return;
    }
}

/**
 * Initialize the coordinattes of the bonus capsules in the shop 
 */
void
controller_capsules::create_shop_sprites_list ()
{
  create_sprites_list (supervisor_shop::MAX_OF_CAPSULES_BOUGHT + 2 ,false);
  Uint32 maxof = supervisor_shop::MAX_OF_CAPSULES_BOUGHT;
  sprite_capsule **caspules = sprites_list;
  sprite_capsule *caspule = caspules[0];
  Uint32 h = caspule->sprite_height + 1;
  Sint32 x = SGADGET_X1 * resolution;
  Sint32 y = SGADGET_Y1 * resolution;
  for (Uint32 i = 0; i < maxof; i++)
    {
      caspule = *(caspules++);
      caspule->set_coordinates (x, y);
      caspule->disable ();
      y += h;
    }

  /* initialize caspule sprite object for the drag and drop */ 
  caspule = *(caspules++);
  caspule->set_coordinates (0, 0);
  caspule->disable ();

  /* initialize caspule sprite object that display indicator */ 
  caspule = *caspules;
  caspule->set_coordinates (SGADGET_X2 * resolution, SGADGET_Y2 * resolution);
  caspule->disable ();
  overview_capsule = caspule;
}

/**
 * Set the overview capsule in the shop
 * @id id capsule identifier
 */
void
controller_capsules::set_overview_capsule (Uint32 id)
{
  overview_capsule->set_in_shop (id);
}

/**
 *  Move and collision with paddles in bricks levels
 */
void
controller_capsules::move_in_bricks_levels ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      capsule->play_animation_loop ();
      sprite_paddle *paddle = capsule->move ();
      if (NULL != paddle)
        {
          Uint32 id = capsule->get_id ();
          gadget_run (paddle, id);
        }
    }
}

/**
 * Move and collision with paddle in guardians levels
 */
void
controller_capsules::move_in_guardians_levels ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      capsule->play_animation_loop ();
      sprite_paddle *paddle = capsule->move ();
      if (NULL != paddle)
        {
          Uint32 id = capsule->get_id ();
          gadgetrun2 (paddle, id);
        }
    }
}

/**
 * Play capsules animation in shop
 * @param speed speed of the animation 1 or 2
 */
void
controller_capsules::play_animation_in_shop (Uint32 speed)
{
  if (--frame_delay <= 0)
    {
      frame_delay = frame_period / speed;
      if (++frame_index >= XXX_IMAGES)
	{
          frame_index = 0;
	}
    }
  Uint32 offset = frame_index;
  sprite_capsule **caspules = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *caspule = *(caspules++);
      caspule->set_image (caspule->frame_index_min + offset);
    }
}

/**
 * Enable a bonus or a penalty with the keyboard
 */
void
controller_capsules::cheat_keys ()
{
  if (!cheat_flag)
    {
      return;
    }
  if (keyboard->key_is_pressed (SDLK_LSHIFT) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
#ifndef UNDER_DEVELOPMENT
      !keyboard->key_is_pressed (SDLK_RSHIFT) ||
      !keyboard->key_is_pressed (SDLK_RCTRL) ||
#else
      keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_RCTRL) ||
#endif
      keyboard->key_is_pressed (SDLK_LALT))
    {
      return;
    }
  Sint16 *liste = keysTriche;
  while (Sint16 k = *(liste++)) //k = SDL key code
    {
      Sint16 g = *(liste++);    //g = gadget code (bonus or malus)
      if (keyboard->key_is_pressed (k)) //key pressed ?
        *(liste++) = 1;         //yes, set key state pressed
      else
        {
          if (*liste)
            gadget_run (paddle_selected, g);
          *(liste++) = 0;
        }
    }
}

//-------------------------------------------------------------------------------
// bricks levels: active a gadget (bonus or malus)
//-------------------------------------------------------------------------------
void
controller_capsules::gadget_run (sprite_paddle * paddle, Sint32 nuGad)
{
  paddle_selected = paddle;
  controller_paddles* paddles = controller_paddles::get_instance ();
  controller_balls *oBall = ptNewBalls;

  if (nuGad == GAD_RANDOM)
    {
      nuGad = randomlist[random_counter & 127];
    }

  switch (nuGad)
    {

      /* glue paddle */
    case GAD_GLUE00:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->send_message_request (16);
      paddle->set_glue ();
      break;

      /* next level */
    case GAD_NEXTLV:
      {
        ptMiniMess->send_message_request (17);
        right_panel_score* panel = right_panel_score::get_instance ();
        panel->set_bricks_counter (0);
      }
      break;

      // fire power 1
    case GAD_FIRE01:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->send_message_request (4);
      paddle->set_fire_1 ();
      break;

      // fire power 2
    case GAD_FIRE02:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->send_message_request (5);
      paddle->set_fire_2 ();
      break;

      // shrink bumper
    case GAD_SIZE_M:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->send_message_request (18);
      paddles->shrink_paddles ();
      break;

      // expand bumper
    case GAD_SIZE_P:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->send_message_request (19);
      paddles->expand_paddles ();
      break;

      // lose a life
    case GAD_LIFE_M:
#ifndef SOUNDISOFF
      audio->play_sound (S_ENLEVVIE);
#endif
      ptMiniMess->send_message_request (10);
      current_player->remove_life (1);
      break;

      // extra life
    case GAD_LIFE_P:
#ifndef SOUNDISOFF
      audio->play_sound (S_AJOUTVIE);
#endif
      ptMiniMess->send_message_request (11);
      current_player->add_life (1);
      break;

      // extra balls
    case GAD_BALLE2:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptMiniMess->send_message_request (20);
      oBall->run_2balls ();
      break;

      // multi balls
    case GAD_BALLE3:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptMiniMess->send_message_request (21);
      oBall->run_3balls ();
      break;

      // power ball 1
    case GAD_POWER1:
      ptMiniMess->send_message_request (22);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_power_1 ();
      break;

      // power ball 2
    case GAD_POWER2:
      ptMiniMess->send_message_request (23);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_power_2 ();
      break;

      // inverse control
    case GAD_INVERS:
      ptMiniMess->send_message_request (24);
      paddles->set_reverse_counter (50 * 4);
      break;

      // maxi ball speed (no gadget)
    case GAD_SPEEDM:
      ptMiniMess->send_message_request (25);
      oBall->maxi_speed ();
      break;

      // bottom bumper[1] enable (no gadget)
    case GAD_BUMP01:
      paddle_selected = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
      break;

      // right bumper[2] enable (no gadget)
    case GAD_BUMP02:
      ptMiniMess->send_message_request (26);
      paddle_selected = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle_selected->enable ();
      break;

      // top bumper[3] enable (no gadget)
    case GAD_BUMP03:
      ptMiniMess->send_message_request (27);
      paddle_selected = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle_selected->enable ();
      break;

      // right bumper[4] enable (no gadget)
    case GAD_BUMP04:
      ptMiniMess->send_message_request (28);
      paddle_selected = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle_selected->enable ();
      break;

      // ball size 2
    case GAD_SIZE01:
      ptMiniMess->send_message_request (29);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_size_2 ();
      break;

      // ball size 3
    case GAD_SIZE02:
      ptMiniMess->send_message_request (30);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_size_3 ();
      break;

      // random
    case GAD_RANDOM:
      break;

      // all options
    case GAD_MEGA00:
      ptMiniMess->send_message_request (31);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      paddles->maxi_bumps ();
      oBall->run_nballs ();
      oBall->set_power_2 ();
      oBall->set_size_3 ();

      paddle->set_glue ();
      paddle->set_fire_2 ();

      paddle_selected = paddles->get_paddle (2);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();

      paddle_selected = paddles->get_paddle (3);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();

      paddle_selected = paddles->get_paddle (4);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();
      break;

      // Bonus price (shop's price at 1 in the shop) (no gadget)
    case GAD_PRICE1:
      ptMiniMess->send_message_request (15);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      current_player->set_Bprice (1);
      break;

      // bottom wall enable
    case GAD_WALL01:
      ptMiniMess->send_message_request (32);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptBob_wall->enable ();
      ptBob_wall->thecounter += 500;
      paddles->deactrobot ();
      break;

      // robot bumper enable
    case GAD_ROBOT1:
      ptMiniMess->send_message_request (33);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      paddles->activrobot ();
      ptBob_wall->disable ();
      ptBob_wall->thecounter = 0;
      break;

      // balls control 
    case GAD_CONTRO:
      oBall->enable_balls_control ();
      ptMiniMess->send_message_request (34);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      break;

      // enable eye (no gadget)
    case GAD_OEIL00:
      {
        controller_magnetic_eyes* eyes = controller_magnetic_eyes::get_instance ();
        eyes->create_eye ();
      }
      break;
    }
}

//-------------------------------------------------------------------------------
// guards levels: active a gadget (bonus)
//-------------------------------------------------------------------------------
void
controller_capsules::gadgetrun2 (sprite_paddle * raket, Sint32 nuGad)
{
  paddle_selected = raket;
  controller_balls *oBall = ptNewBalls;
  switch (nuGad)
    {

      // bumper protect
    case GAD_PROTEC:
      raket->set_invincibility (200);
      break;

      // extra life
    case GAD_LIFE_P:
#ifndef SOUNDISOFF
      audio->play_sound (S_AJOUTVIE);
#endif
      current_player->add_life (1);
      break;

      // multi balls
    case GAD_BALLE3:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->run_3balls ();
      break;

      // power ball 1 (ball size 2)
    case GAD_POWER1:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_size_2 ();
      break;

      // power ball 2 (ball size 3)
    case GAD_POWER2:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_size_3 ();
      break;
    }
}

//-------------------------------------------------------------------------------
// bricks levels: gadgets list of the random gadget
//-------------------------------------------------------------------------------
const Uint16
  controller_capsules::randomlist[128] =
  { GAD_WALL01, GAD_OEIL00, GAD_FIRE02, GAD_SIZE_M, GAD_ROBOT1, GAD_BALLE2,
GAD_BALLE3,
  GAD_POWER1, GAD_CONTRO, GAD_SIZE01, GAD_SIZE02, GAD_OEIL00, GAD_BUMP04,
    GAD_SPEEDM,
  GAD_SPEEDM, GAD_SPEEDM, GAD_LIFE_M, GAD_CONTRO, GAD_GLUE00, GAD_FIRE01,
    GAD_FIRE02,
  GAD_SIZE_M, GAD_SIZE_P, GAD_BALLE2, GAD_BALLE3, GAD_POWER1, GAD_SPEEDM,
    GAD_SIZE01,
  GAD_SIZE02, GAD_SIZE02, GAD_BUMP04, GAD_OEIL00, GAD_NEXTLV, GAD_CONTRO,
    GAD_LIFE_M,
  GAD_SPEEDM, GAD_ROBOT1, GAD_FIRE01, GAD_FIRE02, GAD_SIZE_M, GAD_WALL01,
    GAD_BALLE2,
  GAD_ROBOT1, GAD_ROBOT1, GAD_ROBOT1, GAD_SIZE01, GAD_SIZE02, GAD_SPEEDM,
    GAD_BUMP02,
  GAD_BUMP02, GAD_BUMP03, GAD_INVERS, GAD_LIFE_P, GAD_LIFE_P, GAD_WALL01,
    GAD_OEIL00,
  GAD_PRICE1, GAD_SIZE_M, GAD_ROBOT1, GAD_BALLE2, GAD_BALLE3, GAD_POWER1,
    GAD_SPEEDM,
  GAD_SIZE01, GAD_WALL01, GAD_FIRE01, GAD_ROBOT1, GAD_OEIL00, GAD_ROBOT1,
    GAD_BALLE2,
  GAD_BALLE3, GAD_POWER1, GAD_WALL01, GAD_SIZE01, GAD_SIZE02, GAD_BUMP03,
    GAD_BUMP04,
  GAD_SPEEDM, GAD_SPEEDM, GAD_OEIL00, GAD_LIFE_M, GAD_INVERS, GAD_GLUE00,
    GAD_FIRE01,
  GAD_FIRE02, GAD_SIZE_M, GAD_SIZE_P, GAD_BALLE2, GAD_BALLE3, GAD_POWER1,
    GAD_SPEEDM,
  GAD_SIZE01, GAD_SIZE02, GAD_SIZE02, GAD_BUMP04, GAD_WALL01, GAD_NEXTLV,
    GAD_INVERS,
  GAD_LIFE_M, GAD_WALL01, GAD_ROBOT1, GAD_FIRE01, GAD_FIRE02, GAD_SIZE_M,
    GAD_SIZE_P,
  GAD_BALLE2, GAD_BALLE3, GAD_POWER1, GAD_SPEEDM, GAD_SIZE01, GAD_WALL01,
    GAD_WALL01,
  GAD_BUMP02, GAD_BUMP02, GAD_BUMP03, GAD_INVERS, GAD_WALL01, GAD_WALL01,
    GAD_GLUE00,
  GAD_FIRE01, GAD_PRICE1, GAD_SIZE_M, GAD_SIZE_P, GAD_BALLE2, GAD_BALLE3,
    GAD_POWER1,
  GAD_WALL01, GAD_WALL01
};

//-------------------------------------------------------------------------------
// bricks levels: key enable options (for test only)
//-------------------------------------------------------------------------------
Sint16
  controller_capsules::keysTriche[] = { SDLK_F1, GAD_GLUE00, 0,
  SDLK_F2, GAD_NEXTLV, 0,
  SDLK_F3, GAD_FIRE01, 0,
  SDLK_F4, GAD_FIRE02, 0,
  SDLK_F5, GAD_SIZE_M, 0,
  SDLK_F6, GAD_SIZE_P, 0,
  SDLK_F7, GAD_LIFE_M, 0,
  SDLK_F8, GAD_LIFE_P, 0,
  SDLK_F9, GAD_BALLE2, 0,
  SDLK_F10, GAD_BALLE3, 0,
  SDLK_F11, GAD_POWER1, 0,
  SDLK_F12, GAD_POWER2, 0,
  SDLK_a, GAD_INVERS, 0,
  SDLK_z, GAD_SPEEDM, 0,
  SDLK_e, GAD_BUMP01, 0,
  SDLK_r, GAD_BUMP02, 0,
  SDLK_t, GAD_BUMP03, 0,
  SDLK_y, GAD_BUMP04, 0,
  SDLK_u, GAD_SIZE01, 0,
  SDLK_i, GAD_SIZE02, 0,
  SDLK_o, GAD_RANDOM, 0,
  SDLK_q, GAD_MEGA00, 0,
  SDLK_s, GAD_PRICE1, 0,
  SDLK_d, GAD_WALL01, 0,
  SDLK_g, GAD_CONTRO, 0,
  SDLK_h, GAD_OEIL00, 0,
  SDLK_j, GAD_OEIL00, 0,
  SDLK_k, GAD_ROBOT1, 0,
  0, 0, 0
};
