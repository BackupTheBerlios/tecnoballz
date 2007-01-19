//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeFireBump.cc"
// created		: ?
// updates		: 2004-10-23
// fonction	: manage bumper's fire
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
#include "../include/zeFireBump.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zeFireBump::zeFireBump()
{
	littleInit();
	countTempo = 0;
	objetTotal = NUMBERFIRE;
	objects_have_shades = false;
	BOBtypeNum = BOB_FIREBP;
	if(resolution == 1)
		scie_sinus = &scieSin320[0];
	else
		scie_sinus = &scieSin640[0];
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeFireBump::~zeFireBump()
{
	littleDead();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 zeFireBump::installBOB(tecno_bump * raket)
{
	Sint32 error = init_liste();
	maRaquette = raket;
	tecno_fire **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	tecno_fire *xFire = *(liste++);
		xFire->littleInit(raket);
	}
	fire1RunOn();
	return error;
}

//-----------------------------------------------------------------------------
// check if fire is available
//-----------------------------------------------------------------------------
void zeFireBump::disponible()
{
	
	//###################################################################
	// return if bumper has no fire (bumperFire = 0)
	//###################################################################
	if(!maRaquette->bumperFire)
		return;
	
	Sint32 t = objetTotal;
	tecno_fire **liste = objetListe;

	//###################################################################
	// special fire 7 (circular fire)
	//###################################################################
	if(maRaquette->raketLarge == maRaquette->width_maxi)
	{	Sint32 j = 1;
		for(Sint32 i = 0; i < t; i++)
		{	tecno_fire *xFire = *(liste++);
			if(xFire->flag_actif == j)
				return;
		}
	}

	//###################################################################
	// other fires
	//###################################################################
	else
	{	for(Sint32 i = 0; i < t; i++)
		{ tecno_fire *xFire = *(liste++);
			if(xFire->flag_actif)
				return;
		}
	}
	countTempo = 0;
	maRaquette->bumperFire = 3;	//fire is requested
#ifndef SOUNDISOFF
	audio->play_sound(S_RAK_TIRS);
#endif
}

//-----------------------------------------------------------------------------
// new fire start
//-----------------------------------------------------------------------------
void zeFireBump::nouveauTir()
{
	if(maRaquette->bumperFire)
 	{	Sint32 i = maRaquette->raketLarge;
		raketLarge = i;
		i -= maRaquette->width_mini;	//smallest bumper is of 16/32 pixels width
		i >>= maRaquette->width_deca;	//size of bumper step by 8/16 pixels
		switch (i)
		{	case 0:
				init_type1();
				break;
			case 1:
				init_type2();
				break;
			case 2:
				init_type3();
				break;
			case 3:
				init_type4();
				break;
			case 4:
				init_type5();
				break;
			case 5:
				init_type6();
				break;
			case 6:
				init_type7();
				break;
		}
	}
}

//-----------------------------------------------------------------------------
// fire 1: 16/32 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type1()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		if(raket->bumperType)	//vertical bumper ?
			y += (raketLarge / 2) - (SIZEOFFIRE / 2);
		else
			x += (raketLarge / 2) - (SIZEOFFIRE / 2);
		tecno_fire *xFire = objetListe[0];
		xFire->flag_actif = 1;
		xFire->position_x = x;
		xFire->position_y = y;
	}
}

//-----------------------------------------------------------------------------
// fire 2: 24/48 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type2()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		if(raket->bumperType)	//vertical bumper ?
			y += (raketLarge / 2) - (SIZEOFFIRE / 2);
		else
			x += (raketLarge / 2) - (SIZEOFFIRE / 2);
		tecno_fire **liste = objetListe;
		tecno_fire *xFire;
		Sint32 f = 1;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
	}
}

//-----------------------------------------------------------------------------
// fire 3: 24/48 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type3()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		if(raket->bumperType)//vertical bumper ?
			y += (raketLarge / 2) - (SIZEOFFIRE / 2);
		else
			x += (raketLarge / 2) - (SIZEOFFIRE / 2);
		tecno_fire **liste = objetListe;
		tecno_fire *xFire;
		Sint32 f = 1;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		xFire = *liste;
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
	}
}

//-----------------------------------------------------------------------------
// fire 4: 40/80 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type4()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		tecno_fire **liste = objetListe;
		tecno_fire *xFire;
		Sint32 f = 1;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		if(raket->bumperType)		//vertical bumper ?
			y += 18 * resolution;
		else
			x += 18 * resolution;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		if(raket->bumperType)		//vertical bumper ?
			y = raket->position_y + raket->raketLarge - 4;
		else
			x = raket->position_x + raket->raketLarge - 4;
		xFire = *liste;
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
	}
}

