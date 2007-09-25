/** 
 * @file configfile.cc 
 * @brief Config file handler 
 * @created 2005-01-22
 * @date 2007-09-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.27 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: configfile.cc,v 1.27 2007/09/25 16:00:44 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/configfile.h"
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/handler_players.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *
configfile::language_to_string[MAX_OF_LANGUAGES] =
{
  "en",
  "fr"
};



//..............................................................................

//------------------------------------------------------------------------------
// create object
//------------------------------------------------------------------------------
configfile::configfile ()
{
  thePlayers[0] = &thePlayer1[0];
  thePlayers[1] = &thePlayer2[1];
  thePlayers[2] = &thePlayer3[2];
  thePlayers[3] = &thePlayer4[3];
  thePlayers[4] = &thePlayer5[4];
  thePlayers[5] = &thePlayer6[5];
  for (Uint32 i = 0; i < 6; i++)
    {
      char *p = thePlayers[i];
      for (Uint32 j = 0; j < 8; j++)
        p[j] = 0;
    }
  resetvalue ();
}


//------------------------------------------------------------------------------
// destroy object
//------------------------------------------------------------------------------
configfile::~configfile ()
{
}

/**
 * Reset all values
 */
void
configfile::resetvalue ()
{
#ifndef SOUNDISOFF
  handler_audio::is_audio_enable = 1;
#endif
  resolution = 2;
  has_background = false;
  is_verbose = 0;
  handler_display::optionfull = 0;
  difficulty_level = 1;
  initial_num_of_lifes = 8;
  number_of_players = 1;
  char *pUser = getenv ("USER");
  if (!pUser)
    pUser = stringname;
  for (Uint32 i = 0; i < 6; i++)
    strncpy (thePlayers[i], pUser, 6);
  language = LANGUAGE_EN;
}

//------------------------------------------------------------------------------
// display values
//------------------------------------------------------------------------------
void
configfile::configinfo ()
{
  bool audio;
#ifndef SOUNDISOFF
  audio = handler_audio::is_audio_enable;
#else
  audio = false;
#endif
  fprintf (stdout, "  <config info>\n"
           "- optionfull : %i\n- is_audio_enable: %i\n- resolution:%i\n"
           "- is_verbose: %i\n difficulty_level : %i\n",
           handler_display::optionfull, audio, resolution,
           is_verbose, difficulty_level);
}

//------------------------------------------------------------------------------
// check if config directory exists; if not create it and set config_dir 
//------------------------------------------------------------------------------
Sint32
configfile::tocheckdir ()
{
#ifdef _WIN32

  // opendir don't exist on windows
  // create directory if not exist
  MKDIR (config_dir, S_IRWXU);

#else
  /* test and create .tlkgames */
  if (!opendir (config_dir))
    {
      fprintf (stderr, "couldn't find/open config directory '%s'\n",
               config_dir);
      fprintf (stderr, "attempting to create it... ");
      MKDIR (config_dir, S_IRWXU);
      if (!opendir (config_dir))
        {
          fprintf (stderr, "failed\n");
          return 0;
        }
      else
        fprintf (stderr, "ok\n");
    }
#endif
  return 1;
}

