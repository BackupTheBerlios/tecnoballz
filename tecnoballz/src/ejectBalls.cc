//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ejectBalls.cc"
// created	: ?
// updates	: 2006-10-02
// fonction	: manage ejectors objects 
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
#include "../include/ejectBalls.h"


//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
ejectBalls::ejectBalls()
{
	mentatInit();
	ejector_01 = new sprite_object();
	ejector_02 = new sprite_object();
	ejector_03 = new sprite_object();
	ejector_04 = new sprite_object();
		
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
ejectBalls::~ejectBalls()
{
  delete ejector_01;
  delete ejector_02;
  delete ejector_03;
  delete ejector_04;
  mentatKill();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 ejectBalls::initialise()
{
	// top-left (1)
	error_init(ejector_01->initialise(BOB_EJECT1, image_BOBs, 1));
	if(erreur_num) return erreur_num;
	eject1PosX = EJECT_POS1 * resolution;
	eject1PosY = (EJECT_POS1 * resolution) - ejector_01->getHauteur() / 2;
	ejector_01->coordonnee(eject1PosX, eject1PosY);

	// top-right (2)
	error_init(ejector_02->initialise(BOB_EJECT4, image_BOBs, 1));
	if(erreur_num) return erreur_num;
	eject2PosX = EJECT_POS2 * resolution;
	eject2PosY = (EJECT_POS1 * resolution) - ejector_02->getHauteur() / 2;
	ejector_02->coordonnee(eject2PosX, eject2PosY);

	// bottom-left (3)
	error_init(ejector_03->initialise(BOB_EJECT2, image_BOBs, 1));
	if(erreur_num) return erreur_num;
	eject3PosX = EJECT_POS1 * resolution;
	eject3PosY = (EJECT_POS2 * resolution) - ejector_03->getHauteur() / 2;
	ejector_03->coordonnee(eject3PosX, eject3PosY);

	// bottom-right(4)
	error_init(ejector_04->initialise(BOB_EJECT3, image_BOBs, 1));
	eject4PosX = EJECT_POS2 * resolution;
	eject4PosY = (EJECT_POS2 * resolution) - ejector_04->getHauteur() / 2;	
	if(erreur_num) return erreur_num;
	ejector_04->coordonnee(eject4PosX, eject4PosY);
	
	// bob_ground = 1: ejectors are managed like sprites  
	if(bob_ground)
	{	BOBgestion->ajoute_BOB(ejector_01);
		BOBgestion->ajoute_BOB(ejector_02);
		BOBgestion->ajoute_BOB(ejector_03);
		BOBgestion->ajoute_BOB(ejector_04);
		ejector_01->enable();
		ejector_02->enable();
		ejector_03->enable();
		ejector_04->enable();
	}
	printf("bob_ground : %i\n", bob_ground);
	return erreur_num;
}

//-----------------------------------------------------------------------------
// display shadow
//-----------------------------------------------------------------------------
void ejectBalls::afficheSha()
{
	if(bob_ground) return;
	ejector_01->affich_SHA();
	ejector_02->affich_SHA();
	ejector_03->affich_SHA();
	ejector_04->affich_SHA();
}

//-----------------------------------------------------------------------------
// display ejectors
//-----------------------------------------------------------------------------
void ejectBalls::afficheGfx()
{
	if(bob_ground) return;
	ejector_01->affich_MSK();
	ejector_02->affich_MSK();
	ejector_03->affich_MSK();
	ejector_04->affich_MSK();
}

//-----------------------------------------------------------------------------
// return ejector sprite pointer
//-----------------------------------------------------------------------------
sprite_object *ejectBalls::demandeBOB(Sint32 eject)
{
	switch (eject)
	{	case 1:
			return ejector_01;
		case 2:
			return ejector_02;
		case 3:
			return ejector_03;
		case 4:
			return ejector_04;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// initialize the positions of the balls in the ejectors 
//-----------------------------------------------------------------------------
Uint32 ejectBalls::balPosFlag = 0;
void ejectBalls::ballPosIni(furaxEject *table)
{
	if(balPosFlag) return;
	balPosFlag = 1;
	furaxEject *eject = table;
	eject->position_x = (eject->position_x * resolution) + eject1PosX;
	eject->position_y = (eject->position_y * resolution) + eject1PosY;

	eject = table + 1;
	eject->position_x = (eject->position_x * resolution) + eject3PosX;
	eject->position_y = (eject->position_y * resolution) + eject3PosY;

	eject = table + 2;
	eject->position_x = (eject->position_x * resolution) + eject4PosX;
	eject->position_y = (eject->position_y * resolution) + eject4PosY;

	eject = table + 3;
	eject->position_x = (eject->position_x * resolution) + eject2PosX;
	eject->position_y = (eject->position_y * resolution) + eject2PosY;
	
}
