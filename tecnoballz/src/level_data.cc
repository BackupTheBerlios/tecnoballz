/** 
 * @file level_data.cc 
 * @brief manage levels data 
 * @created 2004-04-06 
 * @date 2007-03-01
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: level_data.cc,v 1.5 2007/03/03 20:59:04 gurumeditation Exp $
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
using namespace std; 
#include "../include/level_data.h"
#include "../include/handler_resources.h"
#include "../include/tinyxml.h"
#include <string.h>
#include <sstream>


/**
 * Create a level data objet
 */
level_data::level_data ()
{
  object_init ();
  time_multiplier = TIME_MULTIPLIER;
  bricks_levels = NULL;
  caspsules_list = NULL;
  levels_counter = 0;
  bricks_levels_counter = 0;
  capsules_lists_counter = 0;
  level_index = -1;
  bricks_level_index = -1;
  appearance_index = -1;
  capsule_list_index = -1;
  capsule_index = -1;
  char *fpath = resources->get_full_pathname (handler_resources::DATA_LEVELS);
  xml_levels = new TiXmlDocument(fpath);
  if (!xml_levels->LoadFile ())
    {
        std::cerr << "(!) level_data::level_data() " << 
          "failed to load file " << fpath << std::endl;
    }

  check_xml (xml_levels, ROOT);


 printf("levels_counter:%i, bricks_levels_counter:%i, capsules_lists_counter:%i\n",
         levels_counter, bricks_levels_counter, capsules_lists_counter);
  bricks_levels = new bricks_levels_desc[bricks_levels_counter];
  caspsules_list = new capsules_struct[capsules_lists_counter]; 

  parse (xml_levels, ROOT);
  //TiXmlHandle handle(xml_levels);
}

void level_data::check_xml (TiXmlNode* parent, Uint32 node) 
{
  
  if (NULL == parent)
    {
      return;
    }
 
  Sint32 type = parent->Type();
  switch ( type )
    {
       case TiXmlNode::ELEMENT:
         {
            string element = parent->Value();
            if (element == "level")
              {
                node = LEVEL_NODE;
                levels_counter++;
                break;
              }
            if (element == "bricks_level")
              {
                node = BRICKS_LEVEL_NODE;
                bricks_levels_counter++;
                break;
              }
            if (element == "capsules")
              {
                printf("capsules_lists_counter:%i, capsules_counter:%i\n", capsules_lists_counter, capsules_counter);
                if (capsules_lists_counter > 0)
                  {
                    if (capsules_counter != MAX_OF_CASPULES)
                      {
                        cerr << "(!)level_data::check_xml() " <<
                          MAX_OF_CASPULES << " <id> childnodes "
                          << " of <capsules> chilnodes expected." 
                          << " But " << capsules_counter <<
                          " found!" << endl;
                        throw runtime_error ("(!)level_data::check_xml()"
                                    "Bad number of <id> childnodes!");
                      }
                  }
                capsules_counter = 0;
                node = CAPSULES_NODE;
                capsules_lists_counter++;
                break;
              }
            if (element == "id" and node == CAPSULES_NODE)
              {
                capsules_counter++;
                break;
              }
         }
       break;
    }
    for (TiXmlNode* child = parent->FirstChild(); NULL != child; child = child->NextSibling()) 
      {
        check_xml (child, node);
      }
}

