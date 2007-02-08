/** 
 * @file handler_keyboard.cc 
 * @brief Handler of the keyboard and mouse
 * @date 2007-01-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_keyboard.cc,v 1.3 2007/02/08 17:00:33 gurumeditation Exp $
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
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"

bool handler_keyboard::command_keys[NUMOFFLAGS];
bool handler_keyboard::last_command_keys[NUMOFFLAGS];
handler_keyboard * handler_keyboard::keyboard_singleton = NULL;

/**
 * Create the handler_keyboard singleton object, and clear members
 */
handler_keyboard::handler_keyboard ()
{
  object_init ();
  is_grab_input = false;
  previous_mouse_x_coord = 0;
  previous_mouse_y_coord = 0;
  mouse_x_offset = 0;
  mouse_y_offset = 0;
  /* x and y coordinates of the mouse */
  mouse_x_coord = 0;
  mouse_y_coord = 0;
  is_left_button_down = false;
  is_right_button_down = false;
  is_middle_button_down = false;
  is_left_button_released = 0;
  is_right_button_released = 0;
  key_delay = 0;
  key_code_down = 0;
  previous_key_code_down = 0;
  string_cursor_pos = 0;
  string_input_size = 0;
  code_keyup = 0;
  current_input_string = NULL;

  for (Uint32 i = 0; i < NUMOFFLAGS; i++)
    {
      command_keys[i] = false;
      last_command_keys[i] = false;
    }
}

/**
 * Release the handler_keyboard singleton object
 */
handler_keyboard::~handler_keyboard ()
{
  object_free ();
}

/**
 * Get the object instance
 * handler_keyboard is a singleton
 * @return the handler_keyboard object 
 */
handler_keyboard *
handler_keyboard::get_instance ()
{
  if (NULL == keyboard_singleton)
    {
      keyboard_singleton = new handler_keyboard ();
    }
  return keyboard_singleton;
}

/**
 * Destroy the object instance
 */
void
handler_keyboard::destroy_instance ()
{
  if (NULL != keyboard_singleton)
    {
      delete keyboard_singleton;
      keyboard_singleton = NULL;
    }
}

/**
 * Grabs mouse and keyboard input, the mouse is confined to the
 * application window, and nearly all keyboard input is passed
 * directly to the application
 * @param mode true to enable grab input, false to disable it
 */
void
handler_keyboard::set_grab_input (bool mode)
{
  return;
  if (mode)
    {
      is_grab_input = true;
      SDL_WM_GrabInput (SDL_GRAB_ON);
      SDL_GetRelativeMouseState (0, 0);
    }
  else
    {
      is_grab_input = false;
      SDL_WM_GrabInput (SDL_GRAB_OFF);
    }
}

/**
 * Read keyboard mouse and exit events
 */