//------------------------------------------------------------------------------
// load config file "~/.tlkgames/tecnoballz.conf"
//------------------------------------------------------------------------------
void
configfile::loadconfig ()
{
  //reset all values
  resetvalue ();

  //generate user directory name
#ifdef _WIN32
  _snprintf (config_dir, sizeof (config_dir) - 1, "%s/%s",
             (getenv ("HOME") ? getenv ("HOME") : "."), CONFIG_DIR_NAME);
#else
  snprintf (config_dir, sizeof (config_dir) - 1, "%s/%s",
            (getenv ("HOME") ? getenv ("HOME") : "."), CONFIG_DIR_NAME);
#endif

  //read config from user directory       
  FILE *pfile = NULL;
  sprintf (configname, "%s/%s", config_dir, CONFIG_FILE_NAME);
  pfile = fopen_data (configname, "r");
  if (pfile == NULL)
    return;

  lisp_stream_t stream;
  lisp_object_t *root_obj = NULL;
  lisp_stream_init_file (&stream, pfile);
  root_obj = lisp_read (&stream);

  if (root_obj->type == LISP_TYPE_EOF
      || root_obj->type == LISP_TYPE_PARSE_ERROR)
    {
      fprintf (stderr, "configfile::loadconfig() / conf parsing failed\n");
      return;
    }

  if (strcmp (lisp_symbol (lisp_car (root_obj)), "tecnoballz-config") != 0)
    {
      fprintf (stderr, "configfile::loadconfig() / conf parsing failed\n");
      return;
    }
  LispReader reader (lisp_cdr (root_obj));

  std::string ptStr;
  if (!reader.read_string ("lang", &ptStr))
    language = LANGUAGE_EN;
  else
    {
      if (ptStr == "fr")
        language = LANGUAGE_FR;
      else
        language = LANGUAGE_EN;
    }
 


  if (!reader.read_bool ("fullscreen", &handler_display::optionfull))
    handler_display::optionfull = -1;
#ifndef SOUNDISOFF
  if (!reader.read_bool ("sound", &handler_audio::is_audio_enable))
    handler_audio::is_audio_enable = 1;
#endif
  if (!reader.read_bool ("verbose", &is_verbose))
    is_verbose = 0;

  /* read window resolution: 1 = 320*240; 2 = 640*480 */
  Sint32 res = 0;
  if (!reader.read_int ("resolution", &res))
    res = 2;
  resolution = res;
  if (resolution < 1 || resolution > 2)
    resolution = 2;
  if (resolution == 2)
    {
      has_background = false;
    }
  else
    {
      has_background = true;
    }
  has_background = false;
  //read number of lifes: 1 to 9
  if (!reader.read_int ("lifes", &initial_num_of_lifes))
    initial_num_of_lifes = 8;
  if (initial_num_of_lifes < 1 || initial_num_of_lifes > 9)
    initial_num_of_lifes = 8;

  //read difficulty 1 (easy), 2 (hard), 3 (madness) or 4 (suicidal)
  if (!reader.read_int ("difficulty", &difficulty_level))
    difficulty_level = 1;
  if (difficulty_level < 1 || difficulty_level > 4)
    difficulty_level = 1;

  //read number of players: 1 to 6
  if (!reader.read_int ("players", &number_of_players))
    number_of_players = 1;
  if (number_of_players < 1 || number_of_players > 6)
    number_of_players = 1;

  //read players names
  std::string sName (6, ' ');
  char cName[8] = { "......." };
  for (Uint32 i = 0; i < 6; i++)
    {                           //sprintf(cName, "%s%01d", "player", i + 1);
      sprintf (cName, "player%01d", i + 1);
      if (reader.read_string (cName, &sName))
        {                       //printf("LOAD : <%s> => <%s> \n", cName, sName.c_str());
          strncpy (thePlayers[i], sName.c_str (), 6);
        }

    }
  lisp_free (root_obj);
  //for(Uint32 i = 0; i < 6; i++) 
  //      printf("name %i: %s\n", i,thePlayers[i]);
}

//------------------------------------------------------------------------------
// return current player name, which read from config file
//------------------------------------------------------------------------------
char *
configfile::get_player (Uint32 nplay)
{
  if (nplay >= 6)
    nplay = 6 - 1;
  return thePlayers[nplay];
}

//------------------------------------------------------------------------------
// set player name
//------------------------------------------------------------------------------
void
configfile::set_player (Uint32 nplay, char *pChar)
{
  if (nplay >= 6)
    nplay = 6 - 1;
  strncpy (thePlayers[nplay], pChar, 6);
}

/*
 * Return current language
 * @return "en" or fr
 */
const char *
configfile::get_language ()
{
    return language_to_string[language];
}



//------------------------------------------------------------------------------
// save config file "~/.tlkgames/tecnoballz.conf"
//------------------------------------------------------------------------------
void
configfile::saveconfig ()
{
  bool audio;
#ifndef SOUNDISOFF
  audio = handler_audio::is_audio_enable;
#else
  audio = false;
#endif
  if (!tocheckdir ())
    return;
  FILE *config = fopen_data (configname, "w");
  if (config)
    {
      fprintf (config, "(tecnoballz-config\n");
      fprintf (config,
               "\t;; the following options can be set to #t or #f:\n");
      fprintf (config, "\t(fullscreen %s)\n",
               handler_display::optionfull ? "#t" : "#f");
      fprintf (config, "\t(sound %s)\n", audio ? "#t" : "#f");
      fprintf (config, "\t(verbose %s)\n", is_verbose ? "#t" : "#f");
      fprintf (config, "\n\t;; window size 1 (low-res) or 2 (high-res):\n");
      fprintf (config, "\t(resolution  %d)\n", resolution);
      fprintf (config,
               "\n\t;; difficulty 1 (easy), 2 (hard), 3 (madness) or 4 (suicidal)\n");
      fprintf (config, "\t(difficulty   %d)\n", difficulty_level);
      fprintf (config, "\n\t;; number of lifes (1 to 9)\n");
      fprintf (config, "\t(lifes   %d)\n", initial_num_of_lifes);
      fprintf (config, "\n\t;; number of players (1 to 6)\n");
      fprintf (config, "\t(players   %d)\n", number_of_players);
      fprintf (config, "\n\t;; players names\n");
      for (Uint32 i = 0; i < 6; i++)
        fprintf (config, "\t(player%i      \"%s\")\n", i + 1, thePlayers[i]);
     fprintf (config, "\n\t;; language en or fr\n");
      fprintf (config, "\t(lang      ");
      switch (language)
        {
        case LANGUAGE_FR:
          fprintf (config, "\"fr\")\n");
          break;
        default:
          fprintf (config, "\"en\")\n");
          break;
        }
      fprintf (config, ")\n");

    }

}