void level_data::parse (TiXmlNode* parent, Uint32 node)
{
  if (NULL == parent)
    {
      return;
    }
  Sint32 type = parent->Type();
  string value_str;
    
  TiXmlText* text;
  stringstream input_stream;
  Sint32 value;
  switch ( type )
    {
       case TiXmlNode::ELEMENT:
           {
            last_element = parent->Value();
            if (last_element == "level")
              {
                node = LEVEL_NODE;
                level_index++;
                break;
              }
            if (last_element == "bricks_level")
              {
                node = BRICKS_LEVEL_NODE;
                appearance_index = -1;
                bricks_level_index++;
                break;
              }
            if (last_element == "capsules")
              {
                node = CAPSULES_NODE;
                capsule_list_index++;
                capsule_index = -1;
                break;
              }
         }
          break;
       case TiXmlNode::TEXT:
          text = parent->ToText();
          value_str = text->Value();
          input_stream << value_str;
          input_stream >> value;

         switch (node)
           {
           
           
           case LEVEL_NODE:
             break;

           case CAPSULES_NODE:
             if (last_element == "id")
               {
                 capsule_index++;
                 caspsules_list[capsule_list_index].capsules[capsule_index] = value;
               }
             break;

           
           case BRICKS_LEVEL_NODE:
             if (last_element == "id")
               {
               }
             if (last_element == "appearance")
               {
                 appearance_index++;
                 switch (appearance_index)
                   {
                     case 0:
                       bricks_levels[bricks_level_index].ship_appearance_delay1 = value * time_multiplier;
                       break;
                     case 1:
                       bricks_levels[bricks_level_index].ship_appearance_delay2 = value * time_multiplier;
                       break;
                     case 2:
                       bricks_levels[bricks_level_index].ship_appearance_delay3 = value * time_multiplier;
                       break;
                     case 3:
                       bricks_levels[bricks_level_index].ship_appearance_delay4 = value * time_multiplier;
                       break;
                     default:
                       cerr << "(!) level_data::parse() " <<
                         "ranking values must be between 0 and 3 inclusive!" << endl;
                       break;
                   }
               }
             if (last_element == "reappearance")
               {
                 bricks_levels[bricks_level_index].reappearance = value * time_multiplier;
               }
             if (last_element == "strength")
               {
                 bricks_levels[bricks_level_index].ships_strength = value;
               }
             if (last_element == "penalties_frequency")
               {
                 bricks_levels[bricks_level_index].penalties_frequency = value;
               }
             if (last_element == "moneys_frequency")
               {
                 bricks_levels[bricks_level_index].moneys_frequency = value;
               }
             if (last_element == "penalties_list_id")
               {
                 //bricks_levels[bricks_level_index]. = value;
               }
             if (last_element == "starting_speed")
               {
                 bricks_levels[bricks_level_index].starting_speed = value * time_multiplier;
               }
             if (last_element == "acceleration_delay")
               {
                 bricks_levels[bricks_level_index].acceleration_delay = value * time_multiplier;
               }
             if (last_element == "ball_release_time")
               {
                 bricks_levels[bricks_level_index].ball_release_time = value * time_multiplier;
               }
             if (last_element == "glue_time")
               {
                 bricks_levels[bricks_level_index].glue_time = value * time_multiplier;
               }
             if (last_element == "tilt_delay")
               {
                 bricks_levels[bricks_level_index].tilt_delay = value * time_multiplier;
               }
            break;
           }



         //text = parent->ToText();
         //printf( "Text: [%s]\n", text->Value() );
         break;
       default:
         break;
    }


    for (TiXmlNode* child = parent->FirstChild(); NULL != child; child = child->NextSibling()) 
      {
        parse (child, node);
      }
}


//-----------------------------------------------------------------------------
// Release the object
//-----------------------------------------------------------------------------
/**
 * Release the level data object
 */
level_data::~level_data ()
{
  if (NULL != bricks_levels)
    {
      delete[]bricks_levels;
      bricks_levels = NULL;
    }
  if (NULL != caspsules_list)
    {
      delete[]caspsules_list;
      caspsules_list = NULL;
    }
}