//-----------------------------------------------------------------------------
// fire 5: 48/96 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type5()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		tecno_fire **liste = objetListe;
		tecno_fire *xFire;
		Sint32 f = 1;
		xFire = *(liste++);
		xFire->flag_actif = f;
		xFire->position_x = x;
		xFire->position_y = y;
		Sint32 quart = raketLarge / 4;
		if(raket->bumperType)	//vertical bumper ?
		{	Sint32 i = raket->bumper_FX1;
			x += i;
			y += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			x += i;
			y += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			x -= i;
			y += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			x -= i;
			y += quart - 2 * resolution;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
		}
		else
		{	Sint32 i = raket->bumper_FY1;
			y += i;
			x += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			y += i;
			x += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			y -= i;
			x += quart;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
			y -= i;
			x += quart - 2 * resolution;
			xFire = *(liste++);
			xFire->flag_actif = f;
			xFire->position_x = x;
			xFire->position_y = y;
		}
	}
}

//-----------------------------------------------------------------------------
// fire 6: 56/112 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type6()
{
	tecno_bump *raket = maRaquette;
	if(raket->bumperFire == 3)
	{	raket->bumperFire = 1;
		tecno_fire **liste = objetListe;
		Sint32 x = raket->position_x;
		Sint32 y = raket->position_y;
		Sint32 offst = 22 * resolution; 

		if(raket->bumperType)	//vertical bumper ?
		{	tecno_fire *xFire;
			Sint32 a = x + raket->bump_xdeca;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = a;
			xFire->position_y = y;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			y += offst;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			y += offst;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = a;
			xFire->position_y = y;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
		}
		else
		{	tecno_fire *xFire;
			Sint32 o = y + raket->bump_ydeca;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = o;
			x += offst;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
			x += offst;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = o;
			xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->position_x = x;
			xFire->position_y = y;
		}
	}
}

//-----------------------------------------------------------------------------
// fire 7: 64/128 pixels bumper's whidth
//-----------------------------------------------------------------------------
void zeFireBump::init_type7()
{
	tecno_bump *raket = maRaquette;
	
	//###################################################################
	// bumper is shotting ?
	//###################################################################
	if(raket->bumperFire == 3) //fire is requested
	{	raket->bumperFire = 1;
		tecno_fire **liste = objetListe;
		Sint32 x = raket->position_x + raket->bump_Xscie;
		Sint32 y = raket->position_y + raket->bump_Yscie;
		Sint32 j = 0;	
		for(Sint32 i = 0; i < 7; i++, j += 8)
		{	tecno_fire *xFire = *(liste++);
			xFire->flag_actif = 1;
			xFire->fire_Xscie = x;
			xFire->fire_Yscie = y;
			xFire->indexSinus = j;
		}
	}

	//###################################################################
	// fire on the bumper
	//###################################################################
	else
	{	tecno_fire **liste = objetListe;
		raket->bumperFire = 1;
		for(Sint32 i = 0; i < 7; i++)
		{	tecno_fire *xFire = *(liste++);
			if(xFire->flag_actif)
				return;
		}
		liste = objetListe;
		Sint32 x = raket->position_x + raket->bump_Xscie;
		Sint32 y = raket->position_y + raket->bump_Yscie;
		Sint32 j = 0;
		for(Sint32 i = 0; i < 7; i++, j += 8)
		{	tecno_fire *xFire = *(liste++);
			xFire->flag_actif = 2;
			xFire->fire_Xscie = x;
			xFire->fire_Yscie = y;
			xFire->indexSinus = j;
		}

	}
}

//-----------------------------------------------------------------------------
// move bumper's fires
//-----------------------------------------------------------------------------
void zeFireBump::deplaceTir()
{
	Sint32 i = maRaquette->raketLarge;
	raketLarge = i;
	i -= maRaquette->width_mini;	//smallest bumper is of 16/32 pixels width
	i >>= maRaquette->width_deca;	//size of bumper step by 8/16 pixels
	switch (i)
	{	case 0:
			move_type1();
			break;
		case 1:
			move_type2();
			break;
		case 2:
			move_type3();
			break;
		case 3:
			move_type4();
			break;
		case 4:
			move_type5();
			break;
		case 5:
			move_type6();
			break;
		case 6:
			move_type7();
			break;
	}
}

//-----------------------------------------------------------------------------
// bumper of 16/32 pixels width: "linear fire"
//-----------------------------------------------------------------------------
void zeFireBump::move_type1()
{
	tecno_fire *xFire = objetListe[0];
	tecno_bump *raket = maRaquette;
	Sint32 i = raket->bumper_FX0;
	xFire->position_x += i;
	i = raket->bumper_FY0;
	xFire->position_y += i;
}

