//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ze_magneye.cc"
// created		: 2004-09-17
// updates		: 2004-09-20
// function	: manage eye magneto (only bricks levels)
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
#include "../include/ze_magneye.h"
#include "../include/clavierMac.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
ze_magneye::ze_magneye()
{
	littleInit();
	objetTotal = 3;
	objetOmbre = 1;
	BOBtypeNum = BOB_MAGEYE;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
ze_magneye::~ze_magneye()
{
	littleDead();
}


//-----------------------------------------------------------------------------
// enable a new eye
//-----------------------------------------------------------------------------
Sint32 ze_magneye::create_eye()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	techno_eye *ptEye = objetListe[i];
		if(ptEye->flag_actif) continue;
		ptEye->flag_actif = 1;
		return 1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// initialize eyes
//-----------------------------------------------------------------------------
Sint32  ze_magneye::initialize()
{
	error_init(init_liste());
	if(erreur_num) return erreur_num;
	Sint32 hval = hasard_val;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	techno_eye *ptEye = objetListe[i];
		ptEye->centerPosx = Xcoordinat[hval & 31]  * resolution;
		hval += (Sint32)ptEye;
		ptEye->centerPosy = Ycoordinat[hval & 31]  * resolution;
		hval += keyGestion->sourisGetY();
		ptEye->finishPosx = Xcoordinat[hval & 31]  * resolution;
		hval += keyGestion->sourisGetX();
		ptEye->finishPosy = Ycoordinat[hval & 31]  * resolution;
		hval++;
		ptEye->radius_360 = 0;
		
	}
	hypotenuse = objetListe[0]->colLargeur - resolution * 2;
	eyeCenterX = hypotenuse / 2;
	eyeCenterY = eyeCenterX;
	hypotenuse = hypotenuse * hypotenuse;
	return erreur_num;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ze_magneye::execution1()
{
	Sint32 hval = hasard_val;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	techno_eye *ptEye = objetListe[i];
		//if(!ptEye->flag_actif) continue;
		ptEye->animRepete();
		
		//###############################################################
		// verify if coordinates of center arrived at destination.
		//###############################################################
		Sint32 j = 3 * resolution;
		if(	ptEye->centerPosx > ptEye->finishPosx - j && 
			ptEye->centerPosx < ptEye->finishPosx + j &&
			ptEye->centerPosy > ptEye->finishPosy - j && 
			ptEye->centerPosy < ptEye->finishPosy + j)
		{		hval += keyGestion->sourisGetY();
				ptEye->finishPosx = Xcoordinat[hval & 31] * resolution;
				hval += keyGestion->sourisGetX();
				ptEye->finishPosy = Ycoordinat[hval & 31] * resolution;
		}

		
		
		//###############################################################
		// move center 
		//###############################################################
		Sint32 inc_x = resolution;
		Sint32 inc_y = resolution;
		Sint32 deltX = ptEye->finishPosx - ptEye->centerPosx;
		if(deltX < 0)
		{	deltX = -deltX;
			inc_x = -resolution;
		}
		Sint32 deltY = ptEye->finishPosy - ptEye->centerPosy;
		if(deltY < 0)
		{	deltY = -deltY;
			inc_y = -resolution;
		}
		Sint32 hflag = 0;
		if (deltY > deltX)
		{	j = deltY;
			deltY = deltX;
			deltX = j;
			hflag = 1;
		}
		Sint32 value = deltY * 2 - deltX;
		deltX *= 2;
		j = deltX - 1;
		do
		{	if(hflag) ptEye->centerPosy+= inc_y;
			else ptEye->centerPosx+= inc_x;
			value -= deltX;
			if(value < 0)
			{	if(!hflag) ptEye->centerPosy += inc_y;
				else ptEye->centerPosx += inc_x;
				break;
			}
			j--;
		} while (j >= 0) ;
	

		//###############################################################
		// move circle
		//###############################################################
		ptEye->radius_360 += 4;
		if(ptEye->radius_360 >= 360)
			ptEye->radius_360 -= 360;
		Sint32 x = (ressources::zesinus360[ptEye->radius_360] * 10 * resolution) >> 7;
		Sint32 y = (ressources::cosinus360[ptEye->radius_360] * 10 * resolution) >> 7;
		ptEye->position_x = ptEye->centerPosx + x + (15 * resolution);
		ptEye->position_y = ptEye->centerPosy + y + (15 * resolution);
		
		/*
		ptEye->position_x = ptEye->centerPosx;
		ptEye->position_y = ptEye->centerPosy;*/
		
		hval += 4;
	}
}
const Uint16 ze_magneye::Xcoordinat[32] =
{	 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 92, 96,100,105,111,115,
	118,120,122,128,130,132,135,140,144,146,150,152,160,164,166,170
};
const Uint16 ze_magneye::Ycoordinat[32] =
{	 40, 44, 56, 58, 60, 62, 70, 78, 80, 82, 85, 90, 92, 94, 96,100,
	101,120,122,124,130,138,144,146,148,150,152,153,154,155,160,170
};