//-----------------------------------------------------------------------------
// return the data of a bricks level
//-----------------------------------------------------------------------------
const bricks_levels_desc *
level_data::bricklevel (Uint32 area, Uint32 lvnu)
{
  if (area < 1 || area > 5)
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : area %i out of range\n", area);
      area = 1;
    }
  if (lvnu < 1 || lvnu > LEVEL_AREA)
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : level %i out of range\n",
               lvnu);
      lvnu = 1;
    }
  if (lvnu == 6 || lvnu == LEVEL_AREA)
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : level %i is a guard level\n",
               lvnu);
      lvnu--;
    }
  Sint32 i = (area - 1) * LEVEL_AREA + (lvnu - 1);
  return giga_amiga[i];
}

//-----------------------------------------------------------------------------
// return the data of a guard level
//-----------------------------------------------------------------------------
const atariLevel *
level_data::guardlevel (Uint32 area, Uint32 lvnu)
{
  if (area < 1 || area > 5)
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : area %i out of range\n", area);
      area = 1;
    }
  if (lvnu < 1 || (area < 5 && lvnu > LEVEL_AREA) ||
      (area == 5 && lvnu > (LEVEL_AREA + 1)))
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : level %i out of range\n",
               lvnu);
      lvnu = 6;
    }
  if (lvnu != 6 && lvnu != LEVEL_AREA && lvnu != (LEVEL_AREA + 1))
    {
      fprintf (stderr,
               "(!) level_data::bricklevel() : level %i is a bricks level\n",
               lvnu);
      lvnu = 6;
    }
  Sint32 i = (area - 1) * LEVEL_AREA + (lvnu - 1);
  return (atariLevel *) giga_amiga[i];
}


// :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)   (-: (-: (-: (-: (-: (-: (-: (-: (-:   
// Liste des malus/bonus pouvant tomber pendant le jeu
// :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)   (-: (-: (-: (-: (-: (-: (-: (-: (-:   
// Table des gardiens
const Sint16
  level_data::zeCourseXX[] = { 4, 22, 20, 24, 20, 20, 4, 20,    // 4=GAD_PROTEC 22=GAD_POWER1 20=GAD_BALLE3
  20, 4, 20, 24, 20, 4, 22, 4,  // 24=GAD_POWER2 16=GAD_LIFE_P
  20, 22, 4, 16, 4, 24, 22, 20,
  20, 22, 20, 4, 24, 20, 22, 20,
};

// Area 1 : level 1
const Sint16
  level_data::zeCourse10[] =
  { 40, 06, 18, 12, 06, 06, 18, 18, 06, 18, 20, 52, 42, 18, 20, 40,
  18, 40, 50, 40, 42, 44, 54, 06, 22, 18, 06, 06, 52, 40, 18, 06,
  18, 06, 18, 12, 06, 42, 18, 18, 06, 44, 20, 18, 18, 18, 20, 18,
  44, 44, 50, 18, 42, 18, 20, 06, 22, 40, 06, 54, 22, 40, 18, 06
};

// Area 1 : levels 2 to 5
const Sint16
  level_data::zeCourse11[] =
  { 40, 10, 40, 12, 18, 10, 40, 12, 06, 10, 02, 18, 18, 42, 02, 12,
  18, 02, 44, 18, 18, 02, 42, 10, 10, 18, 12, 02, 18, 18, 52, 12,
  40, 10, 18, 12, 40, 10, 40, 12, 06, 46, 02, 18, 18, 42, 02, 12,
  18, 02, 18, 44, 54, 02, 42, 10, 10, 44, 12, 02, 18, 18, 10, 12
};

// Area 1 : levels 7 to 11 
const Sint16
  level_data::zeCourse12[] =
  { 40, 26, 42, 18, 02, 26, 18, 18, 06, 52, 02, 42, 18, 16, 02, 18,
  40, 02, 18, 14, 10, 02, 20, 10, 10, 18, 10, 02, 26, 18, 10, 10,
  18, 26, 10, 18, 02, 26, 54, 44, 06, 46, 02, 10, 44, 16, 02, 44,
  40, 02, 40, 14, 10, 02, 20, 10, 10, 18, 10, 02, 26, 40, 10, 10
};

