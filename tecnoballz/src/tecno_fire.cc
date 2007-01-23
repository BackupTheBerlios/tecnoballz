//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_fire.cc"
// created		: ?
// updates		: 2004-10-23
// fonction	: bumper's fire (bricks levels only)
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
//
//******************************************************************************
#include "../include/tecno_fire.h"

//-----------------------------------------------------------------------------
// Create the object
//-----------------------------------------------------------------------------
tecno_fire::tecno_fire()
{
	if(total_fire < maxi_fires)
	{	list_fires[total_fire] = this;
		total_fire++;
	}
	set_method(METHOD_CC1);	
	//affCycling = 1;
}

//-----------------------------------------------------------------------------
// Release the object
//-----------------------------------------------------------------------------
tecno_fire::~tecno_fire()
{
}

//-----------------------------------------------------------------------------
// enable a simple bumper's fire power 1
//-----------------------------------------------------------------------------
void tecno_fire::firePower1()
{
	miniOffset = 4;
	maxiOffset = 7;
	animOffset = 4;
	fire_power = 0;
	firePowerX = 1;
	pt_cycling = &sprite_object::cycling_02[0];
}

//-----------------------------------------------------------------------------
// enable a simple bumper's fire power 2
//-----------------------------------------------------------------------------
void tecno_fire::firePower2()
{
	miniOffset = 0;
	maxiOffset = 3;
	animOffset = 0;
	fire_power = 1;
	firePowerX = 2;
	pt_cycling = &sprite_object::cycling_01[0];
}

//-----------------------------------------------------------------------------
// intialize a simple bumper's fire
//-----------------------------------------------------------------------------
void tecno_fire::littleInit(tecno_bump * raket)
{
	raquettePT = raket;
	indexSinus = 0;
	fire_Xscie = 0;
	fire_Yscie = 0;
	animOffset = 0;
	maxiOffset = 3;
	miniOffset = 0;
	frame_delay = 10;
	frame_period = 10;
	fire_power = 0;
	firePowerX = 0;
}

//-----------------------------------------------------------------------------
// static method: initialise all bumper's fires before a bricks level
//-----------------------------------------------------------------------------
void tecno_fire::start_list(lesBriques *brick, zeBouiBoui *atoms, barreScore *score)
{
	brickObjet = brick;
	atomsObjet = atoms;
	scoreObjet = score;
	total_fire = 0;
	tecno_fire **liste = list_fires;
	Sint32 t = maxi_fires;
	tecno_fire *nFire = (tecno_fire *) 0x0;
	for(Sint32 i = 0; i < t; i++)
		*(liste++) = nFire;
}

//-----------------------------------------------------------------------------
// static method: manage all bumper's fires
//-----------------------------------------------------------------------------
void tecno_fire::gestionTir()
{
	hors_ecran();
	anim_fires();
	collision1();
	collision2();
}

//-----------------------------------------------------------------------------
// static method: test if all bumper's fire go out of the screen of game
//-----------------------------------------------------------------------------
void tecno_fire::hors_ecran()
{
	tecno_fire **liste = list_fires;
	Sint32 t = total_fire;
	Sint32 y1 = 15 * resolution;
	Sint32 y2 = 232 * resolution;
	Sint32 x1 = 15 * resolution;
	Sint32 x2 = 228 * resolution;
	for(Sint32 i = 0; i < t; i++)
	{	tecno_fire *xFire = *(liste++);
		Sint32 a = xFire->y_coord;
		if(a < y1 || a > y2)
			xFire->is_enabled = 0;
		else
 		{	a = xFire->x_coord;
			if(a < x1 || a > x2)
				xFire->is_enabled = 0;
		}
	}
}