void
handler_keyboard::read_events ()
{
  command_keys[FULLSCFLAG] = false;
  command_keys[WAITVBLOFF] = false;
  is_left_button_released = 0;
  is_right_button_released = 0;
  SDL_Event event;
  SDL_KeyboardEvent *kbEvt;

  while (SDL_PollEvent (&event) > 0)
    {
      switch (event.type)
        {
          /* mouse motion event */
        case SDL_MOUSEMOTION:
          {
            /* read the x and y coordinates of the mouse */
            mouse_x_coord = event.motion.x;
            mouse_y_coord = event.motion.y;
          }
          break;

          /* a key is pressed */
        case SDL_KEYDOWN:
          {
            kbEvt = (SDL_KeyboardEvent *) & event;
            Uint8 *keys = SDL_GetKeyState (NULL);

            if (keys[SDLK_RALT] == SDL_RELEASED &&
                keys[SDLK_LALT] == SDL_RELEASED &&
                keys[SDLK_LSHIFT] == SDL_RELEASED &&
                keys[SDLK_RSHIFT] == SDL_RELEASED)
              {

                if (keys[SDLK_RCTRL] == SDL_RELEASED &&
                    keys[SDLK_LCTRL] == SDL_RELEASED)
                  {
                    if (keys[SDLK_p] == SDL_PRESSED)
                      last_command_keys[COMMAND_KEY_PAUSE] = true;
                    if (keys[SDLK_f] == SDL_PRESSED)
                      last_command_keys[FULLSCFLAG] = true;
                    if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                      last_command_keys[ESCAPEMENU] = true;
                    if (keys[SDLK_l] == SDL_PRESSED)
                      last_command_keys[WAITVBLOFF] = true;
                  }
                else
                  {
                    if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                      last_command_keys[TOEXITFLAG] = true;
                    if (keys[SDLK_x] == SDL_PRESSED)
                      last_command_keys[TOOVERFLAG] = true;
                    if (keys[SDLK_q] == SDL_PRESSED)
                      last_command_keys[TOMENUFLAG] = true;
                    if (keys[SDLK_f] == SDL_PRESSED)
                      last_command_keys[SOUND_FLAG] = true;
                    if (keys[SDLK_s] == SDL_PRESSED)
                      last_command_keys[MFXSFXFLAG] = true;
                    if (keys[SDLK_d] == SDL_PRESSED)
                      last_command_keys[MUSIC_FLAG] = true;
                  }
              }

            if (kbEvt->keysym.unicode > 0)
              {
                set_key_code_down (kbEvt->keysym.unicode);
              }
            else
              {
                set_key_code_down (kbEvt->keysym.sym);
              }
          }
          break;

          /* a key is released */
        case SDL_KEYUP:
          {
            kbEvt = (SDL_KeyboardEvent *) & event;
            Uint8 *keys = SDL_GetKeyState (NULL);


            /* enable pause [P] key */
            if (keys[SDLK_p] == SDL_RELEASED
                && last_command_keys[COMMAND_KEY_PAUSE])
              {
                last_command_keys[COMMAND_KEY_PAUSE] = false;
                if (!command_keys[ESCAPEMENU])
                  {
                    command_keys[COMMAND_KEY_PAUSE] =
                      command_keys[COMMAND_KEY_PAUSE] ? false : true;
                    if (is_grab_input && command_keys[COMMAND_KEY_PAUSE])
                      {
                        SDL_WM_GrabInput (SDL_GRAB_OFF);
                      }
                    if (is_grab_input && !command_keys[COMMAND_KEY_PAUSE])
                      {
                        SDL_WM_GrabInput (SDL_GRAB_ON);
                      }
                  }
              }

            /* enable context menu [ESC] key */
            if (keys[ESCAPEMENU] == SDL_RELEASED
                && last_command_keys[ESCAPEMENU])
              {
                last_command_keys[ESCAPEMENU] = false;
                command_keys[ESCAPEMENU] =
                  command_keys[ESCAPEMENU] ? false : true;
                command_keys[COMMAND_KEY_PAUSE] = command_keys[ESCAPEMENU];
                if (is_grab_input && command_keys[COMMAND_KEY_PAUSE])
                  SDL_WM_GrabInput (SDL_GRAB_OFF);
                if (is_grab_input && !command_keys[COMMAND_KEY_PAUSE])
                  SDL_WM_GrabInput (SDL_GRAB_ON);

                if (!command_keys[ESCAPEMENU])
                  SDL_ShowCursor (SDL_DISABLE);
                if (command_keys[ESCAPEMENU])
                  SDL_ShowCursor (SDL_ENABLE);
              }

            if (keys[SDLK_f] == SDL_RELEASED && last_command_keys[FULLSCFLAG])
              {
                last_command_keys[FULLSCFLAG] = false;
                command_keys[FULLSCFLAG] = true;
              }
            {
              if (keys[SDLK_ESCAPE] == SDL_RELEASED
                  && last_command_keys[TOEXITFLAG])
                {
                  last_command_keys[TOEXITFLAG] = false;
                  command_keys[TOEXITFLAG] = true;
                }
              if (keys[SDLK_x] == SDL_RELEASED
                  && last_command_keys[TOOVERFLAG])
                {
                  last_command_keys[TOOVERFLAG] = false;
                  command_keys[TOOVERFLAG] = true;
                }
              if (keys[SDLK_q] == SDL_RELEASED
                  && last_command_keys[TOMENUFLAG])
                {
                  last_command_keys[TOMENUFLAG] = false;
                  command_keys[TOMENUFLAG] = true;
                }
              if (keys[SDLK_f] == SDL_RELEASED
                  && last_command_keys[SOUND_FLAG])
                {
                  last_command_keys[SOUND_FLAG] = false;
                  command_keys[SOUND_FLAG] = true;
                }
              if (keys[SDLK_s] == SDL_RELEASED
                  && last_command_keys[MFXSFXFLAG])
                {
                  last_command_keys[MFXSFXFLAG] = false;
                  command_keys[MFXSFXFLAG] = true;
                }
              if (keys[SDLK_d] == SDL_RELEASED
                  && last_command_keys[MUSIC_FLAG])
                {
                  last_command_keys[MUSIC_FLAG] = false;
                  command_keys[MUSIC_FLAG] = true;
                }
              if (keys[SDLK_l] == SDL_RELEASED
                  && last_command_keys[WAITVBLOFF])
                {
                  last_command_keys[WAITVBLOFF] = false;
                  command_keys[WAITVBLOFF] = true;
                }
            }

            if (kbEvt->keysym.unicode > 0)
              {
                set_keycode_up (kbEvt->keysym.unicode);
              }
            else
              {
                set_keycode_up (kbEvt->keysym.sym);
              }
          }
          break;

          /* a mouse button is pressed */
        case SDL_MOUSEBUTTONDOWN:
          {
            switch (event.button.button)
              {
              case SDL_BUTTON_LEFT:
                is_left_button_down = true;
                break;
              case SDL_BUTTON_RIGHT:
                is_right_button_down = true;
                break;
              case SDL_BUTTON_MIDDLE:
                is_middle_button_down = true;
                break;
              }
          }
          break;

          /* a mouse button is relased */
        case SDL_MOUSEBUTTONUP:
          {
            switch (event.button.button)
              {
              case SDL_BUTTON_LEFT:
                if (is_left_button_down)
                  {
                    is_left_button_released = true;
                  }
                is_left_button_down = false;
                break;
              case SDL_BUTTON_RIGHT:
                if (is_right_button_down)
                  {
                    is_right_button_released = true;
                  }
                is_right_button_down = false;
                break;
              case SDL_BUTTON_MIDDLE:
                is_middle_button_down = false;
                break;
              }
          }
          break;

          /* quit the game */
        case SDL_QUIT:
          {
            command_keys[TOEXITFLAG] = true;
          }
          break;
        }
    }
  hasard_val += mouse_x_coord;
  hasard_val += mouse_y_coord;
  input_string ();
}

