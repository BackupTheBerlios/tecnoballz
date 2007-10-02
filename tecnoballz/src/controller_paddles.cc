/** 
 * @file controller_paddles.cc
 * @brief Paddles controller 
 * @date 2007-10-02
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.25 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_paddles.cc,v 1.25 2007/10/02 11:25:37 gurumeditation Exp $
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
#include "../include/controller_paddles.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"
#include "../include/controller_projectiles.h"
#include "../include/handler_keyboard.h"

/** 
 * Create the paddles controller in bricks levels
 */
controller_paddles::controller_paddles ()
{
  littleInit ();
  sprites_have_shades = true;
  is_draw_pixel_by_pixel = false;
  is_team_mode = false;
  
  if (current_phase == GUARDS_LEVEL)
    {
      max_of_sprites = 1;
      sprite_type_id = BOB_BUMPER;
      paddle_length = 32 * resolution;
      reverse_counter = 0;

      paddle_bottom = new sprite_paddle (false);
      paddle_right = NULL;
      paddle_top = NULL;
      paddle_left = NULL;
      paddle_robot = NULL;

      width_maxi = 32 * resolution;
      width_mini = 32 * resolution;
      bottom_y_coord = (232 - 8) * resolution;
      min_coordinate = 16 * resolution;
      max_coordinate = 300 * resolution;
    }
 else
    {
      max_of_sprites = 5;
      sprite_type_id = BOB_BUMPHR;
      paddle_length = 64 * resolution;
      paddle_bottom = new sprite_paddle ();
      paddle_right = new sprite_paddle ();
      paddle_top = new sprite_paddle ();
      paddle_left = new sprite_paddle ();
      paddle_robot = new sprite_paddle (false);

      min_coordinate = 32 * resolution;
      max_coordinate = 224 * resolution;
      top_y_coord = (20 - 8) * resolution;
      bottom_y_coord = (232 - 8) * resolution;
      left_x_coord = 20 * resolution;
      right_x_coord = 228 * resolution;
      width_maxi = 64 * resolution;
      width_mini = 16 * resolution;
      reverse_counter = 0;
    }
}

/** 
 * Release the paddles controller in guards levels
 */
controller_paddles::~controller_paddles ()
{
  release_sprites_list ();
}

/**
 * Create and initializes the list of the projectiles for the 4 paddles
 */
void
controller_paddles::create_projectiles_list ()
{
  paddle_bottom->create_projectiles_list ();
  paddle_right->create_projectiles_list ();
  paddle_top->create_projectiles_list ();
  paddle_left->create_projectiles_list ();
}

/**
 * Intialize bumpers 
 */
void
controller_paddles::create_paddles_sprites ()
{
  alloc_sprites_list ();

  /*
   * Create one simple paddle in guards levels
   */
  if (max_of_sprites == 1)
    {
      paddle_bottom->set_object_pos (0);
      paddle_bottom->create_sprite (sprite_type_id, sprites_bitmap, 1, 0);
      sprites->add (paddle_bottom);
      sprites_list[0] = paddle_bottom;
      paddle_bottom->set_coordinates (keyboard->get_mouse_x (), bottom_y_coord);
      /* width of the paddle in pixels */
      paddle_bottom->collision_width = paddle_length;
      paddle_bottom->paddle_number = 1;
      paddle_bottom->is_vertical = 0;
      /* ball rebounds table */
      paddle_bottom->rebonds_Ga = midi1_left;
      paddle_bottom->rebonds_Dr = midi1Right;
      paddle_bottom->bump_actif = 1;
      paddle_bottom->enable ();
      paddle_bottom->direct_tab = ballePets1;      // table direction balle collee
    }
  else
    {
      /**
       * create 4 paddles sprites
       */
      Uint32 id;
      if (current_player->get_area_number () > 2)
        {
          id = handler_resources::BITMAP_PADDLES_1;
        }
      else
        {
          id = handler_resources::BITMAP_PADDLES_2;
        }
      resources->load_sprites_bitmap (id);

      /* create bottom paddle sprite */
      paddle_bottom->set_object_pos (0);
      paddle_bottom->create_sprite (BOB_BUMPHR, sprites_bitmap, true, 0);
      sprites->add (paddle_bottom);
      sprites_list[0] = paddle_bottom;

      /* create left paddle sprite */
      paddle_right->set_object_pos (1);
      paddle_right->create_sprite (BOB_BUMPVT, sprites_bitmap, true, 0);
      sprites->add (paddle_right);
      sprites_list[1] = paddle_right;

      /* create top paddle sprite */
      paddle_top->set_object_pos (2);
      paddle_bottom->duplicate_to (paddle_top);
      sprites->add (paddle_top);
      sprites_list[2] = paddle_top;

      /* create right bumper sprite */
      paddle_left->set_object_pos (3);
      paddle_right->duplicate_to (paddle_left);
      sprites->add (paddle_left);
      sprites_list[3] = paddle_left;

      /* release paddles graphic page */
      resources->release_sprites_bitmap ();
    }
}

