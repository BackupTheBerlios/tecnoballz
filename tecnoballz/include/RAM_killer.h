//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "RAM_killer.h"
// created      : 1996-07-05
// updates      : 2004-04-21
// functions    : handler memory allocation
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
#ifndef __RAM_KILLER__
#define __RAM_KILLER__
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class RAM_killer;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "mentatCode.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct
{
	char*	adresseMem;	// pointeur sur la zone memoire
	Sint32	taille_mem;	// taille de la zone en octets
	Uint32	identifier;	// identificateur memoire
}
mem_detail;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class RAM_killer:public virtual mentatCode
{
 	private:
		char*				znPointeur;
		mem_detail*			listeZones;
		mem_detail*			listeZonPt;
		Sint32				tailleList;	//size of the list of the zones memory
		Sint32				memMaximum;	//number of zones memory maximum
		Sint32				mem_nombre;	//numbers zone memory current
		Sint32				octetTotal;	//memory currently occupied 
		Sint32				taille_max;	//maximum memory size reserve reached 
		Sint32				adrNoFound;	//nombre d'adresses non trouvees
		Sint32				maxi_zones;	//maximum number of zones memory which were allocated during the game.

	public:
							RAM_killer();
							~RAM_killer();
		Sint32				initialise(Sint32 nombr);
		char*				reserveMem(Sint32 taille, Uint32 ident);
		void				liberation(char *adres);
		Sint32				placePrise();
		void				libereTout();
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern RAM_killer      *memGestion;
#endif
