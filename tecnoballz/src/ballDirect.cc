//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "ballDirect.cc"
// created      : ?
// updates      : 2004-05-18
// fonction     : bumper's viewfinder
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
//******************************************************************************
#include "../include/ballDirect.h"
#include "../include/sprite_ball.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
ballDirect::ballDirect()
{
	littleInit();
	objetTotal = 4;
	objects_have_shades = false;
	BOBtypeNum = BOB_DIRECT;
	numBumpers = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
ballDirect::~ballDirect()
{
	if(listBumper)
	{	memory->release((char *)listBumper);	
		listBumper = (tecno_bump**)NULL;
	}
	littleDead();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 ballDirect::initialize(zeRaquette* pBump, Uint32 nBump)
{
	if(!nBump) return E_GENRIQUE;
	numBumpers = nBump;
	
	listBumper = (tecno_bump**) memory->alloc
		(nBump * sizeof(tecno_bump**), 0x4C495354);
	error_init(memory->retour_err());
	if(erreur_num)
		return (erreur_num);
	for(Uint32 i = 0; i < nBump; i++)
		listBumper[i] = pBump->demandeRak(i + 1);
	return erreur_num;
}


//-----------------------------------------------------------------------------
// runtime, enable or disable the wiewfinders
//-----------------------------------------------------------------------------
void ballDirect::execution1()
{
	for(Sint32 i = 0; i < numBumpers; i++)
	{	sprite_ball* balle = listBumper[i]->balleColle;
		if(balle && balle->flag_actif)
		{	objetListe[i]->aspireBOB2(balle);
			objetListe[i]->enable();
			objetListe[i]->change_GFX(balle->directBall  / 4);
		}
		else 
		{
			objetListe[i]->disable();
		}
	}
}
