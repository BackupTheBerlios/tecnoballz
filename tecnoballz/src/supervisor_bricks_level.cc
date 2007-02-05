/** 
 * @file supervisor_bricks_level.cc 
 * @brief Bricks levels supervisor 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.cc,v 1.3 2007/02/05 15:44:09 gurumeditation Exp $
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
  gereBricot = new briqueCote ();
  tiles_ground = new tiles_background ();
  tecZ_barre = new barreScore ();
  gereEjects = new ejectBalls ();
  gereCapsul = new zeCapsules ();
  gereGadget = new ze_gadgets (6);
  ptGemstone = new zeGemstone ();
  bricks = new controller_bricks ();
  tete_gugus = new head_anima ();
  les_atomes =
    new zeBouiBoui (gereCapsul, gereGadget, ptGemstone, bricks);
  pt_magneye = new ze_magneye ();
  BottomWall = new sprite_object ();
  ptMiniMess = new zeMiniMess ();
  gereBalles =
    new controller_balls (gereEjects, bricks, gereBricot, tete_gugus,
                          les_atomes, tecZ_barre, BottomWall, ptMiniMess,
                          pt_magneye);
  ptBaDirect = new ballDirect ();
  paddles = new controller_paddles ();
  gere_texte = new zeMoveText ();
  ptGigaBlit = new zeGigaBlit ();
  ptPrntmney = new printmoney ();
  ptGameOver = new zeGameOver ();

  ptBobMoney = new sprite_object ();
  ptBobRever = new tecno_gads ();
  popup_menu = new handler_popup_menu ();

  sprite_projectile::start_list (bricks, les_atomes, tecZ_barre);
  levelTecno = 1;
  areaNumber = 1;
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
  if (NULL != BottomWall)
    {
      BottomWall->release_sprite ();
      BottomWall = (sprite_object *) NULL;
    }
  delete popup_menu;
  delete ptBobRever;
  delete ptBobMoney;
  delete ptGameOver;
  delete ptPrntmney;
  delete ptGigaBlit;
  delete gere_texte;
  delete paddles;
  delete ptBaDirect;
  delete gereBalles;
  delete ptMiniMess;
  delete BottomWall;
  delete pt_magneye;
  delete les_atomes;
  delete tete_gugus;
  delete bricks;
  delete ptGemstone;
  delete gereGadget;
  delete gereCapsul;
  delete gereEjects;
  delete tecZ_barre;
  delete tiles_ground;
  delete gereBricot;
  liberation ();
}

/** 
 * Initialization of a bricks level
 */
