
/**
 * @file handler_menu_events.cc
 * @brief Handler the events of the menu
 * @created 2007-1O-29
 * @date 2007-11-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_menu_events.h,v 1.3 2007/11/17 16:59:43 gurumeditation Exp $
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
#ifndef __HANDLER__MENU__EVENTS__
#define __HANDLER__MENU__EVENTS__

#include "../include/tecnoballz.h"

class handler_menu_events:public virtual tecnoballz
  {
  public:
    handler_menu_events();
    ~handler_menu_events();
    void stop();
    void start(Sint32 spacing, Sint32 min, Sint32 max, Sint32 xcenter,
               Sint32 ytop);
    bool check(Sint32 *pos_y, Sint32 *inc);

  private:
    void check_keys(Uint32 *kcode, Uint32 *prev_kcode);

  private:
    static const Uint32 MAX_OF_KEYS = 6;
    static Uint32 keys[MAX_OF_KEYS];
    bool is_enabled;
    Sint32 y_coord_left_down;
    Sint32 y_coord_right_down;
    Uint32 line_spacing;
    Sint32 x_center;
    Sint32 top_y_coord;
    Uint32 line_min;
    Uint32 line_max;
    Uint32 current_line;
    Sint32 key_delay;
    Uint32 previous_key_code_down;
  };

#endif

