//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "mentatCode.cc"
// created	: 2002-08-18
// updates	: 2005-01-15
// id		: $Id: mentatCode.cc,v 1.5 2005/01/19 20:38:11 gurumeditation Exp $
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
#include "../include/mentatCode.h"
#include "../include/RAM_killer.h"
#include "../include/ecran_hard.h"
#include "../include/clavierMac.h"
#include "../include/liste_BOBs.h"
#include "../include/joueurData.h"
#include "../include/tableaux_Z.h"
#include "../include/shop_tecno.h"
#include "../include/gard_tecno.h"
#include "../include/menu_tecno.h"
#include "../include/GIF_bitMap.h"
#include "../include/audiomixer.h"
#include "../include/level_data.h"
#include "../include/ressources.h"
#include "../include/scoretable.h"
#include "../include/scrolledit.h"
//.............................................................................
Sint32				mentatCode::arg_jumper = -1;
Sint32				mentatCode::bg4_colors = 0;	//force 4 colors background
bool				mentatCode::is_verbose = 0;
Sint32				mentatCode::num_erreur = 0;	//error number
Sint32				mentatCode::counterObj = 0;	//number of objects
Sint32				mentatCode::hasard_val = 0;	//random value
Sint32				mentatCode::countframe = 0;
scoretable*			mentatCode::ptScoreTab = NULL;	//manage best scores
ressources*			mentatCode::pRessource = NULL;	//manage resources
level_data*			mentatCode::ptLev_data = NULL;	//manage levels
RAM_killer*			mentatCode::memGestion = NULL;	//manage memory allocation  
#ifndef SOUNDISOFF
audiomixer*			mentatCode::ptAudiomix = NULL;	//manage sound      
#endif
ecran_hard*			mentatCode::ecran_gere = NULL;	//manage screen
clavierMac*			mentatCode::keyGestion = NULL;	//keyboard handle
liste_BOBs*			mentatCode::BOBgestion = NULL;	//sprites handle
joueurData*			mentatCode::joueurGere = NULL;	//players handle
Sint16*				mentatCode::table_cosL = NULL;	//cosinus table
Sint16*				mentatCode::table_sinL = NULL;	//sinus table
tableaux_Z*			mentatCode::tabGestion = NULL;	//bricks levels handle
shop_tecno*			mentatCode::shpGestion = NULL;	//shop handle
gard_tecno*			mentatCode::garGestion = NULL;	//guard handle
scrolledit*			mentatCode::ptScrollEd = NULL;	//scroll editor handle
menu_tecno*			mentatCode::menGestion = NULL;	//menu handle
// 1:bricks level / 2:shop / 3:guards level / 4:main menu / 5:scrolling editor
Sint32				mentatCode::super_jump = 1;
Sint32				mentatCode::super_exit = 0;
GIF_bitMap*			mentatCode::image_BOBs = 0;
Uint32				mentatCode::cheat_flag = 0;
Uint32				mentatCode::birth_flag = 0;	//all name are "040670"
Uint32				mentatCode::double_mem = 1;	//2=double all allocations
Sint32				mentatCode::hardChoice = 1;
Sint32				mentatCode::vieInitial = 8;
Uint32				mentatCode::nuOfPlayer = 1;
char				mentatCode::chainelog[100];
const char			mentatCode::nomprefix[] = PREFIX;
Sint32				mentatCode::resolution = 2; 
char				mentatCode::zeAreaCode[11] =
					"          ";

//------------------------------------------------------------------------------
// once initialization
//------------------------------------------------------------------------------
Sint32 mentatCode::first_init()
{
	hasard_val = (Sint32)first_init;
	memGestion = new RAM_killer();
	memGestion->initialise(8000);
	num_erreur = memGestion->retour_err();
	
	pRessource = new ressources();
	if(num_erreur) return num_erreur;

	ptScoreTab = new scoretable();
	num_erreur = ptScoreTab->first_init();
	if(num_erreur) return num_erreur;
	
	
	num_erreur = pRessource->load_sinus();
	if(num_erreur) return (num_erreur);

	
#ifndef SOUNDISOFF		
	ptAudiomix = new audiomixer();
	num_erreur = ptAudiomix->retour_err();
	if(num_erreur) return (num_erreur);
#endif	
	ecran_gere = new ecran_hard();
	num_erreur = ecran_gere->ecran_init();
	if(num_erreur) return (num_erreur);

	keyGestion = new clavierMac();
	BOBgestion = new liste_BOBs(200);
	
	ptLev_data = new level_data();
	//Sint32 Ecode = -1; 
	joueurGere = joueurData::joueursADD(MAX_PLAYER);
	if(!joueurGere)
	{	num_erreur = E_GENRIQUE;
		return num_erreur;
	}

	//###################################################################
	// retrieve user
	//###################################################################
	char *pUser = getenv("USER");
	if (pUser)
		joueurData::playerlist[0]->setNewName(pUser);
	else
		joueurData::playerlist[0]->setNewName("ALBERT");

	joueurData::playerlist[1]->setNewName("LEON  ");
	joueurData::playerlist[2]->setNewName("ANDRE ");
	joueurData::playerlist[3]->setNewName("GERARD");
	joueurData::playerlist[4]->setNewName("RAOUL ");
	joueurData::playerlist[5]->setNewName("MARCEL");

	super_jump = 4;	//menu
	//super_jump = 1;		//bricks level (test only)
	//super_jump = 3;		//guards level (test only)
	//super_jump = 5;		//scroll editor (test only)
	

	if(arg_jumper > 0)
		super_jump = arg_jumper;
	return num_erreur;
}

