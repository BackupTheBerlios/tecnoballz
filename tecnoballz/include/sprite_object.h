/** 
 * @file sprite_object.h
 * @brief Draw sprites on the screen 
 * @date 2007-04-09
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.31 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_object.h,v 1.31 2007/04/09 19:55:54 gurumeditation Exp $
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
#ifndef __SPRITE_OBJECT__
#define __SPRITE_OBJECT__
class sprite_object;
#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/handler_memory.h"
#include "../include/tecnoballz.h"
#include "../include/display_text_bitmap.h"

/*
 * sprite's ID
 */
typedef enum
{
  BOB_ATOMES,
  BOB_EJECT1,
  BOB_EJECT2,
  BOB_EJECT3,
  BOB_EJECT4,
  BOB_BRICKV,
  BOB_BRICKH,
  BOB_BRICKS,
  BOB_BUMPHR,
  BOB_BUMPVT,
  BOB_BALLES,
  BOB_FIREBP,
  BOB_MONEYS,
  BOB_GADGET,
  BOB_LETTRE,
  BOB_LEDSHP,
  BOB_GAMEOV,
  BOB_DEFILE,
  BOB_LOGOTB,
  BOB_MOUSE1,
  BOB_MOUSE2,
  BOB_GIGAB1,
  BOB_GIGAB2,
  BOB_GIGAB3,
  BOB_GIGAB4,
  BOB_GIGAB5,
  BOB_GIGAB6,
  BOB_GIGAB7,
  BOB_MONEY0,
  BOB_ROBOT0,
  BOB_FINAL1,
  BOB_FINAL2,
  BOB_FINAL3,
  BOB_FINAL4,
  BOB_FINAL5,
  BOB_FINAL6,
  BOB_INTE1A,
  BOB_INTE1B,
  BOB_INTE2A,
  BOB_INTE2B,
  BOB_INTE3A,
  BOB_INTE3B,
  BOB_INTE4A,
  BOB_INTE4B,
  BOB_INTE5A,
  BOB_INTE5B,
  BOB_BUMPER,
  BOB_MISSIL,
  BOB_EXPLO1,
  BOB_EXPLO2,
  BOB_ARGENT,
  BOB_GRDNRJ,
  BOB_GEMSTO,
  BOB_WALLBO,
  BOB_DIRECT,
  BOB_ESCMEN,
  BOB_MAGEYE,
  BOB_LEDSH2,
  BOB_BRICK1
}
SPRITES_ENUM;

/*
const Sint32 BOB_ATOMES = 0;
const Sint32 BOB_EJECT1 = 1;
const Sint32 BOB_EJECT2 = 2;
const Sint32 BOB_EJECT3 = 3;
const Sint32 BOB_EJECT4 = 4;
const Sint32 BOB_BRICKV = 5;
const Sint32 BOB_BRICKH = 6;
const Sint32 BOB_BRICKS = 7;
const Sint32 BOB_BUMPHR = 8;
const Sint32 BOB_BUMPVT = 9;
const Sint32 BOB_BALLES = 10;
const Sint32 BOB_FIREBP = 11;
const Sint32 BOB_MONEYS = 12;
const Sint32 BOB_GADGET = 13;
const Sint32 BOB_LETTRE = 14;
const Sint32 BOB_LEDSHP = 15;
const Sint32 BOB_GAMEOV = 16;
const Sint32 BOB_DEFILE = 17;
const Sint32 BOB_LOGOTB = 18;
const Sint32 BOB_MOUSE1 = 19;
const Sint32 BOB_MOUSE2 = 20;
const Sint32 BOB_GIGAB1 = 21;
const Sint32 BOB_GIGAB2 = 22;
const Sint32 BOB_GIGAB3 = 23;
const Sint32 BOB_GIGAB4 = 24;
const Sint32 BOB_GIGAB5 = 25;
const Sint32 BOB_GIGAB6 = 26;
const Sint32 BOB_GIGAB7 = 27;
const Sint32 BOB_MONEY0 = 28;
const Sint32 BOB_ROBOT0 = 29;
const Sint32 BOB_FINAL1 = 30;
const Sint32 BOB_FINAL2 = 31;
const Sint32 BOB_FINAL3 = 32;
const Sint32 BOB_FINAL4 = 33;
const Sint32 BOB_FINAL5 = 34;
const Sint32 BOB_FINAL6 = 35;
const Sint32 BOB_INTE1A = 36;
const Sint32 BOB_INTE1B = 37;
const Sint32 BOB_INTE2A = 38;
const Sint32 BOB_INTE2B = 39;
const Sint32 BOB_INTE3A = 40;
const Sint32 BOB_INTE3B = 41;
const Sint32 BOB_INTE4A = 42;
const Sint32 BOB_INTE4B = 43;
const Sint32 BOB_INTE5A = 44;
const Sint32 BOB_INTE5B = 45;
const Sint32 BOB_BUMPER = 46;
const Sint32 BOB_MISSIL = 47;
const Sint32 BOB_EXPLO1 = 48;
const Sint32 BOB_EXPLO2 = 49;
const Sint32 BOB_ARGENT = 50;
const Sint32 BOB_GRDNRJ = 51;
const Sint32 BOB_GEMSTO = 52;
const Sint32 BOB_WALLBO = 53;
const Sint32 BOB_DIRECT = 54;
const Sint32 BOB_ESCMEN = 55;
const Sint32 BOB_MAGEYE = 56;
const Sint32 BOB_LEDSH2 = 57;
const Sint32 BOB_BRICK1 = 58;
*/
/* const Sint32	BOB_BRICK2 = 59;
const Sint32	BOB_BRICK3 = 60;
const Sint32	BOB_BRICK4 = 61;
const Sint32	BOB_BRICK5 = 62;
const Sint32	BOB_BRICK6 = 63;
const Sint32	BOB_BRICK7 = 64;
const Sint32	BOB_BRICK8 = 65;
const Sint32	BOB_BRICK9 = 66; */