/**
 * Create and initialize the robot paddle sprite
 */
void
controller_paddles::initialize_robot ()
{
  paddle_robot->set_object_pos (4);
  paddle_robot->create_sprite (BOB_ROBOT0, sprites_bitmap, true, 0);
  sprites->add (paddle_robot);
  sprites_list[4] = paddle_robot;
}

/**
 * Activate robot paddle in bricks level
 */
void
controller_paddles::enable_robot ()
{
  Sint32 center;
  paddle_robot->enable ();
  center = min_coordinate + (max_coordinate - min_coordinate) / 2 -
    (paddle_robot->collision_width / 2);
  paddle_robot->set_coordinates (center, 232 * resolution);
  paddle_robot->bump_actif = 1;
}

/**
 * Deactivate robot paddle in bricks level
 */
void
controller_paddles::disable_robot ()
{
  paddle_robot->disable ();
  paddle_robot->bump_actif = 0;
}

/**
 * Initialize the four paddles in the bricks levels
 * @param blitz a pointer to the gigablitz controller
 * @param ball a pointer to the balls controller
 */
void
controller_paddles::init_paddles (controller_gigablitz * blitz, controller_balls * ball)
{
  gigablitz = blitz;
  balls = ball;

  paddle_length = current_player->get_paddle_length ();
  Sint32 center = (max_coordinate - min_coordinate) / 2 - (paddle_length / 2);

  /* initialize bottom paddle */ 
  paddle_bottom->set_coordinates (center, bottom_y_coord);
  /* paddles width: 8, 16, 24, 32, 40, 48, 56 or 64 pixels */
  paddle_bottom->collision_width = paddle_length;
  paddle_bottom->paddle_number = BOTTOM_PADDLE;
  paddle_bottom->is_vertical = false;
  paddle_bottom->bumpActive (is_team_mode, paddle_length, 3);
  paddle_bottom->bump_TFIRE = 2;
  paddle_bottom->bumper_FX0 = 0;
  paddle_bottom->bumper_FY0 = -5 * resolution;
  paddle_bottom->bumper_FX1 = -1 * resolution;
  paddle_bottom->bumper_FY1 = -4 * resolution;
  paddle_bottom->bumper_FX2 = 1 * resolution;
  paddle_bottom->bumper_FY2 = -4 * resolution;
  paddle_bottom->bump_Xscie = 32 * resolution;
  paddle_bottom->bump_Yscie = -20 * resolution;
  paddle_bottom->bump_xdeca = 0;
  paddle_bottom->bump_ydeca = -10 * resolution;
  paddle_bottom->rebonds_Ga = midi1_left;  // rebonds raquette va a gauche
  paddle_bottom->rebonds_Dr = midi1Right;  // rebonds raquette va a droite
  paddle_bottom->direct_tab = ballePets1;  // table direction balle collee
  paddle_bottom->width_mini = width_mini;
  paddle_bottom->width_maxi = width_maxi;

  /* initialize right paddle */ 
  paddle_right->set_coordinates (right_x_coord, center);
  paddle_right->collision_height = paddle_length;
  paddle_right->paddle_number = RIGHT_PADDLE;
  paddle_right->is_vertical = true;
  paddle_right->bumpActive (is_team_mode, paddle_length, current_player->get_paddle_alive_counter (RIGHT_PADDLE));
  paddle_right->bump_TFIRE = 2;
  paddle_right->bumper_FX0 = -5 * resolution;
  paddle_right->bumper_FY0 = 0;
  paddle_right->bumper_FX1 = -4 * resolution;
  paddle_right->bumper_FY1 = 1 * resolution;
  paddle_right->bumper_FX2 = -4 * resolution;
  paddle_right->bumper_FY2 = -1 * resolution;
  paddle_right->bump_Xscie = -20 * resolution;
  paddle_right->bump_Yscie = 32 * resolution;
  paddle_right->bump_xdeca = -10 * resolution;
  paddle_right->bump_ydeca = 0;
  paddle_right->rebonds_Ga = midi2_left;
  paddle_right->rebonds_Dr = midi2Right;
  paddle_right->direct_tab = ballePets2;
  paddle_right->width_mini = width_mini;
  paddle_right->width_maxi = width_maxi;
  current_player->set_paddle_alive_counter (2, paddle_right->bump_actif);

  /* initialize top paddle */ 
  paddle_top->set_coordinates (center, top_y_coord);
  paddle_top->collision_width = paddle_length;
  paddle_top->paddle_number = TOP_PADDLE;
  paddle_top->is_vertical = false;
  paddle_top->bumpActive (is_team_mode, paddle_length, current_player->get_paddle_alive_counter (TOP_PADDLE));
  paddle_top->bump_TFIRE = 2;
  paddle_top->bumper_FX0 = 0;
  paddle_top->bumper_FY0 = 5 * resolution;
  paddle_top->bumper_FX1 = 1 * resolution;
  paddle_top->bumper_FY1 = 4 * resolution;
  paddle_top->bumper_FX2 = -1 * resolution;
  paddle_top->bumper_FY2 = 4 * resolution;
  paddle_top->bump_Xscie = 32 * resolution - 5;
  paddle_top->bump_Yscie = 24 * resolution;
  paddle_top->bump_xdeca = 0;
  paddle_top->bump_ydeca = 10 * resolution;
  paddle_top->rebonds_Ga = midi3_left;
  paddle_top->rebonds_Dr = midi3Right;
  paddle_top->direct_tab = ballePets3;
  paddle_top->width_mini = width_mini;
  paddle_top->width_maxi = width_maxi;
  current_player->set_paddle_alive_counter (3, paddle_top->bump_actif);

  /* initialize left paddle */ 
  paddle_left->set_coordinates (left_x_coord, center);
  paddle_left->collision_height = paddle_length;
  paddle_left->paddle_number = LEFT_PADDLE;
  paddle_left->is_vertical = true;
  paddle_left->bumpActive (is_team_mode, paddle_length, current_player->get_paddle_alive_counter (LEFT_PADDLE));
  paddle_left->bump_TFIRE = 2;
  paddle_left->bumper_FX0 = 5 * resolution;
  paddle_left->bumper_FY0 = 0 * resolution;
  paddle_left->bumper_FX1 = 4 * resolution;
  paddle_left->bumper_FY1 = 1 * resolution;
  paddle_left->bumper_FX2 = 4 * resolution;
  paddle_left->bumper_FY2 = -1 * resolution;
  paddle_left->bump_Xscie = 24 * resolution;
  paddle_left->bump_Yscie = 32 * resolution - 5;
  paddle_left->bump_xdeca = 10 * resolution;
  paddle_left->bump_ydeca = 0;
  paddle_left->rebonds_Ga = midi4_left;
  paddle_left->rebonds_Dr = midi4Right;
  paddle_left->direct_tab = ballePets4;
  paddle_left->width_mini = width_mini;
  paddle_left->width_maxi = width_maxi;
  current_player->set_paddle_alive_counter (4, paddle_left->bump_actif);

  /* initialize robot paddle */
  paddle_robot->set_coordinates (center, bottom_y_coord);
  /* paddle width always 128 pixels */
  paddle_robot->collision_width = width_maxi;
  paddle_robot->paddle_number = ROBOT_PADDLE;
  paddle_robot->is_vertical = false;
  paddle_robot->bump_TFIRE = 2;
  paddle_robot->bumper_FX0 = 0;
  paddle_robot->bumper_FY0 = -5 * resolution;
  paddle_robot->bumper_FX1 = -1 * resolution;
  paddle_robot->bumper_FY1 = -4 * resolution;
  paddle_robot->bumper_FX2 = 1 * resolution;
  paddle_robot->bumper_FY2 = -4 * resolution;
  paddle_robot->bump_Xscie = 32 * resolution;
  paddle_robot->bump_Yscie = -20 * resolution;
  paddle_robot->bump_xdeca = 0;
  paddle_robot->bump_ydeca = -10 * resolution;
  paddle_robot->rebonds_Ga = midi1_left;  // rebonds raquette va a gauche
  paddle_robot->rebonds_Dr = midi1Right;  // rebonds raquette va a droite
  paddle_robot->direct_tab = ballePets1;  // table direction balle collee
}

