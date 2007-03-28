/** 
 * @file supervisor_main_menu.cc 
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-03-28
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.15 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.cc,v 1.15 2007/03/28 13:57:51 gurumeditation Exp $
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
  fonts_scrolling = new controller_fontes_menu ();
  text_menu = new sprite_display_menu ();
  mouse_pointer = new sprite_mouse_pointer ();
  offset_xx1 = 0;
}


/** 
 * Release the main menu supervisor 
 */
supervisor_main_menu::~supervisor_main_menu ()
{
  delete mouse_pointer;
  delete text_menu;
  delete fonts_scrolling;
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
  fonts_scrolling->create_fontes_list ();
  mouse_pointer->create_pointer_sprite (sprites_bitmap);
  resources->release_sprites_bitmap ();
  tiles_map->initialize (tilesmap_scrolling::TILES_COLOR_MENU,
                         tilesmap_scrolling::MAPED_MENU);

  text_menu->first_init ();
  keyboard->set_grab_input (false);
  if (is_verbose)
    {
      std::cout << "supervisor_main_menu::first_init() End!" << std::endl;
    }
  return erreur_num;
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
  fonts_scrolling->move_chars ();
  mouse_pointer->move ();
  sprites->draw ();

  /* check and draw the menu text */
  switch (text_menu->check_and_display ())
    {
    case sprite_display_menu::PROGRAM_EXIT:
      end_return = LEAVE_TECNOBALLZ;
      break;
    case sprite_display_menu::START_GAME:
      end_return = start_new_game ();
      break;
    }

  text_menu->draw_copy_from_bitmap ();
  if (keyboard->command_is_pressed (handler_keyboard::TOEXITFLAG))
    {
      end_return = LEAVE_TECNOBALLZ;
      printf("end_return %i\n", end_return);
    }
  display->unlock_surfaces ();

  //###################################################################
  // copy whole buffer surface into screen surface
  //###################################################################
  display->bufferCTab ();

  /* [F5] key jump to map editor */
#ifdef UNDER_DEVELOPMENT
  if (keyboard->key_is_pressed (SDLK_F5))
    {
      end_return = MAP_EDITOR;
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
  is_enabled_cheat_mode = false;
#ifdef UNDER_DEVELOPMENT
  is_enabled_cheat_mode = true;
#endif
  Sint32 iplay;
  
  /*
   * check area password validity
   */
  Uint32 area_num;
  Uint32 level_num;
  Uint32 grdPt;
  Uint32 area_count = check_area_code ();
  if (area_count == 0)
    {
      area_num = 1;
      level_num = 1;
      grdPt = 0;
    }
  else
    {
      if (area_count == 6)
      {
        area_num = 5;
        level_num = 13;
      }
      else
      {
        area_num = area_count;
        level_num = 12;
      }
      if (is_verbose)
      {
        std::cout << "*supervisor_main_menu::start_new_game() " <<
          "password is valid! Password: " << &current_area_code[0] <<
          "; area number:" << area_num << "; level number:" << level_num << 
          "; difficulty level_num: " << difficulty_level << std::endl;

    }
        grdPt = controller_guardians::level2gdpt (area_num, level_num);
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
      handler_players::players_list[iplay]->initialize (nlife, area_num, level_num,
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
supervisor_main_menu::check_area_code ()
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
              if (current_area_code[i] != area_codes[index + i])
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

/**
 * Static method which return a area code
 * @param aera_num area number from 2 to 5
 * @param difficulty 0 to 3
 * @return the area code
 */
const char *
supervisor_main_menu::get_area_code (Uint32 aera_num, Uint32 difficulty)
{
  if (aera_num < 2)
    {
      return NULL;
    }
  return &area_codes[(aera_num - 2) * 40 + (difficulty - 1) * 10];
}
  
 /**
 * Static method which return the current area code
 * @return the current area code
 */
char
*supervisor_main_menu::get_current_area_code ()
{
  return current_area_code;
}

/**
 * Static method which copy current area code
 * @param destination destination string
 */
void
supervisor_main_menu::copy_current_area_code (char *destination)
{
  for (Uint32 i = 0; i < AREA_CODE_LENGTH; i++)
    {
      destination[i] = current_area_code[i];
    }
}





/*
	LARRYHEARD: area 2
	DANCEFLOOR: area 3
	ZULUNATION: area 4
	DANCEMANIA: area 5
	RINGOFFIRE: area 5 level 12 
	SHELLSHOCK: area 5 level 13
*/

const char supervisor_main_menu::area_codes[241] =
{
  /* level 12 area 1 */
	"LARRYHEARD" \
	"SAUNDERSON" \
	"JUANATKINS" \
	"STEPHENSON" \
  /* level 12 area 2 */
	"DANCEFLOOR" \
	"REVOLUTION" \
	"LOOKTOSEXY" \
	"REACHINGUP" \
  /* level 12 area 3 */
	"ZULUNATION" \
	"HOUSEPIMPS" \
	"ANDRONICUS" \
	"DEFINITIVE" \
  /* level 12 area 4 */
	"DANCEMANIA" \
	"PEPPERMINT" \
	"SOLARTRIBE" \
	"PROJECTXYZ" \
  /* level 12 area 5 (with cheat code enabled) */
	"RINGOFFIRE" \
	"POINTBLANK" \
	"TEMPTATION" \
	"BLUEMONDAY" \
  /* level 13 area 5 (with cheat code enabled) */
	"SHELLSHOCK" \
	"HOUSEMUSIC" \
	"DAVECLARKE" \
	"CYBERACTIF"
};
char supervisor_main_menu::current_area_code[AREA_CODE_LENGTH + 1] = 
"          ";

