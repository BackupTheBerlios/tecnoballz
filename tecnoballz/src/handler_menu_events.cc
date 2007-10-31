/** 
 * @file handler_menu_events.cc 
 * @brief Handler the events of the menu 
 * @created 2007-1O-29
 * @date 2007-10-29
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_menu_events.cc,v 1.1 2007/10/31 07:35:29 gurumeditation Exp $
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
#include "../include/handler_menu_events.h"
#include "../include/handler_keyboard.h"

/**
 *
 */
handler_menu_events::handler_menu_events()
{
  stop();
}

handler_menu_events::~handler_menu_events()
{
  stop();
}

/**
 *
 */
void handler_menu_events::stop() 
{
  is_enabled = false;
  y_coord_left_down = handler_keyboard::NULL_YCOORD;
  y_coord_right_down =  handler_keyboard::NULL_YCOORD;
  line_spacing = 0;
  xcenter = 0;
  top_y_coord = 0;
  line_min = 0;
  line_max = 0;
  current_line = 0;
  key_delay = 0;
  previous_key_code_down = 0;
}

/**
 * Start a new vertical menu handler
 * @param spacing Space between lines in pixels
 * @param min Minimum line number
 * @param max Maximum line number
 * @param xcenter X-coordinate center of the menu
 * @param ytop Y-coordinate top of the menu 
 */
void handler_menu_events::start(Sint32 spacing, Sint32 min, Sint32 max,
    Sint32 xcenter, Sint32 ytop)
{
  if (is_verbose)
    {
      std::cout << " handler_mene_events::start_) " <<
        " line_spacing: " << spacing << " line_min: " << min <<
        " line_max: " << max << " xcenter: " << xcenter <<
        " top_y_coord: " << ytop << std::endl;
    }
  is_enabled = true;
  y_coord_left_down = handler_keyboard::NULL_YCOORD;
  y_coord_right_down = handler_keyboard::NULL_YCOORD;
  line_spacing = spacing;
  line_min = min;
  line_max = max;
  xcenter = xcenter;
  current_line = 0;
  top_y_coord = ytop;
  previous_key_code_down = 0;
  Sint32 xmouse, ymouse;
  SDL_GetMouseState  (&xmouse, &ymouse);
  if (ymouse < ytop + min * spacing)
    {
      current_line = min;
    }
  else if (ymouse > ytop + max * spacing)
    {
      current_line = max;
    }
  else
    {
      current_line = (ymouse - ytop) / spacing;
    }
}



Uint32 handler_menu_events::keys[MAX_OF_KEYS] =
{
  handler_keyboard::K_UP,
  handler_keyboard::K_DOWN,
  handler_keyboard::K_FIRE,
  handler_keyboard::K_RELEASE_BALL,
  handler_keyboard::K_GIGABLITZ,
  handler_keyboard::K_TILT
};

/**
 * Check mouses events for the main menu and popup menu 
 * @param pos_y Pointer to a integer which will contain y-coordinate
 *              where the mouse clicked 
 * @param inc Pointer to a integer which will contain -1 if left mouse
 *            button clicked, 1 if right button clicked, otherwise 0
 * @return true if left mouse button clicked, otherwise false
 */
bool
handler_menu_events::check (Sint32 *pos_y, Sint32 *inc)
{
  if (!is_enabled)
    {
      return false;
    }
  bool is_selected = false;
  Uint32 kcode = 0;
  *inc = 0;

  /* check keyboards events */
  if(previous_key_code_down > 0 &&
      !keyboard->control_is_pressed(previous_key_code_down))
    {
      switch(previous_key_code_down)
        {
        case handler_keyboard::K_FIRE:
          *inc = 1;
          is_selected = true;
          break;
        case handler_keyboard::K_RELEASE_BALL:
          *inc = -1;
          is_selected = true;
          break;
        }
      previous_key_code_down = 0;
      key_delay = 0;
    }
  if (key_delay < 1)
    {
      for(Uint32 i = 0; i < MAX_OF_KEYS; i++)
        {
          if(!keyboard->control_is_pressed(keys[i]))
            {
              continue;
            }
          kcode = keys[i];
          if (previous_key_code_down != keys[i])
            {
              previous_key_code_down = keys[i];
              key_delay = 30;
            }
          else
            {
              key_delay = 10;
            }
          break;
        }
    }
  else
    {
      kcode = 0;
      key_delay--;
    }

  /* check if right or left button are pressed */
  Sint32 mposx;
  switch(kcode)
    {
    case handler_keyboard::K_FIRE:
      //*inc = 1;
      //is_selected = true;
      break;
    case handler_keyboard::K_RELEASE_BALL:
      //*inc = -1;
      //is_selected = true;
      break;
    case handler_keyboard::K_UP:
      if(current_line ==  line_min)
        {
          current_line = line_max;
        }
      else
        {
          current_line--;
        }
      SDL_WarpMouse(xcenter, top_y_coord +
                    current_line * line_spacing);
      break;
    case handler_keyboard::K_DOWN:
      if(current_line == line_max)
        {
          current_line =  line_min;
        }
      else
        {
          current_line++;
        }
      SDL_WarpMouse(xcenter, top_y_coord +
                    current_line * line_spacing);
      break;
    }

  
  /*
   * check mouse events
   */
  bool is_left_down = keyboard->is_left_button ();
  bool is_right_down = keyboard->is_right_button ();
  /* read y where is pressed */
  if (is_left_down && y_coord_left_down == handler_keyboard::NULL_YCOORD)
    {
      y_coord_left_down = keyboard->get_mouse_y ();
    }
  else
    {
      if (is_right_down && y_coord_right_down == handler_keyboard::NULL_YCOORD)
        {
          y_coord_right_down = keyboard->get_mouse_y ();
        }
    }

  bool is_right_up = false;
  bool is_left_up = keyboard->is_left_button_up (&mposx, pos_y);
  if (!is_left_up)
    {
      is_right_up = keyboard->is_right_button_up (&mposx, pos_y);
    }
  if ((is_left_up && *pos_y == y_coord_left_down) ||
      (is_right_up && *pos_y == y_coord_right_down))
    {
      if (is_left_up)
        {
          *inc = 1;
          y_coord_left_down = handler_keyboard::NULL_YCOORD;
        }
      if (is_right_up)
        {
          *inc = -1;
          y_coord_right_down = handler_keyboard::NULL_YCOORD;
        }
      is_selected = true;
    }
  if (!is_left_down)
    {
      y_coord_left_down = handler_keyboard::NULL_YCOORD;
    }
  if (!is_right_down)
    {
      y_coord_right_down = handler_keyboard::NULL_YCOORD;
    }
  return is_selected;
}





