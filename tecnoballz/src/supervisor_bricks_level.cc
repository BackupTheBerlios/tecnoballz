/** 
 * @file supervisor_bricks_level.cc 
 * @brief Bricks levels supervisor 
 * @date 2007-03-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.38 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.cc,v 1.38 2007/03/09 17:18:34 gurumeditation Exp $
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
#include "../include/supervisor_bricks_level.h"
#include "../include/handler_resources.h"

/** 
 * Create the bricks level supervisor
 */
supervisor_bricks_level::supervisor_bricks_level ()
{
  initialise ();
  sides_bricks = controller_sides_bricks::get_instance ();
  tiles_ground = new tiles_background ();
  panel_score = right_panel_score::get_instance ();
  ejectors_corners = controller_ejectors::get_instance ();
  money_capsules = controller_moneys::get_instance ();
  power_up_capsules = controller_capsules::get_instance ();
  gem_stones = controller_gems::get_instance ();
  bricks = controller_bricks::get_instance ();
  head_anim = head_animation::get_instance ();
  ships = controller_ships::get_instance ();
  magnetic_eyes = controller_magnetic_eyes::get_instance ();
  bottom_wall = new sprite_object ();
  info_messages = short_info_messages::get_instance ();
  balls = new controller_balls (bottom_wall);
  viewfinders_paddles = controller_viewfinders::get_instance ();
  paddles = controller_paddles::get_instance ();
  fontes_game = controller_fontes_game::get_instance ();
  gigablitz = controller_gigablitz::get_instance ();
  player_indicators = controller_indicators::get_instance ();
  game_over = controller_game_over::get_instance ();

  popup_menu = new handler_popup_menu ();

  sprite_projectile::start_list (bricks, ships);
  level_number = 1;
  area_number = 1;
  next_level = 0;
  isgameover = 0;
  devel_keyw = 0;
  devel_keyx = 0;
}

/** 
 * Release the bricks level supervisor
 */
supervisor_bricks_level::~supervisor_bricks_level ()
{
  delete popup_menu;
  delete game_over;
  delete player_indicators;
  delete gigablitz;
  delete fontes_game;
  delete paddles;
  delete viewfinders_paddles;
  delete balls;
  delete info_messages;
  delete bottom_wall;
  delete magnetic_eyes;
  delete ships;
  delete head_anim;
  delete bricks;
  delete gem_stones;
  delete power_up_capsules;
  delete money_capsules;
  delete ejectors_corners;
  delete panel_score;
  delete tiles_ground;
  delete sides_bricks;
  liberation ();
}

/** 
 * Initialization of a bricks level
 */
Sint32
supervisor_bricks_level::first_init ()
{
  sprites->reset ();
  area_number = current_player->get_area_number ();
  level_number = current_player->get_level_number ();
#ifndef SOUNDISOFF
  audio->play_level_music (area_number, level_number);
  audio->enable_sound ();
#endif

  count_next = 0;
  end_return = 0;
  isgameover = 0;
  if (is_verbose)
    {
      std::cout << "supervisor_bricks_level::first_init() area_number:"
        << area_number << "level_number:" << level_number
        << " difficulty_level:" << difficulty_level << std::endl;
    }

  /* generation of paddles graphics shapes tables */
  paddles->create_paddles_sprites ();

  /* generation of gigablitz graphics shapes tables */
  gigablitz->create_gigablitz_sprites (paddles, head_anim, bricks);

  /* load bitmap of sprites in memory (all other sprites) */
  resources->load_sprites_bitmap ();

  /*
   * generate the data of the spites
   */
  //wall of bottom 
  bottom_wall->create_sprite (BOB_WALLBO, sprites_bitmap, 0);
  sprites->add (bottom_wall);
  bottom_wall->set_coordinates (32 * resolution, 232 * resolution);
  //robot bumper
  paddles->init_robot ();
  bool is_rebuild = current_player->is_rebuild_walls ();
  current_player->set_rebuild_walls (false);
  error_init (sides_bricks->initialise (is_rebuild));
  if (erreur_num)
    return erreur_num;
  ejectors_corners->create_ejectors_sprites ();
  game_over->create_sprites_list ();
  balls->create_sprites_list ();
  ships->create_sprites_list ();
  magnetic_eyes->create_eyes_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list (6);
  gem_stones->create_sprites_list ();
  fontes_game->create_sprites_list ();
  paddles->create_projectiles_list ();
  player_indicators->create_indicators_sprites ();
  viewfinders_paddles->create_sprites_list ();
  popup_menu->first_init (sprites_bitmap, 0, 256 * resolution);
  resources->release_sprites_bitmap ();
  display->lock_surfaces ();

  /* initialize controller of the big letters animated composing the word
   * "game over"  */
  game_over->first_init ();

  head_anim->load_bitmap ();

  init_level ();


  panel_score->first_init (balls);

  background ();
  paddles->init_paddles (gigablitz, balls);

  /* balls initialization */
  balls->init (
      /* time before the ball leaves paddle, at the level beginning */
      level_desc->ball_release_time,
      /* time before the ball leaves (glue option) */
      level_desc->glue_time / difficulty_level,
      /* time before the ball accelerates */
      level_desc->acceleration_delay / difficulty_level,
      /* time before "tilt" is available */
      level_desc->tilt_delay, level_desc->starting_speed);

  ships->initialise (level_desc->reappearance / difficulty_level,
                     level_desc->ship_appearance_delay1 / difficulty_level,
                     level_desc->ship_appearance_delay2 / difficulty_level,
                     level_desc->ship_appearance_delay3 / difficulty_level,
                     level_desc->ship_appearance_delay4 / difficulty_level,
                     level_desc->ships_strength * difficulty_level);

  money_capsules->initialize (level_desc->moneys_frequency * difficulty_level,
                          panel_score, player_indicators);

  //##############################################################
  //Initialize the object which handles gadgets (bonus and malus)
  //##############################################################
  power_up_capsules->initialize (
                           //frequency of appearance of malus 
                           level_desc->penalties_frequency * difficulty_level,
                           //the list of malus
                           level_desc->malusListe,
                           //the object which handles the balls
                           balls,
                           bottom_wall);

  gem_stones->initialize ();

  /* initialize sprite fonts "LEVEL x COMPLETED" */
  fontes_game->initialise (level_number);

  viewfinders_paddles->initialize ();

  display->unlock_surfaces ();
  /* copy the background offscreen to the game offscreen */
  background_screen->blit_to_surface (game_screen);

  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);
  info_messages->send_message_request (short_info_messages::ARE_YOU_READY);
  return erreur_num;
}

