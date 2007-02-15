/** 
 * @file supervisor_main_menu.cc 
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-02-15
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.cc,v 1.10 2007/02/15 20:52:43 gurumeditation Exp $
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
  /* vertical background scrolling */
  tiles_map = new tilesmap_scrolling ();
  /*  big logo "TecnoballZ" */
  tecnoballz_logo = new sprite_object ();
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
  /* vertical scrolling of the screen background */
  tiles_map->scrolling1 (-1);
  display->lock_surfaces ();
  offset_inc ();
  move_tecnoballz_logo ();
  /* scroll text of the menu */
  BOB_defile->move_chars ();
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
#ifdef UNDER_DEVELOPMENT
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
#ifdef UNDER_DEVELOPMENT
  cheat_flag = 1;
#endif
  Sint32 iplay;
  
  /*
   * check area password validity
   */
  Uint32 nArea;
  Uint32 level;
  Uint32 grdPt;
  Uint32 area_count = check_area_password ();
  if (area_count == 0)
    {
      nArea = 1;
      level = 1;
      grdPt = 0;
    }
  else
    {
      if (area_count == 6)
      {
        nArea = 5;
        level = 13;
      }
      else
      {
        nArea = area_count;
        level = 12;
      }
      if (is_verbose)
      {
        std::cout << "*supervisor_main_menu::start_new_game() " <<
          "password is valid! Password: " << &zeAreaCode[0] <<
          "; area number:" << nArea << "; level number:" << level << 
          "; difficulty level: " << difficulty_level << std::endl;

    }
        grdPt = controller_guardians::level2gdpt (nArea, level);
    }
 


  //###################################################################
  // initialize enable player(s)
  //###################################################################
  for (iplay = 0; iplay < number_of_players; iplay++)
    {
      Uint32 nlife = initial_num_of_lifes;
      if (birth_flag)
        nlife = nlife + 10;
      if (chaine_cmp
          (handler_players::players_list[iplay]->get_name (), "ETB   ", 6))
        nlife += 5;
      if (chaine_cmp
          (handler_players::players_list[iplay]->get_name (), "DJI   ", 6))
        nlife += 4;
      if (chaine_cmp
          (handler_players::players_list[iplay]->get_name (), "JMM   ", 6))
        nlife += 3;
      if (chaine_cmp
          (handler_players::players_list[iplay]->get_name (), "ZBB   ", 6))
        nlife += 2;
      if (chaine_cmp
          (handler_players::players_list[iplay]->get_name (), "REG   ", 6))
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


/**
 * Check area password validity
 * @return if the password is valid return the area number from 1 to 5 
 *         or 6 for the very last guardian of the area 5. 
 *         Return 0 if password is not valid
 */
Uint32
supervisor_main_menu::check_area_password ()
{
  Uint32 area_max = 4;
  Uint32 index = 0;
#ifdef UNDER_DEVELOPMENT
  /* The number of zones really goes from 1 to 5.  
   * The value 6 makes it possible to jump directly to the
   * very last guardian of the area 5 */ 
  area_max = 6;
#else
  if (birth_flag)
    {
      area_max = 6;
    }
#endif
  for (Uint32 area_count = 1; area_count <= area_max; area_count++)
    {
      for (Uint32 difficulty_count = 1; difficulty_count <= 4; difficulty_count++)
        {
          bool is_valid = true;
          for (Uint32 i = 0; i < 10; i++)
            {
              if (zeAreaCode[i] != codesarea[index + i])
                {
                  is_valid = false;
                  break;
                }
            }
          index += 10;
          if (is_valid)
            {
              difficulty_level = difficulty_count;
              return area_count;
            }
        }
    }
  return 0;
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
