/** 
 * @file short_info_messages.h 
 * @brief Display short info messages in bricks levels 
 * @created 2002-12-30
 * @date 2007-02-26
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: short_info_messages.h,v 1.4 2007/09/12 06:32:48 gurumeditation Exp $
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
#ifndef __SHORT_INFO_MESSAGES__
#define __SHORT_INFO_MESSAGES__
class short_info_messages;
#include "../include/handler_display.h"
#include "../include/tecnoballz.h"
#include "../include/bitmap_data.h"

class short_info_messages:public virtual tecnoballz
{
public:
  typedef enum
    {
      ARE_YOU_READY,
      YEAH_YOU_WHERE,
      SIMPLE_FIRE,
      COOL_FIRE,
      COOL_GUY,
      LOST_FILE,
      WIN_LIFE,
      TOO_LATE,
      BUDGET_PRICES,
      GLUE_OPTION,
      NEXT_LEVEL,
      SMALL_PADDLE,
      BIG_PADDLE,
      EXTRA_BALLS,
      MULTI_BALLS,
      POWERBALLS,
      MEGA_POWERBALLS,
      INVERSE_COMMANDS,
      MAXIMUM_ACCELERATION,
      RIGHT_PADDLE,
      TOP_PADDLE,
      LEFT_PADDLE,
      BIG_BALLS,
      HUGE_BALLS,
      MAXIMUM_OPTIONS,
      WALL_ENABLE,
      ROBOT_ENABLE,
      CONTROL_BALLS,
      MAX_OF_MESSAGES
    } MESSAGES_ENUM;

private:
  static short_info_messages* info_messages_singleton;
  char **messages;
  bitmap_data * bitmap_small_fonts;
  bitmap_data *messa_fond;
  static const Uint32 MESSAGEPOSX = 96;
  static const Uint32 MESSAGEPOSY = 37;
  /** Maximum number of chars of a message */
  static const Uint32 MAX_OF_CHARS = 16;

  /** Time delay counter, if < 5 then clear the message */
  Uint32 delay_counter;
  Sint32 mess_pnter;            //number of the chars to display 0 to 15
  const char *mess_reque;
  Sint32 off_desti1;            // modulo destination (buffer)
  Sint32 off_source;            // modulo source (graphic page)
  char *ptr_fontes;
  char *ptr_buffer;
  char *ptr_tampon;
  char *pt_mesfond;
  /** Height of a character in pixels */
  Uint32 height_font;
  /** Width of a character in pixels */
  Uint32 width_font;
  Uint32 fonteslarg;            //width of message's buffer  
  bool messages_request[MAX_OF_MESSAGES];

private:
    short_info_messages ();
  void draw ();
  void displaymes ();
  void clear_mess ();
public:
   ~short_info_messages ();
  static short_info_messages * get_instance ();
  void clear_messages_request ();
  void intialize ();
  void send_message_request (Uint32 id);
  void run ();
};
#endif