//-----------------------------------------------------------------------------
// static method: animation of all bumper's fire
//-----------------------------------------------------------------------------
void tecno_fire::anim_fires()
{
	tecno_fire **liste = list_fires;
	Sint32 t = total_fire;
	tecno_fire *xFire = liste[0];
	xFire->animRepete();
	Sint32 o = xFire->litAnimOff();
	Sint32 cycle = o & 0X1;
	if(!cycle) cycle = sprite_object::METHOD_TAB;
	else cycle = sprite_object::METHOD_CC1;
	for(Sint32 i = 1; i < t; i++)
	{	xFire = liste[i];
		xFire->change_GFX(o);
		xFire->put_method = cycle;
	}
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the bricks
//-----------------------------------------------------------------------------
void tecno_fire::collision1()
{

	Sint32 bwght = brickObjet->getBkWidth();	//brick's width in pixels
	Sint32 byoff = brickObjet->getYOffset();	//y-offset between 2 bricks
	Sint32 indus = brickObjet->getBkIndus();	//first indestructible brick
	tecno_fire **liste = list_fires;
	Sint32 t = total_fire;
	brickInfos *tMega = brickObjet->mega_table;
	brickClear *briPT = brickObjet->brique_pnt;
	Sint32 save = brickObjet->briqueSave;
	for(Sint32 i = 0; i < t; i++)
	{	tecno_fire *xFire = *(liste++);
		if(xFire->is_enabled)
		{	Sint32 x = xFire->x_coord + 2;
			Sint32 y = xFire->y_coord + 2;
			brickClear *briP2 = briPT + save;
			briP2->balle_posX = x;
			briP2->balle_posY = y;
			x /= bwght;
			y /= byoff;
			y *= lesBriques::NB_BRICKSH;
			x += y;
			brickInfos *megaT = (tMega + x);
			x = megaT->brique_rel;
			if(x)
			{	if(xFire->is_enabled == 1)
					xFire->is_enabled = 0;
				briP2->raquettePT = xFire->raquettePT;
				if((x -= indus) >= 0)
				{	//###################################################
					// touched indestructible brick
					//###################################################					
					if((x -= bwght) > 0)	//indestructible-destructible bricks?
					{	
						// fire destroys the indestructibles-destructibles bricks
						if(xFire->fire_power)
						{	briP2->balle_posX = -1;
							briP2->adresseAff = megaT->adresseAff;
							briP2->adresseTab = megaT;
							megaT->brique_rel = 0;	// RAZ code brique
							briP2->brique_num = megaT->brique_num;
							briP2->briqueFlag = 1;	//1 = restore background
							save += 1;				// inc. pt/restaure table
							save &= (lesBriques::MAXBRIKCLR - 1);
						}
						else
						{	x = 2;
#ifndef SOUNDISOFF
							audio->play_sound(S_TOINDES2);
#endif
						}
					}
					else
					{	x = 1;	//the brick is really indestructible
#ifndef SOUNDISOFF
						audio->play_sound(S_TOINDES1);
#endif
					}
				}
				else
				{ 	//###################################################
					// touched normal brick
					//###################################################
					briP2->adresseAff = megaT->adresseAff;
					briP2->adresseTab = megaT;
					x = xFire->firePowerX;	// fire power : 1 or 2
					megaT->briquePosX = megaT->briquePosX - (x * 2);
					if(megaT->briquePosX <= 0)
					{	megaT->briquePosX = 0;
						megaT->brique_rel = 0;
						briP2->brique_num = megaT->brique_num;
						briP2->briqueFlag = 1;	// flag restaure background 
					}
					else
					{	megaT->brique_rel = megaT->brique_rel - (x * bwght);
						briP2->brique_num = megaT->brique_rel;
						briP2->briqueFlag = 0;	// flag display brick
					}
					save += 1;	// inc. pt/restaure table
					save &= (lesBriques::MAXBRIKCLR - 1);
				}
			}
		}
	}
	brickObjet->briqueSave = save;
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the BouiBouis
//-----------------------------------------------------------------------------
void tecno_fire::collision2()
{
	tecno_fire **liste = list_fires;
	Sint32 t = atomsObjet->get_max_of_sprites();
	tecno_boui **aList = atomsObjet->get_sprites_list();
	for(Sint32 i = 0; i < total_fire; i++)
	{	tecno_fire *xFire = *(liste++);
		if(xFire->is_enabled)
		{	tecno_boui **monPT = aList;
			Sint32 y1 = xFire->y_coord;
			Sint32 y2 = y1 + 3;
			y1 -= 26;
			Sint32 x1 = xFire->x_coord;
			Sint32 x2 = x1 + 3;
			x1 -= 20;
			for(Sint32 j = 0; j < t; j++)
			{	tecno_boui *atome = *(monPT++);
				if(!atome->atom_actif)
				{	Sint32 k = atome->y_coord;
					if(k < y2 && k > y1)
					{	k = atome->x_coord;
						if(k < x2 && k > x1)
						{	if(xFire->is_enabled == 1)
							xFire->is_enabled = 0;
							scoreObjet->scoreAjout(100);
							k = xFire->firePowerX;
							atome->atom_power -= k;
							if(atome->atom_power < 1)
							atome->explosion1(xFire);
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// static method: collision of all bumper's fire with the BouiBouis
//-----------------------------------------------------------------------------
void tecno_fire::disable_sprites()
{
	tecno_fire **liste = list_fires;
	for(Sint32 i = 0; i < total_fire; i++)
	{	tecno_fire *xFire = *(liste++);
		xFire->is_enabled = 0;
	}		
}

Sint32 tecno_fire::total_fire = 0;
tecno_fire *tecno_fire::list_fires[maxi_fires];
lesBriques *tecno_fire::brickObjet;
zeBouiBoui *tecno_fire::atomsObjet;
barreScore *tecno_fire::scoreObjet;
