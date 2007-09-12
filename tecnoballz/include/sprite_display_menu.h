/** 
 * @file sprite_display_menu.h
 * @brief Sprite wich display text of the menu in the menu principal 
 * @date 2007-03-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_display_menu.h,v 1.5 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __SPRITE_DISPLAY_MENU__
#define __SPRITE_DISPLAY_MENU__

class sprite_display_menu;

#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"

class sprite_display_menu:public virtual display_text_bitmap,
  public virtual sprite_object
{

public:
  typedef enum 
    {
      DO_NO_EXIT,
      PROGRAM_EXIT,
      START_GAME
    }
    EXIT_ENUM;

private:
  static const unsigned char cyclingtab[];
  /** Number of characters by lines */
  static const Sint32 NUM_OF_COLUMNS = 32;
  /** Number of lines of characters */
  static const Sint32 NUM_OF_ROWS = 19;
  typedef enum
    {
      MAIN_SECTION,
      OPTIONS_SECTION,
      ABOUT_SECTION,
      INFOS_SECTION,
      GREETINGS_SECTION,
      SCORE_SECTIONS,
      NUM_OF_SECTIONS
    }
    SECTIONS_ENUM;

  static const Sint32 LINE_START = 5;
  static const Sint32 LINE_PARAM = 6;
  static const Sint32 LINE_ABOUT = 7;
  static const Sint32 LINE_SALUT = 8;
  static const Sint32 LINE_INFOS = 9;
  static const Sint32 LINE_SCORE = 10;
  static const Sint32 LINE_CODE1 = 11;
  static const Sint32 LINE_CODE2 = 12;
  static const Sint32 LINE_SORTI = 13;

  static const Sint32 LINE_ENTRE = 14;
  static const Sint32 YCOORDNULL = -10240;


  /** Width of font in pixels 8 or 16 */
  Uint32 font_width;
  /** Height of font in pixels 8 or 16 */
  Uint32 font_height;
  /** Space between lines in pixels */
  Uint32 line_spacing;
  /** Id of the current menu from MAIN_SECTION to SCORE_SECTIONS */
  Uint32 current_menu_section; 
  Sint32 menu_color;
  Sint32 menu_colww;
  Sint32 y_coord_left_down;
  Sint32 y_coord_right_down;
  bitmap_data *text_offscreen;

  char *clear_addr;
  Uint32 clear_zone_height;
  Uint32 clear_zone_width;
  Uint32 clear_zone_xcoord;
  Uint32 clear_zone_ycoord; 
  Uint32 blink_cursor_delay;


  static char ascii2code[128];
  static char menuTexte0[];     //main menu
  static char menuTexte1[];     //options menu
  static char menuTexte2[];     //credits menu
  static char menuTexte3[];     //infos menu
  static char menuTexte4[];     //greetings menu
  static char menuTexte5[];     //best scores menu
  static char *menu_liste[NUM_OF_SECTIONS];

public:
  static const char difficulte[];
public:
    sprite_display_menu ();
   ~sprite_display_menu ();
  void first_init ();
  Uint32 check_and_display ();

private:
  void mis_a_jour ();
  Uint32 check_events ();
  void clear_text_offscreen ();
  void clear_input_zone ();
  void clear_init (Uint32 xcoor, Uint32 ycoor, Uint32 width, Uint32 lines);
  void input_init (Uint32 xcoor, Uint32 ycoor, Uint32 width, char *strng);
  void clear_stop ();
  void draw_input_cursor ();
  void copyScores ();
};
#endif
