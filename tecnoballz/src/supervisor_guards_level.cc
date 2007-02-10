/** 
 * @file supervisor_guards_level.cc 
 * @brief Guardians level supervisor 
 * @created 2003-01-09
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.15 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_guards_level.cc,v 1.15 2007/02/10 13:22:03 gurumeditation Exp $
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
  guards = new controller_guardians ();
  paddles = new controller_paddles (1);
  ptMoveText = new zeMoveText ();
  explosions = new controller_explosions ();
  sprite_paddle *paddle = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  bullets = new controller_bullets (paddle, explosions);
  money_capsules = new controller_moneys ();
  power_up_capsules = new controller_capsules (6);
  balls = new controller_balls (guards, money_capsules, power_up_capsules);
  viewfinders_paddles = new controller_viewfinders ();
  ptPrntmney = new printmoney ();
  ptMiniMess = new zeMiniMess ();
  gigablitz = new controller_gigablitz ();
  money_indicator = new sprite_object ();
  ptBobLifes = new sprite_capsule ();
  game_over = new controller_game_over ();
  ptCongBall = new zeCongBall ();
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
  delete ptBobLifes;
  delete money_indicator;
  delete gigablitz;
  delete ptMiniMess;
  delete ptPrntmney;
  delete viewfinders_paddles;
  delete balls;
  delete power_up_capsules;
  delete money_capsules;
  delete bullets;
  delete explosions;
  delete ptMoveText;
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
  tecnwinner = 0;
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
  guards->create_guardians_list (bullets, grdP, gigablitz, explosions);
  paddles->create_paddles_sprites ();
  balls->create_sprites_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list ();
  ptPrntmney->create_sprites_list ();
  explosions->create_explosions_list ();

  /* create the money sprite */
  money_indicator->create_sprite (BOB_MONEYS, sprites_bitmap, false);
  sprites->add (money_indicator);

  /* create the extra-life capsule sprite */
  ptBobLifes->create_sprite (BOB_GADGET, sprites_bitmap, false);
  sprites->add (ptBobLifes);

  viewfinders_paddles->create_sprites_list ();

  //mobile characters at the end of the level
  ptMoveText->create_sprites_list ();

  game_over->create_sprites_list ();

  ptCongBall->create_sprites_list ();

  // intialize escape menu
  popup_menu->first_init (sprites_bitmap, 0,    //menu number
                          320 * resolution,     //width of screen (center)
                          0,    //don't restaure background where leaves
                          1     //initialize color table
    );
  resources->release_sprites_bitmap ();

  //###################################################################
  // initialize "Game Over"
  //###################################################################
  error_init (game_over->first_init (32 * resolution));
  if (erreur_num)
    return (erreur_num);
  init_level ();
  display->lock_surfaces ();

  /* initialize background vertical scrolling */
  tiles_map->initialize ();

  //###################################################################
  // initialization balls
  //###################################################################
  balls->init_balle (paddles, levelParam->startCount,   //time before ball leaves bumper
                     0,         //(not applicable)
                     50 * 99,   //time before ball accelerates
                     levelParam->tilt_count,    //time before "tilt"  available
                     levelParam->speedBall1);   //speed ball (3 or 4)

  //###################################################################
  // force "powerball 2" (guards levels only)
  //###################################################################
  balls->run_power2 ();

  money_capsules->initialize (3 + hardChoice, ptPrntmney);

  /* initialize le capsules controller */
  power_up_capsules->initialise (
                          /* delay of appearance of a penalty capsule */
                          levelParam->malusCount * hardChoice,
                          /* number of bonus bought in the shop (not * applicable) */
                          0,
                          /* number of bricks which it is necessary to break * (not applicable) */
                          0,
                          /* list of penalties capsules */ 
                          levelParam->malusListe,
                          /* list of bonus bought in the shop (not applicable) * */
                          NULL,
                          /* object which displays the small messages (not * applicable) */
                          NULL,
                          /* object which control the paddles */
                          paddles,
                          /* object which control the balls */
                          balls,
                          /* object which handles the display of the text (not * applicable) */
                          NULL,
                          NULL, NULL);

  ptPrntmney->init_guard (current_player, paddles, money_indicator, ptBobLifes);

  //initialize mobile characters at the end of the level
  ptMoveText->initialise (level_number, 32 * resolution);

  viewfinders_paddles->initialize (paddles, 1);

  display->unlock_surfaces ();
  display->bufferCopy ();       //copy "buffer memory" to "screen memory"
  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);

  score_over *pOver = game_over->gtScorOver ();
  ptBob_name = pOver->string2bob (current_player->returnName ());
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
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (tecnwinner)
            {
              tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_CONGRATULATIONS, handler_resources::RESEDMAP02);
              ptCongBall->initialize ();        //congra
              scrolSpeed = 1;
              scrollTemp = 300;
            }
        }
      gameover_counter++;
      display->wait_frame ();
      display->lock_surfaces ();

      if (scrollTemp > 0)
        {
          scrollTemp--;
          tiles_map->scrolling1 (0);
        }
      else
        tiles_map->scrolling1 (scrolSpeed);

      if (gameover_counter >= 1)
        {
          game_over->execution1 (tecnwinner);
          if (tecnwinner)
            ptCongBall->execution1 ();  //congra
        }
      viewfinders_paddles->run ();
      ptPrntmney->execution2 (current_player->amount_of_money, current_player->number_of_lifes);
      ptMoveText->goMoveText ();
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
      //######################################################
      // game running !
      //######################################################
      display->wait_frame ();
      display->lock_surfaces ();
      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          run_scroll ();
          tiles_map->scrolling1 (scrolSpeed);
          paddles->move_paddle ();
          paddles->check_if_release_ball ();
          balls->vitusBall2 (); //moving ball(s)
          viewfinders_paddles->run ();
          /* moving guards, and fire bullets and gigablitz */
          guards->run ();
          bullets->execution1 ();    //moving the guards's weapons
          bullets->bumper_col ();    //collision weapons with the bumper
          money_capsules->move_bottom ();
          power_up_capsules->bougegads2 ();
          ptMoveText->goMoveText ();
          ptPrntmney->execution2 (current_player->amount_of_money,
                                  current_player->number_of_lifes);
          gigablitz->execution2 ();    //move the Gigablitz from guards
          explosions->play_animation ();
          bullets->anim_fires ();    //the animation of the guards's weapons

        }
      else
        {
          bullets->anim_fires ();
          tiles_map->scrolling1 (0);
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
  if (guards->is_guardians_destroyed () && !tecnwinner)
    {
      if (count_next > 0)
        {
          count_next++;
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (count_next > 500 || keyboard->key_is_pressed (SDLK_SPACE))
            {
              tecnwinner = current_player->zlastlevel ();
              if (tecnwinner)
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
          ptMoveText->activeText ();
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
    end_return = -1;
  if (keyboard->command_is_pressed (handler_keyboard::TOOVERFLAG) ||
      Ecode == handler_popup_menu::GOGAMEOVER)
    current_player->remove_all_lifes ();
  if (keyboard->command_is_pressed (handler_keyboard::TOMENUFLAG) ||
      Ecode == handler_popup_menu::EXITTOMENU)
    end_return = 4;



  return (end_return);
}

//------------------------------------------------------------------------------
// Calcul le pointeur sur le niveau de la diffculte des gardiens
//------------------------------------------------------------------------------
void
supervisor_guards_level::init_level ()
{
  levelParam = ptLev_data->guardlevel (area_number, level_number);
  scrollType = levelParam->scrollType;
  scrollTemp = levelParam->scrolCount;
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
  sprite_bullet *weapo = bullets->getWeapOne ();
  sprite_ball *balle = balls->first_ball ();
  scrolSpeed = guards->run_scroll (scrollType, scrolSpeed, balle, weapo);
}

//------------------------------------------------------------------------------
// cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
//------------------------------------------------------------------------------
void
supervisor_guards_level::cheat_keys ()
{
  if (!cheat_flag)
    return;
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
