/** 
 * @file level_data.cc 
 * @brief manage levels data 
 * @created 2004-04-06 
 * @date 2007-03-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: level_data.h,v 1.3 2007/03/01 21:14:20 gurumeditation Exp $
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
#ifndef __LEVEL_DATA__
#define __LEVEL_DATA__

class level_data;

#include "../include/tecnoballz.h"
#include "../include/tinyxml.h"

typedef struct
{
  Sint16 level_type;            // 0 = bricks level
  Sint16 atom1Count;            // Temps appartion Atom 1
  Sint16 atom2Count;            // Temps appartion Atom 2
  Sint16 atom3Count;            // Temps appartion Atom 3
  Sint16 atom4Count;            // Temps appartion Atom 4
  Sint16 apparition;            // Temps de reappartion des Atoms
  Sint16 resistance;            // Resistance des atoms
  Sint16 malusCount;            // Frequence d'appartion des malus
  Sint16 monayCount;            // Frequence d'appartion des capsules
  const Sint16 *malusListe;     // pointreur sur la liste des malus
  Sint16 speedBall1;            // pointeur sur la premiere table de vitesse
  Sint16 speedCount;            // Temps avant l'acceleration de la balle
  Sint16 startCount;            // Temps avant que la balle ne parte
  Sint16 glue_count;            // Temps que dure la colle
  Sint16 tilt_count;            // Temps avant que le tilt soit possible
}
amigaLevel;

typedef struct
{
  Sint16 level_type;            // 1 = guard level
  Sint16 speedBall1;            // Pointeur sur la table des vitesses  
  Sint16 startCount;            // Temps avant que la balle parte
  Sint16 tilt_count;            // Temps avant d'autoriser le tilt
  Sint16 scrolCount;            // Temps minimum avant le defilement
  Sint16 scrollType;            // type of background scrolling
  Sint16 malusCount;            // Frequence des gadgets
  const Sint16 *malusListe;     // Table des gagdets
}
atariLevel;

class level_data:public virtual tecnoballz
{

public:
  static const Uint32 NUMOFAREAS = 5;
  static const Uint32 NUMOFLEVEL = 12;

public:
    level_data ();
   ~level_data ();
  const amigaLevel *bricklevel (Uint32, Uint32);
  const atariLevel *guardlevel (Uint32, Uint32);

private:
  static const Sint16 zeCourseXX[];
  static const Sint16 zeCourse10[];
  static const Sint16 zeCourse11[];
  static const Sint16 zeCourse12[];
  static const Sint16 zeCourse20[];
  static const Sint16 zeCourse30[];
  static const Sint16 zeCourse40[];
  static const Sint16 zeCourse50[];
  static const Sint16 zeCourse55[];
  static const amigaLevel amigaTab10;
  static const amigaLevel amigaTab11;
  static const amigaLevel amigaTab12;
  static const amigaLevel amigaTab20;
  static const amigaLevel amigaTab30;
  static const amigaLevel amigaTab40;
  static const amigaLevel amigaTab50;
  static const amigaLevel amigaTab55;
  static const atariLevel atariTab00;
  static const atariLevel atariTab04;
  static const atariLevel atariTab08;
  static const atariLevel atariTab12;
  static const atariLevel atariTab16;
  static const atariLevel atariTab20;
  static const atariLevel atariTab24;
  static const atariLevel atariTab28;
  static const atariLevel atariTab32;
  static const atariLevel atariTab36;
  static const atariLevel atariTab40;
  static const amigaLevel *giga_amiga[];


  TiXmlDocument xml_levels;


};


#endif
