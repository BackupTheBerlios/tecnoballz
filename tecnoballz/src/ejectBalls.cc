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
void ejectBalls::initialise()
{
	// top-left (1)
	ejector_01->create_sprite(BOB_EJECT1, image_BOBs, 1);
	eject1PosX = EJECT_POS1 * resolution;
	eject1PosY = (EJECT_POS1 * resolution) - ejector_01->get_sprite_height() / 2;
	ejector_01->set_coordinates(eject1PosX, eject1PosY);

	// top-right (2)
	ejector_02->create_sprite(BOB_EJECT4, image_BOBs, 1);
	eject2PosX = EJECT_POS2 * resolution;
	eject2PosY = (EJECT_POS1 * resolution) - ejector_02->get_sprite_height() / 2;
	ejector_02->set_coordinates(eject2PosX, eject2PosY);

	// bottom-left (3)
	ejector_03->create_sprite(BOB_EJECT2, image_BOBs, 1);
	eject3PosX = EJECT_POS1 * resolution;
	eject3PosY = (EJECT_POS2 * resolution) - ejector_03->get_sprite_height() / 2;
	ejector_03->set_coordinates(eject3PosX, eject3PosY);

	// bottom-right(4)
	ejector_04->create_sprite(BOB_EJECT3, image_BOBs, 1);
	eject4PosX = EJECT_POS2 * resolution;
	eject4PosY = (EJECT_POS2 * resolution) - ejector_04->get_sprite_height() / 2;	
	ejector_04->set_coordinates(eject4PosX, eject4PosY);
	
	// bob_ground = 1: ejectors are managed like sprites  
	if(bob_ground)
	{	sprites->add(ejector_01);
		sprites->add(ejector_02);
		sprites->add(ejector_03);
		sprites->add(ejector_04);
		ejector_01->enable();
		ejector_02->enable();
		ejector_03->enable();
		ejector_04->enable();
	}
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
	eject->x_coord = (eject->x_coord * resolution) + eject1PosX;
	eject->y_coord = (eject->y_coord * resolution) + eject1PosY;

	eject = table + 1;
	eject->x_coord = (eject->x_coord * resolution) + eject3PosX;
	eject->y_coord = (eject->y_coord * resolution) + eject3PosY;

	eject = table + 2;
	eject->x_coord = (eject->x_coord * resolution) + eject4PosX;
	eject->y_coord = (eject->y_coord * resolution) + eject4PosY;

	eject = table + 3;
	eject->x_coord = (eject->x_coord * resolution) + eject2PosX;
	eject->y_coord = (eject->y_coord * resolution) + eject2PosY;
	
}
