/** 
 * @file supervisor_guards_level.cc 
 * @brief Guardians level supervisor 
 * @created 2003-01-09
 * @date 2007-03-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.35 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_guards_level.cc,v 1.35 2007/04/03 10:15:25 gurumeditation Exp $
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
#include "../include/supervisor_guards_level.h"
#include "../include/handler_resources.h"

/**
 * Create the guards level supervisor
 */
supervisor_guards_level::supervisor_guards_level ()
{
  initialise ();
  tiles_map = new tilesmap_scrolling ();
  guards = controller_guardians::get_instance ();
  paddles = controller_paddles::get_instance ();
  fontes_game = controller_fontes_game::get_instance ();
  explosions = controller_explosions::get_instance ();
  bullets = controller_bullets::get_instance ();
  money_capsules = controller_moneys::get_instance ();
  power_up_capsules = controller_capsules::get_instance ();
  balls = new controller_balls ();
  viewfinders_paddles = controller_viewfinders::get_instance ();
  player_indicators = controller_indicators::get_instance ();
  gigablitz = controller_gigablitz::get_instance ();
  game_over = controller_game_over::get_instance ();
  ptCongBall = new controller_spheres ();
  popup_menu = new handler_popup_menu ();
  ptBob_name = NULL;
}

/**
 * Release the guards level supervisor
 */
supervisor_guards_level::~supervisor_guards_level ()
{
  if (ptBob_name)
    delete ptBob_name;
  delete popup_menu;
  delete ptCongBall;
  delete game_over;
  delete gigablitz;
  delete player_indicators;
  delete viewfinders_paddles;
  delete balls;
  delete power_up_capsules;
  delete money_capsules;
  delete bullets;
  delete explosions;
  delete fontes_game;
  delete paddles;
  delete guards;
  delete tiles_map;
  liberation ();
}

/** 
 * Initializations of the guardian's level
 */
Sint32
supervisor_guards_level::first_init ()
{
#ifndef SOUNDISOFF
  audio->play_music (MUSICGUARD);
  audio->enable_sound ();
#endif
  sprites->reset ();
  end_return = 0;
  gameover_counter = 0;
  count_next = 0;
  is_victory = false;
  area_number = current_player->get_area_number ();
  level_number = current_player->get_level_number ();
  Sint32 grdP = current_player->getGuardPt ();
  if (is_verbose)
  {
    std::cout << ">supervisor_guards_level::first_init() " <<
      "area_number: " << area_number << "; level_number: " <<
      level_number << "; grdP: " << grdP << std::endl;
  }

  /* gigablitz sprites are contained separately and in its own bitmap */
  gigablitz->create_gigablitz_sprites (paddles, explosions);

  /* 
   * sprites who are contained into the large bitmap
   */
  resources->load_sprites_bitmap ();
  bullets->create_sprites_list ();
  guards->create_guardians_list (bullets, grdP);
  paddles->create_paddles_sprites ();
  balls->create_sprites_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list (6);
  player_indicators->create_indicators_sprites ();
  explosions->create_explosions_list ();

  viewfinders_paddles->create_sprites_list ();

  //mobile characters at the end of the level
  fontes_game->create_sprites_list ();

  game_over->create_sprites_list ();

  ptCongBall->create_sprites_list ();

  // intialize escape menu
  popup_menu->first_init (sprites_bitmap, 0,    //menu number
                          320 * resolution,     //width of screen (center)
                          0,    //don't restaure background where leaves
                          1     //initialize color table
    );
  resources->release_sprites_bitmap ();

 /* initialize controller of the big letters animated composing the word
  * "game over"  */
  game_over->first_init (32 * resolution);
  init_level ();
  
  /* initialize background vertical scrolling */
  tiles_map->initialize ();
  display->lock_surfaces ();

  /*
   * initialize the balls controller *
   */
  balls->init (
    /* time delay before ball leaves paddle */
    level_desc->ball_release_time,
    0,
    /* time delay before ball accelerates */
    50 * 99,
   /* time delay before titl available */
   level_desc->tilt_delay,
   /* ball speed 3 or 4 */
   level_desc->starting_speed);
  /* in the guardians levels, the balls are always forced to power 2 */
  balls->set_power_2 ();

  money_capsules->initialize (3 + difficulty_level, player_indicators);

  /* initialize le capsules controller */
  power_up_capsules->initialize (
                          /* delay of appearance of a penalty capsule */
                          level_desc->capsules_frequency * difficulty_level,
                          /* list of penalties capsules */ 
                          level_desc->capsules_list,
                          /* object which control the balls */
                          balls,
                          /* object which handles the display of the text (not * applicable) */
                          NULL);

  //player_indicators->init_guard (paddles, money_indicator, ptBobLifes);

  //initialize mobile characters at the end of the level
  fontes_game->initialise (level_number, 32 * resolution);

  viewfinders_paddles->initialize ();

  display->unlock_surfaces ();



  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);

  sprite_display_scores *pOver = game_over->gtScorOver ();
  ptBob_name = pOver->string2bob (current_player->get_name ());
  sprites->add (ptBob_name);
  ptBob_name->enable ();
  ptBob_name->
    set_coordinates ((display->get_width () -
                      ptBob_name->get_sprite_width ()) / 2, resolution);
  ptBob_name->set_draw_method (sprite_object::COPY_FROM_BITMAP);

  if (is_verbose)
  {
    std::cout << ">supervisor_guards_level::first_init() End!" << std::endl;
  }
  return erreur_num;
}