//-----------------------------------------------------------------------------
// bumper of 24/48 pixels width: "fishtail fire"
//-----------------------------------------------------------------------------
void zeFireBump::move_type2()
{
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;
	Sint32 a = countTempo;
	Sint32 b, c, d;
	a++;
	if(a > 20)
		a = 0;
	countTempo = a;
	if(a > 10)
	{	a = raket->bumper_FX1;
		b = raket->bumper_FY1;
		c = raket->bumper_FX2;
		d = raket->bumper_FY2;
	}
	else
	{	a = raket->bumper_FX2;
		b = raket->bumper_FY2;
		c = raket->bumper_FX1;
		d = raket->bumper_FY1;
	}
	tecno_fire *xFire;
	xFire = *(liste++);
	xFire->position_x += a;
	xFire->position_y += b;
	xFire = *liste;
	xFire->position_x += c;
	xFire->position_y += d;
}

//-----------------------------------------------------------------------------
// bumper of 32/64 pixels width: "triangle fire"
//-----------------------------------------------------------------------------
void zeFireBump::move_type3()
{
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;
	Sint32 i, j;
	tecno_fire *xFire;
	xFire = *(liste++);		//shot leaves to the left
	i = raket->bumper_FX1;
	j = raket->bumper_FY1;
	xFire->position_x += i;
	xFire->position_y += j;
	xFire = *(liste++);		//shot leaves any right
	i = raket->bumper_FX0;
	j = raket->bumper_FY0;
	xFire->position_x += i;
	xFire->position_y += j;
	xFire = *liste;			//shot leaves to the right
	i = raket->bumper_FX2;
	j = raket->bumper_FY2;
	xFire->position_x += i;
	xFire->position_y += j;
}

//-----------------------------------------------------------------------------
// bumper of 40/80 pixels width: "fishtail fire" + "linear fire"
//-----------------------------------------------------------------------------
void zeFireBump::move_type4()
{
	 Sint32 i, j;
	i = countTempo;
	i++;
	if(i > 20)
		i = 0;
	countTempo = i;
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;
	tecno_fire *xFire;

	xFire = *(liste++);				//[1] linar shot
	j = raket->bumper_FX0;
	xFire->position_x += j;
	j = raket->bumper_FY0;
	xFire->position_y += j;

	if(i > 10)
	{	xFire = *(liste++);			//[2] fishtail shot
		i = raket->bumper_FX1;
		j = raket->bumper_FY1;
		xFire->position_x += i;
		xFire->position_y += j;
		xFire = *(liste++);			//[3] fishtail shot
		i = raket->bumper_FX2;
		j = raket->bumper_FY2;
		xFire->position_x += i;
		xFire->position_y += j;
	}
	else
	{	xFire = *(liste++);			//[2] fishtail shot
		i = raket->bumper_FX2;
		j = raket->bumper_FY2;
		xFire->position_x += i;
		xFire->position_y += j;
		xFire = *(liste++);			//[3] fishtail shot
		i = raket->bumper_FX1;
		j = raket->bumper_FY1;
		xFire->position_x += i;
		xFire->position_y += j;
	}

	xFire = *liste;					//[4] linar shot
	i = raket->bumper_FX0;
	j = raket->bumper_FY0;
	xFire->position_x += i;
	xFire->position_y += j;
}

//-----------------------------------------------------------------------------
// bumper of 48/96 pixels width: 5 linears shots
//-----------------------------------------------------------------------------
void zeFireBump::move_type5()
{
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;
	Sint32 x = raket->bumper_FX0;
	Sint32 y = raket->bumper_FY0;
	for(Sint32 i = 0; i < 5; i++)
	{	tecno_fire *xFire = *(liste++);
		xFire->position_x += x;
		xFire->position_y += y;
	}
}

//-----------------------------------------------------------------------------
// bumper of 56/112 pixels width: 4 linears shots + 2 fishtails shots
//-----------------------------------------------------------------------------
void zeFireBump::move_type6()
{
	Sint32 a, i, j;

	a = countTempo;
	a++;
	if(a > 20)
		a = 0;
	countTempo = a;
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;

	i = raket->bumper_FX0;
	j = raket->bumper_FY0;
	tecno_fire *xFire = *(liste++);

	xFire->position_x += i;
	xFire->position_y += j;
	xFire = *(liste++);
	xFire->position_x += i;
	xFire->position_y += j;
	if(a > 10)
	{	xFire = *(liste++);		//[3] fishtail shot
		i = raket->bumper_FX1;
		j = raket->bumper_FY1;
		xFire->position_x += i;
		xFire->position_y += j;
		xFire = *(liste++);		//[4] fishtail shot
		i = raket->bumper_FX2;
		j = raket->bumper_FY2;
		xFire->position_x += i;
		xFire->position_y += j;
	}
	else
	{	xFire = *(liste++);		//[3] fishtail shot
		i = raket->bumper_FX2;
		j = raket->bumper_FY2;
		xFire->position_x += i;
		xFire->position_y += j;
		xFire = *(liste++);		//[4] fishtail shot
		i = raket->bumper_FX1;
		j = raket->bumper_FY1;
		xFire->position_x += i;
		xFire->position_y += j;
	}
	i = raket->bumper_FX0;
	j = raket->bumper_FY0;
	xFire = *(liste++);
	xFire->position_x += i;
	xFire->position_y += j;
	xFire = *liste;
	xFire->position_x += i;
	xFire->position_y += j;
}

