/** 
 * @file supervisor_main_menu.cc 
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-02-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.cc,v 1.7 2007/02/11 16:04:44 gurumeditation Exp $
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
#include "../include/supervisor_main_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"
#include "../include/controller_guardians.h"

/** 
 * Create the main menu supervisor 
 */
supervisor_main_menu::supervisor_main_menu ()
{
  initialise ();
  tiles_map = new tilesmap_scrolling ();       // background scrolling
  tecnoballz_logo = new sprite_object ();    // big logo "TecnoballZ"
  BOB_defile = new controller_fontes_menu ();
  menu_texte = new print_menu ();
  mouse_pointer = new sprite_mouse_pointer ();
  offset_xx1 = 0;
}


/** 
 * Release the main menu supervisor 
 */
supervisor_main_menu::~supervisor_main_menu ()
{
  delete mouse_pointer;
  delete menu_texte;
  delete BOB_defile;
  delete tecnoballz_logo;
  delete tiles_map;
  liberation ();
}

/**
 * Perform some initializations
 */
Sint32
supervisor_main_menu::first_init ()
{
  if (is_verbose)
    {
      std::cout << "supervisor_main_menu::first_init() Begin!" << std::endl;
    }
  sprites->reset ();
#ifndef SOUNDISOFF
  audio->play_music (MUSICINTRO);
#endif

  /*
   * create sprites data
   */
  resources->load_sprites_bitmap ();
  tecnoballz_logo->create_sprite (BOB_LOGOTB, sprites_bitmap, 1);
  sprites->add (tecnoballz_logo);
  tecnoballz_logo->enable ();
  tecnoballz_logo->set_coordinates (64 * resolution, 13 * resolution);
  BOB_defile->create_fontes_list ();
  mouse_pointer->create_pointer_sprite (sprites_bitmap);
  resources->release_sprites_bitmap ();
  tiles_map->initialize (tilesmap_scrolling::TILES_COLOR_MENU,
                         tilesmap_scrolling::MAPED_MENU);

  error_init (menu_texte->first_init ());
  keyboard->set_grab_input (false);
  if (erreur_num)
    return (erreur_num);

  if (is_verbose)
    {
      std::cout << "supervisor_main_menu::first_init() End!" << std::endl;
    }
  return (erreur_num);
}

/**
 * Main loop of the main menu
 */
Sint32
supervisor_main_menu::main_loop ()
{

  display->wait_frame ();
  display->lock_surfaces ();
  offset_inc ();
  move_tecnoballz_logo ();                //move the TecnoballZ logo
  BOB_defile->move_chars ();    //move the characters (scroll-text)
  tiles_map->scrolling1 (-1);  //scrolling of the screen background
  mouse_pointer->move ();
  sprites->draw ();

  Sint32 zeRet = 0;
  zeRet = menu_texte->afficheTxt ();

  switch (zeRet)
    {
    case 1:
      end_return = -1;
      break;
    case 2:
      end_return = start_new_game ();
      break;
    }

  menu_texte->draw_copy_from_bitmap ();
  if (keyboard->command_is_pressed (handler_keyboard::TOEXITFLAG))
    end_return = -1;
  display->unlock_surfaces ();

  //###################################################################
  // copy whole buffer surface into screen surface
  //###################################################################
  display->bufferCTab ();

  /* [F5] key jump to map editor */
#ifdef TU_TRICHES
  if (keyboard->key_is_pressed (SDLK_F5))
    {
      end_return = 5;
    }
#endif
  return end_return;
}

//------------------------------------------------------------------------------
// Increment the offset on the sinus table
//------------------------------------------------------------------------------
void
supervisor_main_menu::offset_inc ()
{
  Sint32 m = SINUS_MASK;
  Sint32 a = offset_xx1 + 6;
  a &= m;
  offset_xx1 = a;
}

/** 
 * Move the logo of TecnoballZ sprite
 */
void
supervisor_main_menu::move_tecnoballz_logo ()
{
  Sint32 a, b;
  a = offset_xx1;
  Sint16 *s = table_sinL + a;
  b = ((*s * 20 * resolution) >> SINUS_DECA) + 32 * resolution;
  tecnoballz_logo->set_x_coord (b);
  if (birth_flag)
    {
      a *= 4;
      a &= SINUS_MASK;
      s = table_cosL + a;
      b = ((*s * 5 * resolution) >> SINUS_DECA) + 7 * resolution;
      tecnoballz_logo->set_y_coord (b);
    }
}

/**
 * Start new TecnoballZ game 
 */