/**
 * The main loop of the guardians phase
 */
Sint32
supervisor_guards_level::main_loop ()
{
  Sint32 Ecode = -1;
  
  /*
   * gameover : the player has no more lives
   */
  if (current_player->get_num_of_lifes () <= 0)
    {
      if (gameover_counter > 0)
        {
#ifndef SOUNDISOFF
          audio->disable_sound ();
#endif
          paddles->disable_all_paddles ();
          power_up_capsules->disable_sprites ();
          money_capsules->disable_sprites ();
          guards->disable_sprites ();
          explosions->disable_sprites ();
          fontes_game->disable_sprites ();
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (is_victory)
            {
              tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_CONGRATULATIONS, handler_resources::RESEDMAP02);
              ptCongBall->initialize ();        //congra
              scrolSpeed = 1;
              scrollTemp = 300;
            }
        }
      gameover_counter++;
      display->wait_frame ();

      if (scrollTemp > 0)
        {
          scrollTemp--;
          tiles_map->scroll (0);
        }
      else
        tiles_map->scroll (scrolSpeed);
      
      display->lock_surfaces ();

      if (gameover_counter >= 1)
        {
          game_over->execution1 (is_victory);
          if (is_victory)
            {
              ptCongBall->execution1 ();  //congra
            }
        }
      viewfinders_paddles->run ();
      player_indicators->display_money_and_lifes ();
      fontes_game->goMoveText ();
      sprites->draw ();
      display->unlock_surfaces ();
      display->bufferCTab ();
      if (keyboard->is_left_button () && gameover_counter > 150)
        {
          current_player = handler_players::nextplayer (current_player,
                                                    &end_return,
                                                    1,
                                                    guards->
                                                    get_max_of_sprites () +
                                                    1);
        }
    }
  else
    {
      /*
       * game running !
       */
      display->wait_frame ();
      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          run_scroll ();
          tiles_map->scroll (scrolSpeed);
          display->lock_surfaces ();
          paddles->move_paddle ();
          paddles->check_if_release_ball ();
          balls->run_in_guardians_level ();
          viewfinders_paddles->run ();
          /* moving guards, and fire bullets and gigablitz */
          guards->run ();
          bullets->move ();
          bullets->check_paddle_collisions ();
          money_capsules->move_bottom ();
          power_up_capsules->move_in_guardians_level ();
          fontes_game->goMoveText ();
          player_indicators->display_money_and_lifes ();
          /* move Gigablitz and check collision with paddle */
          gigablitz->run_in_guardians_level ();
          explosions->play_animation ();
          bullets->play_animation_loop ();

        }
      else
        {
          bullets->play_animation_loop ();
          tiles_map->scroll (0);
          display->lock_surfaces ();
        }

      //###################################################################
      // display all sprites in the buffer's memory
      //###################################################################
      sprites->draw ();
      Ecode = popup_menu->execution1 ();
      display->unlock_surfaces ();
      display->bufferCTab ();   //copy buffer's memory in the screen
    }


  //##############################################################
  // next levels
  //##############################################################
  if (guards->is_guardians_destroyed () && !is_victory)
    {
      if (count_next > 0)
        {
          count_next++;
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (count_next > 500 || keyboard->key_is_pressed (SDLK_SPACE))
            {
              is_victory = current_player->zlastlevel ();
              if (is_victory)
                {
                  current_player->remove_all_lifes ();
                  count_next = 0;
                }
              else
                {
                  current_player = handler_players::nextplayer
                    (current_player,
                     &end_return, 1, guards->get_max_of_sprites () + 1);
                }
            }
        }
      else
        {
          fontes_game->activeText ();
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          count_next = 1;
        }
    }

  //###################################################################
  // cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
  //###################################################################
  cheat_keys ();

  //###################################################################
  // escape key to quit the game !
  //###################################################################
  if (keyboard->command_is_pressed (handler_keyboard::TOEXITFLAG) ||
      Ecode == handler_popup_menu::WEQUITGAME)
    {
      end_return = LEAVE_TECNOBALLZ;
    }
  if (keyboard->command_is_pressed (handler_keyboard::TOOVERFLAG) ||
      Ecode == handler_popup_menu::GOGAMEOVER)
    {
      current_player->remove_all_lifes ();
    }
  if (keyboard->command_is_pressed (handler_keyboard::TOMENUFLAG) ||
      Ecode == handler_popup_menu::EXITTOMENU)
    {
      end_return = MAIN_MENU;
    }



  return end_return;
}

