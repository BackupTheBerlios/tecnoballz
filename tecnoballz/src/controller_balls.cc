/** 
 * @file controller_balls.cc 
 * @brief Control the balls. Move and collisions 
 * @date 2007-04-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.45 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_balls.cc,v 1.45 2007/09/18 13:39:11 gurumeditation Exp $
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
#include "../include/controller_balls.h"
#include "../include/controller_paddles.h"
#include "../include/handler_players.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/list_sprites.h"

/** 
 * Create the balls controller into bricks levels
 */
controller_balls::controller_balls (sprite_object * pwall)
{
  littleInit ();
  num_erreur = 0;
  ptBob_wall = pwall;
  startCount = 60;
  glue_delay = 60;
  tempoVites = 60;
  balle_tilt = 60;
  balleVites = sprite_ball::donneSpeed (1);
  max_of_sprites = 20;
  sprites_have_shades = true;
  balls_are_controlled = false;
  sprite_type_id = BOB_BALLES;
}

/** 
 * Create the balls controller into guards levels
 */
controller_balls::controller_balls ()
{
  littleInit ();
  num_erreur = 0;
  ptBob_wall = NULL;
  startCount = 60;
  glue_delay = 60;
  tempoVites = 60;
  balle_tilt = 60;
  balleVites = sprite_ball::donneSpeed (1);
  max_of_sprites = 20;
  sprites_have_shades = true;
  balls_are_controlled = false;
  sprite_type_id = BOB_BALLES;
}

/** 
 * Release the balls controller
 */
controller_balls::~controller_balls ()
{
  release_sprites_list ();
}

/**
 * Initialize the balls before one level
 * @param start time delay before the ball leaves the bumper (first time)
 * @param glueC time delay before the ball leaves (glue option)
 * @param speed time delay before the ball accelerates
 * @param tiltC time delay before "tilt" is available
 * @param table speed ball (1 to 4)
 */
void
controller_balls::init (Uint32 start,
                        Uint32 glueC, Uint32 speed, Uint32 tiltC,
                        Uint32 table)
{

  controller_paddles *paddles = controller_paddles::get_instance ();

  startCount = start;
  glue_delay = glueC;
  tempoVites = speed;
  balle_tilt = tiltC;
  balleVites = sprite_ball::donneSpeed (table);
  paddle_bottom = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);

  Sint32 w;
  if (super_jump == GUARDS_LEVEL)
    {
      /* the width of a brick in pixels */
      w = 16 * resolution;
    }
  else
    {
      paddle_right = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle_top = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle_left = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      tec_robot0 = paddles->get_paddle (controller_paddles::ROBOT_PADDLE);
      controller_bricks *bricks = controller_bricks::get_instance ();
      w = bricks->get_brick_width ();
    }
  
  /* initialize all balls */
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      ball->once_init (start, speed, paddle_bottom, balleVites, w);
    }

  /* first ball special initialization */
  sprite_ball *ball = sprites_list[0];
  paddle_bottom->ball_glued = ball;
  ball->startBalle (paddle_bottom->collision_width);
  /* one ball on the screen */
  num_of_sprites = 1;
  if (super_jump == BRICKS_LEVEL)
    {
      controller_ejectors *ejectors = controller_ejectors::get_instance ();
      ejectors->ballPosIni (&sprite_ball::furaxTable[0]);
    }
}

/**
 * Move the balls and check the collisions in the bricks levels
 */
void
controller_balls::run_in_bricks_levels ()
{
  check_outside_balls ();
  activate_tilt ();
  check_bricks_collision ();
  vitus_move ();
  check_collisions_with_paddles ();
  check_collisions_with_robot ();
  /* collisions between balls and the 3 walls */
  collision_with_walls ();
  /* collisions between balls and the 4 ejectors */
  handle_ejectors ();
  check_collisions_with_ships ();
  check_collisions_with_eyes ();
  /* control balls with the left mouse button */
  controll_balls ();
  time_2tilt ();
  accelerate ();
}

/**
 * Move the balls and check the collisions in the guardians levels
 */
void
controller_balls::run_in_guardians_level ()
{
  vitussort2 ();                //test if balls go out of the screen
  activate_tilt ();
  vitusmove2 ();                //move the balls
  vituscote2 ();                //collisions balls and walls
  vitusbound ();                //prevent that the ball remains blocked horizontally
  vitusbump2 ();                //collisions balls and the bumper
  vitusGuard ();
  time2tilt2 ();
  accelerate ();
}

/** 
 * Check if balls go out of the screen of game
 */
void
controller_balls::check_outside_balls ()
{
  Sint32 min_x = sprite_ball::MINIMUM_PX * resolution;
  Sint32 max_x = sprite_ball::MAXIMUM_PX * resolution;
  Sint32 min_y = sprite_ball::MINIMUM_PY * resolution;
  Sint32 max_y = sprite_ball::MAXIMUM_PY * resolution;
  controller_ships *ships = controller_ships::get_instance ();
  right_panel_score *panel = right_panel_score::get_instance ();
  head_animation *head_anim = head_animation::get_instance ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      sprite_paddle *paddle = NULL;
      Sint32 j = ball->x_coord;
      if (j < min_x)
        {
          paddle = paddle_left;
        }
      else
        {
          if (j > max_x)
            paddle = paddle_right;
          else
            {
              j = ball->y_coord;
              if (j < min_y)
                paddle = paddle_top;
              else if (j > max_y)
                paddle = paddle_bottom;
            }
        }

      /* ball is out of screen? */
      if (NULL == paddle)
        {
          /* no, continue */
          continue;
        }

      /* paddle id enabled ? */
      if (!paddle->is_enabled)
        {
          /* no, sey he bottom paddle by default */
          paddle = paddle_bottom;
        }

      /*
       * there is still at least one ball
       */
      if (--num_of_sprites > 0)
        {
          ball->remove (paddle_bottom);
          continue;
        }

      /*
       * the player loses his last ball and a life
       */
      /* one starts again with only one ball  */
      num_of_sprites = 1;
      ball->paddle_touched->attachBall (ball);
      ball->reStarting (ball->paddle_touched);
      head_anim->start_interference ();
      current_player->remove_life (1);
      ships->force_explosion ();
#ifndef SOUNDISOFF
      audio->play_lost_music ();
      audio->play_sound (S_ENLEVVIE);
#endif
      short_info_messages* messages = short_info_messages::get_instance ();
      messages->send_message_request (short_info_messages::LOST_FILE);
      messages->send_message_request (short_info_messages::ARE_YOU_READY);
      panel->reset_gigablitz_countdown ();
      controller_bricks *bricks = controller_bricks::get_instance ();
      bricks->start_cycling ();
    }
}

