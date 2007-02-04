/** 
 * @file tecnoballz.cc 
 * @brief Base of all classes, and main static methods of the game 
 * @created 2002-08-18
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tecnoballz.cc,v 1.1 2007/02/04 17:10:17 gurumeditation Exp $
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
#include "../include/joueurData.h"
#include "../include/tableaux_Z.h"
#include "../include/shop_tecno.h"
#include "../include/gard_tecno.h"
#include "../include/menu_tecno.h"
#include "../include/bitmap_data.h"
#include "../include/handler_audio.h"
#include "../include/level_data.h"
#include "../include/handler_resources.h"
#include "../include/scoretable.h"
#include "../include/scrolledit.h"
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
joueurData *
  tecnoballz::joueurGere = NULL;        //players handle
Sint16 *
  tecnoballz::table_cosL = NULL;        //cosinus table
Sint16 *
  tecnoballz::table_sinL = NULL;        //sinus table
tableaux_Z *
  tecnoballz::tabGestion = NULL;        //bricks levels handle
shop_tecno *
  tecnoballz::shpGestion = NULL;        //shop handle
gard_tecno *
  tecnoballz::garGestion = NULL;        //guard handle
scrolledit *
  tecnoballz::ptScrollEd = NULL;        //scroll editor handle
menu_tecno *
  tecnoballz::menGestion = NULL;        //menu handle
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
//------------------------------------------------------------------------------
// once initialization
//------------------------------------------------------------------------------
Sint32
tecnoballz::first_init (configfile * pConf)
{
  if (is_verbose)
    printf ("tecnoballz::first_init() [START]\n");
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
  joueurGere = joueurData::joueursADD (MAX_PLAYER);
  if (!joueurGere)
    {
      num_erreur = E_GENRIQUE;
      return num_erreur;
    }

  //###################################################################
  // retrieve user
  //###################################################################
  for (Uint32 i = 0; i < 6; i++)
    joueurData::playerlist[i]->setNewName (pConf->get_player (i));
  /*
     char *pUser = getenv("USER");
     if (pUser)
     joueurData::playerlist[0]->setNewName(pUser);
     else
     joueurData::playerlist[0]->setNewName("ALBERT");

     joueurData::playerlist[1]->setNewName("LEON  ");
     joueurData::playerlist[2]->setNewName("ANDRE ");
     joueurData::playerlist[3]->setNewName("GERARD");
     joueurData::playerlist[4]->setNewName("RAOUL ");
     joueurData::playerlist[5]->setNewName("MARCEL");
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
        case 1:
          release_objects ();
          tabGestion = new tableaux_Z ();
          num_erreur = tabGestion->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize the shop
        case 2:
          release_objects ();
          shpGestion = new shop_tecno ();
          num_erreur = shpGestion->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize guards level
        case 3:
          release_objects ();
          garGestion = new gard_tecno ();
          num_erreur = garGestion->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize menu
        case 4:
          release_objects ();
          menGestion = new menu_tecno ();
          num_erreur = menGestion->first_init ();
          if (num_erreur)
            return num_erreur;
          break;

          // initialize scrolling editor (menu and guards)
        case 5:
          release_objects ();
          ptScrollEd = new scrolledit ();
          num_erreur = ptScrollEd->first_init ();
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
        case 1:
          super_jump = 0;
          do
            {
              super_jump = tabGestion->zeMainLoop ();
            }
          while (!super_jump);
          break;

          // the shop
        case 2:
          super_jump = 0;
          do
            {
              super_jump = shpGestion->zeMainLoop ();
            }
          while (!super_jump);
          break;

          // guards level
        case 3:
          super_jump = 0;
          do
            {
              super_jump = garGestion->zeMainLoop ();
            }
          while (!super_jump);
          break;

          // the menu
        case 4:
          super_jump = 0;
          do
            {
              super_jump = menGestion->zeMainLoop ();
            }
          while (!super_jump);
          break;

          // scrolling editor (menu and guards)
        case 5:
          super_jump = 0;
          do
            {
              super_jump = ptScrollEd->zeMainLoop ();
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
  if (NULL != tabGestion)
    {
      delete tabGestion;
      tabGestion = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'tableaux_Z' released" << std::endl;
        }
    }
  if (NULL != shpGestion)
    {
      delete shpGestion;
      shpGestion = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'shop_tecno' released" << std::endl;
        }
    }
  if (NULL != garGestion)
    {
      delete garGestion;
      garGestion = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'gard_tecno' released" << std::endl;
        }
    }
  if (NULL != menGestion)
    {
      delete menGestion;
      menGestion = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'menu_tecno' released" << std::endl;
        }
    }
  if (NULL != ptScrollEd)
    {
      delete ptScrollEd;
      ptScrollEd = NULL;
      if (is_verbose)
        {
          std::cout << "tecnoballz::release_objects() " <<
            "'scrolledit' released" << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
// game exit: relase all objects
//------------------------------------------------------------------------------
void
tecnoballz::release_all_objects (configfile * pConf)
{
  //save players names into config file
  for (Uint32 i = 0; i < 6; i++)
    pConf->set_player (i, joueurData::playerlist[i]->returnName ());
  if (is_verbose)
    printf ("==1 release_objects \n");
  release_objects ();
  joueurData::joueursRAZ ();
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

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecnoballz::~tecnoballz ()
{
}

//-------------------------------------------------------------------------------
// perform some initializations of gigablitz
//-------------------------------------------------------------------------------
void
tecnoballz::mentatInit ()
{
  erreur_num = 0;
  numero_obj = counterObj;
  counterObj++;
}

//-------------------------------------------------------------------------------
// object destroyed
//-------------------------------------------------------------------------------
void
tecnoballz::mentatKill ()
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