// Area 2 
const Sint16
  level_data::zeCourse20[] =
  { 42, 26, 10, 10, 10, 42, 18, 44, 10, 14, 10, 10, 10, 10, 10, 10,
  42, 10, 26, 10, 10, 16, 10, 44, 10, 18, 10, 42, 10, 10, 18, 10,
  42, 26, 10, 40, 10, 10, 18, 10, 46, 14, 10, 10, 10, 10, 10, 10,
  42, 10, 26, 10, 10, 16, 44, 54, 10, 18, 10, 10, 42, 10, 18, 10
};

// Area 3
const Sint16
  level_data::zeCourse30[] =
  { 10, 26, 10, 50, 50, 10, 18, 18, 14, 14, 10, 10, 40, 40, 10, 18,
  10, 10, 26, 10, 42, 16, 42, 14, 44, 18, 10, 10, 40, 40, 18, 14,
  10, 26, 10, 44, 46, 10, 54, 18, 14, 14, 40, 40, 10, 10, 10, 18,
  10, 10, 26, 10, 10, 16, 44, 14, 42, 18, 40, 40, 10, 52, 18, 14
};

// Aera 4
const Sint16
  level_data::zeCourse40[] =
  { 10, 26, 26, 20, 42, 44, 20, 10, 44, 14, 26, 10, 40, 40, 10, 10,
  10, 10, 26, 10, 52, 16, 10, 10, 10, 20, 26, 10, 40, 40, 20, 10,
  10, 26, 26, 20, 52, 50, 20, 54, 44, 14, 26, 10, 40, 40, 46, 10,
  10, 10, 26, 10, 42, 16, 10, 50, 10, 20, 26, 10, 40, 40, 20, 10
};

// Area 5  : levels 1 to 5 
const Sint16
  level_data::zeCourse50[] =
  { 42, 26, 26, 10, 10, 10, 20, 44, 10, 14, 26, 10, 10, 10, 20, 10,
  42, 10, 26, 10, 50, 14, 20, 44, 46, 18, 26, 10, 10, 10, 20, 14,
  10, 26, 26, 10, 50, 10, 20, 54, 10, 14, 26, 10, 52, 10, 20, 10,
  10, 10, 26, 10, 10, 14, 20, 44, 10, 18, 26, 10, 52, 10, 20, 14
};

// Area 5 : levels 7 to 11
const Sint16
  level_data::zeCourse55[] =
  { 14, 26, 26, 10, 16, 10, 20, 14, 10, 14, 26, 42, 52, 44, 20, 10,
  14, 10, 26, 44, 18, 14, 20, 10, 10, 18, 26, 42, 52, 10, 20, 14,
  14, 26, 26, 10, 16, 10, 20, 54, 46, 14, 26, 42, 42, 44, 20, 10,
  14, 10, 26, 44, 18, 14, 20, 50, 50, 18, 26, 42, 42, 10, 20, 14
};

// AREA 1 : level 1
const bricks_levels_desc
  level_data::amigaTab10 = { 0, // 0 = bricks level
  1 * 50,                       //time before appearance of the Atom 1
  45 * 50,                      //time before appearance of the Atom 2
  35 * 50,                      //time before appearance of the Atom 3
  10 * 50,                      //time before appearance of the Atom 4
  80 * 50,                      //time before reappearance of the Atoms
  1,                            //resistance
  8,
  2,
  zeCourse10,
  1,
  50 * 99,
  50 * 20,
  50 * 50,
  7 * 50
};

// AREA 1 : level 2 to 5
const bricks_levels_desc
  level_data::amigaTab11 = { 0,
  1 * 50,
  45 * 50,
  25 * 50,
  12 * 50,
  80 * 50,
  1,
  15,
  5,
  zeCourse11,
  1,
  50 * 77,
  50 * 15,
  50 * 40,
  7 * 50
};