/**
 * Check if balls go out of the screen of game
 */
void
controller_balls::vitussort2 ()
{
  Sint32 max_y = sprite_ball::MAXIMUM_PY * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      if (ball->y_coord <= max_y)
        {
          continue;
        }
      if (--num_of_sprites > 0)
        {
          ball->remove (paddle_bottom);
          continue;
        }

      /*
       * the player loses a life 
       */
      /* one starts again with only one ball */
      num_of_sprites = 1;
      paddle_bottom->attachBall (ball);
      ball->reStarting (paddle_bottom);
      ball->set_power_2 ();
      current_player->remove_life (1);
#ifndef SOUNDISOFF
      audio->play_sound (S_ENLEVVIE);
#endif
    }
}

/**
 * Activate the tilt if 2 buttons are pressed simultaneously
 */
void
controller_balls::activate_tilt ()
{
  if (!keyboard->is_right_left_buttons ())
    {
      return;
    }
  bool ftilt = false;
  Sint32 t = balle_tilt;
  Sint32 rand = random_counter;
  rand = rand & 15;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled || ball->tilt_delay < t)
        {
          continue;
        }
      if (ball->directBall < 64)
        {
          Sint32 d = (ball->directBall >> 2) & 0xf;
          ball->directBall = sprite_ball::tilt_table[d][rand];
        }
      ball->tilt_delay = 0;
      if (!ftilt)
        {
          ftilt = true;
#ifndef SOUNDISOFF
          audio->play_sound (S_TECNOBAL);
#endif
        }
    }
  if (ftilt)
    {
      display->tilt_screen ();
    }
}

/**
 * handle the acceleration of the balls 
 */
void
controller_balls::accelerate ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = sprites_list[i];
      if (balle->is_enabled)
        balle->accelerate ();
    }
}

/**
 * Displacement of the balls sticked to the paddle in bricks levels
 */
void
controller_balls::vitus_move ()
{
  Sint32 j;
  Sint32 *monPT;
  sprite_paddle *paddle;
  for (Uint32 i = 0; i < max_of_sprites; i++)
  {
    sprite_ball *ball = sprites_list[i];
    if (!ball->is_enabled)
    {
      continue;
    }

    /*  the ball is not sticked */
    if (ball->sticky_paddle_num == 0)
    {
      /* the balle moves */
      j = ball->directBall;
      if (j > 64)
      {
        fprintf (stderr,
            "controller_balls::vitus_move() ball->directBall = %i\n",
            j);
        j = 60;
      }
      Sint16 *table = ball->speedBallT;
      table = (Sint16 *) ((char *) table + j);
      Sint32 k = *(table++);
      ball->x_coord += (k * resolution);
      k = *table;
      ball->y_coord += (k * resolution);
      continue;
    }

    paddle = ball->paddle_touched;
    if (--ball->startCount == 0)
    {
      tiles_background *tiles = tiles_background::get_instance ();
      tiles->set_scroll_type(tiles_background::TILES_NO_SCROLL);
      ball->sticky_paddle_num = 0;
      if (paddle->is_glue == 2)
        {
          paddle->is_glue = 1;
        }
      paddle->ball_glued = (sprite_ball *) NULL;
      continue;
    }
    
    /* the ball is sticked */
    switch (ball->sticky_paddle_num)
    {
      case controller_paddles::BOTTOM_PADDLE:
        j = (paddle->collision_width >> 1) -
          ((ball->collision_width >> 1) + 1);
        j += paddle->x_coord;
        ball->x_coord = j;
        j = (paddle->y_coord) - (ball->collision_height + 1);
        ball->y_coord = j;
        break;

      case controller_paddles::RIGHT_PADDLE:
        j = (paddle->x_coord) - (ball->collision_width - 1);
        ball->x_coord = j;
        j =
          (paddle->collision_height >> 1) -
          ((ball->collision_height >> 1) + 1);
        j += paddle->y_coord;
        ball->y_coord = j;
        break;

      case controller_paddles::TOP_PADDLE:
        j =
          (paddle->collision_width >> 1) -
          ((ball->collision_width >> 1) + 1);
        j += paddle->x_coord;
        ball->x_coord = j;
        j = (paddle->y_coord) + paddle->collision_height + 1;
        ball->y_coord = j;
        break;

      case controller_paddles::LEFT_PADDLE:
        j = (paddle->x_coord) + (paddle->collision_width) + 1;
        ball->x_coord = j;
        j =
          (paddle->collision_height >> 1) -
          ((ball->collision_height >> 1) + 1);
        j += paddle->y_coord;
        ball->y_coord = j;
        break;

    }


    if (--ball->tempo_rota < 0)
    {
      ball->tempo_rota = 8;
      if (++ball->balle_rota > 13)
      {
        ball->balle_rota = 0;
      } 
      paddle = ball->paddle_touched;
      monPT = paddle->direct_tab + ball->balle_rota;
      ball->directBall = *monPT;
    }
  }
}

