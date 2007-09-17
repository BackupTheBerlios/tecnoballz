/** 
 * @file supervisor_shop.h
 * @brief Shop supervisor 
 * @date 2007-04-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.21 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_shop.h,v 1.21 2007/09/17 05:40:35 gurumeditation Exp $
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
#ifndef __SUPERVISOR_SHOP__
#define __SUPERVISOR_SHOP__

class supervisor_shop;

#include "../include/list_sprites.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/sprite_object.h"
#include "../include/controller_capsules.h"
#include "../include/display_text_bitmap.h"
#include "../include/handler_players.h"
#include "../include/supervisor.h"
#include "../include/handler_popup_menu.h"
#include "../include/tiles_background.h"

class supervisor_shop:public supervisor
{
public:
  /* number maximum of bonus capsules bought */
  static const Uint32 MAX_OF_CAPSULES_BOUGHT = 19;
private:
  typedef enum
    {
      TEXT_INFOS = 21,
      TEXT_WELCOME = 32,
      TEXT_PADDLES = 21,
      TEXT_LIVES_LEFT = 22,
      TEXT_HOPING_HELP = 24,
      TEXT_WAITING_CHEAT_MODE = 25,
      TEXT_ENABLED_CHEAT_MODE = 26,
      TEXT_PADDLE_RIGHT = 27, 
      TEXT_PADDLE_TOP = 28, 
      TEXT_PADDLE_LEFT = 29, 
      TEXT_AREA_CODE = 30,
      TEXT_NO_AREA_CODE = 31,
      TEXT_NOT_ENOUGH_MONEY = 33,
      TEXT_CANNOT_BUY_MORE = 34,
      TEXT_ONLY_FOR_AREA_5 = 35,
    } TEXT_ENUM;

   typedef enum  
    {
      INFO_PADDLES,
      INFO_LIVES,
      INFO_AREA_CODE,
      INFO_END
    } INFO_ENUM;


  static const Uint32 BOX_LENGTH_STRING = 22;  //largeur d'une ligne texte en caracteres
  char** box_texts;
  tiles_background *tiles_ground;
  sprite_mouse_pointer *mouse_pointer;
  sprite_object *led_indicator;
  controller_capsules *power_up_capsules;
  display_text_bitmap *display_text;
  handler_popup_menu *popup_menu;
  Sint32 shop_point;            //pointeur
  Sint32 shoppoint3;            //pointeur

  Sint32 optioninfo;            // 0;1;2;3
  /** If true info already seen at least once */
  bool is_already_view_info;
  /** Current price of the selected option */
  Uint32 current_price;
  /** The number of bought capsules */
  Uint32 num_of_bought_capsules;
  //char *shop_line1;
  //char *shop_line2;
  //char *shop_line3;
  char *text_lines[3];
  Sint32 mouse_x_coord;
  Sint32 mouse_y_coord;
  Sint32 get_object;            // drag object : pointer to the table "options_prices" (-1 = no drag object)
  Sint32 pt_get_obj;
  /** Capsule which blinks in the list when it is draged with the mouse */
  sprite_capsule *blink_capsule;
  /** Capsule which draged with the mouse */
  sprite_capsule *drag_sprite;
  Sint32 *courseList;

  // temporary list of the bonuses bought
  Sint32 coursetemp[MAX_OF_CAPSULES_BOUGHT + 1];

  Sint32 shop_xmini;
  Sint32 shop_xmaxi;
  Sint32 shop_xmax2;
  Sint32 shop_ymax1;
  Sint32 shop_ymax2;

  Sint32 cadre_offs;
  Sint32 cadre_flag;            // 1 = display box
  Sint32 cadre_ymin;
  Sint32 cadre_ymax;
  Sint32 cadre_posx;
  Sint32 cadre_posy;            //mouse y coordinate (collision box)
  Sint32 cadre_haut;
  Sint32 cadre_larg;
  Sint32 angleValue;
  Sint32 box_colour;
  /** Code of the last key pressed, used for input cheat code */
  Uint32 previous_key_code_down;
  Uint32 triche_etb;
  /** Cheat code is an unsigned 32 bits integer,
   * a string of 4 chars containing the keycodes "ETB\n" */
  Uint32 cheat_code;

  // table de pointeurs sur "options_prices"
  static Sint32 sh_tablept[MAX_OF_CAPSULES_BOUGHT];

  static Sint32 case_types[];
  /** Prices of all the available options in the shop */ 
  static Uint32 options_prices[];
  static char shoptext00[];
  static char shoptext41[];
  //static char shoptext63[];
  static char shoptext56[];
  static char shoptextPT[];
 // static char shoptext12[];
  static char info_text1[];
  static char info_text2[];
  static char info_text3[];

public:
    supervisor_shop ();
   ~supervisor_shop ();
  void first_init ();
  Sint32 main_loop ();

private:
  Sint32 testkursor (Sint32 x, Sint32 y);
  void faitcourse (Sint32 gadnu);
  void display_info(); 
  void display_capsules_bought ();
  void purchase_bonus_capsule (Sint32 option_num);
  void message_ok ();
  Sint32 led_moving (Sint32 point);

  bool decrease_money_amount ();
  void putthetext (char *ligne);
  void display_box_text ();
  void sh_ballade ();
  Sint32 collisions ();
  void pos_select ();
  void draw_select_cursor ();
  void check_if_enable_cheat ();

  static const unsigned char cyclingtab[];
};
#endif