//-------------------------------------------------------------------------------
// bricks levels : reads the parameters of the current level
//-------------------------------------------------------------------------------
void
supervisor_bricks_level::init_level ()
{
  level_desc = ptLev_data->get_bricks_levels_params (area_number, level_number);
}

/**
 * main loop in the bricks level
 */
Sint32
supervisor_bricks_level::main_loop ()
{
  Sint32 Ecode = -1;

  /*
   * the player has no more lives: Game Over
   */
  if (current_player->get_num_of_lifes () <= 0)
    {
      if (!isgameover)
        {
#ifndef SOUNDISOFF
          audio->disable_sound ();
          audio->stop_music ();
#endif
          paddles->disable_all_paddles ();
          bricks->clr_bricks ();
          fontes_game->disable_sprites ();
          gem_stones->disable_sprites (); 
          power_up_capsules->disable_sprites ();
          money_capsules->disable_sprites ();
          balls->disable_sprites ();
          sprite_projectile::disable_sprites ();
          info_messages->clear_messages_request ();
          isgameover++;
        }
      info_messages->run ();
      display->wait_frame ();
      head_anim->play ();
      display->lock_surfaces ();
      gigablitz->execution1 ();
      sprites->clear ();
      if (!(random_counter & 0x00f))
        {
          head_anim->start_interference ();
        }
      if (isgameover >= 2)
        {
          isgameover++;
          game_over->execution1 ();
        }
      if (!bricks->update () && isgameover < 2) //restore bricks
        isgameover = 2;
      sides_bricks->execution1 ();
      viewfinders_paddles->run ();
      ships->move ();
      sprites->draw ();
      panel_score->text_refresh ();
      panel_score->draw_gigablizt_gauge ();
      player_indicators->display_money_and_reverse ();
      display->unlock_surfaces ();
      display->bufferCTab ();
      if (keyboard->is_left_button () && isgameover > 60)
        current_player = handler_players::nextplayer (current_player, &end_return, 1);
    }

  /*
   * game is running!
   */
  else
    {
      display->wait_frame ();
      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          head_anim->play ();
        }
      display->lock_surfaces ();
      sprites->clear ();
      bricks->update ();        //restore bricks
      sides_bricks->execution1 ();        //restore bricks on side
      changebkgd ();

      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          info_messages->run ();
          gigablitz->execution1 ();

          //handle the "less bricks" option
          bricks->less_bricks ();

          paddles->move_paddles ();
          if (panel_score->get_bricks_counter () > 0)
            {
              paddles->check_if_release_balls ();
              paddles->fire_projectiles ();
            }
          paddles->move_robot ();
          balls->run_in_bricks_levels ();
          viewfinders_paddles->run ();
          sprite_projectile::gestionTir ();
          ships->move ();
          magnetic_eyes->execution1 ();
          money_capsules->move ();
          power_up_capsules->move_in_bricks_level ();
          power_up_capsules->check_cheat_keys ();
          gem_stones->move ();
          fontes_game->goMoveText ();
          if (bottom_wall->thecounter < 1)
            bottom_wall->disable ();
          else
            bottom_wall->thecounter--;

          panel_score->draw_gigablizt_gauge ();
          player_indicators->display_money_and_reverse ();
        }

      //tiles_ground->draw();
      sprites->draw ();
      panel_score->text_refresh ();
      Ecode = popup_menu->execution1 ();
      display->unlock_surfaces ();
      display->bufferCTab ();

      //###################################################################
      // next level or next player
      //###################################################################
      if (panel_score->get_bricks_counter () == 0)
        {
          if (count_next > 0)
            {
              count_next++;
              bool music_finished = false;
              if (count_next > 350)
                {
                  music_finished = true;
#ifndef SOUNDISOFF
                  music_finished = audio->is_win_music_finished ();
#endif
                }
              if (count_next > 20000000 ||
                  keyboard->key_is_pressed (SDLK_SPACE) || music_finished)
                {
                  sides_bricks->sauve_etat ();
                  current_player = handler_players::nextplayer (current_player,
                                                       &end_return, 1);
#ifndef SOUNDISOFF
                  audio->stop_music ();
#endif
                }
              balls->disable_sprites ();
              sprite_projectile::disable_sprites ();
            }
          else
            {
              fontes_game->activeText ();
              sprite_projectile::disable_sprites ();
              balls->disable_sprites ();
#ifndef SOUNDISOFF
              audio->play_win_music ();
#endif
              info_messages->send_message_request (short_info_messages::NEXT_LEVEL);
#ifndef SOUNDISOFF
              audio->disable_sound ();
#endif
              count_next = 1;
            }
        }
    }

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

  //###################################################################
  // control position music's module
  //###################################################################