/**
 * Check if a key is pressed
 * @param code a SDL Keysym definition
 * @return true if the key is pressed 
 */
bool handler_keyboard::key_is_pressed (Sint32 code)
{
  Uint8 *
    keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[code] == SDL_PRESSED)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Check if a key is released
 * @param code a SDL Keysym definition
 * @return true if the key is released
 */
bool handler_keyboard::key_is_released (Sint32 code)
{
  Uint8 *
    keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[code] == SDL_RELEASED)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Clear some command keys and set grab input 
 */
void
handler_keyboard::clear_command_keys ()
{
  command_keys[COMMAND_KEY_PAUSE] = false;
  command_keys[TOMENUFLAG] = false;
  command_keys[TOOVERFLAG] = false;
  command_keys[MFXSFXFLAG] = false;
  command_keys[SOUND_FLAG] = false;
  command_keys[MUSIC_FLAG] = false;
  command_keys[ESCAPEMENU] = false;
  last_command_keys[COMMAND_KEY_PAUSE] = false;
  last_command_keys[TOMENUFLAG] = false;
  last_command_keys[TOOVERFLAG] = false;
  last_command_keys[MFXSFXFLAG] = false;
  last_command_keys[SOUND_FLAG] = false;
  last_command_keys[MUSIC_FLAG] = false;
  last_command_keys[ESCAPEMENU] = false;
  if (is_grab_input)
    {
      SDL_WM_GrabInput (SDL_GRAB_ON);
    }
  previous_mouse_x_coord = mouse_x_coord;
  previous_mouse_y_coord = mouse_y_coord;
}

