/**
 * @file handler_keyboard.cc 
 * @brief Handler of the keyboard and mouse
 * @date 2007-10-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.14 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_keyboard.cc,v 1.14 2007/10/12 15:30:07 gurumeditation Exp $
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
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"

bool handler_keyboard::command_keys[NUMOF_COMMAND_KEYS];
bool handler_keyboard::last_command_keys[NUMOF_COMMAND_KEYS];
Uint32 handler_keyboard::key_codes[K_MAXOF] =
  {
    SDLK_LEFT,
    SDLK_RIGHT,
    SDLK_UP,
    SDLK_DOWN,
    SDLK_SPACE,
    SDLK_LCTRL,
    SDLK_LALT,
    SDLK_y,
    SDLK_x,
    SDLK_ESCAPE,
    SDLK_PAGEUP,
    SDLK_PAGEDOWN
  };
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
  stop_menu_events();
 
  for (Uint32 i = 0; i < NUMOF_COMMAND_KEYS; i++)
    {
      command_keys[i] = false;
      last_command_keys[i] = false;
    }
  init_joysticks();
}

/**
 * Release the handler_keyboard singleton object
 */
handler_keyboard::~handler_keyboard ()
{
  object_free ();
  keyboard_singleton = NULL;
  if (numof_joysticks > 0)
    {
      for (Uint32 i = 0; i < numof_joysticks; i++)
        {
          if (sdl_joysticks[i] != NULL)
            {
              SDL_JoystickClose (sdl_joysticks[i]);
              sdl_joysticks[i] = NULL;
            }
        }
      delete[] sdl_joysticks;
      numof_joysticks = 0;
    }
}

/**
 * Open joystick(s) if available 
 */
void
handler_keyboard::init_joysticks()
{
  fire_button_down = false;
  option_button_down = false;
  start_button_down = false;
  joy_left = false;
  joy_right = false;
  joy_top = false;
  joy_down = false;
  numof_joysticks = SDL_NumJoysticks ();
  if (is_verbose)
    {
      std::cout << "handler_keyboard::init_joysticks()" <<
      " number of joysticks available: " << numof_joysticks << std::endl;
    }
  if (numof_joysticks > 0)
    {
      sdl_joysticks = new SDL_Joystick *[numof_joysticks];
      for (Uint32 i = 0; i < numof_joysticks; i++)
        {

          sdl_joysticks[i] = SDL_JoystickOpen (0);
          if (sdl_joysticks[i] == NULL)
            {
              std::cerr << "(!)handler_keyboard::init_joysticks()" <<
              " couldn't open joystick " << i << ": "
              << SDL_GetError () << std::endl;
            }
          else
            {
              if (is_verbose)
                {
                  std::cout << "- joystick  : " << SDL_JoystickName (i) << std::endl;
                  std::cout << "- axes      : " << SDL_JoystickNumAxes (sdl_joysticks[i]) << std::endl;
                  std::cout << "- buttons   : " << SDL_JoystickNumButtons (sdl_joysticks[i]) << std::endl;
                  std::cout << "- trackballs: " << SDL_JoystickNumButtons (sdl_joysticks[i]) << std::endl;
                  std::cout << "- hats      : " << SDL_JoystickNumHats (sdl_joysticks[i]) << std::endl;
                }
            }
        }
    }
}

bool
handler_keyboard::is_joy_left()
{
  return joy_left ? true : false;
}

