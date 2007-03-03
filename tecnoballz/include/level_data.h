/** 
 * @file level_data.cc 
 * @brief manage levels data 
 * @created 2004-04-06 
 * @date 2007-03-03
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: level_data.h,v 1.5 2007/03/03 20:59:04 gurumeditation Exp $
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
#include <string.h>

typedef struct
{
  /* 0 = bricks level */
  Uint32 level_type;
  /** Delay time before first appearance of ship 1 and 5 */
  Uint32 ship_appearance_delay1;
  /** Delay time before first appearance of ship 2 and 6 */
  Uint32 ship_appearance_delay2;
  /** Delay time before first appearance of ship 3 and 7 */
  Uint32 ship_appearance_delay3;
  /** Delay time before first appearance of ship 4 and 8 */
  Uint32 ship_appearance_delay4;
  /** Delay time of the reappearance of any ship after its destruction */
  Uint32 reappearance;
  /** strength of the ships */
  Uint32 ships_strength;
  /** Appearance frequency of the penalty capsules */
  Uint32 penalties_frequency;
  /** Appearance frequency of the monay capsules */
  Uint32 moneys_frequency;
  const Sint16 *malusListe;     // pointreur sur la liste des malus
  Uint32 starting_speed;            // pointeur sur la premiere table de vitesse
  /** Delay time before the ball accelerates */
  Uint32 acceleration_delay;
  /** Delay time before the ball leaves paddle, at the level beginning */
  Uint32 ball_release_time;
  /** Delay time before the ball leaves paddle with glue option */
  Uint32 glue_time;
  /** Delay time before tilt is available */
  Uint32 tilt_delay;
}
bricks_levels_desc;


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
  static const Uint32 TIME_MULTIPLIER = 50;
  static const Uint32 MAX_OF_CASPULES = 64;


typedef struct
{
  Uint32 id;
  Uint32 capsules[level_data::MAX_OF_CASPULES];
} capsules_struct;


public:
    level_data ();
   ~level_data ();
  const bricks_levels_desc *bricklevel (Uint32, Uint32);
  const atariLevel *guardlevel (Uint32, Uint32);
private:
   typedef enum 
     {
       ROOT,
       LEVEL_NODE,
       BRICKS_LEVEL_NODE,
       CAPSULES_NODE  
     }
     NODES_ENUM;

   void check_xml (TiXmlNode* parent, Uint32 node);
   void parse (TiXmlNode* parent, Uint32 node);

 private:
     Uint32 time_multiplier;
     
     Uint32 levels_counter;
     Uint32 bricks_levels_counter;
     Uint32 capsules_lists_counter;
     Uint32 capsules_counter;
     
     Sint32 level_index;
     Sint32 bricks_level_index;
     Sint32 appearance_index;
     Sint32 capsule_list_index;
     Sint32 capsule_index;
     
     std::string last_element;

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
  static const bricks_levels_desc amigaTab10;
  static const bricks_levels_desc amigaTab11;
  static const bricks_levels_desc amigaTab12;
  static const bricks_levels_desc amigaTab20;
  static const bricks_levels_desc amigaTab30;
  static const bricks_levels_desc amigaTab40;
  static const bricks_levels_desc amigaTab50;
  static const bricks_levels_desc amigaTab55;
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
  static const bricks_levels_desc *giga_amiga[];


  TiXmlDocument *xml_levels;
  
  bricks_levels_desc* bricks_levels;
  capsules_struct* caspsules_list;


};



#endif
