/** 
 * @file handler_levels.cc 
 * @brief manage levels data 
 * @created 2004-04-06 
 * @date 2007-03-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_levels.h,v 1.2 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __LEVEL_DATA__
#define __LEVEL_DATA__

class handler_levels;

#include "../include/tecnoballz.h"
#include "../include/tinyxml.h"
#include <string.h>

typedef struct
{
  /* 0 = bricks level */
  Uint32 level_type;
  Uint32 id;
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
  /** List of the capsules */
  const Uint32 *malusListe;
  Uint32 starting_speed;        // pointeur sur la premiere table de vitesse
  /** Delay time before the ball accelerates */
  Uint32 acceleration_delay;
  /** Delay time before the ball leaves paddle, at the level beginning */
  Uint32 ball_release_time;
  /** Delay time before the ball leaves paddle with glue option */
  Uint32 glue_time;
  /** Delay time before tilt is available */
  Uint32 tilt_delay;
}
bricks_level_desc;


typedef struct
{
  /* 1 = guard level */
  Uint32 level_type;
  Uint32 id;
  Uint32 starting_speed;
  Uint32 ball_release_time;
  Uint32 tilt_delay;
  /** Delay time before the scrolling starts */
  Uint32 scroll_delay;
  /** Determines the behavior of the scrolling */
  Uint32 scroll_id;
  /** Appearance frequency of the capsules */
  Uint32 capsules_frequency;
  /** List of the capsules */
  const Uint32 *capsules_list;
}
guardians_level_desc;

typedef struct
{
  Uint32 id;
  Uint32 type;
}
level_desc;



class handler_levels:public virtual tecnoballz
{

public:
  static const Uint32 MAX_OF_AREAS = 5;
  static const Uint32 NUM_OF_LEVELS_PER_AREA = 12;
  static const Uint32 TIME_MULTIPLIER = 50;
  static const Uint32 MAX_OF_CASPULES = 64;


  typedef struct
  {
    Uint32 id;
    Uint32 codes[handler_levels::MAX_OF_CASPULES];
  } capsules_struct;


public:
    handler_levels ();
   ~handler_levels ();
  const bricks_level_desc *get_bricks_levels_params (Uint32, Uint32);
  const guardians_level_desc *get_guardians_levels_params (Uint32, Uint32);
private:


  typedef enum
  {
    BRICKS_LEVEL,
    GUARDIANS_LEVEL,
    MAX_OF_LEVEL_TYPES
  }
  LEVEL_TYPES;

  typedef enum
  {
    ROOT,
    LEVEL_NODE,
    BRICKS_LEVEL_NODE,
    CAPSULES_NODE,
    GUARDIANS_LEVEL_NODE
  }
  NODES_ENUM;

  void check_levels ();
  void check_xml (TiXmlNode * parent, Uint32 node);
  void parse (TiXmlNode * parent, Uint32 node);

  Uint32 *get_capsules_list (Uint32 id);
  guardians_level_desc *get_guardians_level (Uint32 id);
  bricks_level_desc *get_bricks_level (Uint32 id);

private:
    Uint32 time_multiplier;

  Uint32 levels_counter;
  Uint32 bricks_levels_counter;
  Uint32 guardians_levels_counter;
  Uint32 capsules_lists_counter;
  Uint32 capsules_counter;

  Sint32 level_index;
  Sint32 bricks_level_index;
  Sint32 guardians_level_index;
  Sint32 appearance_index;
  Sint32 capsule_list_index;
  Sint32 capsule_index;

    std::string last_element;

private:
/*
  static const Uint32 zeCourseXX[];
  static const Uint32 zeCourse10[];
  static const Uint32 zeCourse11[];
  static const Uint32 zeCourse12[];
  static const Uint32 zeCourse20[];
  static const Uint32 zeCourse30[];
  static const Uint32 zeCourse40[];
  static const Uint32 zeCourse50[];
  static const Uint32 zeCourse55[];
  static const bricks_level_desc amigaTab10;
  static const bricks_level_desc amigaTab11;
  static const bricks_level_desc amigaTab12;
  static const bricks_level_desc amigaTab20;
  static const bricks_level_desc amigaTab30;
  static const bricks_level_desc amigaTab40;
  static const bricks_level_desc amigaTab50;
  static const bricks_level_desc amigaTab55;
  static const guardians_level_desc atariTab00;
  static const guardians_level_desc atariTab04;
  static const guardians_level_desc atariTab08;
  static const guardians_level_desc atariTab12;
  static const guardians_level_desc atariTab16;
  static const guardians_level_desc atariTab20;
  static const guardians_level_desc atariTab24;
  static const guardians_level_desc atariTab28;
  static const guardians_level_desc atariTab32;
  static const guardians_level_desc atariTab36;
  static const guardians_level_desc atariTab40;
  static const bricks_level_desc *giga_amiga[];
  */


    TiXmlDocument * xml_levels;

  guardians_level_desc *guardians_levels;
  bricks_level_desc *bricks_levels;
  capsules_struct *caspsules_list;
  level_desc *levels_list;


};



#endif
