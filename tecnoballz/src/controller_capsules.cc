/** 
 * @file controller_capsules.cc 
 * @brief Capsules controller 
 * @date 2007-02-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.12 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_capsules.cc,v 1.12 2007/02/18 21:07:00 gurumeditation Exp $
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
  Sint16 *monPT;
  monPT = courseList;
  /* clear the list of bonus capsules bought in shop */
  for (Sint32 i = 0; i < NB_OPTIONS; i++)
    {
      *(monPT++) = 0;
    }
  *(monPT++) = -1;
  frame_delay = 0;
  frame_period = 5;
  frame_index = 0;
  malus_step = 0;
  malus_frek = 0;
  brick_kass = 0;
  bonusAchet = 0;
  bonusTombe = 0;
  bonus_step = 0;
  course_ptr = 0;
  max_of_sprites = 0;
  sprites_have_shades = false;
  /* additional table to drawing pixel by pixel is required
   * for color cycling of the chance capsule */
  is_draw_pixel_by_pixel = true;
  sprite_type_id = BOB_GADGET;
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
 * @param bKauf number of capsules bought in shop
 * @param brCnt number of bricks to be destroyed
 * @param table pointer to the list of capsules
 * @param cours pointer to list of capsules bought in shop
 */
void
controller_capsules::initialize (Sint32 mStep, Sint32 bKauf, Sint32 brCnt,
                                 const Sint16 * table, Sint32 * cours,
                                 zeMiniMess * ptMes,
                                 controller_balls * pBall,
                                 sprite_object * pWall)
{
  ptNewBalls = pBall;
  ptBob_wall = pWall;
  controller_paddles* paddles = controller_paddles::get_instance ();
  paddle_selected = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  malus_frek = mStep;
  bonusAchet = bKauf;
  bonus_step = 0;
  malusTable = table;
  malus_step = 0;               //"malus" drop frequency 
  brick_kass = 0;               //number of bricks current destroyed
  bonusTombe = 0;               //number of bonuses dropped 
  ptMiniMess = ptMes;
  sprite_capsule *bonus = sprites_list[0];
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      bonus = sprites_list[i];
      bonus->init_members ();
    }
  frame_period = bonus->frame_period;

  /* clear the list of bonuses bought in the shop */
  for (Uint32 i = 0; i < NB_OPTIONS; i++)
    {
      courseList[i] = 0;
    }
  /* mark the end of the list */
  courseList[NB_OPTIONS] = -1;

  //###################################################################
  // copy list of bonuses bought in the shop
  //###################################################################
  if (cours)
    {
      for (Sint32 i = 0; i < bonusAchet; i++)
        courseList[i] = cours[i];
    }
  else
    courseList[0] = -1;

  if (bonusAchet > 0)           //at least a bought bonus?
    {
      Sint32 v = brCnt - (brCnt / 2);
      bonus_step = v / bonusAchet;
    }
  else
    {
      //no bonus bought in the shop. Initialize with the maximum value.
      bonus_step = brCnt + 1;
    }
}

//-------------------------------------------------------------------------------
// bricks level: test if send a gadget (malus or bonus)
//-------------------------------------------------------------------------------
void
controller_capsules::envoieGads (brickClear * briPT)
{
  brick_kass++;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *gadg = sprites_list[i];
      if (!gadg->is_enabled)
        {                       //###########################################################
          // handle maluses
          //###########################################################
          malus_step++;
          if (malus_step > malus_frek)
            {
              Sint16 j = random_counter & 0x3F;     //value 0 to 63 
              j = *(malusTable + j);
              malus_step = 0;
              //j = GAD_MEGA00;       //test only
              gadg->nouveauGad (briPT, j);
            }
          else
            //###########################################################
            // handle bonuses
            //###########################################################
            {
              if (bonusAchet > bonusTombe)
                {
                  if (brick_kass > bonus_step)
                    {
                      brick_kass = 0;
                      bonusTombe++;
                      Sint32 j = courseList[course_ptr];
                      if (!j)
                        {
                          j = courseList[0];
                          course_ptr = 0;
                        }
                      course_ptr++;
                      gadg->nouveauGad (briPT, j);
                    }
                }
            }
          return;
        }
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
          j = *(malusTable + j);
          gadg->nouveauGad (pball, j);
          return;
        }
    }
}

//-------------------------------------------------------------------------------
// bricks levels: send a malus (from BouiBoui)
//-------------------------------------------------------------------------------
void
controller_capsules::send_malus (sprite_projectile * pfire)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *gadg = sprites_list[i];
      if (!gadg->is_enabled)
        {
          Sint16 j = random_counter & 0x1F; //value 0 to 31 
          j = *(malusTable + j);
          gadg->nouveauGad (pfire, j);
          return;
        }
    }
}


//-------------------------------------------------------------------------------
// guards level : test if send a gadget (malus or bonus)
//-------------------------------------------------------------------------------
void
controller_capsules::envoieGads (sprite_ball * pball)
{
  malus_step++;
  if (malus_step <= malus_frek)
    return;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *gadg = sprites_list[i];
      if (!gadg->is_enabled)
        {
          Sint16 j = random_counter & 0x1F; //value 0 to 31 
          j = *(malusTable + j);
          malus_step = 0;
          gadg->new_gadget (pball, j);
          return;
        }
    }
}