/**
 * Fire the projectiles
 */
void
controller_paddles::fire_projectiles ()
{

  /* mode  solo */
  if (!is_team_mode)
    {
      if (keyboard->is_left_button ())
        {
          paddle_bottom->fire_projectiles ();
          paddle_right->fire_projectiles ();
          paddle_top->fire_projectiles ();
          paddle_left->fire_projectiles ();
        }
    }
  else
    /* mode team no implemented */
    {
    }
  paddle_bottom->move_projectiles ();
  paddle_right->move_projectiles ();
  paddle_top->move_projectiles ();
  paddle_left->move_projectiles ();
}

/**
 * Check if the player release all glued balls in the bricks levels
 */
void
controller_paddles::check_if_release_balls ()
{
  /*
   * mode solo
   */
  if (!is_team_mode)
    {
      if (keyboard->is_right_button () ||
	  keyboard->key_is_pressed (SDLK_SPACE))
        {
          tiles_background *tiles = tiles_background::get_instance ();
          tiles->set_scroll_type(tiles_background::TILES_NO_SCROLL);
          paddle_bottom->release_ball ();
          paddle_right->release_ball ();
          paddle_top->release_ball ();
          paddle_left->release_ball ();
#ifndef SOUNDISOFF
          audio->stop_lost_music ();
#endif
        }
    }
  /*
   * mode team not implemented
   */
  /* else
     {

     } */
}

