//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeBouiBoui.cc"
// created		: ?
// updates		: 2004-10-13
// function	: manage the BouiBoui
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
#include "../include/zeBouiBoui.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zeBouiBoui::zeBouiBoui(zeCapsules* pCaps, ze_gadgets* ptGad, zeGemstone* ptGem,
						lesBriques* pBrik)
{
	ptCapsules = pCaps;
	pt_gadgets = ptGad;
	ptGemstone = ptGem;
	pt_briques = pBrik;
	littleInit();
	max_of_sprites = 8;
	objects_have_shades = true;
	sprite_type_id = BOB_ATOMES;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeBouiBoui::~zeBouiBoui()
{
	release_sprites_list();
}

//-------------------------------------------------------------------------------
// Initialize the BouiBoui
// input	=> time0: time before apparition
//			=> time1: time before the first apparition
//			=> time2: time before the first apparition
//			=> time3: time before the first apparition
//			=> time4: time before the first apparition
//			=> power: strength
//-------------------------------------------------------------------------------
void zeBouiBoui::initialise(Sint32 time0, Sint32 time1, Sint32 time2, Sint32 time3,
	Sint32 time4, Sint32 power)
{
	//time0 = time1 = time2 = time3 = time4 = 10;	//test only
	
	reappTemps = time0;
	apparition[0] = time1;
	apparition[1] = time2;
	apparition[2] = time3;
	apparition[3] = time4;
	power_init = power;
	Sint32 x = sprites_list[0]->atom_xmini;
	Sint32 y = sprites_list[0]->atom_ymini;
	Sint32 j = 0;
	Sint32 offs1 = 20 * tecno_boui::ATOM_ANIMA;
	Sint32 offs2 = 21 * tecno_boui::ATOM_ANIMA;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	tecno_boui *atome = sprites_list[i];
		atome->littleInit
		(	time0,						//time before activation
			apparition[j] + i,			//time before first activation	
			j,							//number of the table of standby values (0 to 3)
			power,						//strength
			x + (i * 16 * resolution),	//absciss
			y + (i * 16 * resolution),	//ordinate
			offs1,						//number of the image's explosion
			ptCapsules,					//object "zeCapsules"
			pt_gadgets,					//object "ze_gadgets"
			ptGemstone,					//object "zeGemstone"
			pt_briques);				//object "lesBriques
		j++;
		j &= 3;
		Sint32 k = offs2;
		offs2 = offs1;
		offs1 = k;
	}
}

//-------------------------------------------------------------------------------
// move all BouiBoui
//-------------------------------------------------------------------------------
void zeBouiBoui::atom_depla()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	tecno_boui *atome = sprites_list[i];
		atome->gere_atome();
		//return; //test only
	}
}

//-------------------------------------------------------------------------------
// force the explosion of all BouiBoui
//-------------------------------------------------------------------------------
void zeBouiBoui::atomexplos()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	tecno_boui *atome = sprites_list[i];
		atome->explosion2();
	}
}
