//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "fond_ecran.h"
// created		: ?
// updates		: 2004-10-17
// fonctions	: display background (bricks levels)
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
//.............................................................................
#ifndef __FOND_ECRAN__
#define __FOND_ECRAN__
class fond_ecran;
//#include "../include/tableaux_Z.h"
#include "../include/mentatCode.h"
//.............................................................................
extern Sint32 VBL_compte;
//.............................................................................
class fond_ecran:public virtual mentatCode
{
	private:
		static Sint32		table_pos1[16];
		static Sint32		table_pos2[16];
		static char			couleurs[448];
		static const Sint32	FONDLARGE1 = 32;
		static const Sint32	FONDHAUTE1 = 32;
		static const Sint32	FONDLARGE2 = 64;
		static const Sint32	FONDHAUTE2 = 64;
		Sint32				iFond_type;	//map type (0 = original, 2 = new)
		Sint32				colorActif;	//number of the color active

	private:
		void				coulDuFond(Sint32 zecol);

	public:
							fond_ecran();
							~fond_ecran();
		Sint32				instalFond(Sint32 nbkdg);	//select and display background
		void				coulDuFond();				//select color background
		void				next_color();
		void				prev_color();
};
#endif
