//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "menu_tecno.cc"
// created	: ?
// updates	: 2005-01-18
// fonction	: management of the menu principal
// id		: $Id: menu_tecno.cc,v 1.2 2005/01/18 13:21:39 gurumeditation Exp $
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
//******************************************************************************
#include "../include/menu_tecno.h"
#include "../include/ressources.h"
#include "../include/joueurData.h"
#include "../include/zeguardian.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
menu_tecno::menu_tecno()
{
	initialise();
	defilement = new lastScroll();	// background scrolling
	BOBtecLogo = new BOB_killer();	// big logo "TecnoballZ"
	BOB_defile = new zeDefilTxt();
	menu_texte = new print_menu();
	objetMouse = new mousePoint();
	offset_xx1 = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
menu_tecno::~menu_tecno()
{
	delete objetMouse;
	delete menu_texte;
	delete BOB_defile;
	delete BOBtecLogo;
	delete defilement;
	liberation();
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 menu_tecno::first_init()
{
	if(is_verbose)
		fprintf (stdout,
			"menu_tecno::first_init() : [START]\n");	
	BOBgestion->clear_list();
#ifndef SOUNDISOFF
	ptAudiomix->playModule(MUSICINTRO);
#endif
	
	error_init(pRessource->loadSprite());
	if(erreur_num) return (erreur_num);

	error_init(BOBtecLogo->initialise(BOB_LOGOTB, image_BOBs, 1));
	if(erreur_num) return (erreur_num);
	BOBgestion->ajoute_BOB(BOBtecLogo);
	BOBtecLogo->BOB_active();
	BOBtecLogo->coordonnee(64, 13);

	error_init(BOB_defile->init_chars());
	if(erreur_num) return (erreur_num);
  
	error_init(objetMouse->create_BOB(image_BOBs));
	if(erreur_num) return (erreur_num);

	pRessource->freeSprite();
	if(erreur_num) return (erreur_num);

	error_init(defilement->initialise(lastScroll::COLOR_MENU,
		lastScroll::MAPED_MENU));
	if(erreur_num) return (erreur_num);

	error_init(menu_texte->first_init());
	keyGestion->setGrabOff();
	if(erreur_num) return (erreur_num);

	if(is_verbose)
		fprintf (stdout,
			"menu_tecno::first_init() : [END]\n");	
	return (erreur_num);
}

//------------------------------------------------------------------------------
// menu : main loop
//------------------------------------------------------------------------------
Sint32 menu_tecno::zeMainLoop()
{

	ecran_gere->waitVBlank();
	ecran_gere->verouiller();
	offset_inc();
	moveZeLogo();			//move the TecnoballZ logo
	BOB_defile->move_chars();	//move the characters (scroll-text)
	defilement->scrolling1(-1);	//scrolling of the screen background
	objetMouse->bouge_test();
	BOBgestion->listeBOBgo();

	Sint32 zeRet = 0;
	zeRet = menu_texte->afficheTxt();
    
	switch(zeRet)
	{	case 1:
			end_return = -1;
			break;
		case 2:
			end_return = start_tecz();
			break;
	}
    
	menu_texte->MSKaffiche();
	if(keyGestion->specialKey(clavierMac::TOEXITFLAG))
		end_return = -1;
	ecran_gere->deverouill();
	
	//###################################################################
	// copy whole buffer surface into screen surface
	//###################################################################
	ecran_gere->bufferCTab();

	//###################################################################
	// go to map editor
	//###################################################################
#ifdef TU_TRICHES	
	if(keyGestion->test_Kcode(SDLK_F5))
		end_return = 5;
#endif
	return end_return;
}

//------------------------------------------------------------------------------
// Increment the offset on the sinus table
//------------------------------------------------------------------------------
void menu_tecno::offset_inc()
{
	Sint32 m = SINUS_MASK;
	Sint32 a = offset_xx1 + 6;
	a &= m;
	offset_xx1 = a;
}

//------------------------------------------------------------------------------
// move the TecnoballZ logo
//------------------------------------------------------------------------------
void menu_tecno::moveZeLogo()
{
	Sint32 a, b;
	a = offset_xx1;
	Sint16 *s = table_sinL + a;
	b = ((*s * 20 * resolution) >> SINUS_DECA) + 32 * resolution;
	BOBtecLogo->changePosX(b);
	if(birth_flag)
	{	a*=4 ;
		a &= SINUS_MASK;
		s = table_cosL + a;
		b = ((*s * 5 * resolution) >> SINUS_DECA) + 7 * resolution;     
		BOBtecLogo->changePosY(b);
	}
}

//------------------------------------------------------------------------------
// start new TecnoballZ game 
//------------------------------------------------------------------------------
Sint32 menu_tecno::start_tecz()
{
	cheat_flag = 0;
#ifdef TU_TRICHES
	cheat_flag = 1;
#endif
	Uint32 nArea = 1;
	Uint32 level = 1;
	Uint32 grdPt = 0;
	Uint32 iplay;
	Uint32 index = 0;

	//###################################################################
	//check area password validity
	//###################################################################
	Uint32 aMaxi = 4;
#ifdef TU_TRICHES
	//allows to jump directly to the last guards with a password 
	aMaxi = 6;
#else
	if(birth_flag) aMaxi = 6;
#endif
	for(Uint32 areaN = 1; areaN <= aMaxi; areaN++)
	{	for(Uint32 hardN = 1; hardN <= 4; hardN++)
		{	Uint32 f = 1;
			for(Uint32 i = 0; i < 10; i++)
			{	if(zeAreaCode[i] != codesarea[index + i])
				{	f = 0;
					break;
				}
			}
			index += 10;
			if(f)
			{	if(areaN == 6)
				{	nArea = 5;
					level = 13;
				}
				else
				{	nArea = areaN;
					level =  12;
				}
				hardChoice = hardN;
				if(is_verbose)
					printf("menu_tecno::start_tecz() %s "
						"is OK; nArea = %i;" \
						"level = %i, hardN = %i\n",
						&zeAreaCode[0], nArea, level,
						hardN);
				grdPt = zeguardian::level2gdpt(nArea, level);
				goto sortie;
			}
		}
	}
	sortie:
	
	//nArea = 5; level = 13; grdPt = zeguardian::level2gdpt(nArea, level); //test only
	//nArea = 4; level = 12; grdPt = zeguardian::level2gdpt(nArea, level); //test only
	//return 0;
	
	//level = 6;	//test only

	//###################################################################
	// initialize enable player(s)
	//###################################################################
	for(iplay = 0; iplay < nuOfPlayer; iplay++)
	{	Uint32 nlife = vieInitial;
		if(birth_flag)
			nlife = nlife + 10;
		if(chaine_cmp(joueurData::playerlist[iplay]->returnName(), "ETB   ", 6))
			nlife += 5;
		if(chaine_cmp(joueurData::playerlist[iplay]->returnName(), "DJI   ", 6))
			nlife += 4;
		if(chaine_cmp(joueurData::playerlist[iplay]->returnName(), "JMM   ", 6))
			nlife += 3;
		if(chaine_cmp(joueurData::playerlist[iplay]->returnName(), "ZBB   ", 6))
			nlife += 2;
		if(chaine_cmp(joueurData::playerlist[iplay]->returnName(), "REG   ", 6))
			nlife += 1;
		joueurData::playerlist[iplay]->initialise(nlife, nArea, level,
													600, grdPt);
	}

	//###################################################################
	// initialize disable player(s)
	//###################################################################
	for(Uint32 i = iplay; i < MAX_PLAYER; i++)
		joueurData::playerlist[i]->initialise(0, 1, 1, 0, 0);
	joueurGere = joueurData::playerlist[0];
	Sint32 value = joueurGere->level2jump();
	if(value == 2) value = 1;	//convert shop code in bricks level code
	return value;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
/*
unsigned char menu_tecno::colors_map[] =
{ 0, 0, 0, 0, 0, 0, 48, 80, 112, 16, 48, 80, 64, 96, 128, 112, 112, 144, 80,
  80, 112, 32, 64, 96, 144, 144, 176
};*/

const char*	menu_tecno::getTheCode(Uint32 arean, Uint32 hardc)
{	if(arean < 2)
		return NULL;
	return &codesarea[(arean -2) * 40 + (hardc - 1) * 10];
	
}

/*
	LARRYHEARD: area 2
	DANCEFLOOR: area 3
	ZULUNATION: area 4
	DANCEMANIA: area 5
	RINGOFFIRE: area 5 level 12 
	SHELLSHOCK: area 5 level 13
*/

const char menu_tecno::codesarea[241] =
{
	"LARRYHEARD" \
	"SAUNDERSON" \
	"JUANATKINS" \
	"STEPHENSON" \
	"DANCEFLOOR" \
	"REVOLUTION" \
	"LOOKTOSEXY" \
	"REACHINGUP" \
	"ZULUNATION" \
	"HOUSEPIMPS" \
	"ANDRONICUS" \
	"DEFINITIVE" \
	"DANCEMANIA" \
	"PEPPERMINT" \
	"SOLARTRIBE" \
	"PROJECTXYZ" \
	"RINGOFFIRE" \
	"POINTBLANK" \
	"TEMPTATION" \
	"BLUEMONDAY" \
	"SHELLSHOCK" \
	"HOUSEMUSIC" \
	"DAVECLARKE" \
	"CYBERACTIF"
};
