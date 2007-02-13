//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "controller_magnetic_eyes.h"
// created              : 2004-09-17
// updates              : 2004-09-17
// function     : manage eye magneto (only bricks levels)
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#ifndef __ZE_MAGNEYE__
#define __ZE_MAGNEYE__
class controller_magnetic_eyes;
//...............................................................................
#include "../include/list_sprites.h"
#include "../include/sprite_eye.h"
#include "../include/objects_list.h"
//..............................................................................
class controller_magnetic_eyes:public objects_list < sprite_eye, controller_magnetic_eyes >
{
  friend class sprite_eye;

private:
  static const Uint16 Xcoordinat[32];
  static const Uint16 Ycoordinat[32];

public:
    Sint32 hypotenuse;
  Sint32 eyeCenterX;
  Sint32 eyeCenterY;

public:
    controller_magnetic_eyes ();
   ~controller_magnetic_eyes ();
  Sint32 create_eye ();
  void create_eyes_list ();
  void execution1 ();
};
#endif
