/** 
 * @file controller_sides_bricks.h
 * @brief Sides bricks controller. The small bricks on the side, the walls top
 *        left and right
 * @date 2007-02-14
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_sides_bricks.h,v 1.2 2007/02/14 07:15:30 gurumeditation Exp $
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
#ifndef __CONTROLLER_SIDES_BRICKS__
#define __CONTROLLER_SIDES_BRICKS__

class controller_sides_bricks;

#include "../include/objects_list.h"
#include "../include/sprite_object.h"
#include "../include/tecnoballz.h"
//...............................................................................

//-----------------------------------------------------------------------------
// struct of the redraw bricks table
//-----------------------------------------------------------------------------
typedef struct
{
  Sint32 iCote_type;            //1=bricote haut ; 2=droite ; 3=gauche
  Sint32 iMapOffset;            //pointer to the background buffer
} coteStruct;

class controller_sides_bricks:public objects_list < sprite_object,
  controller_sides_bricks >
{
public:
  /** Maximum number of bricks per wall */
  static const Uint32 MAX_OF_SIDES_BRICKS = 12;

private:
  static const Uint32 BRICOTEGAX = 12;  //x coordinate (bricks of left)
  static const Uint32 BRICOTEGAY = 24;  //y coordinate (bricks of left)
  static const Uint32 BRICOTEDRX = 240; //x coordinate (bricks of right)
  static const Uint32 BRICOTEDRY = 24;  //y coordinate (bricks of right)
  static const Uint32 BRICOTEHRX = 32;  //x coordinate (bricks of top)
  static const Uint32 BRICOTEHRY = 4;   //y coordinate (bricks of top)

  static const Uint32 iMAXBRICOT = 64;

  /** Small horizontal side brick */
  sprite_object *horizontal_brick;
  /** Small vertical side brick */
  sprite_object *vertical_brick;
  char *fond_sauve;             //buffer to save background under bricks
  /** True if the wall of the top is breakable */
  bool is_top_wall_breakable;
  /** True if the wall of the right is breakable */
  bool is_right_wall_breakable;
  /** True if the wall of the left is breakable */
  bool is_left_wall_breakable;
  Sint32 fconstruit;            //1=rebuild wall

  Sint32 collisionG;            //x coordinate of the wall of the left
  Sint32 collisionD;            //x coordinate of the wall of the right
  Sint32 collisionH;            //y coordinate of the wall of the top

  bool map_top_side[MAX_OF_SIDES_BRICKS + 4];
  bool map_right_side[MAX_OF_SIDES_BRICKS + 4];
  bool map_left_side[MAX_OF_SIDES_BRICKS + 4];

  // wall of sprites (bob_ground = 1)
  sprite_object *bobwal_lef[MAX_OF_SIDES_BRICKS + 4];
  sprite_object *bobwal_rgh[MAX_OF_SIDES_BRICKS + 4];
  sprite_object *bobwal_top[MAX_OF_SIDES_BRICKS + 4];

  bool *map_top_wall;
  bool *map_right_wall;
  bool *map_left_wall;

  char *adr_gauche[MAX_OF_SIDES_BRICKS];
  char *adr_droite[MAX_OF_SIDES_BRICKS];
  char *adr_duHaut[MAX_OF_SIDES_BRICKS];

  // state the 3 walls, 2 bricks before and after prevent overflows 
  Sint32 pos_gauche[MAX_OF_SIDES_BRICKS][2];
  Sint32 pos_droite[MAX_OF_SIDES_BRICKS][2];
  Sint32 pos_duHaut[MAX_OF_SIDES_BRICKS][2];

  /** Width of a horizontal side brick in pixels */
  Uint32 horizontal_brick_width;
  /** Height of a horizontal side brick in pixels */
  Uint32 horizontal_brick_height;
  /** Height of a vertical side brick in pixels */
  Uint32 vertical_brick_width;
  /** Height of a vertical side brick in pixels */
  Uint32 vertical_brick_height;

  coteStruct *pCoteTable;       //redraw bricks table
  Sint32 iCoteSauve;
  Sint32 iCoteRemap;

public:
    controller_sides_bricks ();
   ~controller_sides_bricks ();
  Sint32 initialise (Sint32 build);
  void sauve_etat ();
  void sauveFond ();
  void execution1 ();
  void afficheSha ();
  void afficheGfx ();
  Sint32 getCollisH ();
  Sint32 getCollisD ();
  Sint32 getCollisG ();
  bool collision_with_left_wall (Sint32 ycoord);
  bool collision_with_right_wall (Sint32 ycoord);
  bool collision_with_top_wall (Sint32 xcoord);

private:
  void bobbg_init ();
  void execution2 ();
  void execution3 ();
};

#endif