Sint32
supervisor_main_menu::start_new_game ()
{
  cheat_flag = 0;
#ifdef TU_TRICHES
  cheat_flag = 1;
#endif
  Uint32 nArea = 1;
  Uint32 level = 1;
  Uint32 grdPt = 0;
  Sint32 iplay;
  Uint32 index = 0;

  /*
   * check area password validity
   */
  Uint32 aMaxi = 4;
#ifdef TU_TRICHES
  //allows to jump directly to the last guards with a password 
  aMaxi = 6;
#else
  if (birth_flag)
    aMaxi = 6;
#endif
  for (Uint32 areaN = 1; areaN <= aMaxi; areaN++)
    {
      for (Uint32 hardN = 1; hardN <= 4; hardN++)
        {
          Uint32 f = 1;
          for (Uint32 i = 0; i < 10; i++)
            {
              if (zeAreaCode[i] != codesarea[index + i])
                {
                  f = 0;
                  break;
                }
            }
          index += 10;
          if (f)
            {
              if (areaN == 6)
                {
                  nArea = 5;
                  level = 13;
                }
              else
                {
                  nArea = areaN;
                  level = 12;
                }
              hardChoice = hardN;
              if (is_verbose)
                printf ("supervisor_main_menu::start_new_game() %s "
                        "is OK; nArea = %i;"
                        "level = %i, hardN = %i\n",
                        &zeAreaCode[0], nArea, level, hardN);
              grdPt = controller_guardians::level2gdpt (nArea, level);
              goto sortie;
            }
        }
    }
sortie:

  //nArea = 5; level = 13; grdPt = controller_guardians::level2gdpt(nArea, level); //test only
  //nArea = 4; level = 12; grdPt = controller_guardians::level2gdpt(nArea, level); //test only
  //return 0;

  //level = 6;    //test only

  //###################################################################
  // initialize enable player(s)
  //###################################################################
  for (iplay = 0; iplay < nuOfPlayer; iplay++)
    {
      Uint32 nlife = initial_num_of_lifes;
      if (birth_flag)
        nlife = nlife + 10;
      if (chaine_cmp
          (handler_players::players_list[iplay]->returnName (), "ETB   ", 6))
        nlife += 5;
      if (chaine_cmp
          (handler_players::players_list[iplay]->returnName (), "DJI   ", 6))
        nlife += 4;
      if (chaine_cmp
          (handler_players::players_list[iplay]->returnName (), "JMM   ", 6))
        nlife += 3;
      if (chaine_cmp
          (handler_players::players_list[iplay]->returnName (), "ZBB   ", 6))
        nlife += 2;
      if (chaine_cmp
          (handler_players::players_list[iplay]->returnName (), "REG   ", 6))
        nlife += 1;
      handler_players::players_list[iplay]->initialize (nlife, nArea, level,
                                                 600, grdPt);
    }

  
  /* disable other player(s) */
  for (Sint32 i = iplay; i < MAX_PLAYER; i++)
    {
      handler_players::players_list[i]->initialize (0, 1, 1, 0, 0);
    }
  
  current_player = handler_players::players_list[0];
  
  Uint32 value = current_player->get_next_phase ();
  if (value == SHOP)
    {
      value = BRICKS_LEVEL;
    }
  return value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
/*
unsigned char supervisor_main_menu::colors_map[] =
{ 0, 0, 0, 0, 0, 0, 48, 80, 112, 16, 48, 80, 64, 96, 128, 112, 112, 144, 80,
  80, 112, 32, 64, 96, 144, 144, 176
};*/

const char *
supervisor_main_menu::getTheCode (Uint32 arean, Uint32 hardc)
{
  if (arean < 2)
    return NULL;
  return &codesarea[(arean - 2) * 40 + (hardc - 1) * 10];

}

/*
	LARRYHEARD: area 2
	DANCEFLOOR: area 3
	ZULUNATION: area 4
	DANCEMANIA: area 5
	RINGOFFIRE: area 5 level 12 
	SHELLSHOCK: area 5 level 13
*/

const char
  supervisor_main_menu::codesarea[241] = {
  "LARRYHEARD"
    "SAUNDERSON"
    "JUANATKINS"
    "STEPHENSON"
    "DANCEFLOOR"
    "REVOLUTION"
    "LOOKTOSEXY"
    "REACHINGUP"
    "ZULUNATION"
    "HOUSEPIMPS"
    "ANDRONICUS"
    "DEFINITIVE"
    "DANCEMANIA"
    "PEPPERMINT"
    "SOLARTRIBE"
    "PROJECTXYZ"
    "RINGOFFIRE"
    "POINTBLANK"
    "TEMPTATION"
    "BLUEMONDAY" "SHELLSHOCK" "HOUSEMUSIC" "DAVECLARKE" "CYBERACTIF"
};
