/** 
 * @file supervisor_bricks_level.cc 
 * @brief Bricks levels supervisor 
 * @date 2007-02-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.28 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.cc,v 1.28 2007/02/18 12:47:23 gurumeditation Exp $
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
  sides_bricks = new controller_sides_bricks ();
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
  ptMiniMess = new zeMiniMess ();
  balls =
    new controller_balls (ejectors_corners, sides_bricks, bottom_wall, ptMiniMess);
  viewfinders_paddles = controller_viewfinders::get_instance ();
  paddles = controller_paddles::get_instance ();
  gere_texte = controller_fontes_game::get_instance ();
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
  delete gere_texte;
  delete paddles;
  delete viewfinders_paddles;
  delete balls;
  delete ptMiniMess;
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
  Sint32 build = current_player->getRebuild ();
  current_player->setRebuild (0);
  error_init (sides_bricks->initialise (build));
  if (erreur_num)
    return erreur_num;
  ejectors_corners->create_ejectors_sprites ();
  balls->create_sprites_list ();
  ships->create_sprites_list ();
  magnetic_eyes->create_eyes_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list (6);
  gem_stones->create_sprites_list ();
  gere_texte->create_sprites_list ();
  paddles->create_projectiles_list ();
  player_indicators->create_indicators_sprites (paddles, money_capsules->get_first_sprite (), power_up_capsules->get_first_sprite (), NULL);
  game_over->create_sprites_list ();
  viewfinders_paddles->create_sprites_list ();
  popup_menu->first_init (sprites_bitmap, 0, 256 * resolution);
  resources->release_sprites_bitmap ();
  display->lock_surfaces ();

  //###################################################################
  // initialize "Game Over"
  //###################################################################
  error_init (game_over->first_init ());
  if (erreur_num)
    return (erreur_num);

  head_anim->load_bitmap ();

  init_level ();


  panel_score->first_init (balls);

  background ();
  paddles->init_paddles (gigablitz, balls);

  /* balls initialization */
  balls->init (
      /* time before the ball leaves paddle (at the game beginning) */
      levelParam->startCount,
      //time before the ball leaves (glue option)
      levelParam->glue_count / difficulty_level,
      //time before the ball accelerates
      levelParam->speedCount / difficulty_level,
      //time before "tilt" is available
      levelParam->tilt_count, levelParam->speedBall1);

  ships->initialise (levelParam->apparition / difficulty_level,
                          levelParam->atom1Count / difficulty_level,
                          levelParam->atom2Count / difficulty_level,
                          levelParam->atom3Count / difficulty_level,
                          levelParam->atom4Count / difficulty_level,
                          levelParam->resistance * difficulty_level);

  money_capsules->initialize (levelParam->monayCount * difficulty_level,
                          panel_score, player_indicators);

  //##############################################################
  //Initialize the object which handles gadgets (bonus and malus)
  //##############################################################
  Sint32 *cours = current_player->get_shopping_cart ();
  Sint32 counb = current_player->get_cou_nb ();
  Sint32 brCnt = bricks->get_num_of_bricks ();
  power_up_capsules->initialise (
                           //frequency of appearance of malus 
                           levelParam->malusCount * difficulty_level,
                           //number of bonus bought in the shop
                           counb,
                           //number of bricks which it is necessary to break
                           brCnt,
                           //the list of malus
                           levelParam->malusListe,
                           //the list of bonus bought in the shop
                           cours,
                           //the object which displays the small messages
                           ptMiniMess,
                           //the object which handles the bumpers
                           paddles,
                           //the object which handles the balls
                           balls,
                           //the object which handles the text on left scores panel
                           panel_score, bottom_wall, magnetic_eyes);

  gem_stones->initialize (panel_score, player_indicators, paddles);

  //##############################################################
  // initialize mobiles characters ("LEVEL x COMPLETED")
  //##############################################################
  gere_texte->initialise (level_number);

  //player_indicators->initialise (paddles, money_indicator, ptBobRever);


  viewfinders_paddles->initialize (paddles, 4);

  display->unlock_surfaces ();
  /* copy the background offscreen to the game offscreen */
  background_screen->blit_to_surface (game_screen);

  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);
  ptMiniMess->mesrequest (1);
  return erreur_num;
}

//-------------------------------------------------------------------------------
// bricks levels : reads the parameters of the current level
//-------------------------------------------------------------------------------
void
supervisor_bricks_level::init_level ()
{
  levelParam = ptLev_data->bricklevel (area_number, level_number);
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
          power_up_capsules->disable_sprites ();
          money_capsules->disable_sprites ();
          balls->disable_sprites ();
          sprite_projectile::disable_sprites ();
          ptMiniMess->erase_mess ();
          isgameover++;
        }
      ptMiniMess->execution1 ();
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
      player_indicators->execution1 (current_player->amount_of_money);
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
          ptMiniMess->execution1 ();
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
          power_up_capsules->bouge_gads ();    //move bonuses and maluses
          power_up_capsules->gadgetKeys ();
          gem_stones->move ();
          gere_texte->goMoveText ();
          if (bottom_wall->thecounter < 1)
            bottom_wall->disable ();
          else
            bottom_wall->thecounter--;

          panel_score->draw_gigablizt_gauge ();
          player_indicators->execution1 (current_player->amount_of_money);
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
              gere_texte->activeText ();
              sprite_projectile::disable_sprites ();
              balls->disable_sprites ();
#ifndef SOUNDISOFF
              audio->play_win_music ();
#endif
              ptMiniMess->mesrequest (17);
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
    end_return = -1;
  if (keyboard->command_is_pressed (handler_keyboard::TOOVERFLAG) ||
      Ecode == handler_popup_menu::GOGAMEOVER)
    current_player->remove_all_lifes ();
  if (keyboard->command_is_pressed (handler_keyboard::TOMENUFLAG) ||
      Ecode == handler_popup_menu::EXITTOMENU)
    end_return = MAIN_MENU;

  //###################################################################
  // control position music's module
  //###################################################################
#ifndef SOUNDISOFF
  Uint32 phase = audio->get_portion_music_played ();
  if (phase == handler_audio::LOST_PORTION &&
      phase != audio->get_portion_music_played ())
    {
      ptMiniMess->mesrequest (2);
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

  //###################################################################
  // initialize the mini messages
  //###################################################################
  error_init (ptMiniMess->intialise1 ());
  if (erreur_num)
    return erreur_num;

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
