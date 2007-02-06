/** 
 * @file tecnoballz.cc 
 * @brief Base of all classes, and main static methods of the game 
 * @created 2002-08-18
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tecnoballz.cc,v 1.4 2007/02/06 12:26:01 gurumeditation Exp $
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
#include "../include/tecnoballz.h"
#include "../include/handler_memory.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/list_sprites.h"
#include "../include/handler_players.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/supervisor_shop.h"
#include "../include/supervisor_guards_level.h"
#include "../include/supervisor_main_menu.h"
#include "../include/bitmap_data.h"
#include "../include/handler_audio.h"
#include "../include/level_data.h"
#include "../include/handler_resources.h"
#include "../include/scoretable.h"
#include "../include/supervisor_map_editor.h"
//.............................................................................
Sint32
  tecnoballz::arg_jumper = -1;
Sint32
  tecnoballz::bg4_colors = 0;   //force 4 colors background
bool
  tecnoballz::is_verbose = 0;
Sint32
  tecnoballz::num_erreur = 0;   //error number
Sint32
  tecnoballz::counterObj = 0;   //number of objects
Sint32
  tecnoballz::hasard_val = 0;   //random value
Sint32
  tecnoballz::countframe = 0;
scoretable *
  tecnoballz::ptScoreTab = NULL;        //manage best scores
handler_resources *
  tecnoballz::resources = NULL; //manage resources
level_data *
  tecnoballz::ptLev_data = NULL;        //manage levels
handler_memory *
  tecnoballz::memory = NULL;    //manage memory allocation  
#ifndef SOUNDISOFF
handler_audio *
  tecnoballz::audio = NULL;     //manage sound      
#endif
handler_display *
  tecnoballz::display = NULL;   //manage screen
handler_keyboard *
  tecnoballz::keyboard = NULL;  //keyboard handle
list_sprites *
  tecnoballz::sprites = NULL;   //sprites handle
handler_players *
  tecnoballz::joueurGere = NULL;        //players handle
Sint16 *
  tecnoballz::table_cosL = NULL;        //cosinus table
Sint16 *
  tecnoballz::table_sinL = NULL;        //sinus table
supervisor_bricks_level *
  tecnoballz::bricks_level = NULL;        //bricks levels handle
supervisor_shop *
  tecnoballz::shop = NULL;        //shop handle
supervisor_guards_level *
  tecnoballz::guards_level = NULL;        //guard handle
supervisor_map_editor *
  tecnoballz::map_editor = NULL;        //scroll editor handle
supervisor_main_menu *
  tecnoballz::main_menu = NULL;        //menu handle
// 1:bricks level / 2:shop / 3:guards level / 4:main menu / 5:scrolling editor
Sint32
  tecnoballz::super_jump = 1;
Sint32
  tecnoballz::super_exit = 0;
bitmap_data *
  tecnoballz::sprites_bitmap = 0;
Uint32
  tecnoballz::cheat_flag = 0;
Uint32
  tecnoballz::birth_flag = 0;   //all name are "040670"
Uint32
  tecnoballz::double_mem = 1;   //2=double all allocations
Sint32
  tecnoballz::hardChoice = 1;
Sint32
  tecnoballz::vieInitial = 8;
Sint32
  tecnoballz::nuOfPlayer = 1;
char
  tecnoballz::chainelog[100];
const char
  tecnoballz::nomprefix[] = PREFIX;
Sint32
  tecnoballz::resolution = 2;
bool
  tecnoballz::bob_ground = false;
char
  tecnoballz::zeAreaCode[11] = "          ";

offscreen_surface *
  tecnoballz::game_screen = NULL;
offscreen_surface *
  tecnoballz::background_screen = NULL;

/**
 * Once initialization, create persistent objects
 */