//-----------------------------------------------------------------------------
// bumper of 64/128 pixels width: 7 circular shots
//-----------------------------------------------------------------------------
void zeFireBump::move_type7()
{
	tecno_fire **liste = objetListe;
	tecno_bump *raket = maRaquette;
	for(Sint32 i = 0; i < 7; i++)
	{	tecno_fire *xFire = *(liste++);
		if(xFire->flag_actif)
		{	Sint32 j = xFire->indexSinus;
			j += 2;
			const Sint16 *table = scie_sinus + j;
			if(*table == 99)
			{	j = 0;
				table = scie_sinus;
			}
			xFire->indexSinus = j;
			if(xFire->flag_actif == 2)
			{	Sint32 k = *(table++);
				k += raket->position_x + raket->bump_Xscie;
				xFire->position_x = k;
				k = *table;
				k += raket->position_y + raket->bump_Yscie;
				xFire->position_y = k;
			}
			else
			{	xFire->fire_Xscie += raket->bumper_FX0;
				xFire->fire_Yscie += raket->bumper_FY0;
				Sint32 k = *(table++);
				k += xFire->fire_Xscie;
				xFire->position_x = k;
				k = *table;
				k += xFire->fire_Yscie;
				xFire->position_y = k;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// enable fire power 1
//-----------------------------------------------------------------------------
void zeFireBump::fire1RunOn()
{
	tecno_fire **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	tecno_fire *xFire = *(liste++);
		xFire->firePower1();
	}
}

//-----------------------------------------------------------------------------
// enable fire power 2
//-----------------------------------------------------------------------------
void zeFireBump::fire2RunOn()
{
	tecno_fire **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	tecno_fire *xFire = *(liste++);
		xFire->firePower2();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sint16 zeFireBump::bob11_fire[] =
{	16, 5,
	8, 5,
	3, 0,	//fire power 2
	4, 0,
	5, 0,
	6, 0,
	7, 0,	//fire power 1
	8, 0,
	9, 0,
	10, 0
};

//-----------------------------------------------------------------------------
// sinus fire 7 (640 pixels)
//-----------------------------------------------------------------------------
const Sint16 zeFireBump::scieSin640[] =
{	15 * 2, -3 * 2, 13 * 2, -7 * 2, 11 * 2, -10 * 2, 9 * 2, -12 * 2, 7 * 2,
	13 * 2, 3 * 2, -15 * 2, 0 * 2, -15 * 2,
 	-3 * 2, -15 * 2, -6 * 2, -14 * 2, -9 * 2, -12 * 2, -11 * 2, -10 * 2,
 	-13 * 2, -07 * 2, -14 * 2, -04 * 2, -15 * 2, -01 * 2,
 	-15 * 2, 3 * 2, -13 * 2, 7 * 2, -11 * 2, 10 * 2, -9 * 2, 12 * 2, -7 * 2,
 	13 * 2, -3 * 2, 15 * 2, 0 * 2, 15 * 2,
 	3 * 2, 15 * 2, 6 * 2, 14 * 2, 9 * 2, 12 * 2, 11 * 2, 10 * 2, 13 * 2, 7 * 2,
 	14 * 2, 4 * 2, 15 * 2, 1 * 2,
 	99, 99, 99, 99
};

//-----------------------------------------------------------------------------
// sinus fire 7 (320 pixels)
//-----------------------------------------------------------------------------
const Sint16 zeFireBump::scieSin320[] =
{	 15, -3, 13, -7, 11,-10, 9, -12,  7,-13,  3,-15,  0,-15,
	 -3,-15, -6,-14, -9,-12,-11,-10,-13, -7,-14, -4,-15, -1,
	-15,  3,-13,  7,-11, 10, -9, 12, -7, 13, -3, 15,  0, 15,
	  3, 15,  6, 14,  9, 12, 11, 10, 13,  7, 14,  4, 15,  1,
	 99, 99, 99, 99
};