#ifndef SOUNDISOFF
  Uint32 phase = audio->get_portion_music_played ();
  if (phase == handler_audio::LOST_PORTION &&
      phase != audio->get_portion_music_played ())
    {
      info_messages->send_message_request (short_info_messages::YEAH_YOU_WHERE);
      paddles->release_all_balls ();
    }
#endif
  return end_return;
}

/**
 * Change the tiles background
 */
void
supervisor_bricks_level::changebkgd ()
{
#ifdef UNDER_DEVELOPMENT
  if (keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_LSHIFT) ||
      keyboard->key_is_pressed (SDLK_RCTRL) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
      !keyboard->key_is_pressed (SDLK_LALT))
    return;

  if (keyboard->key_is_pressed (SDLK_w))
    devel_keyw = 1;
  if (keyboard->key_is_pressed (SDLK_x))
    devel_keyx = 1;

  if ((keyboard->key_is_released (SDLK_w) && devel_keyw) ||
      (keyboard->key_is_released (SDLK_x) && devel_keyx))
    {
      game_screen->clear ();

      if (devel_keyw)
        {
          devel_keyw = 0;
          if (--indexbgrnd < 0)
            indexbgrnd = 49;
        }
      if (devel_keyx)
        {
          devel_keyx = 0;
          if (++indexbgrnd > 49)
            indexbgrnd = 0;
        }
      if (is_verbose)
        printf ("supervisor_bricks_level::background() : changebkgd:%i\n",
                indexbgrnd);
      background (indexbgrnd);
      background_screen->blit_to_surface (game_screen);
    }

  if (keyboard->key_is_pressed (SDLK_v))
    head_anim->start_yawn ();
  if (keyboard->key_is_pressed (SDLK_b))
    head_anim->start_yawn ();
  if (keyboard->key_is_pressed (SDLK_n))
    head_anim->start_interference ();
  if (keyboard->key_is_pressed (SDLK_g))
    gigablitz->initDepart ();
/* 

	if(keyboard->key_is_pressed(SDLK_w))
 { 
 }

 if(keyboard->key_is_pressed(SDLK_w))
 { keyw = 1;
 }


 if(keyboard->key_is_released(SDLK_w) && keyw == 1)
 { tiles_ground->prev_color();
 keyw = 0;
 }
 if(keyboard->key_is_released(SDLK_x) && keyx == 1)
 { tiles_ground->next_color();
 keyx = 0;
 }
*/


#endif
}

//-------------------------------------------------------------------------------
// initialize background
//-------------------------------------------------------------------------------
Sint32
supervisor_bricks_level::background (Sint32 nbkdg)
{
  if (nbkdg == -1)
    {
      nbkdg = ((area_number - 1) * 10) + level_number;
      if (level_number > 5)
        nbkdg--;
      if (is_verbose)
        printf ("supervisor_bricks_level::background() : nbkdg = %i\n",
                nbkdg);
    }

  /* Initialize and draw the tiles background */
  tiles_ground->setup (nbkdg);

  /* short info messages displayed */
  info_messages->intialize ();

  //###################################################################
  // display the ejectors and small bricks
  //###################################################################
  ejectors_corners->draw_shadow ();
  sides_bricks->sauveFond ();     //save background under small bricks
  sides_bricks->afficheSha ();    //display small bricks shadows
  sides_bricks->afficheGfx ();    //display small bricks of the three walls
  ejectors_corners->draw ();

  /* intialize the bricks level */
  bricks->first_init ();
  bricks->initialize ();
  return erreur_num;
}