/**
 * Check if the player release all glued balls in the guards levels
 */
void
controller_paddles::check_if_release_ball ()
{
  if (keyboard->is_right_button ())
    {
      paddle_bottom->release_ball ();
    }
}

/**
 * Release all balls in bricks levels
 */
void
controller_paddles::release_all_balls ()
{
  paddle_bottom->release_ball ();
  paddle_right->release_ball ();
  paddle_top->release_ball ();
  paddle_left->release_ball ();
}

/** 
 * Control the movements of paddle(s) in the bricks levels
 */
void
controller_paddles::move_paddles ()
{
  Sint32 speed = 0;
  const Sint32 **tabB1, **tabB2, **tabB3, **tabB4;
  Sint32 x = paddle_bottom->x_coord;
  Sint32 off_x = keyboard->get_mouse_x_offset ();
  
  if (keyboard->key_is_released(SDLK_LEFT))
    {
      off_x+= 6; 
    }
  if (keyboard->key_is_released(SDLK_RIGHT))
    {
      off_x-= 6; 
    }


  // Mode Solo
  if (!is_team_mode)
    {
      raketDepla = 0;           // pas de deplacement
      rakVgauche = 0;
      rakVdroite = 0;

      /* if 2 mouse buttons are pressed or GigaBlitz run also no test */
      if (!keyboard->is_right_left_buttons () && !gigablitz->is_enable ())
        {
          if (reverse_counter > 0)
            {
              off_x = -off_x;
            }

          x += off_x;
          if (off_x < 0)
            {
              if (x <= min_coordinate)
                x = min_coordinate;
              raketDepla = 1;   // deplacement a gauche 
              rakVgauche = -off_x;
              speed = rakVgauche;
              tabB1 = paddle_bottom->rebonds_Ga;
              tabB2 = paddle_right->rebonds_Ga;
              tabB3 = paddle_top->rebonds_Ga;
              tabB4 = paddle_left->rebonds_Ga;
            }
          else
            {
              Sint32 i = max_coordinate - paddle_length;
              if (x >= i)
                x = i;
              raketDepla = 2;   // deplacement a droite
              rakVdroite = off_x;
              speed = rakVdroite;
              tabB1 = paddle_bottom->rebonds_Dr;
              tabB2 = paddle_right->rebonds_Dr;
              tabB3 = paddle_top->rebonds_Dr;
              tabB4 = paddle_left->rebonds_Dr;
            }

          // selectionne table de rebond balle suivant le deplacement
          if (speed > 10)
            speed = 10;
          paddle_bottom->rebonds_GD = *(tabB1 + speed);
          paddle_right->rebonds_GD = *(tabB2 + speed);
          paddle_top->rebonds_GD = *(tabB3 + speed);
          paddle_left->rebonds_GD = *(tabB4 + speed);

          // Change position des raquettes
          paddle_bottom->set_x_coord (x);  // raquette du bas
          paddle_right->set_y_coord (x - 16);     // raquette de droite
          paddle_top->set_x_coord (x);  // raquette du haut
          paddle_left->set_y_coord (x - 16);     // raquette de gauche
        }

    }

  // Mode Team (On Amiga I had two mice simultaneously)
  else
    {

    }
}