typedef struct
{
  Sint16 BB_COORDX;             // real x = BB_COORDX*16
  Sint16 BB_COORDY;             // real y = BB_COORDY*2
}
bbPosition;

typedef struct
{
  Sint16 BB_LARGEUR;            // real width = BB_LARGEUR * 2
  Sint16 BB_HAUTEUR;            // real height = BB_HAUTEUR * 2
  Sint16 BB_ANIMATE;            // number of animation(s)
  bbPosition *BBPOSITION;
}
bb_describ;

typedef struct
{
  Sint32 COUNTERTAB;            // size of table offsets / loops 
  Sint32 OFFSETLEFT;
  Sint16 *TABAFFICH1;           // table of offsets and loops counters
  char *TABAFFICH2;             // table of pixels
}
bb_afligne;

class sprite_object:public virtual tecnoballz
{
  friend class controller_game_over;
  friend class controller_explosions;
  friend class controller_guardians;
  friend class controller_gems;
  friend class controller_bullets;
  friend class controller_spheres;
  friend class display_text_bitmap;

public:

  typedef enum {
    DRAW_WITH_TABLES,
    COPY_FROM_BITMAP,
    DRAW_LINE_BY_LINE,
    DRAW_REPEAT_SPRITE,
    DRAW_COLOR_CYCLING_MASK,
    CYCLE_PTAB
  }
  DRAW_METHOD_ENUM;

private:
  /** Maximum number of images in the animation */
  Sint32 max_of_images;
  /** List of pointers of all images of the sprite
   * in the source bitmap page */
  char **images_pixel_data;
  /** Pointer to sprite in game offscreen */
  char *screen_ptr;
  /** Pointer to the background in restore offscreen */
  char *restore_ptr;
  char *adresseEC2;             // adresse ecran ombre
  char *adresseTA2;             // adresse tampon ombre
  Sint32 offsetSrce;            // offset source
  Sint32 offsetDest;            // offset destination

  /* 
   * Drawing tables
   */ 
  /** List of tables of the drawing offsets and repeats values */
  Sint16 **drawing_values;
  /** Table of drawing offsets and repeats values for
   * current sprite image frame */
  Sint16 *current_drawing_values;
  /** List of tables of the drawing pixels data */
  char **drawing_data;
  /** Table of drawing pixels data for current sprite image frame */
  char *current_drawing_data;
  /** List of tables of the drawing pixels data. Used for drawing sprite
   * pixel by pixel, with color cycling.
   * Only used for the chance capsule sprite */
  Sint16 **drawing_pixels; 
  /** Table of drawing data for current sprite image frame */
  Sint16 *current_drawing_pixels;
  /** Data structure of drawing sprite line by line.
   * Only used for the Gigablitz sprite */
  bb_afligne **drawing_peer_line;

  static Sint32 ombredecax;
  static Sint32 ombredecay;
  static Sint32 ombrepixe4;
  static char ombrepixel;


protected:
  /** The pixel data of the sprite */
  char *pixel_data;
  Uint32 row_size;
  Uint32 destNextLn;
  /** true if the sprite is enabled and drawn */
  Sint32 is_enabled;
  /** true if the sprite has a shadow */
  bool sprite_has_shadow;
  /** X coordinate */
  Sint32 x_coord;
  /** Y coordinate */
  Sint32 y_coord;
  /** Width of the sprite in pixels */
  Sint32 sprite_width;
  /** Height of the sprite in pixels */
  Sint32 sprite_height;

