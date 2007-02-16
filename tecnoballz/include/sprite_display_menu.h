/** 
 * @file sprite_display_menu.h
 * @brief Sprite wich display text of the menu in the menu principal 
 * @date 2007-02-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_display_menu.h,v 1.1 2007/02/16 12:38:24 gurumeditation Exp $
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
#ifndef __SPRITE_DISPLAY_MENU__
#define __SPRITE_DISPLAY_MENU__

class sprite_display_menu;

#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"

class sprite_display_menu:public virtual display_text_bitmap,
  public virtual sprite_object
{
private:
  static const unsigned char cyclingtab[];
  static const Sint32 MENU_TOTAL = 6;
  static const Sint32 MENU_LARGE = 32;  // number of characters by lines
  static const Sint32 MENU_HAUTE = 19;  // number of lines of characters

  static const Sint32 MENU_ENTRE = 0;
  static const Sint32 MENU_PARAM = 1;
  static const Sint32 MENU_ABOUT = 2;
  static const Sint32 MENU_INFOS = 3;
  static const Sint32 MENU_SALUT = 4;
  static const Sint32 MENU_SCORE = 5;

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


  Sint32 width_font;            //fonte's width font 8 or 16
  Sint32 heightfont;            //fonte's height
  Sint32 space2next;            //number of lines to next fonte 9 or 18

  Sint32 numeroMenu;            //Numero du texte menu
  Sint32 menu_color;
  Sint32 menu_colww;
  Sint32 yPressLeft;
  Sint32 yPressRigh;
  bitmap_data *graphTexte;

  char *clear_addr;
  Uint32 clearHeigh;
  Uint32 clearWidth;
  Sint32 curs_tempo;


  static char ascii2code[128];
  static char menuTexte0[];     //main menu
  static char menuTexte1[];     //options menu
  static char menuTexte2[];     //credits menu
  static char menuTexte3[];     //infos menu
  static char menuTexte4[];     //greetings menu
  static char menuTexte5[];     //best scores menu
  static char *menu_liste[MENU_TOTAL];

public:
  static const char difficulte[];
public:
    sprite_display_menu ();
   ~sprite_display_menu ();
  Sint32 first_init ();
  Sint32 afficheTxt ();

private:
  void mis_a_jour ();
  Sint32 testLeMenu ();
  void efface_BOB ();
  void clear_zone ();
  void clear_init (Uint32 xcoor, Uint32 ycoor, Uint32 width, Uint32 lines);
  void input_init (Uint32 xcoor, Uint32 ycoor, Uint32 width, char *strng);
  void clear_stop ();
  void curs_print ();
  void copyScores ();
};
#endif
