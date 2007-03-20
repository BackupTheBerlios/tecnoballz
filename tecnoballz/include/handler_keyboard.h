/** 
 * @file handler_keyboard.cc 
 * @brief Handler of the keyboard and mouse
 * @date 2007-03-20
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_keyboard.h,v 1.3 2007/03/20 08:05:44 gurumeditation Exp $
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
#ifndef __HANDLER_KEYBOARD
#define __HANDLER_KEYBOARD

#include "../include/tecnoballz.h"

class handler_keyboard:public virtual tecnoballz
{
public:
  
  typedef enum
    {
      COMMAND_KEY_PAUSE,
      TOEXITFLAG,
      TOMENUFLAG,
      TOOVERFLAG,
      MFXSFXFLAG,
      SOUND_FLAG,
      MUSIC_FLAG,
      FULLSCFLAG,
      ESCAPEMENU,
      WAITVBLOFF,
      NUMOFFLAGS
    }
    COMAND_KEYS_ENUM;

private:
  static bool last_command_keys[NUMOFFLAGS];
  static bool command_keys[NUMOFFLAGS];
  static handler_keyboard* keyboard_singleton;

private:
  /* 1 =  grabs mouse and keyboard input */
  bool is_grab_input;
  
  Sint32 mouse_x_offset;
  Sint32 mouse_y_offset;
  Sint32 previous_mouse_x_coord;
  Sint32 previous_mouse_y_coord;
  
  /* x and y coordinates of the mouse pointer */
  Sint32 mouse_x_coord;
  Sint32 mouse_y_coord;
  
  bool is_left_button_down;
  bool is_right_button_down;
  bool is_middle_button_down;
  bool is_left_button_released;
  bool is_right_button_released;

  /* keyboard input delay */
  Sint32 key_delay;
  Uint32 key_code_down;
  Uint32 previous_key_code_down;
  Uint32 code_keyup;
  /* cursor position into input string */
  Sint32 string_cursor_pos;
  /* input string size */
  Sint32 string_input_size;
  /* pointer to the current input string */
  char *current_input_string;

private:
    handler_keyboard ();

public:
   ~handler_keyboard ();
  static handler_keyboard* get_instance ();
  
  void set_grab_input (bool mode);
  void read_events ();
  void clear_command_keys ();
  bool command_is_pressed (Uint32 code, bool clear = false);
  bool is_left_button ();
  bool is_right_button ();
  bool is_right_left_buttons ();
  bool is_left_button_up (Sint32 * off_x, Sint32 * off_y);
  bool is_right_button_up (Sint32 * off_x, Sint32 * off_y);
  Sint32 get_mouse_x_offset ();
  Sint32 get_mouse_x ();
  Sint32 get_mouse_y ();
  bool key_is_pressed (Sint32 code);
  bool key_is_released (Sint32 code);

  void set_input_string (char *str, Uint32 size);
  Sint32 get_input_cursor_pos ();
  void stop_string_input ();
  Uint32 get_key_down_code ();

private:
  void input_string ();
  void input_string (Uint32 code);
  void set_key_code_down (Uint32 code);
  void set_keycode_up (Uint32 code);
};

#endif