//------------------------------------------------------------------------------
// main loop of the game
//------------------------------------------------------------------------------
Sint32 mentatCode::game_begin()
{
	do
	{
		if(is_verbose)
			printf("mentatCode::game_begin: game phase=%i (super_jump) \n",
				super_jump);
		//###############################################################
		// initialize
		//###############################################################
		switch (super_jump)
		{
			// exit of game
			case -1:
				super_exit = 1;
				break;
			
			// initialize bricks level
			case 1:
				objectKill();
				tabGestion = new tableaux_Z();
				num_erreur = tabGestion->first_init();
				if(num_erreur) return num_erreur;
				break;
				
			// initialize the shop
			case 2:
				objectKill();
				shpGestion = new shop_tecno();
				num_erreur = shpGestion->first_init();
				if(num_erreur) return num_erreur;
				break;
				
			// initialize guards level
			case 3:
				objectKill();
				garGestion = new gard_tecno();
				num_erreur = garGestion->first_init();
				if(num_erreur) return num_erreur;
				break;
			
			// initialize menu
			case 4:
				objectKill();
				menGestion = new menu_tecno();
				num_erreur = menGestion->first_init();
				if(num_erreur) return num_erreur;
				break;

			// initialize scrolling editor (menu and guards)
			case 5:
				objectKill();
				ptScrollEd = new scrolledit();
				num_erreur = ptScrollEd->first_init();
				if(num_erreur) return num_erreur;
				break;
		}
    
		//###############################################################
		// running
		//###############################################################
		switch (super_jump)
		{
			// bricks level
			case 1:
				super_jump = 0;
				do
				{	super_jump = tabGestion->zeMainLoop();
				}
				while (!super_jump);
				break;

			// the shop
			case 2:
				super_jump = 0;
				do
				{	super_jump = shpGestion->zeMainLoop();
				}
				while (!super_jump);
				break;
				
			// guards level
			case 3:
				super_jump = 0;
				do
				{	super_jump = garGestion->zeMainLoop();
				}
				while (!super_jump);
				break;
				
			// the menu
			case 4:
				super_jump = 0;
				do
				{	super_jump = menGestion->zeMainLoop();
				}
				while (!super_jump);
				break;

			// scrolling editor (menu and guards)
			case 5:
				super_jump = 0;
				do
				{	super_jump = ptScrollEd->zeMainLoop();
				}
				while (!super_jump);
				break;
				
				
		}
	} while (!super_exit);
	return num_erreur;
}

//------------------------------------------------------------------------------
// release the major objects
//------------------------------------------------------------------------------
void mentatCode::objectKill()
{
	
	if(tabGestion)
	{	delete tabGestion;
		tabGestion = NULL;
		if(is_verbose)
			printf("mentatCode::objectKill() object 'tableaux_Z' released\n");
	}
	if(shpGestion)
	{	delete shpGestion;
		shpGestion = NULL;
		if(is_verbose)
			printf("mentatCode::objectKill() object 'shop_tecno' released\n");
	}
	if(garGestion)
	{	delete garGestion;
		garGestion = NULL;
		if(is_verbose)
			printf("mentatCode::objectKill() object 'gard_tecno' released\n");
	}
	if(menGestion)
	{	delete menGestion;
		menGestion = NULL;
		if(is_verbose)
			printf("mentatCode::objectKill() object 'menu_tecno' released\n");
	}
	if(ptScrollEd)
	{	delete ptScrollEd;
		ptScrollEd = NULL;
		if(is_verbose)
			printf("mentatCode::objectKill() object 'scrolledit' released\n");
	}
}

