/** 
 * @file controller_gems.cc 
 * @brief The gems controller 
 * @created 2004-04-12 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gems.h,v 1.5 2007/02/13 17:11:02 gurumeditation Exp $
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
#ifndef __CONTROLLER_GEMS__
#define __CONTROLLER_GEMS__

class controller_gems;

#include "../include/objects_list.h"
#include "../include/controller_paddles.h"
#include "../include/sprite_gem.h"
#include "../include/handler_players.h"
#include "../include/controller_indicators.h"

class controller_gems:public objects_list < sprite_gem, controller_gems >
{
public:
  static const Uint32 MAX_OF_GEMS = 6;

private:
  controller_paddles * paddles;
  right_panel_score *ptBarScore;
  controller_indicators *ptPrintmon;

public:
    controller_gems ();
   ~controller_gems ();
  void initialize (right_panel_score *, controller_indicators *, controller_paddles *);
  void send (sprite_ball * ball);
  void send (sprite_projectile * blast);
  void move ();
};
#endif
