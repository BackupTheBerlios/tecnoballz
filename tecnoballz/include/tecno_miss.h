//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_miss.h"
// created		: ?
// updates		: 2004-05-14
// function	: handle weapons of guards
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//*****************************************************************************
#ifndef __TECNO_MISS__
#define __TECNO_MISS__
//...............................................................................
class tecno_miss;
//...............................................................................
#include "../include/BOB_killer.h"
#include "../include/tecno_gard.h"
#include "../include/tecno_bump.h"
//...............................................................................
class tecno_miss:public BOB_killer
{
	friend class zeMissiles;

	private:
		Sint32				type_depla;
		Sint32				flagDepla1;
		Sint32				flagDepla2;
		Sint32				flagDepla3;
		Sint32				flagDepla4;
		Sint32				flagDepla5;
		Sint32				flagDepla6;
		Sint32				flagDepla7;
		const short*		tablesinus;
		const short*		tablecosin;
		tecno_bump*			ptbumper01;
		tecno_gard*			ptguardian;
		Sint32				tir_maxi_x;
		Sint32				tir_maxi_y;
		Sint32				tir_minixy;

	public:
							tecno_miss();
							~tecno_miss();
		void				execution1();

	private:
		void				dmissile01();
		void				dmissile02();
		void				dmissile03();
		void				dmissile04();
		void				dmissile05();
		void				dmissile06();
		void				dmissile07();
		void				dmissile08();
		void				dmissile09();
		void				dmissile10();
		void				dmissile11();
		void				dmissile12();
		void				screenOver(Sint32 xmini);
		void				screenStop(Sint32 vmini);
};
#endif