//------------------------------------------------------------------------------
// game exit: relase all objects
//------------------------------------------------------------------------------
Sint32 mentatCode::desinstall()
{
	if(is_verbose)
		printf("==1 objectKill \n");
	objectKill();
	joueurData::joueursRAZ();
	if(is_verbose)
		printf("==3 level_data \n");
	delete ptLev_data;
	if(is_verbose)
		printf("==4 BOBgestion \n");
	delete BOBgestion;
	if(is_verbose)
		printf("==5 keyGestion\n");
	delete keyGestion;
	if(is_verbose)
		printf("==6 ptScoreTab\n");
	delete ptScoreTab;
	if(is_verbose)
		printf("==7 ecran_gere %x\n", (Sint32)ecran_gere);
	delete ecran_gere;
#ifndef SOUNDISOFF
	if(is_verbose)
		printf("==8 audiomixer %x\n", (Sint32)ptAudiomix);
	delete ptAudiomix;
#endif
	delete pRessource;
	if(is_verbose)
		printf("==9 memGestion %x \n", (Sint32)memGestion);
	delete memGestion;
	return num_erreur;
}

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
mentatCode::mentatCode()
{
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
mentatCode::~mentatCode()
{
}

//-------------------------------------------------------------------------------
// perform some initializations of gigablitz
//-------------------------------------------------------------------------------
void mentatCode::mentatInit()
{
	erreur_num = 0;
	numero_obj = counterObj;
	counterObj++;
}

//-------------------------------------------------------------------------------
// object destroyed
//-------------------------------------------------------------------------------
void mentatCode::mentatKill()
{
	counterObj--;
}

//-------------------------------------------------------------------------------
// return the number of error
//-------------------------------------------------------------------------------
Sint32 mentatCode::retour_err()
{
	return	erreur_num;
}

//-------------------------------------------------------------------------------
// return the number of the object
//-------------------------------------------------------------------------------
Sint32 mentatCode::get_number()
{
  return (numero_obj);
}

//-------------------------------------------------------------------------------
// initialize the number of error
//-------------------------------------------------------------------------------
void mentatCode::error_init(Sint32 error)
{
  erreur_num = error;
}

//-------------------------------------------------------------------------------
// compare two character strings
//-------------------------------------------------------------------------------
Sint32 mentatCode::chaine_cmp(char *srcPT, char *desPT, Sint32 taille)
{
	Sint32 t = taille;
	char *s = srcPT;
	char *d = desPT;
	for(Sint32 i = 0; i < t; i++)
	{	char a = s[i];
		char b = d[i];
		if(a != b)
			return (0);
	}
	return 1;
}

//------------------------------------------------------------------------------
// convert integer into ASCII string 
// input => value:	number to convert
//       => strng:	pointer to ASCII string (finished by  zero)
//       => reste:	maximum length (1 = 2 chars, 2 = 3 chars, ...)
//------------------------------------------------------------------------------
void mentatCode::intToASCII(Sint32 value, char *strng, Uint32 reste)
{
	Uint32 index = 0;
	Uint32 zsize = 1;
	if(value < 0)
	{	value = -value;
		*(strng++) = '-';
		if(reste > 0)
			reste--;
	}
	for(index = 0; index < reste; index++)
		zsize = zsize * 10;
	index = 0;
	while (zsize > 0)
	{	Uint32 reste = value / zsize;
		char zchar = (char)('0' + reste);
		 if(zchar > '9' || zchar < '0')
			zchar = '?';
		strng[index++] = zchar;
		value = value - (zsize * reste);
		zsize = zsize / 10;
	}
}

//-------------------------------------------------------------------------------
// read little endian 16 bits word 
//-------------------------------------------------------------------------------
Sint32 mentatCode::littleWord(char *memPT)
{
	Sint32 i = 0;
	unsigned char *p = (unsigned char *)memPT;
	i = p[1];
	i <<= 8;
	i += p[0];
	return i;
}
void mentatCode::bigendianw(Uint32* ptsrc, Uint32* ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	*ptdes = *ptsrc;	
#else
	char* s = (char *)ptsrc;
	char* d = (char *)ptdes;
	d[0] = s[3];
	d[1] = s[2];
	d[2] = s[1];
	d[3] = s[0];
#endif
}
void mentatCode::bigendianr(Uint32* ptsrc, Uint32* ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	*ptdes = *ptsrc;	
#else
	char* s = (char *)ptsrc;
	char* d = (char *)ptdes;
	d[3] = s[0];
	d[2] = s[1];
	d[1] = s[2];
	d[0] = s[3];
#endif
}