Sint32
supervisor_bricks_level::first_init ()
{
  sprites->reset ();
  areaNumber = joueurGere->getAreaNum ();
  levelTecno = joueurGere->getLevelNu ();
  //levelTecno = 5; //test only
#ifndef SOUNDISOFF
  audio->play_level_music (areaNumber, levelTecno);
  audio->enable_sound ();
#endif

  count_next = 0;
  end_return = 0;
  isgameover = 0;
  if (is_verbose)
    {
      std::cout << "supervisor_bricks_level::first_init() areaNumber:"
        << areaNumber << "levelTecno:" << levelTecno
        << " hardChoice:" << hardChoice << std::endl;
    }

  /* generation of paddles graphics shapes tables */
  paddles->create_paddles_sprites ();

  /* generation of gigablitz graphics shapes tables */
  error_init (ptGigaBlit->init_liste (paddles, tete_gugus, bricks));
  if (erreur_num)
    return (erreur_num);

  /* load bitmap of sprites in memory (all other sprites) */
  resources->load_sprites_bitmap ();

  //###################################################################
  // generation of graphics shapes tables
  //###################################################################

  //wall of bottom 
  BottomWall->create_sprite (BOB_WALLBO, sprites_bitmap, 0);
  sprites->add (BottomWall);
  BottomWall->set_coordinates (32 * resolution, 232 * resolution);
  //robot bumper
  paddles->init_robot ();
  Sint32 build = joueurGere->getRebuild ();
  joueurGere->setRebuild (0);
  error_init (gereBricot->initialise (build));
  if (erreur_num)
    return erreur_num;
  //ejectors 
  gereEjects->initialise ();
  //all balls
  gereBalles->create_sprites_list ();
  //atoms (aka "bouisbouis")
  les_atomes->create_sprites_list ();
  //eye magneto 
  pt_magneye->create_eyes_list ();
  //capsules of money
  gereCapsul->create_sprites_list ();
  //gadgets (bonuses and maluses)
  gereGadget->create_sprites_list ();
  //gems stones
  ptGemstone->create_sprites_list ();
  //mobiles characters
  gere_texte->create_sprites_list ();
  paddles->create_projectiles_list ();
  //credits value (left-bottom)
  ptPrntmney->create_sprites_list ();
  //GAME OVER sprites
  ptGameOver->create_sprites_list ();
  //money sprite (left-bottom)
  ptBobMoney->create_sprite (BOB_MONEYS, sprites_bitmap, 0);
  sprites->add (ptBobMoney);
  //reverser sprite (right-bottom)
  ptBobRever->create_sprite (BOB_GADGET, sprites_bitmap, 0);
  sprites->add (ptBobRever);
  //bumper's viewfinder
  ptBaDirect->create_sprites_list ();
  //ESC menu
  popup_menu->first_init (sprites_bitmap, 0, 256 * resolution);
  resources->release_sprites_bitmap ();
  display->lock_surfaces ();

  //###################################################################
  // initialize "Game Over"
  //###################################################################
  error_init (ptGameOver->first_init ());
  if (erreur_num)
    return (erreur_num);

  //###################################################################
  // initialize the head animation
  //###################################################################
  error_init (tete_gugus->initialise ());
  if (erreur_num)
    return (erreur_num);

  init_level ();


  //###################################################################
  // initialize left scores pannel
  //###################################################################
  error_init (tecZ_barre->first_init (joueurGere, ptGigaBlit, gereBalles));
  if (erreur_num)
    return erreur_num;
  error_init (tecZ_barre->affiche_me ());
  if (erreur_num)
    return erreur_num;

  background ();
  paddles->initBumper (tecZ_barre, ptGigaBlit, gereBalles);

  /* balls initialization */
  gereBalles->init_balle (paddles,
                          /* time before the ball leaves paddle (at the game beginning) */
                          levelParam->startCount,
                          //time before the ball leaves (glue option)
                          levelParam->glue_count / hardChoice,
                          //time before the ball accelerates
                          levelParam->speedCount / hardChoice,
                          //time before "tilt" is available
                          levelParam->tilt_count, levelParam->speedBall1);

  //##############################################################
  // initialization "atoms" (aka "bouibouis") 
  //##############################################################
  les_atomes->initialise (levelParam->apparition / hardChoice,
                          levelParam->atom1Count / hardChoice,
                          levelParam->atom2Count / hardChoice,
                          levelParam->atom3Count / hardChoice,
                          levelParam->atom4Count / hardChoice,
                          levelParam->resistance * hardChoice);

  //##############################################################
  //initialize capsules of money
  //##############################################################
  gereCapsul->initialise (levelParam->monayCount * hardChoice,
                          tecZ_barre, ptPrntmney);

  //##############################################################
  //Initialize the object which handles gadgets (bonus and malus)
  //##############################################################
  Sint32 *cours = joueurGere->get_course ();
  Sint32 counb = joueurGere->get_cou_nb ();
  Sint32 brCnt = bricks->get_num_of_bricks ();
  gereGadget->initialise (
                           //frequency of appearance of malus 
                           levelParam->malusCount * hardChoice,
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
                           gereBalles,
                           //the object which handles the text on left scores panel
                           tecZ_barre, BottomWall, pt_magneye);

  //##############################################################
  //initialize the gems tones
  //##############################################################
  error_init (ptGemstone->initialise (joueurGere,
                                      tecZ_barre, ptPrntmney, paddles));
  if (erreur_num)
    return erreur_num;

  //##############################################################
  // initialize mobiles characters ("LEVEL x COMPLETED")
  //##############################################################
  gere_texte->initialise (levelTecno);

  ptPrntmney->initialise (joueurGere, paddles, ptBobMoney, ptBobRever);


  error_init (ptBaDirect->initialize (paddles, 4));
  if (erreur_num)
    return (erreur_num);

  display->unlock_surfaces ();
  /* copy the background offscreen to the game offscreen */
  background_screen->blit_surface (game_screen);

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
  levelParam = ptLev_data->bricklevel (areaNumber, levelTecno);
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
  if (joueurGere->getLifeNum () <= 0)
    {
      if (!isgameover)
        {
#ifndef SOUNDISOFF
          audio->disable_sound ();
          audio->stop_music ();
#endif
          paddles->bumpersOff ();
          bricks->clr_bricks ();
          gereGadget->disable_sprites ();
          gereCapsul->disable_sprites ();
          gereBalles->disable_sprites ();
          sprite_projectile::disable_sprites ();
          ptMiniMess->erase_mess ();
          isgameover++;
        }
      ptMiniMess->execution1 ();
      display->wait_frame ();
      display->lock_surfaces ();
      tete_gugus->execution1 ();
      ptGigaBlit->execution1 ();
      sprites->clear ();
      if (!(hasard_val & 0x00f))
        tete_gugus->teteparasi ();
      if (isgameover >= 2)
        {
          isgameover++;
          ptGameOver->execution1 ();
        }
      if (!bricks->brickRemap () && isgameover < 2) //restore bricks
        isgameover = 2;
      gereBricot->execution1 ();
      ptBaDirect->execution1 ();        //handle ball viewfinder
      les_atomes->atom_depla ();
      sprites->draw ();
      tecZ_barre->scoreEcran ();
      tecZ_barre->barreTemoin ();
      ptPrntmney->execution1 (joueurGere->creditFric);
      display->unlock_surfaces ();
      display->bufferCTab ();
      if (keyboard->is_left_button () && isgameover > 60)
        joueurGere = joueurData::nextplayer (joueurGere, &end_return, 1);
    }

  /*
   * game is running!
   */
  else
    {
      display->wait_frame ();
      display->lock_surfaces ();
      sprites->clear ();
      bricks->brickRemap ();        //restore bricks
      gereBricot->execution1 ();        //restore bricks on side
      changebkgd ();

      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          ptMiniMess->execution1 ();
          tete_gugus->execution1 ();
          ptGigaBlit->execution1 ();

          //handle the "less bricks" option
          bricks->less_bricks ();

          paddles->bp_deplace ();    //move bumpers
          if (tecZ_barre->resteBrick ())
            {
              paddles->lacheBalle ();
              paddles->fire_projectiles ();
            }
          paddles->move_robot ();
          gereBalles->vitusBalle ();    //move balls
          ptBaDirect->execution1 ();    //handle ball viewfinder
          sprite_projectile::gestionTir ();
          les_atomes->atom_depla ();
          pt_magneye->execution1 ();
          gereCapsul->bouge_fric ();    //move capsules of money
          gereGadget->bouge_gads ();    //move bonuses and maluses
          gereGadget->gadgetKeys ();
          ptGemstone->moving_gem ();    //move gems
          gere_texte->goMoveText ();
          if (BottomWall->thecounter < 1)
            BottomWall->disable ();
          else
            BottomWall->thecounter--;

          tecZ_barre->barreTemoin ();
          ptPrntmney->execution1 (joueurGere->creditFric);
        }

      //tiles_ground->draw();
      sprites->draw ();
      tecZ_barre->scoreEcran ();
      Ecode = popup_menu->execution1 ();
      display->unlock_surfaces ();
      display->bufferCTab ();

      //###################################################################
      // next level or next player
      //###################################################################
      if (!tecZ_barre->resteBrick ())
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
                  gereBricot->sauve_etat ();
                  joueurGere = joueurData::nextplayer (joueurGere,
                                                       &end_return, 1);
#ifndef SOUNDISOFF
                  audio->stop_music ();
#endif
                }
              gereBalles->disable_sprites ();
              sprite_projectile::disable_sprites ();
            }
          else
            {
              gere_texte->activeText ();
              sprite_projectile::disable_sprites ();
              gereBalles->disable_sprites ();
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
    joueurGere->lifesReset ();
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
      paddles->free_balls ();
    }
