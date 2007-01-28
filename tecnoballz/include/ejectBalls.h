//******************************************************************************
// copyright (c) 1991-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ejectBalls.h"
// created		: ?
// updates		: 2004-05-21
// fonction	: manage ejectors objects 
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
//******************************************************************************
#ifndef __EJECTBALLS__
#define __EJECTBALLS__
//...............................................................................
class ejectBalls;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/mentatCode.h"
#include "../include/sprite_ball.h"
//...............................................................................
class ejectBalls:public virtual mentatCode
{
	private:
		static const Sint32	EJECT_POS1 = 16;
		static const Sint32	EJECT_POS2 = 224;

	private:
		static Uint32		balPosFlag;	
	
		sprite_object*			ejector_01;
		sprite_object*			ejector_02;
		sprite_object*			ejector_03;
		sprite_object*			ejector_04;
	
		Sint32				eject1PosX;
		Sint32				eject1PosY;
		Sint32				eject2PosX;
		Sint32				eject2PosY;
		Sint32				eject3PosX;
		Sint32				eject3PosY;
		Sint32				eject4PosX;
		Sint32				eject4PosY;

	public:
							ejectBalls();
							~ejectBalls();
		void				initialise();
		void				afficheSha();
		void				afficheGfx();
		sprite_object*			demandeBOB(Sint32 eject);
		void				ballPosIni(furaxEject *table);
};
#endif