// AREA 1 : level 6 to 11
const bricks_levels_desc
  level_data::amigaTab12 = { 0,
  1 * 50,
  35 * 50,
  25 * 50,
  12 * 50,
  70 * 50,
  1,
  17,
  6,
  zeCourse12,
  1,
  50 * 67,
  50 * 10,
  50 * 30,
  7 * 50
};

// AREA 2
const bricks_levels_desc
  level_data::amigaTab20 = { 0,
  1 * 50,
  40 * 50,
  30 * 50,
  10 * 50,
  50 * 50,
  5,
  19,
  7,
  zeCourse20,
  1,
  50 * 67,
  50 * 10,
  50 * 10,
  10 * 50
};

// AREA 3
const bricks_levels_desc
  level_data::amigaTab30 = { 0,
  1 * 50,
  30 * 50,
  20 * 50,
  10 * 50,
  40 * 50,
  10,
  20,
  8,
  zeCourse30,
  1,
  50 * 50,
  50 * 10,
  50 * 10,
  10 * 50
};

// AREA 4
const bricks_levels_desc
  level_data::amigaTab40 = { 0,
  1 * 50,
  21 * 50,
  14 * 50,
  17 * 50,
  30 * 50,
  20,
  15,
  8,
  zeCourse40,
  1,
  50 * 50,
  50 * 10,
  50 * 7,
  111 * 50
};

// AREA 5 : level 1 to 5
const bricks_levels_desc
  level_data::amigaTab50 = { 0,
  1 * 50,
  15 * 50,
  10 * 50,
  05 * 50,
  20 * 50,
  30,
  15,
  8,
  zeCourse50,
  1,
  50 * 40,
  50 * 10,
  50 * 7,
  11 * 50
};

// AREA 5 : level 7 to 11
const bricks_levels_desc
  level_data::amigaTab55 = { 0,
  1 * 50,
  4 * 50,
  3 * 50,
  2 * 50,
  5 * 50,
  50,
  15,
  15,
  zeCourse55,
  2,
  50 * 30,
  50 * 10,
  50 * 5,
  15 * 50
};

//#######################################################################
// guards levls
//#######################################################################
// area 1 : level 6
const atariLevel
  level_data::atariTab00 = { 1, // 1 = guards levels
  3,                            // speed ball
  50 * 20,                      // time before ball start
  10 * 50,                      // time before enable tilt
  10 * 50,                      // time before scroll start
  1,                            // scroll type
  7,                            // frequency of gadgets
  zeCourseXX
};

// Area  1 : Level 12
const atariLevel
  level_data::atariTab04 = { 1,
  3,
  50 * 115,
  10 * 50,
  10 * 50,
  2,
  8,
  zeCourseXX
};

// Area 2 : Level 6
const atariLevel
  level_data::atariTab08 = { 1,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  3,
  9,
  zeCourseXX
};

// Area 2 : Level 12
const atariLevel
  level_data::atariTab12 = { 1,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  4,
  9,
  zeCourseXX
};

// Area 3 : Level 6
const atariLevel
  level_data::atariTab16 = { 1,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  5,
  10,
  zeCourseXX
};

// Area 3 : Level 12
const atariLevel
  level_data::atariTab20 = { 1,
  4,
  50 * 10,
  10 * 50,
  3 * 50,
  6,
  10,
  zeCourseXX
};

// Area 4 : Level 6
const atariLevel
  level_data::atariTab24 = { 1,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  7,
  12,
  zeCourseXX
};

// Area 4 : Level 12
const atariLevel
  level_data::atariTab28 = { 1,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  8,
  12,
  zeCourseXX
};

// Area 5 : Level 6
const atariLevel
  level_data::atariTab32 = { 1,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  9,
  15,
  zeCourseXX
};

// Area 5 : Level 12
const atariLevel
  level_data::atariTab36 = { 1,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  10,
  15,
  zeCourseXX
};