//-------------------------------------------------------------------------------
// shop : initialize the positions of bonuses
//-------------------------------------------------------------------------------
void
controller_capsules::create_shop_sprites_list ()
{
  //set_max_of_sprites (NB_OPTIONS + 2);
  create_sprites_list (NB_OPTIONS + 2 ,0);
  Sint32 t = NB_OPTIONS;
  sprite_capsule **liste = sprites_list;
  sprite_capsule *bonus = *liste;
  Sint32 h = bonus->sprite_height + 1;
  Sint32 x = SGADGET_X1 * resolution;
  Sint32 y = SGADGET_Y1 * resolution;
  for (Sint32 i = 0; i < t; i++)
    {
      bonus = *(liste++);
      bonus->set_coordinates (x, y);
      bonus->disable ();
      y += h;
    }

  //initialize bonus sprite object for the drag and drop 
  bonus = *(liste++);
  bonus->set_coordinates (0, 0);
  bonus->disable ();

  //initialize bonus sprite object that display indicator   
  bonus = *liste;
  bonus->set_coordinates (SGADGET_X2 * resolution, SGADGET_Y2 * resolution);
  bonus->disable ();
  temoin_gad = bonus;
}

//-------------------------------------------------------------------------------
// shop : change bonus indicator
//-------------------------------------------------------------------------------
void
controller_capsules::gadgetShop (Sint32 nuGad)
{
  temoin_gad->nouveauGad (nuGad);
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
          Sint32 g = capsule->get_gadget ();
          gadget_run (paddle, g);
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
          Sint32 g = capsule->get_gadget ();
          gadgetrun2 (paddle, g);
        }
    }
}

//-------------------------------------------------------------------------------
// shop: animation of bonus
//-------------------------------------------------------------------------------
void
controller_capsules::animations (Sint32 value)
{
  if ((--frame_delay) <= 0)
    {
      frame_delay = frame_period / value;
      if (++frame_index >= XXX_IMAGES)
        frame_index = 0;
    }
  Sint32 a = frame_index;
  sprite_capsule **liste = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *bonus = *(liste++);
      Sint32 o = bonus->frame_index_min + a;
      bonus->set_image (o);
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
      ptMiniMess->mesrequest (16);
      paddle->set_glue ();
      break;

      /* next level */
    case GAD_NEXTLV:
      {
        ptMiniMess->mesrequest (17);
        right_panel_score* panel = right_panel_score::get_instance ();
        panel->set_bricks_counter (0);
      }
      break;

      // fire power 1
    case GAD_FIRE01:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->mesrequest (4);
      paddle->set_fire_1 ();
      break;

      // fire power 2
    case GAD_FIRE02:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->mesrequest (5);
      paddle->set_fire_2 ();
      break;

      // shrink bumper
    case GAD_SIZE_M:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->mesrequest (18);
      paddles->shrink_paddles ();
      break;

      // expand bumper
    case GAD_SIZE_P:
#ifndef SOUNDISOFF
      audio->play_sound (S_TRANSFOR);
#endif
      ptMiniMess->mesrequest (19);
      paddles->expand_paddles ();
      break;

      // lose a life
    case GAD_LIFE_M:
#ifndef SOUNDISOFF
      audio->play_sound (S_ENLEVVIE);
#endif
      ptMiniMess->mesrequest (10);
      current_player->remove_life (1);
      break;

      // extra life
    case GAD_LIFE_P:
#ifndef SOUNDISOFF
      audio->play_sound (S_AJOUTVIE);
#endif
      ptMiniMess->mesrequest (11);
      current_player->add_life (1);
      break;

      // extra balls
    case GAD_BALLE2:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptMiniMess->mesrequest (20);
      oBall->run_2balls ();
      break;

      // multi balls
    case GAD_BALLE3:
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptMiniMess->mesrequest (21);
      oBall->run_3balls ();
      break;

      // power ball 1
    case GAD_POWER1:
      ptMiniMess->mesrequest (22);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_power_1 ();
      break;

      // power ball 2
    case GAD_POWER2:
      ptMiniMess->mesrequest (23);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_power_2 ();
      break;

      // inverse control
    case GAD_INVERS:
      ptMiniMess->mesrequest (24);
      paddles->set_reverse_counter (50 * 4);
      break;

      // maxi ball speed (no gadget)
    case GAD_SPEEDM:
      ptMiniMess->mesrequest (25);
      oBall->maxi_speed ();
      break;

      // bottom bumper[1] enable (no gadget)
    case GAD_BUMP01:
      paddle_selected = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
      break;

      // right bumper[2] enable (no gadget)
    case GAD_BUMP02:
      ptMiniMess->mesrequest (26);
      paddle_selected = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle_selected->enable ();
      break;

      // top bumper[3] enable (no gadget)
    case GAD_BUMP03:
      ptMiniMess->mesrequest (27);
      paddle_selected = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle_selected->enable ();
      break;

      // right bumper[4] enable (no gadget)
    case GAD_BUMP04:
      ptMiniMess->mesrequest (28);
      paddle_selected = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle_selected->enable ();
      break;

      // ball size 2
    case GAD_SIZE01:
      ptMiniMess->mesrequest (29);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      oBall->set_size_2 ();
      break;

      // ball size 3
    case GAD_SIZE02:
      ptMiniMess->mesrequest (30);
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
      ptMiniMess->mesrequest (31);
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
      ptMiniMess->mesrequest (15);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      current_player->set_Bprice (1);
      break;

      // bottom wall enable
    case GAD_WALL01:
      ptMiniMess->mesrequest (32);
#ifndef SOUNDISOFF
      audio->play_sound (S_GADGETGO);
#endif
      ptBob_wall->enable ();
      ptBob_wall->thecounter += 500;
      paddles->deactrobot ();
      break;

      // robot bumper enable
    case GAD_ROBOT1:
      ptMiniMess->mesrequest (33);
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
      ptMiniMess->mesrequest (34);
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
