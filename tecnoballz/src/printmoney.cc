//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "printmoney.cpp"
// created		: 2002-11-28
// updates		: 2004-10-12
// fonction	: print credit of money and reverser malus
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
#include "../include/printmoney.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
printmoney::printmoney()
{
	littleInit();
	max_of_sprites = 8;
	objects_have_shades = false;
	y_position = YINVERSEUR * resolution;
	BOBtypeNum = BOB_MONEY0;
	money_posy = y_position;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
printmoney::~printmoney()
{
	release_sprites_list();
}

//-----------------------------------------------------------------------------
// bricks levels: perform some initializations
// input	=> gamer:
//			=> raket:
//			=> money:
//			=> rever:
//-----------------------------------------------------------------------------
void printmoney::initialise(joueurData *gamer, zeRaquette* raket,
	sprite_object *money, tecno_gads *rever)
{
	ptJoueurDa = gamer;
	ptZraquett = raket;
	ptBobMoney = money;
	ptBobRever = rever;
	
	// credit of money sprites
	init_money();
	
	// initialize "reverser malus" sprites
	Sint32 x = 215 * resolution;
	ptBobRever->new_gadget(GAD_INVERS);
	ptBobRever->set_coordinates(x, money_posy);
	ptBobRever->tempo_init(5);
	x += ptBobRever->get_sprite_width();
	sprite_object **liste = sprites_list + 6; 
	for (Sint32 i = 0; i < 2 ; i++)
	{	sprite_object *x_bob = *(liste++);
		x_bob->set_coordinates(x, money_posy);
		x += 8 * resolution;
	}
}

//-----------------------------------------------------------------------------
// guard levels: perform some initializations
// input	=> gamer:
//			=> raket:
//			=> money:
//			=> lifes:
//-----------------------------------------------------------------------------
void printmoney::init_guard(joueurData *gamer, zeRaquette* raket,
	sprite_object *money, tecno_gads *lifes)
{
	ptJoueurDa = gamer;
	ptZraquett = raket;
	ptBobMoney = money;
	ptBobLifes = lifes;

	// credit of money sprites
	init_money();
	
	// extra life sprites
	Sint32 x = 264 * resolution;
	ptBobLifes->new_gadget(GAD_LIFE_P);
	ptBobLifes->set_coordinates(x, money_posy);
	ptBobLifes->tempo_init(5);
	x += ptBobLifes->get_sprite_width();
	sprite_object **liste = sprites_list + 6; 
	for (Sint32 i = 0; i < 2 ; i++)
	{	sprite_object *x_bob = *(liste++);
		x_bob->set_coordinates(x, money_posy);
		x += 8 * resolution;
		x_bob->enable();
	}
}

//-----------------------------------------------------------------------------
// initialize credit of money sprites
//-----------------------------------------------------------------------------
void printmoney::init_money()
{
	// gadget sprite
	Sint32 x = 24 * resolution;
	ptBobMoney->set_coordinates(x, money_posy);
	ptBobMoney->tempo_init(5);
	ptBobMoney->enable();
	x += ptBobMoney->get_sprite_width();
	
	// characters sprites
	sprite_object **liste = sprites_list;
	for (Sint32 i=0; i < 6 ; i++)
	{	sprite_object *x_bob = *(liste++);
		x_bob->set_coordinates(x, money_posy);
		x_bob->enable();
		x += 8 * resolution;
	}
}

//-----------------------------------------------------------------------------
// display credit of money and "reverser malus"
//-----------------------------------------------------------------------------
void printmoney::execution1(Sint32 value)
{
	exec_money(value);

	//###################################################################
	// display "reverser malus" if enable
	//###################################################################
	sprite_object **liste = sprites_list + 6; 
	Sint32 inves = ptZraquett->get_invers();
	if(inves > 0)
	{	inves--;
		ptZraquett->initinvers(inves);
		inves = inves / 2;
		Sint32 baseN = 10;
		while (baseN > 0)
		{	Sint32 i = 0;
			while (inves >= baseN)
 			{	inves -= baseN;
				i++;
			}
			baseN /= 10;
			sprite_object *x_bob = *(liste++);
			x_bob->change_GFX(i);
			x_bob->enable();
		}
		ptBobRever->enable();
		ptBobRever->animRepete();
	}
	else
	{	for (Sint32 i=0; i<2; i++)
		{	sprite_object *x_bob = *(liste++);
			x_bob->disable();
		}
		ptBobRever->disable();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void printmoney::execution2(Sint32 value, Sint32 lifes)
{
	exec_money(value);
	Sint32 baseN = 10;
	sprite_object **liste = sprites_list + 6; 
	while (baseN > 0)
	{	Sint32 i = 0;
		while (lifes >= baseN)
		{	lifes -= baseN;
			i++;
		}
		baseN /= 10;
		sprite_object *x_bob = *(liste++);
		x_bob->change_GFX(i);
	}
	ptBobLifes->animRepete();
}

//-----------------------------------------------------------------------------
// display credit of money
//-----------------------------------------------------------------------------
void printmoney::exec_money(Sint32 value)
{
	Sint32 baseN = 100000;
	sprite_object **liste = sprites_list;
	while (baseN > 0)
	{	Sint32 i = 0;
		while (value >= baseN)
		{	value -= baseN;
			i++;
		}
		baseN /= 10;
		sprite_object *x_bob = *(liste++);
		x_bob->set_y_coord(money_posy);
		x_bob->change_GFX(i);
	}
	ptBobMoney->set_y_coord(money_posy);
	ptBobMoney->animRepete();
	if(money_posy < y_position)
		money_posy += resolution;
}

//-----------------------------------------------------------------------------
// increase the credit
// input	=> value : credit (10, 20, 30 or 500)
//-----------------------------------------------------------------------------
void printmoney::creditPlus(Sint32 value)
{
	money_posy = y_position - 5 * resolution; 
	ptJoueurDa->creditFric += value;
	if(ptJoueurDa->areaNumber >= 3)
		ptJoueurDa->creditFric += value;
	if(ptJoueurDa->areaNumber >= 5)
		ptJoueurDa->creditFric += value;
}