// Area 5 : Level 13
const atariLevel
  level_data::atariTab40 = { 1,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  11,
  15,
  zeCourseXX
};


// Une table par chacun des 61 niveaux
const bricks_levels_desc *level_data::giga_amiga[] =
{
  &amigaTab10,     // area 1 ; level 1
  &amigaTab11,                  // area 1 ; level 2
  &amigaTab11,                  // area 1 ; level 3
  &amigaTab11,                  // area 1 ; level 4
  &amigaTab11,                  // area 1 ; level 5
  (bricks_levels_desc *) & atariTab00,  // area 1 ; level 6
  &amigaTab12,                  // area 1 ; level 7
  &amigaTab12,                  // area 1 ; level 8
  &amigaTab12,                  // area 1 ; level 9
  &amigaTab12,                  // area 1 ; level 10
  &amigaTab12,                  // area 1 ; level 11
  (bricks_levels_desc *) & atariTab04,  // area 1 ; level 12
  &amigaTab20,                  // area 2 ; level 1
  &amigaTab20,                  // area 2 ; level 2
  &amigaTab20,                  // area 2 ; level 3
  &amigaTab20,                  // area 2 ; level 4
  &amigaTab20,                  // area 2 ; level 5
  (bricks_levels_desc *) & atariTab08,  // area 2 ; level 6
  &amigaTab20,                  // area 2 ; level 7
  &amigaTab20,                  // area 2 ; level 8
  &amigaTab20,                  // area 2 ; level 9
  &amigaTab20,                  // area 2 ; level 10
  &amigaTab20,                  // area 2 ; level 11
  (bricks_levels_desc *) & atariTab12,  // area 2 ; level 12
  &amigaTab30,                  // area 3 ; level 1
  &amigaTab30,                  // area 3 ; level 2
  &amigaTab30,                  // area 3 ; level 3
  &amigaTab30,                  // area 3 ; level 4
  &amigaTab30,                  // area 3 ; level 5
  (bricks_levels_desc *) & atariTab16,  // area 3 ; level 6
  &amigaTab30,                  // area 3 ; level 7
  &amigaTab30,                  // area 3 ; level 8
  &amigaTab30,                  // area 3 ; level 9
  &amigaTab30,                  // area 3 ; level 10
  &amigaTab30,                  // area 3 ; level 11
  (bricks_levels_desc *) & atariTab20,  // area 3 ; level 12
  &amigaTab40,                  // area 4 ; level 1
  &amigaTab40,                  // area 4 ; level 2
  &amigaTab40,                  // area 4 ; level 3
  &amigaTab40,                  // area 4 ; level 4
  &amigaTab40,                  // area 4 ; level 5
  (bricks_levels_desc *) & atariTab24,  // area 4 ; level 6
  &amigaTab40,                  // area 4 ; level 7
  &amigaTab40,                  // area 4 ; level 8
  &amigaTab40,                  // area 4 ; level 9
  &amigaTab40,                  // area 4 ; level 10
  &amigaTab40,                  // area 4 ; level 11
  (bricks_levels_desc *) & atariTab28,  // area 4 ; level 12
  &amigaTab50,                  // area 5 ; level 1
  &amigaTab50,                  // area 5 ; level 2
  &amigaTab50,                  // area 5 ; level 3
  &amigaTab50,                  // area 5 ; level 4
  &amigaTab50,                  // area 5 ; level 5
  (bricks_levels_desc *) & atariTab32,  // area 5 ; level 6
  &amigaTab55,                  // area 5 ; level 7
  &amigaTab55,                  // area 5 ; level 8
  &amigaTab55,                  // area 5 ; level 9
  &amigaTab55,                  // area 5 ; level 10
  &amigaTab55,                  // area 5 ; level 11
  (bricks_levels_desc *) & atariTab36,  // area 5 ; level 12
  (bricks_levels_desc *) & atariTab40   // area 5 ; level 13
};
