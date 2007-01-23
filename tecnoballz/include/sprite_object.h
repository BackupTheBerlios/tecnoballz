/** 
 * @file sprite_object.h
 * @brief Draw sprites on the screen 
 * @date 2007-01-23
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.12 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_object.h,v 1.12 2007/01/23 20:51:30 gurumeditation Exp $
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
#include "../include/GIF_bitMap.h"
#include "../include/handler_memory.h"
#include "../include/mentatCode.h"
#include "../include/print_text.h"
//-------------------------------------------------------------------------------
// Sprite's number
//-------------------------------------------------------------------------------
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
/* const Sint32	BOB_BRICK2 = 59;
const Sint32	BOB_BRICK3 = 60;
const Sint32	BOB_BRICK4 = 61;
const Sint32	BOB_BRICK5 = 62;
const Sint32	BOB_BRICK6 = 63;
const Sint32	BOB_BRICK7 = 64;
const Sint32	BOB_BRICK8 = 65;
const Sint32	BOB_BRICK9 = 66; */
//-------------------------------------------------------------------------------
typedef struct
{
  Sint16 BB_COORDX;             // real x = BB_COORDX*16
  Sint16 BB_COORDY;             // real y = BB_COORDY*2
}
bbPosition;
//-------------------------------------------------------------------------------
typedef struct
{
  Sint16 BB_LARGEUR;            // real width = BB_LARGEUR * 2
  Sint16 BB_HAUTEUR;            // real height = BB_HAUTEUR * 2
  Sint16 BB_ANIMATE;            // number of animation(s)
  bbPosition *BBPOSITION;
}
bb_describ;
//-------------------------------------------------------------------------------
typedef struct
{
  Sint32 COUNTERTAB;            // size of table offsets / loops 
  Sint32 OFFSETLEFT;
  Sint16 *TABAFFICH1;           // table of offsets and loops counters
  char *TABAFFICH2;             // table of pixels
}
bb_afligne;
//-------------------------------------------------------------------------------
class sprite_object:public virtual mentatCode
{
  friend class zeGameOver;
  friend class zexplosion;
  friend class zeguardian;
  friend class zeGemstone;
  friend class zeMissiles;
  friend class zeCongBall;
  friend class print_text;

public:
  static const Uint32 METHOD_TAB = 0;
  static const Uint32 METHOD_MSK = 1;
  static const Uint32 METHOD_LIN = 2;
  static const Uint32 METHOD_REP = 3;
  static const Uint32 METHOD_CC1 = 4;
  static const Uint32 METHOD_CC2 = 5;
  static const Uint32 CYCLE_PTAB = 6;

private:
  /** Maximum number of images in the animation */
  Sint32 max_of_images;
  char **adresseTAB;            // table adresse des graphismes
  /** Pointer to sprite in game offscreen */
  char *screen_ptr;
  /** Pointer to the background in restore offscreen */
  char *restore_ptr;
  char *adresseEC2;             // adresse ecran ombre
  char *adresseTA2;             // adresse tampon ombre
  Sint32 offsetSrce;            // offset source
  Sint32 offsetDest;            // offset destination

  // table d'affichage
  Sint16 **BOBtableP1;          // pt/les tables aff. offsets
  char **BOBtableP2;            // pt/les tables aff. pixels
  Sint16 **BOBtableP3;          // pt/les tables aff. offsets (byte peer byte)
  Sint16 *tabAffich1;           // table des offsets en cours
  char *tabAffich2;             // table des pixels en cours
  Sint16 *tabAffich3;           // table des offsets en cours (byte peer byte)
  bb_afligne **tafflignes;      // tables d'affichage pour l'affichage par lignes

  static Sint32 ombredecax;
  static Sint32 ombredecay;
  static Sint32 ombrepixe4;
  static char ombrepixel;


protected:
  char *adresseGFX;             // memory address 
  Sint32 srceNextLn;
  Sint32 destNextLn;
  Sint32 is_enabled;            // 1=le BOB peut etre affiche
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
  Sint32 y_maxium;
  /** Type ID of the sprite */
  Sint32 sprite_type_id;
  Sint32 affligFrSv;            // premiere ligne a afficher (si afflignesF=1)
  Sint32 affligLaSv;            // derniere ligne a afficher (si afflignesF=1)
  Sint32 memoryflag;            // This object reserved some memory 
  Sint32 object_pos;            // Numero du BOB dans la liste
  Sint32 releaseGFX;            //if set release adresseGFX
  Sint32 fTableByte;            //if set, generate additional table to 
  //copy byte by byte

public:
  /** Width used for the collisions */
  Sint32 collision_width;
  /** Height used for the collisions */
  Sint32 collision_height;
  Sint32 affligFrst;            // premiere ligne a afficher (si afflignesF=1)
  Sint32 affligLast;            // derniere ligne a afficher (si afflignesF=1)
  Sint32 mirrorVert;            // 1=mirror sprite lines in initialise() function
  Sint32 affRepeatF;            // > 1 repeat
  Uint32 put_method;


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
  void set_display_pos (Sint32 num);
  void set_object_pos (Sint32 num);
  sprite_object & operator= (const sprite_object &sprite);
  void duplicaBOB (sprite_object *);
  void set_coordinates (Sint32 xcoord, Sint32 ycoord);
  Sint32 reservBOBt (Sint32 anima);
  Sint32 create_sprite (Sint32 BOBnu, GIF_bitMap * image, bool shadow,
                     Sint32 ftpix = 0);
  Sint32 make_sprite (GIF_bitMap * image, Sint32 ombre);
  void initCommun (GIF_bitMap * image, Sint32 ombre);
  void set_x_coord (Sint32 xcoord);
  void set_y_coord (Sint32 ycoord);
  void move_x (Sint32 x_offset);
  void move_y (Sint32 y_offset);
  bool has_shadow ();
  Sint32 get_x_coord ();
  Sint32 get_y_coord ();
  void set_image ();
  void set_image (Sint32 index);
  Sint32 litAnimOff ();
  void restore_background_under_sprite ();
  void efface_lin ();
  void draw ();
  void method_tab ();
  void restore_background_under_shadow ();
  void afficheSHA ();
  void affich_MSK ();
  void afficheCyc ();
  void afficheCC2 ();
  void cycle_ptab ();
  void afficheRep ();
  void afficheLin ();
  void affich_SHA ();
  void draw_copy ();
  void MSKbitcopy ();
  void MSK_bitclr ();
  void aspire_BOB (sprite_object * bobPT, Sint32 offsX = 0, Sint32 offsY = 0);
  void aspireBOB2 (sprite_object * bobPT, Sint32 offsX = 0, Sint32 offsY = 0);
  Sint32 collision1 (sprite_object * bobPT);
  void tempo_init (Sint32 tempo);
  void set_frame_period (Sint32 period);
  bool play_animation_once ();
  void play_animation_loop ();
  void new_offset (Sint32 nume);
  void clip_coordinates ();
  Uint32 get_sprite_width ();
  Uint32 get_sprite_height ();
  Uint32 get_collision_width ();
  void initRepeat (Sint32 value);

  void set_method (Uint32 vtype);
  void set_memGFX (char *pGfx, Sint32 rGfx = 0);

public:
  static const Sint32 cycling_01[8];
  static const Sint32 cycling_02[8];
  static const bb_describ *zelistBOB[];

};
#endif
