/** 
 * @file handler_resources.h
 * @brief Handler of the files resources 
 * @created 2004-04-20 
 * @date 2007-01-29
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_resources.h,v 1.2 2007/01/29 16:25:22 gurumeditation Exp $
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
#ifndef __RESSOURCES__
#define __RESSOURCES__
#include "../include/mentatCode.h"
#include "../include/handler_memory.h"

class handler_resources:public virtual mentatCode
{
private:
  static const char *folderlist[];
  static const char *graphfiles[];
  static const char *standfiles[];
  static const char *musicfiles[];
  static const char *soundfiles[];
  static const char *folderdata;
  static const char *folder_320;
  static const char *folder_640;
  static char *fnamescore;
  static char stringtemp[512];
  static char pathstring[512];
  static char ze_mapfile[];

public:
  static const Uint32 RESCOSLIST = 0;
  static const Uint32 RESEDMAP01 = 1;
  static const Uint32 RESEDMAP02 = 2;
  static const Uint32 RESEDMAP03 = 3;
  static const Uint32 RESGCURVES = 4;
  static const Uint32 RESBLEVELS = 5;
  static const Uint32 RES60BACKG = 6;

  static const Uint32 RESHEADANI = 4096;
  static const Uint32 RESBASCORE = 4097;
  static const Uint32 RESBUMPER1 = 4098;
  static const Uint32 RESBUMPER2 = 4099;
  static const Uint32 RESFONTGAM = 4100;
  static const Uint32 RESFONTMEN = 4101;
  static const Uint32 RESFONTMES = 4102;
  static const Uint32 RESGIGABLZ = 4103;
  static const Uint32 RESMAPEDIT = 4104;
  static const Uint32 RESMONEYFT = 4105;
  static const Uint32 RESNEWSHOP = 4106;
  static const Uint32 RESPAGEBOB = 4107;
  static const Uint32 RESZEBRICK = 4108;
  static const Uint32 RESFONTSCR = 4109;

  static const Uint32 MUSICAREA1 = 8192;

  static const Sint16 cosinus360[720];
  static const Sint16 *zesinus360;
  static const Uint32 tabledegas[180];

private:
    Uint32 zeLastSize;          //size last file charged in memory

public:
    handler_resources ();
   ~handler_resources ();
  char *load_data (Sint32);
  char *getMusFile (Sint32);
  char *getSndFile (Sint32);
  char *getTexFile (Sint32);
  char *locate_res (Sint32);
  void load_sprites_bitmap (Sint32 resource_id = RESPAGEBOB);
  void release_sprites_bitmap ();
  void writingLog (char *fname, char *strng);
  Uint32 gtLastSize ();
  Sint32 load_sinus ();
  char *loadScores ();
  Sint32 saveScores (char *ptScr, Uint32 fsize);
  char *locate_data_file (const char *const name);

private:
  char *get_filename (Sint32 resource_id);
  char *load_file (char *fname);
  char *load_file (char *fname, Uint32 * fsize);
};

#endif