//-------------------------------------------------------------------------------
// guards levels: displacement of the balls glued on the bumper 
//-------------------------------------------------------------------------------
void
controller_balls::vitusmove2 ()
{
  Sint32 j;
  Sint32 *monPT;
  sprite_paddle *raket;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = sprites_list[i];
      if (balle->is_enabled)
        {

          //###########################################################
          // the ball is glued 
          //###########################################################
          if (balle->sticky_paddle_num > 0)
            {
              raket = balle->paddle_touched;
              if (!(--balle->startCount))
                {
                  balle->sticky_paddle_num = 0;
                  if (raket->is_glue == 2)
                    {
                      raket->is_glue = 1;
                    }
                  raket->ball_glued = (sprite_ball *) NULL;
                }
              else
                {
                  switch (balle->sticky_paddle_num)
                    {
                    case 1:
                      j = (raket->collision_width >> 1) -
                        ((balle->collision_width >> 1) + 1);
                      j += raket->x_coord;
                      balle->x_coord = j;
                      j = (raket->y_coord) - (balle->collision_height + 1);
                      balle->y_coord = j;
                      break;
                    }

                  if (--balle->tempo_rota < 0)
                    {
                      balle->tempo_rota = 8;
                      if (++balle->balle_rota > 13)
                        {
                        balle->balle_rota = 0;
                        }
                      raket = balle->paddle_touched;
                      monPT = raket->direct_tab + balle->balle_rota;
                      balle->directBall = *monPT;
                    }

                }
            }

          //###########################################################
          // the ball is not glued
          //###########################################################
          else
            {
              j = balle->directBall;    //ball is moving
              if (j > 64)
                {
                  fprintf (stderr,
                           "controller_balls::vitus_move() balle->directBall = %i\n",
                           j);
                  j = 60;
                }
              Sint16 *table = balle->speedBallT;
              table = (Sint16 *) ((char *) table + j);
              Sint32 k;
              k = *(table++);
              balle->x_coord += (k * resolution);
              k = *table;
              balle->y_coord += (k * resolution);
            }
        }
    }
}

/**
 * Collisions betweeb balls and paddles in bricks level 
 */
void
controller_balls::check_collisions_with_paddles ()
{
  Sint32 j, x, y;
  const Sint32 *monPT;
  sprite_paddle *paddle, *touched_paddle;
  paddle_bottom->touch_ball = false;
  paddle_right->touch_ball = false;
  paddle_top->touch_ball = false;
  paddle_left->touch_ball = false;
  right_panel_score *panel = right_panel_score::get_instance ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled || ball->sticky_paddle_num > 0 || ball->directBall >= 64)
        {
          continue;
        }
          /* bottom paddle */
          paddle = paddle_bottom;
          touched_paddle = NULL;
          if (paddle->is_enabled)
            {
              x = paddle->x_coord;
              y = paddle->y_coord;
              if (ball->x_coord + (Sint32)ball->collision_width > x &&
                  ball->y_coord + (Sint32)ball->collision_height > y &&
                  ball->x_coord < x + (Sint32)paddle->collision_width &&
                  ball->y_coord < y + (Sint32)paddle->collision_height)
                {
                  ball->y_coord = y - ball->collision_height;
                  touched_paddle = paddle;
                  panel->reset_gigablitz_countdown ();
                }
            }

          /* right paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_right;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->x_coord = x - ball->collision_width;
                      touched_paddle = paddle;
                    }
                }
            }

          /* top paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_top;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->y_coord = y + paddle->collision_height;
                      touched_paddle = paddle;
                    }
                }
            }

          /* left paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_left;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->x_coord = x + paddle->collision_width;
                      touched_paddle = paddle;
                    }
                }
            }

          /* does the ball touch a paddle?  */
          if (NULL != touched_paddle)
            {
              touched_paddle->touch_ball = true;
#ifndef SOUNDISOFF
              audio->play_sound (S_TOUCHRAK);
#endif
              ball->paddle_touched = touched_paddle;
              ball->tilt_delay = 0;
              j = ball->directBall;
              if (j > 64)
                {
                  std::cerr <<
                    "controller_balls::check_collisions_with_paddles() "
                    << "(1) ball->directBall " << j;
                  j = 64;
                }
              monPT = touched_paddle->rebonds_GD;
              monPT = (Sint32 *) ((char *) monPT + j);
              j = *monPT;

              if (j > 64)
                {
                  fprintf (stderr,
                           "controller_balls::check_collisions_with_paddles() (2) ball->directBall = %i (%i)\n",
                           j, ball->directBall);
                  for (Sint32 v = 0; v < 16; v++)
                    printf ("%i ; ", touched_paddle->rebonds_GD[v]);
                  monPT = touched_paddle->rebonds_GD;
                  j = ball->directBall;
                  //(char *)monPT += j;
                  monPT = (Sint32 *) ((char *) monPT + j);
                  /*printf("monPT = %x / touched_paddle->rebonds_GD = %x / *monPT = %i\n",
                     (long)monPT, (long)touched_paddle->rebonds_GD, *monPT);
                     printf("%i %i\n", touched_paddle->rebonds_GD[16], touched_paddle->rebonds_GD[64]); */
                  j = 60;
                }


              //ball->directBall = *monPT;
              ball->directBall = j;
              if (touched_paddle->is_glue == 1 && !touched_paddle->ball_glued)
                {
                  touched_paddle->is_glue = 2;   //ball glued to the bumper 
                  touched_paddle->ball_glued = (sprite_ball *) ball;
                  ball->raket_glue = touched_paddle;
                  ball->startCount = glue_delay;       //time of the glue 
                  ball->sticky_paddle_num = paddle->paddle_number;
                }

        }
    }
}