/** 
 * Check if a command key is pressed
 * @param code of the command key
 * @param clear true if clear flag after read
 * @return true if the command key is pressed
 */
bool handler_keyboard::command_is_pressed (Uint32 code, bool clear)
{
  bool
    is_pressed = command_keys[code];
  if (clear)
    {
      command_keys[code] = false;
    }
  return is_pressed;
}

/** 
 * Test if mouse left button is pressed
 * @return true if mouse left button is down
 */
bool handler_keyboard::is_left_button ()
{
  return is_left_button_down;
}

/** 
 * Test if mouse right button is pressed
 * @return true if mouse right button is down
 */
bool handler_keyboard::is_right_button ()
{
  return is_right_button_down;
}

/**
 * Test if mouse right and left button are pressed (start gigablitz)
 * @return true if mouse right and left buttons are down
 */
bool handler_keyboard::is_right_left_buttons ()
{
  if (is_right_button_down && is_left_button_down)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Test if the left mouse button were released 
 * @param x_coord pointer to integer in which to return x mouse
 * @param y_coord pointer to integer in which to return y mouse
 * @return true if the left mouse button is released
 */
bool handler_keyboard::is_left_button_up (Sint32 * x_coord, Sint32 * y_coord)
{
  *x_coord = mouse_x_coord;
  *y_coord = mouse_y_coord;
  return is_left_button_released;
}

/**
 * Test if the right mouse button were released 
 * @param x_coord pointer to integer in which to return x mouse
 * @param y_coord pointer to integer in which to return y mouse
 * @return true if the right mouse button is released
 */
bool handler_keyboard::is_right_button_up (Sint32 * x_coord, Sint32 * y_coord)
{
  *x_coord = mouse_x_coord;
  *y_coord = mouse_y_coord;
  return is_right_button_released;
}

/**
 * Caculate and return offset of horizontal displacement of the mouse
 * @return the mouse horizonal offset 
 */
Sint32 handler_keyboard::get_mouse_x_offset ()
{
  mouse_x_offset = mouse_x_coord - previous_mouse_x_coord;
  mouse_y_offset = mouse_y_coord - previous_mouse_y_coord;
  previous_mouse_x_coord = mouse_x_coord;
  previous_mouse_y_coord = mouse_y_coord;
  hasard_val += previous_mouse_x_coord;
  hasard_val += previous_mouse_y_coord;
  return mouse_x_offset;
}

/**
 * Return absolute mouse y coordinate 
 * @return the mouse pointer y coordinate
 */
Sint32 handler_keyboard::get_mouse_y ()
{
  return mouse_y_coord;
}

/**
 * Return absolute mouse x coordinate 
 * @return the mouse pointer x coordinate
 */
Sint32 handler_keyboard::get_mouse_x ()
{
  return mouse_x_coord;
}

/**
 * Set new pressed key for handle string input
 * @param kCode key code of the pressed key
 */
void
handler_keyboard::set_key_code_down (Sint32 kCode)
{
  if (kCode != SDLK_LSHIFT && kCode != SDLK_RSHIFT && kCode != SDLK_LCTRL &&
      kCode != SDLK_RCTRL)
    key_code_down = kCode;
}

/**
 * Set new relased key for handle string input
 * @param kCode key code of the pressed key
 */
void
handler_keyboard::set_keycode_up (Sint32 kCode)
{
  if (kCode != SDLK_LSHIFT && kCode != SDLK_RSHIFT && kCode != SDLK_LCTRL &&
      kCode != SDLK_RCTRL)
    {
      code_keyup = kCode;
      if (code_keyup == key_code_down)
        {
          key_code_down = 0;
        }
      key_delay = 0;
      previous_key_code_down = 0;
    }
}

/**
 * Handle string input
 */
void
handler_keyboard::input_string ()
{
  Sint32 kCode = 0;
  if (key_delay < 1)
    {
      kCode = key_code_down;
      if (key_code_down > 0)
        {
          /* it is key pressed for the first time? */
          if (previous_key_code_down != key_code_down)
            {
              previous_key_code_down = key_code_down;
              key_delay = 30;
            }
          else
            {
              /* repeat key press delay */
              key_delay = 10;
            }
        }
    }
  else
    {
      kCode = 0;
      key_delay--;
    }
  if (current_input_string)
    {
      input_string (kCode);
    }
}

/**
 * Handle input string
 * @param kCode key code enter
 */
void
handler_keyboard::input_string (Sint32 kCode)
{
  Sint32 j, i;
  if (0 == kCode)
    {
      return;
    }

  /* check pressed key */
  switch (kCode)
    {
    case SDLK_LEFT:
      string_cursor_pos--;
      break;
    case SDLK_RIGHT:
      string_cursor_pos++;
      break;

      /* backspace key pressed */
    case SDLK_BACKSPACE:
      if (string_cursor_pos > 0)
        j = string_cursor_pos;
      else
        j = 1;
      for (i = j; i < string_input_size; i++)
        {
          current_input_string[i - 1] = current_input_string[i];
        }
      current_input_string[string_input_size - 1] = ' ';
      string_cursor_pos--;
      break;

      /* [Suppr] key pressed? */
    case SDLK_DELETE:
      for (i = string_cursor_pos; i < string_input_size - 1; i++)
        {
          current_input_string[i] = current_input_string[i + 1];
        }
      current_input_string[string_input_size - 1] = ' ';
      break;

      /* [Return] or [Enter] pressed, stop string input */
    case SDLK_RETURN:
      stop_string_input ();
      return;
      break;

    default:
      kCode = kCode & 127;
      if (kCode >= 'a' && kCode <= 'z')
        {
          kCode = kCode - 32;
        }

      /* space (32) / ! (33) 
       * , (44) /  - (45) / . (46) /
       * : (58) / 0-9 (48-57) 
       * A-Z (65 to 90)
       */
      if ((kCode >= ' ' && kCode <= '!') ||
          (kCode >= '-' && kCode <= '.') ||
          (kCode >= '0' && kCode <= ':') ||
          (kCode >= 'A' && kCode <= 'Z') || kCode == '\'')
        {
          for (i = string_input_size - 1; i > string_cursor_pos; i--)
            {
              current_input_string[i] = current_input_string[i - 1];
            }
          current_input_string[string_cursor_pos] = kCode;
          string_cursor_pos++;
        }
    }

  /* verify overflow position cursor */
  if (string_cursor_pos < 0)
    {
      string_cursor_pos = 0;
    }
  if (string_cursor_pos > string_input_size - 1)
    {
      string_cursor_pos = string_input_size - 1;
    }
}

/**
 * Initializes a string input
 * @param strng pointer to a string
 * @param ssize size of the string
 */
void
handler_keyboard::set_input_string (char *strng, Uint32 ssize)
{
  string_cursor_pos = 0;
  string_input_size = ssize;
  current_input_string = strng;
}

/**
 * Return the current cursor position in the input string
 * @return current cursor position 0 to n
 */
Sint32 handler_keyboard::get_input_cursor_pos ()
{
  if (NULL == current_input_string)
    {
      /* not current input string */
      return -1;
    }
  return string_cursor_pos;
}

/** 
 * Stop using string input
 */
void
handler_keyboard::stop_string_input ()
{
  string_cursor_pos = 0;
  string_input_size = 0;
  current_input_string = NULL;
}

Sint32 handler_keyboard::get_key_down_code ()
{
  return key_code_down;
}
