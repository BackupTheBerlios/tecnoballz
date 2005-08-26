//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "RAM_killer.cc"
// created	: 1996-07-05
// updates	: 2005-08-26
// functions	: handler memory allocation
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
#include "../include/RAM_killer.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
RAM_killer::RAM_killer()
{
	mentatInit();
	octetTotal = 0;
	adrNoFound = 0;
	maxi_zones = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
RAM_killer::~RAM_killer()
{
	libereTout();
	mentatKill();
	if(!znPointeur)
	{	free(znPointeur);
		znPointeur = NULL;
	}
}

//-----------------------------------------------------------------------------
// Initialize the object
//-----------------------------------------------------------------------------
Sint32 RAM_killer::initialise(Sint32 nombr)
{
	mem_detail *monPt;
	mem_nombre = 0;			//counter of zones reserved to zero
	memMaximum = nombr;		//numbers maximum zones being able to be reserved
	tailleList = (Sint32)(memMaximum * sizeof(mem_detail));
	octetTotal = tailleList;
	taille_max = octetTotal;
	znPointeur = (char *) malloc((Sint32)tailleList);
	if(!znPointeur) return (erreur_num = E_NOMEMORY);
	listeZones = (mem_detail *) znPointeur;
	listeZonPt = listeZones;
	monPt = listeZones;
	for(Sint32 i = 0; i < memMaximum; i++, monPt++)
	{	monPt->adresseMem = NULL;
		monPt->taille_mem = 0;
		monPt->identifier = 0x46524545;
	}
	return E_NO_ERROR;
}


//-----------------------------------------------------------------------------
// Allocate memory zone
//-----------------------------------------------------------------------------
char *RAM_killer::reserveMem(Sint32 place, Uint32 ident)
{
	place *= double_mem;
	char *memPt = NULL;
	char nulle = 0;
	char *monPT;
	Sint32 i ;
 
	if(mem_nombre == memMaximum)
	{	erreur_num = E_OUT_ZONE;	//out of zones 
		return 0;
	}
	memPt = (char *) malloc((Sint32)place);
	if(!memPt)
	{	erreur_num = E_NOMEMORY;	//allocate error
		return 0;
	}
	octetTotal += place;
	if(octetTotal > taille_max)
		taille_max = octetTotal;
	listeZonPt->adresseMem = memPt;	//memory pointer
	listeZonPt->taille_mem = place;	//size in bytes
	listeZonPt->identifier = ident;	//identifier
	listeZonPt++;
	mem_nombre += 1;
	if(mem_nombre > maxi_zones)
		maxi_zones = mem_nombre;
	monPT = memPt;
	for(i = 0; i < place; i++)
		*(monPT++) = nulle;		//clear memory
	//printf("RAM_killer::reserveMem(%i, %x)\n", place, ident);
	return memPt;
}

//-----------------------------------------------------------------------------
// free memory zone
//-----------------------------------------------------------------------------
void RAM_killer::liberation(char *adres)
{
	if(!adres) return;
	mem_detail *memPT = listeZones;
	for(Sint32 i = 0; i < mem_nombre; i++, memPT++)
	{	if(memPT->adresseMem == adres)
		{	free(adres);	//release memory
			mem_detail *monPT = memPT + 1;
			octetTotal -= memPT->taille_mem;
			mem_nombre--;
			listeZonPt--;
			while(i < mem_nombre)
			{	memPT->adresseMem = monPT->adresseMem;
				memPT->taille_mem = monPT->taille_mem;
				memPT->identifier = monPT->identifier;
				i++;
				memPT++;
				monPT++;
			}
			monPT->adresseMem = NULL;
			monPT->taille_mem = 0;
			monPT->identifier = 0x46524545;
			adres = (char*)NULL;
			break;
		}
	}
	if(adres)
	{	adrNoFound++;
#if __WORDSIZE == 64
		fprintf(stderr, "RAM_killer::liberation() : %i) address %x not found\n",
			adrNoFound, (long)adres);
#else
		fprintf(stderr, "RAM_killer::liberation() : %i) address %x not found\n",
			adrNoFound, (Sint32)adres);
#endif
	}
}

//-----------------------------------------------------------------------------
// Free ALL memory zone
//-----------------------------------------------------------------------------
void RAM_killer::libereTout()
{ 
	if(mem_nombre>0)
	{	 mem_detail *memPT = listeZones;
		for(Sint32 i = 0; i < mem_nombre; i++, memPT++)
		{	char* adres = memPT->adresseMem;
			if(adres)
			{	
				
#if __WORDSIZE == 64
				fprintf(stderr,
					"RAM_killer::libereTout() : addr:%x; size:%i; id:%x\n",
					(long)memPT->adresseMem, memPT->taille_mem,
					(Sint32)memPT->identifier);
#else
				fprintf(stderr,
					"RAM_killer::libereTout() : addr:%x; size:%i; id:%x\n",
					(Sint32)memPT->adresseMem, memPT->taille_mem,
					(Sint32)memPT->identifier);
#endif
				free(adres);	//release memory
				octetTotal -= memPT->taille_mem;
				memPT->adresseMem = NULL;
				memPT->taille_mem = 0;
				memPT->identifier = 0x46524545;
			} 
		}
		fprintf(stderr,
			"RAM_killer::libereTout(): remain %i not released zones\n",
			mem_nombre);
	}
 	if (adrNoFound)
		fprintf(stderr,
			"RAM_killer::libereTout() : release %i already released addresses\n",
			adrNoFound); 
	if(is_verbose)
	{	fprintf(stdout,
			"RAM_killer::libereTout() : maximum number of zones memory which were allocated during the game : %i\n",
			maxi_zones); 
		fprintf(stdout,
			"RAM_killer::libereTout() : maximum of memory which were allocated during the game : %i\n",
			taille_max);
	}
}

//-----------------------------------------------------------------------------
// Return total allocate memory
//-----------------------------------------------------------------------------
Sint32 RAM_killer::placePrise()
{
	return octetTotal;
}
