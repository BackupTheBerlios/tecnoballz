/** 
 * @file configfile.h
 * @brief Config file handler 
 * @created 2005-01-22
 * @date 2007-02-17
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.9 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: configfile.h,v 1.9 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __CONFIGFILE__
#define __CONFIGFILE__

#include <SDL/SDL.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <dirent.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include "../include/tecnoballz.h"
#include "../include/lispreader.h"

#ifdef _WIN32
#define MKDIR(d, p) mkdir(d)
#else
#define MKDIR mkdir
#endif
#ifdef _WIN32
#define CONFIG_DIR_NAME "tlkgames"
#else
#define CONFIG_DIR_NAME ".tlkgames"
#endif
#define CONFIG_FILE_NAME "tecnoballz.conf"

class configfile:public virtual tecnoballz
{
public:
  /*bool          fullscreen;     //0=windowed mode / 1=full screen
     bool         nosound;        //1=force no sound
     bool         resolution;     //320 or 640
     bool         is_verbose;     //1=verbose mode 
     Uint32               difficulty; */

private:
  typedef enum
  {
    LANGUAGE_EN,
    LANGUAGE_FR,
    MAX_OF_LANGUAGES
  }
  LANGUAGES_ENUM;


  static const char *language_to_string[MAX_OF_LANGUAGES]; 
  Uint32 language;
  static char stringname[7];
  char config_dir[512];
  char configname[512];
  char thePlayer1[7];
  char thePlayer2[7];
  char thePlayer3[7];
  char thePlayer4[7];
  char thePlayer5[7];
  char thePlayer6[7];
  char thePlayer7[7];
  char *thePlayers[6];

public:
    configfile ();
   ~configfile ();
  void configinfo ();
  void loadconfig ();
  void saveconfig ();
  Sint32 scanZeArgs (Sint32 nbArg, char **ptArg);
  char *get_player (Uint32);
  void set_player (Uint32 nplay, char *pChar);
  const char * get_language ();

private:
  Sint32 tocheckdir ();
  void resetvalue ();
  FILE *fopen_data (const char *rel_filename, const char *mode);

};
#endif