Sint32
tecnoballz::first_init (configfile * pConf)
{
  if (is_verbose)
    {
      std::cout << "tecnoballz::first_init() start!" << std::endl;
    }
#if __WORDSIZE == 64
  hasard_val = (long) first_init;
#else
  hasard_val = (Sint32) first_init;
#endif
  memory = new handler_memory ();
  memory->init (17000);

  resources = new handler_resources ();

  ptScoreTab = new scoretable ();
  num_erreur = ptScoreTab->first_init ();
  if (num_erreur)
    return num_erreur;

  resources->load_sinus ();


#ifndef SOUNDISOFF
  audio = handler_audio::get_instance ();
#endif
  if (is_verbose)
    {
      printf ("tecnoballz::first_init() [handler_display::initialize]\n");
    }
  display = new handler_display ();
  num_erreur = display->initialize ();
  if (num_erreur)
    return (num_erreur);

  keyboard = handler_keyboard::get_instance ();
  sprites = new list_sprites ();
  num_erreur = sprites->init (400);
  if (num_erreur)
    return num_erreur;



  ptLev_data = new level_data ();
  //Sint32 Ecode = -1; 
  joueurGere = handler_players::joueursADD (MAX_PLAYER);
  if (!joueurGere)
    {
      num_erreur = E_GENRIQUE;
      return num_erreur;
    }

  //###################################################################
  // retrieve user
  //###################################################################
  for (Uint32 i = 0; i < 6; i++)
    handler_players::playerlist[i]->setNewName (pConf->get_player (i));
  /*
     char *pUser = getenv("USER");
     if (pUser)
     handler_players::playerlist[0]->setNewName(pUser);
     else
     handler_players::playerlist[0]->setNewName("ALBERT");

     handler_players::playerlist[1]->setNewName("LEON  ");
     handler_players::playerlist[2]->setNewName("ANDRE ");
     handler_players::playerlist[3]->setNewName("GERARD");
     handler_players::playerlist[4]->setNewName("RAOUL ");
     handler_players::playerlist[5]->setNewName("MARCEL");
   */

  super_jump = 4;               //menu
  //super_jump = 1;               //bricks level (test only)
  //super_jump = 3;               //guards level (test only)
  //super_jump = 5;               //scroll editor (test only)


  if (arg_jumper > 0)
    super_jump = arg_jumper;
  if (is_verbose)
    printf ("tecnoballz::first_init() [STOP]\n");
  return num_erreur;
}

//------------------------------------------------------------------------------
// main loop of the game
//------------------------------------------------------------------------------
Sint32
tecnoballz::game_begin ()
{
  do
    {
      if (is_verbose)
        printf ("tecnoballz::game_begin: game phase=%i (super_jump) \n",
                super_jump);
      //###############################################################
      // initialize
      //###############################################################
      switch (super_jump)
        {
          // exit of game
        case -1:
          super_exit = 1;
          break;

          // initialize bricks level
        case BRICKS_LEVEL:
          release_objects ();
          bricks_level = new supervisor_bricks_level ();
          num_erreur = bricks_level->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize the shop
        case SHOP:
          release_objects ();
          shop = new supervisor_shop ();
          num_erreur = shop->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize guards level
        case GUARDS_LEVEL:
          release_objects ();
          guards_level = new supervisor_guards_level ();
          num_erreur = guards_level->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize menu
        case MAIN_MENU:
          release_objects ();
          main_menu = new supervisor_main_menu ();
          num_erreur = main_menu->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize scrolling editor (menu and guards)
        case MAP_EDITOR:
          release_objects ();
          map_editor = new supervisor_map_editor ();
          num_erreur = map_editor->first_init ();
          if (num_erreur)
            return num_erreur;
          break;
        }

      //###############################################################
      // running
      //###############################################################
      switch (super_jump)
        {
          // bricks level
        case BRICKS_LEVEL:
          super_jump = 0;
          do
            {
              super_jump = bricks_level->main_loop ();
            }
          while (!super_jump);
          break;

          // the shop
        case SHOP:
          super_jump = 0;
          do
            {
              super_jump = shop->main_loop ();
            }
          while (!super_jump);
          break;

          // guards level
        case GUARDS_LEVEL:
          super_jump = 0;
          do
            {
              super_jump = guards_level->main_loop ();
            }
          while (!super_jump);
          break;

          // the menu
        case MAIN_MENU:
          super_jump = 0;
          do
            {
              super_jump = main_menu->main_loop ();
            }
          while (!super_jump);
          break;

          // scrolling editor (menu and guards)
        case MAP_EDITOR:
          super_jump = 0;
          do
            {
              super_jump = map_editor->main_loop ();
            }
          while (!super_jump);
          break;


        }
    }
  while (!super_exit);
  return num_erreur;
}

/**
 * Release the main objects
 */
void
tecnoballz::release_objects ()
{
  if (NULL != bricks_level)
    {
      delete bricks_level;
      bricks_level = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'supervisor_bricks_level' released" << std::endl;
        }
    }
  if (NULL != shop)
    {
      delete shop;
      shop = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'supervisor_shop' released" << std::endl;
        }
    }
  if (NULL != guards_level)
    {
      delete guards_level;
      guards_level = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'supervisor_guards_level' released" << std::endl;
        }
    }
  if (NULL != main_menu)
    {
      delete main_menu;
      main_menu = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'supervisor_main_menu' released" << std::endl;
        }
    }
  if (NULL != map_editor)
    {
      delete map_editor;
      map_editor = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'supervisor_map_editor' released" << std::endl;
        }
    }
}