#endif
  return end_return;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void
supervisor_bricks_level::changebkgd ()
{
#ifdef TU_TRICHES
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
      background_screen->blit_surface (game_screen);
    }

  if (keyboard->key_is_pressed (SDLK_v))
    tete_gugus->teterigole ();
  if (keyboard->key_is_pressed (SDLK_b))
    tete_gugus->tetebaille ();
  if (keyboard->key_is_pressed (SDLK_n))
    tete_gugus->teteparasi ();
  if (keyboard->key_is_pressed (SDLK_g))
    ptGigaBlit->initDepart ();
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
      nbkdg = ((areaNumber - 1) * 10) + levelTecno;
      if (levelTecno > 5)
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
  gereEjects->afficheSha ();    //display ejectors shadows
  gereBricot->sauveFond ();     //save background under small bricks
  gereBricot->afficheSha ();    //display small bricks shadows
  gereBricot->afficheGfx ();    //display small bricks of the three walls
  gereEjects->afficheGfx ();    //display ejectors

  //###################################################################
  // intialize the bricks level
  //###################################################################
  bricks->first_init (tecZ_barre, gereCapsul, gereGadget);
  Sint32 lbrik = joueurGere->get_lessBk ();
  joueurGere->set_lessBk (0);
  bricks->initialize (areaNumber, levelTecno, lbrik);
  return erreur_num;
}