bool
handler_keyboard::is_joy_right()
{
  return joy_right ? true : false;
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
 * Grabs mouse and keyboard input, the mouse is confined to the
 * application window, and nearly all keyboard input is passed
 * directly to the application
 * @param mode true to enable grab input, false to disable it
 */
void
handler_keyboard::set_grab_input (bool mode)
{

#ifdef UNDER_DEVELOPMENT
  return;
#else
  if (mode)
    {
      is_grab_input = true;
      SDL_WM_GrabInput (SDL_GRAB_ON);
      SDL_EventState( SDL_MOUSEMOTION, SDL_IGNORE );
      SDL_WarpMouse(display->get_width() >> 1, display->get_height() >> 1);
      SDL_EventState( SDL_MOUSEMOTION, SDL_ENABLE );
    }
  else
    {
      is_grab_input = false;
      SDL_WM_GrabInput (SDL_GRAB_OFF);
    }
#endif
}

/**
 * Read keyboard mouse and exit events
 */
void
handler_keyboard::read_events ()
{
  command_keys[TOGGLE_FULLSCREEN] = false;
  command_keys[DISABLE_TIMER] = false;
  command_keys[VOLUME_DOWN] = false;
  command_keys[VOLUME_UP] = false;
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
                    {
                      last_command_keys[COMMAND_KEY_PAUSE] = true;
                    }
                  if (keys[SDLK_f] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_FULLSCREEN] = true;
                    }
                  if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_POPUP_MENU] = true;
                    }
                  if (keys[SDLK_l] == SDL_PRESSED)
                    {
                      last_command_keys[DISABLE_TIMER] = true;
                    }
                  if (keys[SDLK_PAGEDOWN] == SDL_PRESSED)
                    {
                      last_command_keys[VOLUME_DOWN] = true;
                    }
                  if (keys[SDLK_PAGEUP] == SDL_PRESSED)
                    {
                      last_command_keys[VOLUME_UP] = true;
                    }
                }
              else
                {
                  if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                    {
                      last_command_keys[QUIT_TECNOBALLZ] = true;
                    }
                  if (keys[SDLK_x] == SDL_PRESSED)
                    {
                      last_command_keys[CAUSE_GAME_OVER] = true;
                    }
                  if (keys[SDLK_q] == SDL_PRESSED)
                    {
                      last_command_keys[QUIT_TO_MAIN_MENU] = true;
                    }
                  if (keys[SDLK_f] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_SOUND] = true;
                    }
                  if (keys[SDLK_s] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_AUDIO] = true;
                    }
                  if (keys[SDLK_d] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_MUSIC] = true;
                    }
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
              if (!command_keys[TOGGLE_POPUP_MENU])
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
          if (keys[TOGGLE_POPUP_MENU] == SDL_RELEASED
              && last_command_keys[TOGGLE_POPUP_MENU])
            {
              last_command_keys[TOGGLE_POPUP_MENU] = false;
              command_keys[TOGGLE_POPUP_MENU] =
                command_keys[TOGGLE_POPUP_MENU] ? false : true;
              command_keys[COMMAND_KEY_PAUSE] = command_keys[TOGGLE_POPUP_MENU];
              if (is_grab_input && command_keys[COMMAND_KEY_PAUSE])
                {
                  SDL_WM_GrabInput (SDL_GRAB_OFF);
                }
              if (is_grab_input && !command_keys[COMMAND_KEY_PAUSE])
                {
                  SDL_WM_GrabInput (SDL_GRAB_ON);
                }

              if (!command_keys[TOGGLE_POPUP_MENU])
                {
                  SDL_ShowCursor (SDL_DISABLE);
                }
              if (command_keys[TOGGLE_POPUP_MENU])
                {
                  SDL_ShowCursor (SDL_ENABLE);
                }
            }

          if (keys[SDLK_f] == SDL_RELEASED && last_command_keys[TOGGLE_FULLSCREEN])
            {
              last_command_keys[TOGGLE_FULLSCREEN] = false;
              command_keys[TOGGLE_FULLSCREEN] = true;
            }
          {
            if (keys[SDLK_ESCAPE] == SDL_RELEASED
                && last_command_keys[QUIT_TECNOBALLZ])
              {
                last_command_keys[QUIT_TECNOBALLZ] = false;
                command_keys[QUIT_TECNOBALLZ] = true;
              }
            if (keys[SDLK_x] == SDL_RELEASED
                && last_command_keys[CAUSE_GAME_OVER])
              {
                last_command_keys[CAUSE_GAME_OVER] = false;
                command_keys[CAUSE_GAME_OVER] = true;
              }
            if (keys[SDLK_q] == SDL_RELEASED
                && last_command_keys[QUIT_TO_MAIN_MENU])
              {
                last_command_keys[QUIT_TO_MAIN_MENU] = false;
                command_keys[QUIT_TO_MAIN_MENU] = true;
              }
            if (keys[SDLK_f] == SDL_RELEASED
                && last_command_keys[TOGGLE_SOUND])
              {
                last_command_keys[TOGGLE_SOUND] = false;
                command_keys[TOGGLE_SOUND] = true;
              }
            if (keys[SDLK_s] == SDL_RELEASED
                && last_command_keys[TOGGLE_AUDIO])
              {
                last_command_keys[TOGGLE_AUDIO] = false;
                command_keys[TOGGLE_AUDIO] = true;
              }
            if (keys[SDLK_d] == SDL_RELEASED
                && last_command_keys[TOGGLE_MUSIC])
              {
                last_command_keys[TOGGLE_MUSIC] = false;
                command_keys[TOGGLE_MUSIC] = true;
              }
            if (keys[SDLK_l] == SDL_RELEASED
                && last_command_keys[DISABLE_TIMER])
              {
                last_command_keys[DISABLE_TIMER] = false;
                command_keys[DISABLE_TIMER] = true;
              }
            if (keys[SDLK_PAGEDOWN] == SDL_RELEASED
                && last_command_keys[VOLUME_DOWN])
              {
                last_command_keys[VOLUME_DOWN] = false;
                command_keys[VOLUME_DOWN] = true;
              }
            if (keys[SDLK_PAGEUP] == SDL_RELEASED
                && last_command_keys[VOLUME_UP])
              {
                last_command_keys[VOLUME_UP] = false;
                command_keys[VOLUME_UP] = true;
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

        case SDL_JOYAXISMOTION:
        {
          Sint32 deadzone = 4096;
          /* x axis */
          if (event.jaxis.axis == 0)
            {
              if (event.jaxis.value < -deadzone)
                {
                  joy_left = true;
                  //sprites_string_set_joy (IJOY_LEFT);
                  joy_right = false;
                }
              else if (event.jaxis.value > deadzone)
                {
                  joy_left = false;
                  joy_right = true;
                  //sprites_string_set_joy (IJOY_RIGHT);
                }
              else
                {
                  joy_left = false;
                  joy_right = false;
                  //sprites_string_clr_joy (IJOY_RIGHT);
                  //sprites_string_clr_joy (IJOY_LEFT);
                }
            }
          /* y axis */
          else if (event.jaxis.axis == 1)
            {
              if (event.jaxis.value < -deadzone)
                {
                  joy_down = false;
                  joy_top = true;
                  //sprites_string_set_joy (IJOY_TOP);
                }
              else if (event.jaxis.value > deadzone)
                {
                  joy_down = true;
                  joy_top = false;
                  //sprites_string_set_joy (IJOY_DOWN);
                }
              else
                {
                  joy_down = false;
                  joy_top = false;
                  //sprites_string_clr_joy (IJOY_TOP);
                  //sprites_string_clr_joy (IJOY_DOWN);
                }
            }
        }
        break;
        case SDL_JOYBUTTONDOWN:
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
          //handle_console_buttons (&event);
#else
          if (event.jbutton.button == 2)
            {
              start_button_down = true;
            }
          else if (event.jbutton.button == 0)
            {
              fire_button_down = true;
              //sprites_string_set_joy (IJOY_FIRE);
            }
          else if (event.jbutton.button == 1)
            {
              option_button_down = true;
              //sprites_string_set_joy (IJOY_OPT);
            }
          break;
#endif
        case SDL_JOYBUTTONUP:
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
          //handle_console_buttons (&event);
#else
          if (event.jbutton.button == 2)
            {
              start_button_down = false;
            }
          else if (event.jbutton.button == 0)
            {
              fire_button_down = false;
              //sprites_string_clr_joy (IJOY_FIRE);
            }
          else if (event.jbutton.button == 1)
            {
              option_button_down = false;
              //sprites_string_clr_joy (IJOY_OPT);
            }
#endif
          break;

          /* quit the game */
        case SDL_QUIT:
        {
          command_keys[QUIT_TECNOBALLZ] = true;
        }
        break;
        }
    }
  random_counter += mouse_x_coord;
  random_counter += mouse_y_coord;
  input_string ();
}