//------------------------------------------------------------------------------
// Calcul le pointeur sur le niveau de la diffculte des gardiens
//------------------------------------------------------------------------------
void
supervisor_guards_level::init_level ()
{
  level_desc = ptLev_data->get_guardians_levels_params (area_number, level_number);
  scrollType = level_desc->scroll_id;
  scrollTemp = level_desc->scroll_delay;
  scrolSpeed = 0;
}

//------------------------------------------------------------------------------
// manage scrolling speed
//------------------------------------------------------------------------------
void
supervisor_guards_level::run_scroll ()
{
  if (scrollTemp > 0)
    {
      scrollTemp--;
      scrolSpeed = 0;
      return;
    }
  sprite_bullet *bullet = bullets->get_last_bullet ();
  sprite_ball *ball = balls->first_ball ();
  scrolSpeed = guards->get_scrolling_speed (scrollType, scrolSpeed, ball, bullet);
}

//------------------------------------------------------------------------------
// cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
//------------------------------------------------------------------------------
void
supervisor_guards_level::cheat_keys ()
{
  if (!is_enabled_cheat_mode)
    {
      return;
    }
  if (!keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_LSHIFT) ||
      !keyboard->key_is_pressed (SDLK_RCTRL) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
      keyboard->key_is_pressed (SDLK_LALT))
    return;

  if (keyboard->key_is_pressed (SDLK_F2))
    guards->killguards ();
  if (keyboard->key_is_pressed (SDLK_F3))
    guards->killguards (1);
  if (keyboard->key_is_pressed (SDLK_F4))
    guards->killguards (2);
}