//------------------------------------------------------------------------------
// open a file
//------------------------------------------------------------------------------
FILE *
configfile::fopen_data (const char *fname, const char *fmode)
{
  FILE *fi;
  fi = fopen (fname, fmode);
  if (fi == NULL)
    {
      fprintf (stderr, "configfile::fopen_data(): Warning: Unable "
               "to open the file \"%s\" ", fname);
      if (strcmp (fmode, "r") == 0)
        fprintf (stderr, "for read!!!\n");
      else if (strcmp (fmode, "w") == 0)
        fprintf (stderr, "for write!!!\n");
    }
  return (fi);
}

//------------------------------------------------------------------------------
// analyse command line paramaters
//------------------------------------------------------------------------------
Sint32
configfile::scanZeArgs (Sint32 nbArg, char **ptArg)
{
  Sint32 _iIndex;
  for (_iIndex = 1; _iIndex < nbArg; _iIndex++)
    {
      if (*ptArg[_iIndex] != '-')
        continue;

      if (!strcmp (ptArg[_iIndex], "-h") ||
          !strcmp (ptArg[_iIndex], "--help"))
        {
          printf ("\noptions:\n");
          printf ("-h, --help     print Help (this message) and exit\n");
          printf ("--version      print version information and exit\n");
          printf ("--full         full screen\n");
          printf ("--320          game run in a 320*200 window\n");
          printf ("--verbose      verbose mode\n");
          printf ("--nosound      force no sound\n");
          printf
            ("--------------------------------------------------------------\n");
          printf ("keys recognized during the game:\n");
          printf ("CTRL+S         enable/disable sound\n");
          printf ("CTRL+D         enable/disable music\n");
          printf ("CTRL+F         enable/disable sound effects\n");
          printf ("CTRL+X         finish the play current\n");
          printf ("CTRL+Q         return to the main menu\n");
          printf ("CTRL+ESC       quit TecnoballZ\n");
          printf ("P              enable/disable pause\n");
          printf ("F              full screen/window mode\n");
          printf ("\n");
          return 0;
        }

      if (!strcmp (ptArg[_iIndex], "--version"))
        {
          printf (TECNOBALLZ_VERSION);
          printf ("\n");
          printf ("copyright (c) 1991-2006 TLK Games\n");
          printf ("website: http://linux.tlk.fr\n");
          return 0;
        }

      if (!strcmp (ptArg[_iIndex], "--full"))
        {
          handler_display::optionfull = true;
          continue;
        }

      if (!strcmp (ptArg[_iIndex], "--window"))
        {
          handler_display::optionfull = false;
          continue;
        }

      if (!strcmp (ptArg[_iIndex], "--verbose") ||
          !strcmp (ptArg[_iIndex], "-v"))
        {
          is_verbose = true;
          continue;
        }

      if (!strcmp (ptArg[_iIndex], "--320"))
        {
          resolution = 1;
          continue;
        }
      if (!strcmp (ptArg[_iIndex], "--640"))
        {
          resolution = 2;
          continue;
        }

      if (!strcmp (ptArg[_iIndex], "--nosync"))
        {
          handler_display::optionsync = false;
          continue;
        }
#ifndef SOUNDISOFF
      if (!strcmp (ptArg[_iIndex], "--sound"))
        {
          handler_audio::is_audio_enable = true;
          continue;
        }
      if (!strcmp (ptArg[_iIndex], "--nosound"))
        {
          handler_audio::is_audio_enable = false;
          continue;
        }
#endif
      // use 4 colors background in 640x480
      if (!strcmp (ptArg[_iIndex], "--bg4"))
        {
          force_4_colors_tiles = true;
          continue;
        }

      //start at brick or guard level
      if (!strcmp (ptArg[_iIndex], "--guard"))
        {
          arg_jumper = 3;
          continue;
        }
      if (!strcmp (ptArg[_iIndex], "--brick"))
        {
          arg_jumper = 1;
          continue;
        }
    }
  return 1;
}

char
  configfile::stringname[7] = "YANIS ";
