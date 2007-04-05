/** 
 * @file controller_spheres.h
 * @brief Metallic spheres controller used in congratulations 
 * @created 2004-08-05 
 * @date 2007-04-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_spheres.h,v 1.5 2007/04/05 16:04:01 gurumeditation Exp $
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
#ifndef __CONTROLLER_SPHERES__
#define __CONTROLLER_SPHERES__

class controller_spheres;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
#include "../include/sprite_display_scores.h"

class controller_spheres:public objects_list < sprite_object, controller_spheres >
{
private:
  Sint32 speed_rad0;
  Sint32 speed_rad1;
  Sint32 speed_rad2;
  Sint32 speed_rad3;
  Sint32 speed_rad4;
public:
    controller_spheres ();
   ~controller_spheres ();
  void initialize ();
  void execution1 ();

};
#endif