/** 
 * Control the movements of paddle in the guardians levels
 */
void
controller_paddles::move_paddle ()
{
  Sint32 speed = 0;
  const Sint32 **tabB1;
  Sint32 x = paddle_bottom->x_coord;
  Sint32 off_x = keyboard->get_mouse_x_offset ();
  // mode solo
  if (!is_team_mode)
    {
      raketDepla = 0;           //no move
      rakVgauche = 0;
      rakVdroite = 0;
      if (reverse_counter > 0)
        off_x = -off_x;
      x += off_x;
      if (off_x < 0)
        {
          if (x <= min_coordinate)
	    {
              x = min_coordinate;
	    }
          raketDepla = 1;       //moving on the left
          rakVgauche = -off_x;
          speed = rakVgauche;
          tabB1 = paddle_bottom->rebonds_Ga;
        }
      else
        {
          Sint32 i = max_coordinate - paddle_length;
          if (x >= i)
            x = i;
          raketDepla = 2;       //moving on the right
          rakVdroite = off_x;
          speed = rakVdroite;
          tabB1 = paddle_bottom->rebonds_Dr;
        }

      //select table of rebound ball according to bumper's moving 
      if (speed > 10)
        speed = 10;
      paddle_bottom->rebonds_GD = *(tabB1 + speed);

      //change position of bumpers
      paddle_bottom->set_x_coord (x);      //bottom bumper
      paddle_bottom->flickerRun ();        //flick the bumper
    }

  // mode team, no implemented (on Amiga I had two mice simultaneously)
  else
    {

    }
}