//-------------------------------------------------------------------------------
// guards level: collisions balls and bumper 
//-------------------------------------------------------------------------------
void
controller_balls::vitusbump2 ()
{
  Sint32 j, x, y;
  const Sint32 *monPT;
  sprite_paddle *raket, *bumpX;
  paddle_bottom->touch_ball = false;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = sprites_list[i];
      if (balle->is_enabled)
        {
          raket = paddle_bottom;
          bumpX = 0;
          if (raket->is_enabled)
            {
              x = raket->x_coord;
              y = raket->y_coord;
              if (balle->x_coord + (Sint32)balle->collision_width > x &&
                  balle->y_coord + (Sint32)balle->collision_height > y &&
                  balle->x_coord < x + (Sint32)raket->collision_width &&
                  balle->y_coord < y + (Sint32)raket->collision_height)
                {
                  balle->y_coord = y - balle->collision_height;
                  bumpX = raket;
                }
            }

          //###########################################################
          // does the ball touch a bumper?
          //###########################################################
          if (bumpX)
            {
              bumpX->touch_ball = true;
#ifndef SOUNDISOFF
              audio->play_sound (S_TOUCHRAK);
#endif
              balle->paddle_touched = bumpX;
              balle->tilt_delay = 0;
              j = balle->directBall;
              monPT = bumpX->rebonds_GD;
              //(char *)monPT += j;
              monPT = (Sint32 *) ((char *) monPT + j);
              balle->directBall = *monPT;
              if (bumpX->is_glue == 1)
                {
                  bumpX->is_glue = 2;   //ball glued to the bumper 
                  bumpX->ball_glued = (sprite_ball *) balle;
                  balle->raket_glue = bumpX;
                  balle->startCount = glue_delay;       //time of the glue 
                  balle->sticky_paddle_num = raket->paddle_number;
                }
            }
        }
    }
}

/**
 * Handle collisions balls with robot paddle
 */
void
controller_balls::check_collisions_with_robot ()
{
  if (!tec_robot0->bump_actif)
    {
      return;
    }
  sprite_paddle *paddle = tec_robot0;
  paddle->touch_ball = false;
  Sint32 x1 = paddle->x_coord;
  Sint32 y1 = paddle->y_coord;
  Sint32 x2 = paddle->x_coord + paddle->collision_width;
  Sint32 y2 = paddle->y_coord + paddle->collision_height + 8;
  const Sint32 *monPT;
  Sint32 j;
  for (Uint32 i = 0; i < max_of_sprites; i++)
  {
    sprite_ball *ball = sprites_list[i];
    if (!ball->is_enabled)
    {
      continue;
    }
    if (ball->x_coord + (Sint32)ball->collision_width > x1 &&
        ball->y_coord + (Sint32)ball->collision_height > y1 &&
        ball->x_coord < x2 && ball->y_coord < y2)
    {
      ball->y_coord = y1 - ball->collision_height;
      paddle->touch_ball = true;
      j = ball->directBall;
      monPT = paddle->rebonds_GD;
      monPT = (Sint32 *) ((char *) monPT + j);
      ball->directBall = *monPT;
#ifndef SOUNDISOFF
      audio->play_sound (S_TOUCHRAK);
#endif
    }
  }
}

/**
 * Handle ejectors in bricks level
 */
void
controller_balls::handle_ejectors ()
{
  controller_ejectors *ejectors = controller_ejectors::get_instance ();
  sprite_object *coin1 =
    ejectors->get_ejector (controller_ejectors::TOP_LEFT_EJECTOR);
  sprite_object *coin2 =
    ejectors->get_ejector (controller_ejectors::TOP_RIGHT_EJECTOR);
  sprite_object *coin3 =
    ejectors->get_ejector (controller_ejectors::BOTTOM_LEFT_EJECTOR);
  sprite_object *coin4 =
    ejectors->get_ejector (controller_ejectors::BOTTOM_RIGHT_EJECTOR);

  for (Uint32 i = 0; i < max_of_sprites; i++)
  {
    sprite_ball *ball = sprites_list[i];
    if (!ball->is_enabled)
    {
      continue;
    }
    /* test if the ball is already into an ejector */
    Sint32 *flag_pt = ball->eject_ball;
    Sint32 *table = NULL;
    if (*(flag_pt++))
      {
        table = ball_eject1;
      }
    else
    {
      if (*(flag_pt++))
        {
          table = ball_eject2;
        }
      else
      {
        if (*(flag_pt++))
          {
            table = ball_eject3;
          }
        else
        {
          if (*(flag_pt++))
            {
              table = ball_eject4;
            }
        }
      }
    }

    /*
     * the ball is in an ejector
     */
    if (table != NULL)
    {
      Sint32 j = ++(*(--flag_pt));
      /* time before ejection */
      if (j == 160)
      {
        Sint32 j = random_counter & 0xF;
        table += j;
        ball->directBall = *table;
#ifndef SOUNDISOFF
        audio->play_sound (S_COINEJEC);
#endif
      }
      else
      {
        if (j == 200)
          {
            *flag_pt = 0;
          }
      }
      return;
    }

    /*
     * if not test if a ball is aspired by an ejector
     */
      /* top-left */
      if (ball->collision (coin1))
      {
        ball->pull (coin1, 10 * resolution, 10 * resolution);
        ball->eject_ball[0] = 1;
        ball->directBall = 64;
        current_player->add_score (10);
#ifndef SOUNDISOFF
        audio->play_sound (S_COINASPI);
#endif
      }
      else
      {
        /* top-right */
        if (ball->collision (coin2))
        {
          ball->pull (coin2, 5 * resolution,
              10 * resolution);
          ball->eject_ball[3] = 1;
          ball->directBall = 64;
          current_player->add_score (10);
#ifndef SOUNDISOFF
          audio->play_sound (S_COINASPI);
#endif
        }
        else
        {
          /* bottom-left */
          if (ball->collision (coin3))
          {
            ball->pull (coin3, 10 * resolution,
                5 * resolution);
            ball->eject_ball[2] = 1;
            ball->directBall = 64;
            current_player->add_score (10);
#ifndef SOUNDISOFF
            audio->play_sound (S_COINASPI);
#endif
          }
          else
          {
            /* bottom-right */
            if (ball->collision (coin4))
            {
              ball->pull (coin4, 5 * resolution,
                  5 * resolution);
              ball->eject_ball[1] = 1;
              ball->directBall = 64;
              current_player->add_score (10);
#ifndef SOUNDISOFF
              audio->play_sound (S_COINASPI);
#endif
            }
          }
        }
      }
  }
}