/**
 * Check if a key is pressed
 * @param code a SDL Keysym definition
 * @return true if the key is pressed 
 */
bool handler_keyboard::key_is_pressed (Sint32 code)
{
  Uint8 * keys;
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
  Uint8 * keys;
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
 * Check if a control key is pressed
 * @param code A code of key 
 * @return true if the key is pressed 
 */
bool
handler_keyboard::control_is_pressed (Uint32 code)
{
  Uint8 * keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[key_codes[code]] == SDL_PRESSED)
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
  command_keys[QUIT_TO_MAIN_MENU] = false;
  command_keys[CAUSE_GAME_OVER] = false;
  command_keys[TOGGLE_AUDIO] = false;
  command_keys[TOGGLE_SOUND] = false;
  command_keys[TOGGLE_MUSIC] = false;
  command_keys[TOGGLE_POPUP_MENU] = false;
  last_command_keys[COMMAND_KEY_PAUSE] = false;
  last_command_keys[QUIT_TO_MAIN_MENU] = false;
  last_command_keys[CAUSE_GAME_OVER] = false;
  last_command_keys[TOGGLE_AUDIO] = false;
  last_command_keys[TOGGLE_SOUND] = false;
  last_command_keys[TOGGLE_MUSIC] = false;
  last_command_keys[TOGGLE_POPUP_MENU] = false;
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
  bool is_pressed = command_keys[code];
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
  random_counter += previous_mouse_x_coord;
  random_counter += previous_mouse_y_coord;
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
 * @param kcode key code of the pressed key
 */
void
handler_keyboard::set_key_code_down (Uint32 kcode)
{
  if (kcode != SDLK_LSHIFT && kcode != SDLK_RSHIFT && kcode != SDLK_LCTRL &&
      kcode != SDLK_RCTRL)
    key_code_down = kcode;
}

/**
 * Set new relased key for handle string input
 * @param kcode key code of the pressed key
 */
void
handler_keyboard::set_keycode_up (Uint32 kcode)
{
  if (kcode != SDLK_LSHIFT && kcode != SDLK_RSHIFT && kcode != SDLK_LCTRL &&
      kcode != SDLK_RCTRL)
    {
      code_keyup = kcode;
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
  Sint32 kcode = 0;
  if (key_delay < 1)
    {
      kcode = key_code_down;
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
      kcode = 0;
      key_delay--;
    }
  if (current_input_string)
    {
      input_string (kcode);
    }
}

/**
 * Handle input string
 * @param kcode Key code enter
 */
void
handler_keyboard::input_string (Uint32 kcode)
{
  Sint32 j, i;
  if (0 == kcode)
    {
      return;
    }

  /* check pressed key */
  switch (kcode)
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
        {
          j = string_cursor_pos;
        }
      else
        {
          j = 1;
        }
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
      kcode = kcode & 127;
      if (kcode >= 'a' && kcode <= 'z')
        {
          kcode = kcode - 32;
        }

      /* space (32) / ! (33)
       * , (44) /  - (45) / . (46) /
       * : (58) / 0-9 (48-57) 
       * A-Z (65 to 90)
       */
      if ((kcode >= ' ' && kcode <= '!') ||
          (kcode >= '-' && kcode <= '.') ||
          (kcode >= '0' && kcode <= ':') ||
          (kcode >= 'A' && kcode <= 'Z') || kcode == '\'')
        {
          for (i = string_input_size - 1; i > string_cursor_pos; i--)
            {
              current_input_string[i] = current_input_string[i - 1];
            }
          current_input_string[string_cursor_pos] = kcode;
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

Uint32 handler_keyboard::get_key_down_code ()
{
  return key_code_down;
}


void
handler_keyboard::stop_menu_events()
{
  menu_events.is_enabled = false;
  menu_events.y_coord_left_down = NULL_YCOORD;
  menu_events.y_coord_right_down = NULL_YCOORD;
  menu_events.line_spacing = 0;
  menu_events.xcenter = 0;
  menu_events.top_y_coord = 0;
  menu_events.line_min = 0;
  menu_events.line_max = 0;
  menu_events.current_line = 0;
  menu_events.key_delay = 0;
}

/**
 * Start a new vertical menu handler
 * @param spacing Space between lines in pixels
 * @param min Minimum line number
 * @param max Maximum line number
 * @param xcenter X-coordinate center of the menu
 * @param ytop Y-coordinate top of the menu 
 */
void
handler_keyboard::start_menu_events(Sint32 spacing, Sint32 min, Sint32 max,
    Sint32 xcenter, Sint32 ytop)
{
  menu_events.is_enabled = true;
  menu_events.y_coord_left_down = NULL_YCOORD;
  menu_events.y_coord_right_down = NULL_YCOORD;
  menu_events.line_spacing = spacing;
  menu_events.line_min = min;
  menu_events.line_max = max;
  menu_events.xcenter = xcenter;
  menu_events.current_line = 0;
  menu_events.top_y_coord = ytop;
  Sint32 xmouse, ymouse;
  SDL_GetMouseState  (&xmouse, &ymouse);
  if (ymouse < ytop + min * spacing)
    {
      menu_events.current_line = min;
    }
  else if (ymouse > ytop + max * spacing)
    {
      menu_events.current_line = max;
    }
  else
    {
      menu_events.current_line = (ymouse - ytop) / spacing;
    }
}


Uint32 handler_keyboard::menu_events_keys[6] =
  { K_UP, K_DOWN, K_FIRE, K_RELEASE_BALL, K_GIGABLITZ, K_TILT };

/**
 * Check mouses events for the main menu and popup menu 
 * @param pos_y Pointer to a integer which will contain y-coordinate
 *              where the mouse clicked 
 * @param inc Pointer to a integer which will contain -1 if left mouse
 *            button clicked, 1 if right button clicked, otherwise 0
 * @return true if left mouse button clicked, otherwise false
 */
bool
handler_keyboard::check_menu_events (Sint32 *pos_y, Sint32 *inc)
{
  if (!menu_events.is_enabled)
    {
      return false;
    }
  bool is_selected = false;
  Uint32 kcode = 0;
  *inc = 0;

  /* check keyboards events */
  if(menu_events.previous_key_code_down > 0 &&
      !control_is_pressed(menu_events.previous_key_code_down))
    {
      menu_events.previous_key_code_down = 0;
      menu_events.key_delay = 0;
    }
  if (menu_events.key_delay < 1)
    {
      for(Uint32 i = 0; i < 6; i++)
        {
          if(!control_is_pressed(menu_events_keys[i]))
            {
              continue;
            }
          kcode = menu_events_keys[i];
          if (menu_events.previous_key_code_down != menu_events_keys[i])
            {
              menu_events.previous_key_code_down = menu_events_keys[i];
              menu_events.key_delay = 30;
            }
          else
            {
              menu_events.key_delay = 10;
            }
          break;
        }
    }
  else
    {
      kcode = 0;
      menu_events.key_delay--;
    }


  /* check if right or left button are pressed */
  Sint32 mposx;
  switch(kcode)
    {
    case K_FIRE:
      *inc = 1;
      is_selected = true;
      break;
    case K_RELEASE_BALL:
      *inc = -1;
      is_selected = true;
      break;
    case K_UP:
      if(menu_events.current_line ==  menu_events.line_min)
        {
          menu_events.current_line = menu_events.line_max;
        }
      else
        {
          menu_events.current_line--;
        }
      SDL_WarpMouse(menu_events.xcenter, menu_events.top_y_coord +
                    menu_events.current_line * menu_events.line_spacing);
      break;
    case K_DOWN:
      if(menu_events.current_line == menu_events.line_max)
        {
          menu_events.current_line =  menu_events.line_min;
        }
      else
        {
          menu_events.current_line++;
        }
      SDL_WarpMouse(menu_events.xcenter, menu_events.top_y_coord +
                    menu_events.current_line * menu_events.line_spacing);
      break;
    }

  
  /*
   * check mouse events
   */
  bool is_left_down = keyboard->is_left_button ();
  bool is_right_down = keyboard->is_right_button ();
  /* read y where is pressed */
  if (is_left_down && menu_events.y_coord_left_down == NULL_YCOORD)
    {
      menu_events.y_coord_left_down = keyboard->get_mouse_y ();
    }
  else
    {
      if (is_right_down && menu_events.y_coord_right_down == NULL_YCOORD)
        {
          menu_events.y_coord_right_down = keyboard->get_mouse_y ();
        }
    }

  bool is_right_up = false;
  bool is_left_up = keyboard->is_left_button_up (&mposx, pos_y);
  if (!is_left_up)
    {
      is_right_up = keyboard->is_right_button_up (&mposx, pos_y);
    }
  if ((is_left_up && *pos_y == menu_events.y_coord_left_down) ||
      (is_right_up && *pos_y == menu_events.y_coord_right_down))
    {
      if (is_left_up)
        {
          *inc = 1;
          menu_events.y_coord_left_down = NULL_YCOORD;
        }
      if (is_right_up)
        {
          *inc = -1;
          menu_events.y_coord_right_down = NULL_YCOORD;
        }
      is_selected = true;
    }
  if (!is_left_down)
    {
      menu_events.y_coord_left_down = NULL_YCOORD;
    }
  if (!is_right_down)
    {
      menu_events.y_coord_right_down = NULL_YCOORD;
    }
  return is_selected;
}