//------------------------------------------------------------------------------
// move the bottom robot bumper
//------------------------------------------------------------------------------
void
controller_paddles::move_robot ()
{
  if (paddle_robot->bump_actif)
    {
      Sint32 t = balls->get_max_of_sprites ();
      sprite_ball **aList = balls->get_sprites_list ();
      Sint32 pos_y = 0;
      sprite_ball *balle = 0x0;
      for (Sint32 i = 0; i < t; i++)
        {
          sprite_ball *b = *(aList++);
          if (b->is_enabled)
            {
              if (b->direction >= 36 && b->direction <= 60)
                {
                  if (b->y_coord > pos_y)
                    {
                      pos_y = b->y_coord;
                      balle = b;
                    }
                }
            }
        }

      if (pos_y > 0)
        {
          Sint32 bumpx = paddle_robot->x_coord;
          Sint32 ballx = balle->x_coord - 64;
          Sint32 offsx = bumpx - ballx;
          if (offsx > 10)
            {
              offsx = 10;
            }
          if (offsx < -10)
            {
            offsx = -10;
            }
          bumpx = bumpx - offsx;

          if (bumpx < 64)
            {
              bumpx = 64;
            }
          if (bumpx > 320)
            {
              bumpx = 320;
            }

          offsx = paddle_robot->x_coord;
          paddle_robot->x_coord = bumpx;
          offsx = bumpx - offsx;
          const Sint32 **tabB1;
          if (offsx < 0)
            {
              offsx = -offsx;
              tabB1 = paddle_robot->rebonds_Ga;
            }
          else
            {
              tabB1 = paddle_robot->rebonds_Dr;
            }
          paddle_robot->rebonds_GD = *(tabB1 + offsx);
        }
    }
}

/**
 * Return a pointer to a paddle sprite from a paddle identifier number
 * @param id identifier number of the paddle
 * @return pointer to a paddle sprite
 */
sprite_paddle *
controller_paddles::get_paddle (Uint32 id)
{
  switch (id)
    {
    case BOTTOM_PADDLE:
      return paddle_bottom;
    case RIGHT_PADDLE:
      return paddle_right;
    case TOP_PADDLE: 
      return paddle_top;
    case LEFT_PADDLE:
      return paddle_left;
    case ROBOT_PADDLE:
      return paddle_robot;
    }
  return NULL;
}

/**
 * Set the maximum paddles size in the bricks levels
 */
void
controller_paddles::set_maximum_paddles_size ()
{
  if (paddle_length >= 64 * resolution)
    {
      return;
    }
  paddle_length = 64 * resolution;
  Sint32 x = paddle_bottom->get_x_coord ();
  Sint32 i = max_coordinate - paddle_length;
  if (x >= i)
    {
      x = i;
      paddle_bottom->set_x_coord (x);
      paddle_right->set_y_coord (x);
      paddle_top->set_x_coord (x);
      paddle_left->set_y_coord (x);
    }
  paddle_bottom->set_width (paddle_length);
  paddle_right->set_height (paddle_length);
  paddle_top->set_width (paddle_length);
  paddle_left->set_height (paddle_length);
  current_player->set_paddle_length (paddle_length);
}

/**
 * Expand the size of the paddle(s) in the bricks levels
 */
void
controller_paddles::expand_paddles ()
{
  if (paddle_length >= (64 * resolution))
    {
      return;
    }
  paddle_length += (8 * resolution);
  Sint32 x = paddle_bottom->get_x_coord ();
  Sint32 i = max_coordinate - paddle_length;
  if (x >= i)
    {
      x = i;
      paddle_bottom->set_x_coord (x);
      paddle_right->set_y_coord (x);
      paddle_top->set_x_coord (x);
      paddle_left->set_y_coord (x);
    }
  paddle_bottom->set_width (paddle_length);
  paddle_right->set_height (paddle_length);
  paddle_top->set_width (paddle_length);
  paddle_left->set_height (paddle_length);
  current_player->set_paddle_length (paddle_length);
}

/**
 * Shrink the size of the paddle(s) in the bricks levels
 */
void
controller_paddles::shrink_paddles ()
{
  if (paddle_length <= (16 * resolution))
    {
      return;
    }
  paddle_length -= (8 * resolution);
  paddle_bottom->set_width (paddle_length);
  paddle_right->set_height (paddle_length);
  paddle_top->set_width (paddle_length);
  paddle_left->set_height (paddle_length);
  current_player->set_paddle_length (paddle_length);
}