/**
 * Collisions of the balls with the 4 walls in the bricks levels
 */
void
controller_balls::collision_with_walls ()
{
  controller_sides_bricks *sides_bricks =
    controller_sides_bricks::get_instance ();
  Sint32 left_xcoord = sides_bricks->get_left_xcoord ();
  Sint32 right_xcoord = sides_bricks->get_right_xcoord ();
  Sint32 top_ycoord = sides_bricks->get_top_ycoord ();
  Sint32 bottom_ycoord = ptBob_wall->get_y_coord ();
  bool is_wall = ptBob_wall->is_enable ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 x = ball->x_coord;
      Sint32 y = ball->y_coord;
      Sint32 *monPT = NULL;

      /* collision with the bottom wall, if it's enable */
      if (is_wall && y > (bottom_ycoord - (Sint32)ball->collision_height))
        {
          monPT = rb7;
        }
      else
        {
          /* collision with the left wall */
          if (x < left_xcoord)
            {
              if (sides_bricks->collision_with_left_wall (y))
                {
                  monPT = rb5;
                }
            }
          else
            {
              /* collision with the right wall */
              x += ball->collision_width;
              if (x > right_xcoord)
                {
                  if (sides_bricks->collision_with_right_wall (y))
                    monPT = rb1;
                }
              else
                {
                  /* collision with the up wall */
                  if (y < top_ycoord)
                    {
                      if (sides_bricks->collision_with_top_wall (x))
                        monPT = rb3;
                    }
                }
            }
        }

      /* collision dectected */
      if (NULL != monPT)
        {
          monPT = (Sint32 *) ((char *) monPT + ball->directBall);
          ball->directBall = *monPT;
#ifndef SOUNDISOFF
          audio->play_sound (S_BRICOTES);
#endif
        }
    }
}

/**
 * Collisions of the balls with the 3 walls in the guarduans levels
 */
void
controller_balls::vituscote2 ()
{
  Sint32 left_xcoord = 16 * resolution;
  Sint32 right_xcoord = 300 * resolution;
  Sint32 top_ycoord = 8 * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
          Sint32 x = ball->x_coord;
          Sint32 y = ball->y_coord;
          Sint32 *monPT = 0;
          if (x < left_xcoord)
            {
              monPT = rb5;
              ball->x_coord = left_xcoord;
#ifndef SOUNDISOFF
              audio->play_sound (S_BRICOTES);
#endif
              ball->colli_wall = 4;
            }
          else
            {
              if (x > right_xcoord)
                {
                  monPT = rb1;
                  ball->x_coord = right_xcoord;
#ifndef SOUNDISOFF
                  audio->play_sound (S_BRICOTES);
#endif
                  ball->colli_wall = 2;
                }
              else
                {
                  if (y < top_ycoord)
                    {
                      monPT = rb3;
                      ball->y_coord = top_ycoord;
#ifndef SOUNDISOFF
                      audio->play_sound (S_BRICOTES);
#endif
                      ball->colli_wall = 3;
                    }
                  else
                    ball->colli_wall = 0;
                }
            }
          if (monPT)
            {                   //(char *)monPT += ball->directBall;
              monPT = (Sint32 *) ((char *) monPT + ball->directBall);
              ball->directBall = *monPT;
            }
    }
}

//------------------------------------------------------------------------------
// guards levels: prevent that the ball remains blocked horizontally 
//------------------------------------------------------------------------------
void
controller_balls::vitusbound ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = sprites_list[i];
      if (!balle->is_enabled)
        continue;
      Sint32 dball = balle->directBall;
      if (dball >= 32)
        dball -= 32;
      if (dball == balle->save_Dball)
        {
          if (balle->countDball++ > 360 && dball == 0 && balle->colli_wall)
            if (balle->directBall == 32)
              balle->directBall = 28;
            else
              balle->directBall = 4;
        }
      else
        {
          balle->save_Dball = dball;
          balle->countDball = 0;
        }
    }
}

/** 
 * Check for collision balls with bricks 
 */
