//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "sprite_eye.h"
// created              : 2004-09-17
// updates              : 2004-09-17
// function     : handle eye (only bricks levels)
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
#ifndef __TECHNO_EYE__
#define __TECHNO_EYE__
//.......................................................................
class sprite_eye;
//.......................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/controller_magnetic_eyes.h"
//.......................................................................
class sprite_eye:public sprite_object
{
  friend class controller_balls;
  friend class controller_magnetic_eyes;

private:
    Sint32 finishPosx;
  Sint32 finishPosy;
  Sint32 centerPosx;
  Sint32 centerPosy;
  Sint32 radius_360;

    public:sprite_eye ();
   ~sprite_eye ();
};
#endif
