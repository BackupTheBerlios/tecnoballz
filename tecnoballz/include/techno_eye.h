//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "techno_eye.h"
// created		: 2004-09-17
// updates		: 2004-09-17
// function	: handle eye (only bricks levels)
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
class techno_eye ;
//.......................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/lesBriques.h"
#include "../include/controller_balls.h"
#include "../include/ze_magneye.h"
//.......................................................................
class techno_eye : public sprite_object
{	friend class controller_balls;
	friend class ze_magneye;
		
	private:
		Sint32			finishPosx;		
		Sint32			finishPosy;
		Sint32			centerPosx;	
		Sint32			centerPosy;	
		Sint32			radius_360;
	
	public :
						techno_eye();
						~techno_eye();
};
#endif
