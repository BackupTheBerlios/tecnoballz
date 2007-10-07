/** 
 * @file supervisor_shop.h
 * @brief Shop supervisor 
 * @date 2007-10-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.24 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_shop.h,v 1.24 2007/10/07 06:35:36 gurumeditation Exp $
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

   /** Enumerate differents pages of the informations option */
   typedef enum  
    {
      INFO_PADDLES,
      INFO_LIVES,
      INFO_AREA_CODE,
      INFO_END
    } INFO_ENUM;
  /** Length of a text line in chars */
  static const Uint32 BOX_LENGTH_STRING = 22;
  /** All strings loaded from a file text */
  char** box_texts;
  /** tiles_background object only used to initialize
   * the colors palette */
  tiles_background *tiles_ground;
  /** Pointer to the sprite mouse pointer */
  sprite_mouse_pointer *mouse_pointer;
  /** Led who indicates the option over the mouse */
  sprite_object *led_indicator;
  /** Control all capsules */
  controller_capsules *power_up_capsules;
  /** Object which draws three lines of texts */
  display_text_bitmap *display_text;
  /** Object who handles the "ESC" menu */
  handler_popup_menu *popup_menu;
  /** Option currently selected by pressing the left mouse button */ 
  Sint32 current_selected_option;
  /** Index of the currently displayed page: INFO_PADDLES,
   * INFO_LIVES or INFO_AREA_CODE */
  Uint32 some_infos_index;
  /** If true info already seen at least once */
  bool is_already_view_info;
  /** Current price of the selected option */
  Uint32 current_price;
  /** The number of bought capsules */
  Uint32 num_of_bought_capsules;
  /** Pointer to the 3 lignes currently displayed */
  char *current_text_displayed[3];
  /** Current x-coordinate of the led indicator */
  Sint32 led_indicator_xcoord;
  /** Current y-coordinate of the led indicator */
  Sint32 led_indicator_ycoord;
  Sint32 get_object;            // drag object : pointer to the table "options_prices" (-1 = no drag object)
  Sint32 pt_get_obj;
  /** Capsule which blinks in the list when it is draged with the mouse */
  sprite_capsule *blink_capsule;
  /** Capsule which draged with the mouse */
  sprite_capsule *drag_sprite;
  // temporary list of the bonuses bought
  Sint32 *courseList;
  Sint32 coursetemp[MAX_OF_CAPSULES_BOUGHT + 1];

  Sint32 shop_xmini;
  Sint32 shop_xmaxi;
  Sint32 shop_xmax2;
  Sint32 shop_ymax1;
  Sint32 shop_ymax2;

  Sint32 cadre_offs;
  /** True if drawn the select cursor */
  bool is_drawn_select_cursor;
  Sint32 cadre_ymin;
  Sint32 cadre_ymax;
  Sint32 cadre_posx;
  Sint32 cadre_posy;
  /** Maximum height of the select cursor in pixels */
  Sint32 select_cursor_height;
  /** Maximum width of the select cursor in pixels */
  Uint32 select_cursor_width;
  /** Sinus table index from 0 to 511 used to modify the size of the cursor */
  Sint32 select_cursor_sin_index;
  /** Indexed color of the select cursor from 0 to 32 */
  Sint32 select_cursor_color;
  /** Code of the last key pressed, used for input cheat code */
  Uint32 previous_key_code_down;
  /** Current cheat code which was typed by the player */
  Uint32 cheat_code_input;
  /** Cheat code is an unsigned 32 bits integer,
   * a string of 4 chars containing the keycodes "ETB\n" */
  Uint32 cheat_code;

  // table de pointeurs sur "options_prices"
  static Sint32 sh_tablept[MAX_OF_CAPSULES_BOUGHT];

  /** Identifiers of the available options and bonus capsules */
  static Sint32 available_options_id[];
  /** Prices of all the available options in the shop */ 
  static Uint32 options_prices[];
  static char shoptext00[];
  static char shoptext41[];
  static char shoptext56[];
  static char shoptextPT[];
  static char info_text1[];
  static char info_text2[];
  static char info_text3[];

public:
    supervisor_shop ();
   ~supervisor_shop ();
  void first_init ();
  Uint32 main_loop ();

private:
  Sint32 get_option_over_mouse_cursor (Sint32 x, Sint32 y);
  void purchase_option_or_capsule (Uint32 option_id);
  void display_info(); 
  void display_capsules_bought ();
  void purchase_bonus_capsule (Sint32 option_num);
  void message_ok ();
  Uint32 led_moving (Sint32 point);

  bool decrease_money_amount ();
  void put_current_text (char *str);
  void display_box_text ();
  void sh_ballade ();
  Sint32 collisions ();
  void pos_select ();
  void draw_select_cursor ();
  void check_if_enable_cheat ();

  static const unsigned char color_cycling[];
};
#endif