void
controller_balls::check_bricks_collision ()
{
  controller_bricks *bricks = controller_bricks::get_instance ();

  /* brick's width in pixels */
  Uint32 brick_width = bricks->get_brick_width ();
  /* y-offset between 2 bricks */
  Sint32 byoff = bricks->getYOffset ();
  /* first indestructible brick */
  Sint32 indus = bricks->getBkIndus ();

  sprite_ball **balls = sprites_list;
  brick_info *bricks_map = bricks->get_bricks_map ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 indes = 0;         // flag brique indestructible touchee
      Sint32 *colTB = ball->collisionT; // table des 4 points de collision 
      Sint32 incre = 1;         // incremente flag rebond
      Sint32 rebon = 0;         // flag rebond
      /* for the 4 collision pointof the ball */
      for (Sint32 j = 0; j < 4; j++, incre += incre)
        {
          Sint32 x = ball->x_coord;
          x += *(colTB++);
          Sint32 y = ball->y_coord;
          y += *(colTB++);
	  /* list of bricks to clear or redraw */
          brick_redraw *redraw = bricks->get_bricks_redraw ();
          redraw->xcoord_collision = x;
          redraw->ycoord_collision = y;
          x /= brick_width;
          y /= byoff;
          y *= controller_bricks::NB_BRICKSH;
          x += y;
          brick_info *brick = (bricks_map + x);
          x = brick->brique_rel;
          /* collision between a ball and a brick? */
	  if (0 == x)
            {
	      /* no collision */
              continue;
            }
          redraw->is_gigablitz_destroyed = false;
          redraw->paddle = ball->paddle_touched;
	  if (bob_ground)
	    {
	       brick->sprite->touch();
	    }
          x = x - indus;
          if (x >= 0)
            {
              /* 
               * indestructible brick touched!
               */
              /* collision with indestructible brick */
              indes = 1;
              /* indestructible/destructible bricks? */
              if ((x -= brick_width) > 0)
                {
                  if (ball->ballPowerX == sprite_ball::BALLPOWER2)
                    {
                      redraw->pixel_offset = brick->pixel_offset;
                      redraw->brick_map = brick;
                      redraw->is_indestructible = true;
                      redraw->number = brick->number;
                      redraw->sprite = brick->sprite;
                      redraw->is_background = true;
                      /* clear brick code */
                      brick->brique_rel = 0;
		      /* restore background under brick */
		      bricks->bricks_redraw_next ();
                    }
                  else
                    {
                      x = 2;
#ifndef SOUNDISOFF
                      audio->play_sound (S_TOINDES2);
#endif
                    }
                }
              else
                {
		  /* brick's really indestructible */
                  x = 1; 
#ifndef SOUNDISOFF
                  audio->play_sound (S_TOINDES1);
#endif
                }
            }

          /* 
           * normal brick touched
           */
          else
            {
              redraw->is_indestructible = false;
              redraw->pixel_offset = brick->pixel_offset;
              redraw->brick_map = brick;
              redraw->sprite = brick->sprite;
              brick->h_pos = brick->h_pos - (ball->powerBall1 * 2);
              if (brick->h_pos <= 0)
                {
                  brick->h_pos = 0;
                  /* clear brick code */
                  brick->brique_rel = 0;
                  redraw->number = brick->number;
                  /* restore background */
                  redraw->is_background = true;
                }
              else
                {
                  brick->brique_rel = brick->brique_rel - ball->powerBall2;
                  redraw->number = brick->brique_rel;
                  /* redraw a new brick */
                  redraw->is_background = false;
                }
	      bricks->bricks_redraw_next ();
            }
          /* inc. rebound flag */
          rebon += incre;

      if (--rebon >= 0)
        {
          if (indes > 0 || ball->ballPowerX == sprite_ball::BALLNORMAL)
            {
              Sint32 *rebPT = *(brick_jump + rebon);
              rebPT = (Sint32 *) ((char *) rebPT + ball->directBall);
              ball->directBall = *rebPT;
            }
        }
    }
  }
}

/**
 * Collisions between balls and eyes
 */
void
controller_balls::check_collisions_with_eyes ()
{
  controller_magnetic_eyes *eyes = controller_magnetic_eyes::get_instance ();
  Sint32 hypo = eyes->hypotenuse;
  sprite_ball **liste = sprites_list;
  Sint32 numof_eyes = eyes->get_max_of_sprites ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(liste++);
      if (!ball->is_enabled)
        {
          continue;
        }
      sprite_eye **eyes_list = eyes->get_sprites_list ();
      for (Sint32 j = 0; j < numof_eyes; j++)
        {
          sprite_eye *eye = *(eyes_list++);
          if (!eye->is_enabled)
            {
              continue;
            }
          Sint32 center_x = eye->x_coord + eyes->center_x;
          Sint32 center_y = eye->y_coord + eyes->center_y;
          Sint32 delta_x =
            ball->x_coord + (ball->collision_width / 2) - center_x;
          delta_x = delta_x * delta_x;
          Sint32 delta_y =
            ball->y_coord + (ball->collision_width / 2) - center_y;
          delta_y = delta_y * delta_y;
          if (delta_x + delta_y >= hypo)
            {
              continue;
            }
          delta_x = ball->x_coord + (ball->collision_width / 2) - center_x;
          delta_y = ball->y_coord + (ball->collision_width / 2) - center_y;
          if (delta_y == 0)
            {
              if (delta_x < 0)
               {
                  ball->directBall = 32;
               }
              else
               {
                  ball->directBall = 0;
               }
            }

          if (delta_y > 0)
            {
              if (delta_x == 0)
                {
                  ball->directBall = 48;
                }
              if (delta_x < 0)
                {
                  delta_x = -delta_x;
                  if (delta_x == delta_y)
                    {
                      ball->directBall = 40;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->directBall = 44;
                    }
                  else
                    {
                      ball->directBall = 36;
                    }
                }
              else
                {
                  if (delta_x == delta_y)
                    {
                      ball->directBall = 56;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->directBall = 52;
                    }
                  else
                    {
                      ball->directBall = 56;
                    }
                }
            }

          if (delta_y < 0)
            {
              delta_y = -delta_y;
              if (delta_x == 0)
                {
                  ball->directBall = 16;
                }
              if (delta_x < 0)
                {
                  delta_x = -delta_x;
                  if (delta_x == delta_y)
                    {
                      ball->directBall = 24;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->directBall = 20;
                    }
                  else
                    {
                      ball->directBall = 28;
                    }
                }
              else
                {
                  if (delta_x == delta_y)
                    {
                      ball->directBall = 8;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->directBall = 12;
                    }
                  else
                    {
                      ball->directBall = 4;
                    }
                }
            }
        }
    }
}

