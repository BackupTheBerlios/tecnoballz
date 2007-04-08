/** 
 * @file controller_sides_bricks.h
 * @brief Sides bricks controller. The small bricks on the side, the walls top
 *        left and right
 * @date 2007-04-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_sides_bricks.h,v 1.3 2007/04/08 17:28:20 gurumeditation Exp $
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

class controller_sides_bricks:public objects_list < sprite_object,
  controller_sides_bricks >
{
public:
  /** Maximum number of bricks per wall */
  static const Uint32 MAX_OF_SIDES_BRICKS = 12;
private:
  typedef enum 
    {
      TOP_WALL = 1,
      RIGHT_WALL,
      LEFT_WALL
    } WALLS_ENUM;

/**
 * Structure for redraw side bricks
 */
typedef struct
{
  /** Wall identifier TOP_WALL, RIGHT_WALL or LEFT_WALL */
  Uint32 wall_id;
  /** Index of the side brick from 0 to 11 */
  Uint32 side_brick_index;
} restaure_struct;

  static const Uint32 MAX_OF_RESTORED_BRICKS = 64;
  /** X-coordinate of the left wall */
  static const Uint32 XCOORD_LEFT_WALL = 12;
  /** Y-coordinate of the left wall */
  static const Uint32 YCOORD_LEFT_WALL = 24;
  /** X-coordinate of the right wall */
  static const Uint32 XCOORD_RIGHT_WALL = 240;
  /** Y-coordinate of the right wall */
  static const Uint32 YCOORD_RIGHT_WALL = 24;
  /** X-coordinate of the top wall */
  static const Uint32 XCOORD_TOP_WALL = 32;
  /** Y-coordinate of the top wall */
  static const Uint32 YCOORD_TOP_WALL = 4;
  /** X-coordinate of the left wall */
  Uint32 xcoord_left_wall;
  /** Y-coordinate of the left wall */
  Uint32 ycoord_left_wall;
  /** X-coordinate of the right wall */
  Uint32 xcoord_right_wall;
  /** Y-coordinate of the right wall */
  Uint32 ycoord_right_wall;
  /** X-coordinate of the top wall */
  Uint32 xcoord_top_wall;
  /** Y-coordinate of the top wall */
  Uint32 ycoord_top_wall;
  /** Small horizontal side brick */
  sprite_object *horizontal_brick;
  /** Small vertical side brick */
  sprite_object *vertical_brick;
  char *fond_sauve;             //buffer to save background under bricks
  /** Bitmap surface to save background under top bricks */
  bitmap_data *background_top_side;
  /** Bitmap surface to save background under right bricks */
  bitmap_data *background_right_side;
  /** Bitmap surface to save background under left bricks */
  bitmap_data *background_left_side;
  /** True if the wall of the top is breakable */
  bool is_top_wall_breakable;
  /** True if the wall of the right is breakable */
  bool is_right_wall_breakable;
  /** True if the wall of the left is breakable */
  bool is_left_wall_breakable;
  /** Collision x-coordinate of the left wall */
  Sint32 left_collision_xcoord;
  /** Collision x-coordinate of the right wall */
  Sint32 right_collision_xcoord;
  /** Collision y-coordinate of the top wall */
  Sint32 top_collision_ycoord;
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

  restaure_struct *restore_background;       //redraw bricks table
  Uint32 restore_save_index;
  Uint32 restore_index;

public:
    controller_sides_bricks ();
   ~controller_sides_bricks ();
  Sint32 initialize (bool build);
  void save_state_of_walls ();
  void save_background ();
  void run ();
  void draw_shadows_to_brackground ();
  void draw_to_brackground ();
  Sint32 get_top_ycoord ();
  Sint32 get_right_xcoord ();
  Sint32 get_left_xcoord ();
  bool collision_with_left_wall (Sint32 ycoord);
  bool collision_with_right_wall (Sint32 ycoord);
  bool collision_with_top_wall (Sint32 xcoord);

private:
  void bobbg_init ();
  void execution2 ();
  void execution3 ();
};

#endif
