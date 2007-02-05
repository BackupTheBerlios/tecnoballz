/** 
 * @file sprite_gigablitz.h
 * @brief The sprite of the Gigablitz 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_gigablitz.h,v 1.1 2007/02/05 20:16:33 gurumeditation Exp $
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
#ifndef __SPRITE_GIGABLITZ__
#define __SPRITE_GIGABLITZ__

class sprite_gigablitz;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
class sprite_gigablitz:public sprite_object
{
private:

public:
  sprite_gigablitz ();
  ~sprite_gigablitz ();
};
#endif