/** 
 * Enable reverse movements of the paddle
 * @param counter value of the reverse
 */
void
controller_paddles::set_reverse_counter (Uint32 counter)
{
  reverse_counter = counter;
}

/**
 * Return counter value of the reverse paddle
 * @return counter value of the reverse
 */
Uint32
controller_paddles::get_reverse_counter ()
{
  return reverse_counter;
}

/**
 * Disable all paddles during the game over
 */
void
controller_paddles::disable_all_paddles ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_paddle * paddle= sprites_list[i];
      paddle->disable ();
    }
}

Sint32
  controller_paddles::ballePets1[14] = { 4, 8, 12, 16, 20, 24, 28,
  28, 24, 20, 16, 12, 8, 4
};
Sint32
  controller_paddles::ballePets2[14] = { 20, 24, 28, 32, 36, 40, 44,
  44, 40, 36, 32, 28, 24, 20
};
Sint32
  controller_paddles::ballePets3[14] = { 60, 56, 52, 48, 44, 40, 36,
  36, 40, 44, 48, 52, 56, 60
};
Sint32
  controller_paddles::ballePets4[14] = { 12, 8, 4, 0, 60, 56, 52,
  52, 56, 60, 0, 4, 8, 12
};

//      table de rebonds de la balle raquette du bas 
const Sint32
controller_paddles::rb09[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 32, 32, 32, 32, 20, 24, 28 };
const Sint32
controller_paddles::rb10[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 32, 32, 32, 28, 20, 20, 16 };
const Sint32
controller_paddles::rb11[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 28, 28, 24, 16, 16, 12 };
const Sint32
controller_paddles::rb12[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 28, 24, 20, 16, 12, 8 };
const Sint32
controller_paddles::rb13[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 28, 24, 20, 16, 12, 8, 4 };
const Sint32
controller_paddles::rb14[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 20, 20, 20, 12, 8, 4, 4 };
const Sint32
controller_paddles::rb15[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 16, 16, 16, 8, 8, 4, 0 };
const Sint32
controller_paddles::rb16[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 8, 12, 12, 4, 4, 0, 0 };
const Sint32
controller_paddles::rb17[16] =
  { 4, 4, 8, 12, 16, 20, 24, 28, 28, 4, 8, 12, 0, 0, 0, 0 };
const Sint32 *
controller_paddles::midi1_left[] =
  { rb13, rb14, rb14, rb15, rb15, rb16, rb16, rb16, rb16, rb16, rb17 };
const Sint32 *
controller_paddles::midi1Right[] =
  { rb13, rb12, rb12, rb11, rb11, rb10, rb10, rb10, rb10, rb10, rb09 };

// table de rebonds de la balle raquette de droite 
const Sint32
  controller_paddles::rb18[] =
  { 32 + 16, 20 + 16, 24 + 16, 28 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 32 + 16, 32 + 16, 32 + 16
};
const Sint32
  controller_paddles::rb19[] =
  { 28 + 16, 20 + 16, 20 + 16, 16 + 16, 4 + 16, 04 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 32 + 16, 32 + 16, 32 + 16
};
const Sint32
  controller_paddles::rb20[] =
  { 24 + 16, 16 + 16, 16 + 16, 12 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16
};
const Sint32
  controller_paddles::rb21[] =
  { 20 + 16, 16 + 16, 12 + 16, 8 + 16, 4 + 16, 04 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 28 + 16, 24 + 16
};
const Sint32
  controller_paddles::rb22[] =
  { 16 + 16, 12 + 16, 8 + 16, 4 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 28 + 16, 24 + 16, 20 + 16
};
const Sint32
  controller_paddles::rb23[] =
  { 12 + 16, 8 + 16, 4 + 16, 4 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 20 + 16, 20 + 16, 20 + 16
};
const Sint32
  controller_paddles::rb24[] =
  { 8 + 16, 8 + 16, 4 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 16 + 16, 16 + 16, 16 + 16
};
const Sint32
  controller_paddles::rb25[] =
  { 4 + 16, 4 + 16, 0 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 8 + 16, 12 + 16, 12 + 16
};
const Sint32
  controller_paddles::rb26[] =
  { 0 + 16, 0 + 16, 0 + 16, 0 + 16, 4 + 16, 4 + 16, 8 + 16, 12 + 16,
  16 + 16, 20 + 16, 24 + 16, 28 + 16, 28 + 16, 4 + 16, 8 + 16, 12 + 16
};
const Sint32 *
controller_paddles::midi2_left[] =
  { rb22, rb21, rb21, rb20, rb20, rb19, rb19, rb19, rb19, rb19, rb18 };