/**
 * Collisions between balls and ships
 */
void
controller_balls::check_collisions_with_ships ()
{
  controller_ships *ships = controller_ships::get_instance ();
  sprite_ball **balls = sprites_list;
  Uint32 t = ships->get_max_of_sprites ();
  sprite_ship **ships_list = ships->get_sprites_list ();
  Sint32 nouve = (random_counter & 0xF) << 2;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 h = ball->collision_width;
      h = h - 2;
      Sint32 x1 = ball->x_coord;
      Sint32 x2 = x1 + h;
      x1 -= 20;
      Sint32 y1 = ball->y_coord;
      Sint32 y2 = y1 + h;
      y1 -= 26;
      sprite_ship **ships = ships_list;
      for (Uint32 j = 0; j < t; j++)
        {
          sprite_ship *ship = *(ships++);
          if (ship->atom_actif > 0)
            {
              continue;
            }

          Sint32 k = ship->y_coord;
          if (k < y2 && k > y1)
            {
              k = ship->x_coord;
              if (k < x2 && k > x1)
                {
                  current_player->add_score (100);
#ifndef SOUNDISOFF
                  audio->play_sound (S_TO_ATOMS);
#endif
                  k = (ball->ballPowerX + 1) * 4;
                  ship->atom_power -= k;
                  if (ship->atom_power < 1)
                    ship->explosion1 (ball);
                  ball->directBall = nouve;
                }
            }
        }
    }
}

/**
 * Collision of balls with the guardians in the guardians levels
 */
void
controller_balls::vitusGuard ()
{
  controller_guardians *guards = controller_guardians::get_instance ();
  controller_capsules *capsules = controller_capsules::get_instance ();
  controller_moneys *moneys = controller_moneys::get_instance ();

  /* number of balls from 1 to n */
  Uint32 numof_balls = max_of_sprites;
  sprite_ball **balls = sprites_list;
  /* number of guardians from 1 to 2 */
  Uint32 numof_guards = guards->get_max_of_sprites ();
  sprite_guardian **guards_list = guards->get_sprites_list ();
  for (Uint32 j = 0; j < numof_guards; j++)
    {
      sprite_guardian *guardian = guards_list[j];
      if (!guardian->is_enabled || guardian->energy_level <= 0)
        {
          continue;
        }
      sprite_ball *balok = NULL;
      Sint32 grdx1 = guardian->x_coord;
      Sint32 grdx2 = grdx1 + guardian->gard_colx2;
      grdx1 += guardian->gard_colx1;
      Sint32 grdy1 = guardian->y_coord;
      Sint32 grdy2 = grdy1 + guardian->gard_coly2;
      grdy1 += guardian->gard_coly1;
      for (Uint32 i = 0; i < numof_balls; i++)
        {
          sprite_ball *balle = balls[i];
          if (!balle->is_enabled)
            {
              continue;
            }
          Sint32 x = balle->x_coord;
          if (x <= grdx2)
            {
              Sint32 y = balle->y_coord;
              if (y <= grdy2)
                {
                  x += balle->collision_width;
                  if (x > grdx1)
                    {
                      y += balle->collision_width;
                      if (y > grdy1)
                        {
                          x = ((random_counter + i) & 0xF) << 2;
#ifndef SOUNDISOFF
                          audio->play_sound (S_GARDIENT);
#endif
                          balle->directBall = x;
                          guardian->recently_touched = 5;
                          guardian->energy_level -= balle->powerBall1;
                          if (guardian->energy_level <= 0)
                            {
                              /* guardian is dead */
                              guardian->energy_level = 0;
                              /* make exploding guardian! */
                              guardian->explode_delay_counter = 500;
                            }
                          balok = balle;
                        }
                    }
                }
            }
        }
      if (NULL != balok)
        {

          moneys->send_money_from_guardian (balok);
          capsules->check_if_send_capsule (balok);
        }
    }
}

/**
 * Return the first enable ball
 * @return a pointer to ball sprite
 */
sprite_ball *
controller_balls::first_ball ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          return ball;
        }
    }
  return sprites_list[0];
}

/**
 * Enable control of the balls with left mouse button into bricks levels
 */
void
controller_balls::enable_balls_control ()
{
  balls_are_controlled = true;
}

//-------------------------------------------------------------------------------
// bricks levels: extra balls; add 2 balls into ejectors
//-------------------------------------------------------------------------------
void
controller_balls::run_2balls ()
{
  run_nballs (2);
}

/**
 * Add n ball(s) into the ejector(s) in the bricks levels
 * @param nball from 1 to n
 */
void
controller_balls::run_nballs (Uint32 nball)
{
  if (nball < 1)
    {
      nball = max_of_sprites;
    }
  sprite_ball **balls = sprites_list;
  Uint32 count = 0;
  Uint32 ejector_id = random_counter & 3;
  Uint32 delay = 1;
  for (Uint32 i = 0; i < max_of_sprites && count < nball; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          continue;
        }
      ball->enbale_on_ejector (ejector_id++, delay);
      count++;
      num_of_sprites++;
      delay += 2;
    }
}

/** 
 * Add 3 balls starting from the first enable ball
 */
void
controller_balls::run_3balls ()
{
  sprite_ball *model = first_ball ();
  /* direction of the current ball */
  Uint32 j = model->directBall;
  Uint32 i = 0;
  Uint32 count = 0;
  sprite_ball **balls = sprites_list;
  while (i < max_of_sprites && count < 3)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          j += 8;
          j &= 60;
          ball->duplicate_from (model, j);
          num_of_sprites++;
          count++;
        }
      i++;
    }
}

