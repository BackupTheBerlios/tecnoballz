//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "liste_BOBs.h"
// created		: ?
// updates		: 2004-04-19
// fonctions	: Gestion de l'affichage des BOBs
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
//
//*****************************************************************************
#ifndef __LISTE_BOB__
#define __LISTE_BOB__
//..............................................................................
class                   liste_BOBs;
//..............................................................................
#include "../include/BOB_killer.h"
#include "../include/RAM_killer.h"
#include "../include/mentatCode.h"
//..............................................................................
const Sint32 BOBNumMaxi = 200;
class liste_BOBs:public mentatCode
{
	private:
		Sint32			BOB_nombre;
		Sint32			SHA_nombre;
		Sint32			BOBsnombre;
		BOB_killer*		BOB__liste[BOBNumMaxi];
		BOB_killer*		SHA__liste[BOBNumMaxi];

	public:
						liste_BOBs(Sint32 nombr);
						~liste_BOBs();
		void			clear_list();
		void			ajoute_BOB(BOB_killer * unBOB);
		void			listeBOBgo();
		void			listeBOBrz();
};
extern liste_BOBs *BOBgestion;
#endif