const Sint32 *
controller_paddles::midi2Right[] =
  { rb22, rb23, rb23, rb24, rb24, rb25, rb25, rb25, rb25, rb25, rb26 };


// table de rebonds de la balle raquette du haut 
const Sint32
  controller_paddles::rb27[] = { 60, 0, 0, 0, 0, 52, 56, 60,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb28[] = { 60, 0, 0, 0, 60, 52, 52, 48,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb29[] = { 60, 60, 60, 60, 56, 48, 48, 44,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb30[] = { 60, 60, 60, 56, 52, 48, 48, 40,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb31[] = { 60, 60, 56, 52, 48, 44, 40, 36,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb32[] = { 60, 52, 52, 48, 44, 40, 36, 36,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb33[] = { 60, 48, 48, 16 + 32, 40, 40, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb34[] = { 60, 40, 44, 12 + 32, 36, 36, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32
  controller_paddles::rb35[] = { 60, 36, 40, 12 + 32, 32, 32, 32, 32,
  4 + 32, 4 + 32, 8 + 32, 12 + 32, 16 + 32, 20 + 32, 24 + 32, 28 + 32
};
const Sint32 *
controller_paddles::midi3_left[] =
  { rb31, rb30, rb30, rb29, rb29, rb28, rb28, rb28, rb28, rb28, rb27 };
const Sint32 *
controller_paddles::midi3Right[] =
  { rb31, rb32, rb32, rb33, rb33, rb34, rb34, rb34, rb34, rb34, rb35 };


// table de rebonds de la balle raquette de gauche 
const Sint32
  controller_paddles::rb36[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 32 - 16, 32 - 16, 32 - 16,
  32 - 16, 20 - 16, 24 - 16, 28 - 16, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb37[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 32 - 16, 32 - 16, 32 - 16,
  28 - 16, 20 - 16, 20 - 16, 16 - 16, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb38[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16,
  24 - 16, 16 - 16, 16 - 16, 12 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb39[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 28 - 16, 24 - 16,
  20 - 16, 16 - 16, 12 + 48, 8 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb40[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 28 - 16, 24 - 16, 20 - 16,
  16 - 16, 12 + 48, 8 + 48, 4 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb41[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 20 - 16, 20 - 16, 20 - 16,
  12 + 48, 8 + 48, 4 + 48, 4 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb42[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 16 - 16, 16 - 16, 16 - 16,
  8 + 48, 8 + 48, 4 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb43[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 8 + 48, 12 + 48, 12 + 48,
  4 + 48, 4 + 48, 0 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32
  controller_paddles::rb44[] =
  { 16 - 16, 20 - 16, 24 - 16, 28 - 16, 28 - 16, 4 + 48, 8 + 48, 12 + 48,
  0 + 48, 0 + 48, 0 + 48, 0 + 48, 4 + 48, 4 + 48, 8 + 48, 12 + 48
};
const Sint32 *
controller_paddles::midi4_left[] =
  { rb40, rb41, rb41, rb42, rb42, rb43, rb43, rb43, rb43, rb43, rb44 };
const Sint32 *
controller_paddles::midi4Right[] =
  { rb40, rb39, rb39, rb38, rb38, rb37, rb37, rb37, rb37, rb37, rb36 };