/** 
 * Transform all the enable balls into balls power 1
 */
void
controller_balls::set_power_1 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_power_1 ();
        }
    }
}

/** 
 * Transform all the enable balls into balls power 2
 */
void
controller_balls::set_power_2 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_power_2 ();
        }
    }
}

/** 
 * Transform all the enable balls into balls size 2 (7*7 or 14*14)
 */
void
controller_balls::set_size_2 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_size_2 ();
        }
    }
}

/**
 * Transform all the enable balls into balls size 3 (10*10 or 20*20)
 */
void
controller_balls::set_size_3 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_size_3 ();
        }
    }
}

/**
 * Increase the speed of the balls to the maximum
 */
void
controller_balls::maxi_speed ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_maximum_speed ();
        }
    }
}

/**
 * Check if the player can use the tilt in the bricks levels
 */
void
controller_balls::time_2tilt ()
{
  bool tilt = false;
  head_animation *head_anim = head_animation::get_instance ();
  sprite_ball **balls = sprites_list;
  Sint32 delay = balle_tilt;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled || ball->sticky_paddle_num > 0)
        {
          continue;
        }
      if (ball->tilt_delay == delay && !tilt)
        {
          head_anim->start_yawn ();
#ifndef SOUNDISOFF
          audio->play_sound (S_TILTALAR);
#endif
          tilt = true;
        }
      ball->tilt_delay++;
    }
}

/**
 * Check if the player can use the tilt in guards level
 */
void
controller_balls::time2tilt2 ()
{
  Uint32 tilt = 0;
  sprite_ball **liste = sprites_list;
  Sint32 v = balle_tilt;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = *(liste++);
      if (balle->is_enabled)
        {
          if (!balle->sticky_paddle_num)
            {
              if (balle->tilt_delay == v)
                {
                  if (!tilt)
                    {
                      tilt = 1;
#ifndef SOUNDISOFF
                      audio->play_sound (S_TILTALAR);
#endif
                    }
                  balle->tilt_delay++;
                }
              else
                balle->tilt_delay++;
            }
        }
      else
        balle->tilt_delay++;
    }
}

/**
 * Handle the control of the balls with the left mouse button
 */
void
controller_balls::controll_balls ()
{
  if (!balls_are_controlled or ! keyboard->is_right_button ())
    {
      return;
    }
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 dball = ball->directBall;
      if (dball < 64)
        {
          dball = dball + 4;
          dball = dball & 60;
          ball->directBall = dball;
        }
    }
}

/**
 * Check if there remains at least a ball glue
 * @return True if remains at least a ball glue
 */
Sint32
controller_balls::least_glue ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->sticky_paddle_num > 0)
        {
          return 1;
        }
    }
  return 0;
}


//------------------------------------------------------------------------------
// directions of the ball when it is leave an ejector. 
//------------------------------------------------------------------------------
// top-left
Sint32
  controller_balls::ball_eject1[] = {
  52, 56, 60, 60, 52, 56, 60, 60, 52, 52, 56, 52, 52, 60, 56, 52, 56, 56
};

// bottom-left
Sint32
  controller_balls::ball_eject2[] = {
  8, 4, 12, 12, 8, 4, 4, 12, 8, 4, 12, 4, 8, 12, 4, 8, 12, 4, 4
};

// bottom-right
Sint32
  controller_balls::ball_eject3[] = {
  20, 28, 24, 20, 20, 28, 28, 24, 20, 28, 24, 24, 28, 28, 20, 20, 24, 24, 28
};

// top-right 
Sint32
  controller_balls::ball_eject4[] = {
  36, 44, 40, 36, 36, 44, 44, 40, 40, 36, 44, 40, 40, 36, 36, 44, 44, 40, 36
};

/*
 * directions of the ball after a rebound on a brick
 */
Sint32 controller_balls::rb0[16] =
{
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
/* right rebound */
Sint32 controller_balls::rb1[16] =
{
  32, 28, 24, 20, 20, 24, 24, 28, 32, 36, 40, 40, 44, 44, 40, 36
};
Sint32 controller_balls::rb2[16] =
{
  48, 36, 40, 44, 32, 44, 24, 28, 32, 36, 40, 44, 48, 48, 44, 40
};
/* top rebond */
Sint32 controller_balls::rb3[16] = {
  60, 60, 56, 52, 48, 44, 40, 36, 36, 40, 40, 44, 48, 52, 56, 56
};
Sint32 controller_balls::rb4[16] =
{
  0, 4, 8, 0, 0, 52, 56, 60, 48, 52, 56, 44, 48, 52, 56, 60
};
/* left rebond */
Sint32 controller_balls::rb5[16] =
{
  0, 4, 8, 8, 12, 12, 8, 4, 0, 60, 56, 52, 52, 36, 56, 60
};
Sint32 controller_balls::rb6[16] = {
  0, 4, 8, 12, 16, 20, 24, 12, 16, 12, 8, 4, 0, 4, 8, 60
};
/* bottom rebond */
Sint32 controller_balls::rb7[16] =
{
  4, 8, 12, 12, 16, 20, 20, 24, 28, 28, 24, 20, 16, 12, 8, 4
};
Sint32 controller_balls::rb8[16] =
{
  16, 20, 24, 12, 16, 20, 24, 28, 32, 36, 40, 28, 32, 20, 24, 28
};
Sint32 *
  controller_balls::brick_jump[15] =
  { rb1, rb3, rb2, rb5, rb1, rb4, rb3, rb7, rb8, rb2, rb1, rb6, rb7, rb5,
  rb1
};
