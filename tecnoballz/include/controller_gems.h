/** 
 * @file controller_gems.cc 
 * @brief The gems controller 
 * @created 2004-04-12 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gems.h,v 1.3 2007/02/10 20:22:17 gurumeditation Exp $
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
#include "../include/printmoney.h"

class controller_gems:public objects_list < sprite_gem >
{
public:
  static const Uint32 MAX_OF_GEMS = 6;

private:
  controller_paddles * paddles;
  right_panel_score *ptBarScore;
  printmoney *ptPrintmon;

public:
    controller_gems ();
   ~controller_gems ();
  void initialize (right_panel_score *, printmoney *, controller_paddles *);
  void send (sprite_ball * ball);
  void send (sprite_projectile * blast);
  void move ();
};
#endif