/**
 * Game exit, relase all objects
 */
void
tecnoballz::release_all_objects (configfile * pConf)
{
  //save players names into config file
  for (Uint32 i = 0; i < 6; i++)
    pConf->set_player (i, handler_players::playerlist[i]->returnName ());
  if (is_verbose)
    printf ("==1 release_objects \n");
  release_objects ();
  handler_players::joueursRAZ ();
  if (is_verbose)
    printf ("==3 level_data \n");
  delete ptLev_data;
  if (is_verbose)
    printf ("==4 sprites \n");
  delete sprites;
  if (is_verbose)
    printf ("==5 keyboard\n");
  keyboard->destroy_instance ();
  if (is_verbose)
    printf ("==6 ptScoreTab\n");
  delete ptScoreTab;
  if (is_verbose)
    printf ("==7 display\n");
  delete display;
#ifndef SOUNDISOFF
  if (is_verbose)
    printf ("==8 handler_audio\n");
  delete audio;
#endif
  delete resources;
  if (is_verbose)
    printf ("==9 memory\n");
  delete memory;
}

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecnoballz::tecnoballz ()
{
}

/** 
 * release object
 */
tecnoballz::~tecnoballz ()
{
}

/** 
 * Initialize some members
 */ 
void
tecnoballz::object_init ()
{
  erreur_num = 0;
  numero_obj = counterObj;
  counterObj++;
}

/**
 * Object destroyed
 */
void
tecnoballz::object_free ()
{
  counterObj--;
}

//-------------------------------------------------------------------------------
// return the number of error
//-------------------------------------------------------------------------------
Sint32
tecnoballz::retour_err ()
{
  return erreur_num;
}

//-------------------------------------------------------------------------------
// return the number of the object
//-------------------------------------------------------------------------------
Sint32
tecnoballz::get_number ()
{
  return (numero_obj);
}

//-------------------------------------------------------------------------------
// initialize the number of error
//-------------------------------------------------------------------------------
void
tecnoballz::error_init (Sint32 error)
{
  erreur_num = error;
}

//-------------------------------------------------------------------------------
// compare two character strings
//-------------------------------------------------------------------------------
Sint32
tecnoballz::chaine_cmp (char *srcPT, char *desPT, Sint32 taille)
{
  Sint32 t = taille;
  char *s = srcPT;
  char *d = desPT;
  for (Sint32 i = 0; i < t; i++)
    {
      char a = s[i];
      char b = d[i];
      if (a != b)
        return (0);
    }
  return 1;
}

//------------------------------------------------------------------------------
// convert integer into ASCII string 
// input => value:      number to convert
//       => strng:      pointer to ASCII string (finished by  zero)
//       => reste:      maximum length (1 = 2 chars, 2 = 3 chars, ...)
//------------------------------------------------------------------------------
void
tecnoballz::intToASCII (Sint32 value, char *strng, Uint32 reste)
{
  Uint32 index = 0;
  Uint32 zsize = 1;
  if (value < 0)
    {
      value = -value;
      *(strng++) = '-';
      if (reste > 0)
        reste--;
    }
  for (index = 0; index < reste; index++)
    zsize = zsize * 10;
  index = 0;
  while (zsize > 0)
    {
      Uint32 reste = value / zsize;
      char zchar = (char) ('0' + reste);
      if (zchar > '9' || zchar < '0')
        zchar = '?';
      strng[index++] = zchar;
      value = value - (zsize * reste);
      zsize = zsize / 10;
    }
}

//-------------------------------------------------------------------------------
// read little endian 16 bits word 
//-------------------------------------------------------------------------------
Sint32
tecnoballz::littleWord (char *memPT)
{
  Sint32 i = 0;
  unsigned char *p = (unsigned char *) memPT;
  i = p[1];
  i <<= 8;
  i += p[0];
  return i;
}

void
tecnoballz::bigendianw (Uint32 * ptsrc, Uint32 * ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  *ptdes = *ptsrc;
#else
  char *s = (char *) ptsrc;
  char *d = (char *) ptdes;
  d[0] = s[3];
  d[1] = s[2];
  d[2] = s[1];
  d[3] = s[0];
#endif
}

void
tecnoballz::bigendianr (Uint32 * ptsrc, Uint32 * ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  *ptdes = *ptsrc;
#else
  char *s = (char *) ptsrc;
  char *d = (char *) ptdes;
  d[3] = s[0];
  d[2] = s[1];
  d[1] = s[2];
  d[0] = s[3];
#endif
}