  /** Time delay before next image of the animation */
  Sint32 frame_period;
  /** Counter time delay before next image */
  Sint32 frame_delay;
  /** Index of the current image */
  Sint32 frame_index;
  Sint32 frame_index_max;
  Sint32 frame_index_min;
  
  Sint32 display_pos;           // numero identificateur BOB
  /** Width of the game screen in pixels */
  Sint32 screen_width;
  /** Height of the game screen in pixels */
  Sint32 screen_height;
  /** X coordinate minimum */
  Sint32 x_minimum;
  /** Y coordinate minimum */
  Sint32 y_minimum;
  /** X coordinate maximum */
  Sint32 x_maximum;
  /** Y coordinate maximum */
  Sint32 y_maximum;
  /** Type identifier of the sprite, integer from 0 to n */
  Uint32 sprite_type_id;
  Sint32 affligFrSv;            // premiere ligne a afficher (si afflignesF=1)
  Sint32 affligLaSv;            // derniere ligne a afficher (si afflignesF=1)
  /** If true, object has allocated memory */
  bool has_allocated_memory;
  Sint32 object_pos;            // Numero du BOB dans la liste
 /** true if the object must release
  * the pixel data memory at its destruction */
  bool is_release_pixel_data;
  /** If true generate additional table to drawing pixel by pixel.
   * Used with for color cycling */
  bool is_draw_pixel_by_pixel;

public:
  /** Width used for the collisions */
  Sint32 collision_width;
  /** Height used for the collisions */
  Sint32 collision_height;
  Sint32 affligFrst;            // premiere ligne a afficher (si afflignesF=1)
  Sint32 affligLast;            // derniere ligne a afficher (si afflignesF=1)
  Sint32 mirrorVert;            // 1=mirror sprite lines in initialise() function
  Sint32 num_of_repeats;            // > 1 repeat
  Uint32 draw_method;


  Sint32 indexCycle;
  const Sint32 *pt_cycling;
  Sint32 thecounter;            // 

public:
    sprite_object ();
   ~sprite_object ();
  void clear_sprite_members ();
  void release_sprite ();
  void enable ();
  void disable ();
  Sint32 is_enable ();
  Uint32 get_sprite_type_id ();
  void set_display_pos (Sint32 num);
  void set_object_pos (Sint32 num);
  void duplicate_to (sprite_object * sprite_dest);
  void set_coordinates (Sint32 xcoord, Sint32 ycoord);
  void create_sprite (Sint32 type_id, surface_sdl * bitmap, bool shadow,
                     bool by_pixel = false);
  void make_sprite (surface_sdl * image, bool shadow = false);
  void set_x_coord (Sint32 xcoord);
  void set_y_coord (Sint32 ycoord);
  void move_x (Sint32 x_offset);
  void move_y (Sint32 y_offset);
  bool has_shadow ();
  void set_shadow (bool shadow = true);
  Sint32 get_x_coord ();
  Sint32 get_y_coord ();
  void set_image ();
  void set_image (Sint32 index);
  Sint32 get_frame_index ();
  void restore_background_under_sprite ();
  void efface_lin ();
  void draw ();
  void restore_background_under_shadow ();
  void afficheSHA ();
  void draw_to_brackground ();
  void afficheCyc ();
  void cycle_ptab ();
  void affich_SHA ();
  void draw_copy_from_bitmap ();
  void MSKbitcopy ();
  void MSK_bitclr ();
  void aspire_BOB (sprite_object * bobPT, Sint32 offsX = 0, Sint32 offsY = 0);
  void aspireBOB2 (sprite_object * bobPT, Sint32 offsX = 0, Sint32 offsY = 0);
  Sint32 collision1 (sprite_object * bobPT);
  void set_frame_delay (Sint32 delay);
  void set_frame_period (Sint32 period);
  bool play_animation_once ();
  void play_animation_loop ();
  void new_offset (Sint32 nume);
  void clip_coordinates ();
  Uint32 get_sprite_width ();
  Uint32 get_sprite_height ();
  Uint32 get_collision_width ();
  void enable_vertical_repeat (Uint32 numof_repeats);

  void set_draw_method (Uint32 method);
  void set_pixel_data (char *pixel, bool is_release = false);

public:
  static const Sint32 cycling_01[8];
  static const Sint32 cycling_02[8];
  static const bb_describ *zelistBOB[];

private:
  void init_common (surface_sdl * image, bool shadow);
  void alloc_drawing_tables (Sint32 num_images);
  void draw_line_by_line ();
  void draw_with_tables ();
  void draw_vertically_repeated ();

};
#endif
