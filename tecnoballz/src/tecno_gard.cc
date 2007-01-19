//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_gard.cc"
// created		: 2003-01-09
// updates		: 2004-10-23
// fonction	: handle of a vessel guard 
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
//******************************************************************************
#include "../include/tecno_gard.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
tecno_gard::tecno_gard()
{
	gard_power = 0;
	gard_xcent = 0;
	gardwaitf1 = 0;
	gardwaitf2 = 0;
	gard_speed = 0;
	gard_colx1 = 0;
	gard_coly1 = 0;
	gard_colx2 = 0;
	gard_coly2 = 0;
	gard_ycent = 0;
	gard_wait1 = 0;
	gard_wait2 = 0;
	gard_lissa = 0;
	gardptfire = 0;
	gard_touch = 0;
	gard_clign = 0;
	hasardval2 = hasard_val;
	if(hasardval2 < 0) hasardval2 = -hasardval2;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_gard::~tecno_gard()
{
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 tecno_gard::init_guard(gardlevel *guard, unsigned char* ptLis, 
	zeMissiles *pMiss, zeGigaBlit *pBliz, zexplosion* pexpl)
{
	ptGigaBlit = pBliz;
	pExplosion = pexpl;
	gard_power = guard->para_power * hardChoice;			//strength 
	gard_xcent = (guard->para_xcent * resolution) -
					(11 * resolution / 2);					//middle x from where weapons starts 
	gardwaitf1 = guard->para_waitf / hardChoice;			//shoot frequency of gigaBlitz
	gardwaitf2 = gardwaitf1 / hardChoice;					//shoot frequency of gigaBlitz
	gard_speed = guard->para_speed;							//speed of moving
	if(hardChoice == 4)  gard_speed *= 2;
	gard_colx1 = guard->para_colx1 * resolution;
	gard_coly1 = guard->para_coly1 * resolution;
	gard_colx2 = guard->para_colx2 * resolution;
	gard_coly2 = guard->para_coly2 * resolution;
	gard_ycent = (guard->para_ycent * resolution) - (11 * resolution /2);
	gard_wait1 = guard->para_wait2 / hardChoice;
	gard_wait2 = guard->para_wait2 / hardChoice;
	gard_tfire = &guard->para_tfire[0];
	ptr_lissa1 = ptLis;
	ptr_lissa2 = ptLis;
	minimum_X1 = 6 * resolution;
	minimum_Y1 = 2 * resolution;
	maximum_X1 = ecranLarge - BOBlargeur;
	maximum_Y1 = ecranHaute - BOBhauteur;
	maximum_Y1 = (232 - 8 - 1 - 16) * resolution;
	ptMissiles = pMiss;
	if(BOBlargeur == resolution * 32)
		explotempo = 7;
	else
		explotempo = 3;
	return erreur_num;
}
//-----------------------------------------------------------------------------
// execution
//-----------------------------------------------------------------------------
void tecno_gard::execution1(Sint32 offzt)
{
	//###################################################################
	// guards is alive
	//###################################################################
	if (gard_power > 0)
	{	
		Sint32 x = (ptr_lissa1[0]);
		x = x * (ecranLarge - BOBlargeur) / 192;
		Sint32 y = ptr_lissa1[1] * resolution;
		position_x = x;
		position_y = y;
		ptr_lissa1 = ptr_lissa1 + gard_speed;
		Sint16 *ptest = (Sint16 *)ptr_lissa1;
		if (ptest[0]  == -1)
			ptr_lissa1 = ptr_lissa2;
		gard_clign = !gard_clign;
		if(gard_touch > 0 && gard_clign > 0)
		{	flag_actif = 0;
			gard_touch--;
		}
		else
			flag_actif = 1;
		start_fire();
		startBlitz();
	}
	else
	{	
		//###############################################################
		//	the guard explodes 
		//###############################################################
		if(explo_time > 0)
		{	explo_time--;
			if(!(explo_time & explotempo))
			{	Sint32 pos_x = position_x;
				Sint32 pos_y = position_y;
				hasard_val += pos_x;
				Sint32 vrand = hasard_val;
				if(vrand < 0) vrand = -vrand;
				Sint32 val_1 =
					(vrand + explo_time - position_y) % BOBlargeur;
				Sint32 val_2 =
					(hasardval2 - explo_time + position_y) % BOBhauteur;
				pos_x += val_1;
				pos_y += val_2;
				pExplosion->add_explos(pos_x, pos_y);
				hasardval2 = vrand;
			}
			gard_clign = !gard_clign;
			if(gard_clign > 0)
				flag_actif = 0;
			else 
				flag_actif = 1;
		}
		//###############################################################
		// the guard is dead 
		//###############################################################
		else
			flag_actif = 0;
	}
}

//-----------------------------------------------------------------------------
// start a new weapon (composed of several objects "tecno_miss")
//-----------------------------------------------------------------------------
void tecno_gard::start_fire()
{
	if(gard_wait1-- <= 0)
	{	if(position_y <= (80 * resolution)) 
		{	gard_wait1 = gard_wait2;
#ifndef SOUNDISOFF
			audio->play_sound(S_TIR_GARD);
#endif
			Sint32 nfire = gard_tfire[gardptfire];
			if(nfire < 0)
			{	nfire = gard_tfire[0];
				gardptfire = 0;
			}
			gardptfire++;	// pt/next weapons
			ptMissiles->newMissile(nfire, this);
		}
	}
}

//-----------------------------------------------------------------------------
// start a new GigaBlitz
//-----------------------------------------------------------------------------
void tecno_gard::startBlitz()
{
	if(gardwaitf1-- <= 0)
	{	Sint32 v = hasard_val & 7;
		//v = 7;	//test only
		if(BOBlargeur > (32 * resolution))
			v = table_gga1[v] ;
		else
			v = table_gga2[v] ;
		//v = 0;	//test only 
		if(ptGigaBlit->guard_shoot(v, position_x, position_y,
			BOBlargeur, BOBhauteur))
			gardwaitf1 = gardwaitf2;
	}
}

//-----------------------------------------------------------------------------
// list of differents gigablitz
//-----------------------------------------------------------------------------
// Guard of 64/128 pixels
const Sint32 tecno_gard::table_gga1[8] =
{	0,1,2,3,4,5,6,5
};
// Guard of 32/64 pixels
const Sint32 tecno_gard::table_gga2[8] =
{	6,5,4,6,5,4,6,5
};
