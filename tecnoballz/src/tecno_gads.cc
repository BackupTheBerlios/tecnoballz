//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "tecno_gads.cc"
// created	: ?
// updates	: 2005-08-26
// fonction	: objects malus and bonus gadgets
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
#include "../include/tecno_gads.h"

//-----------------------------------------------------------------------------
// Create the object
//-----------------------------------------------------------------------------
tecno_gads::tecno_gads()
{
	clear_sprite_members();
	directionX = 0;
	gadget_num = 0;
	raquettePT = 0x0;
	pt_cycling = &sprite_object::cycling_01[0];
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
tecno_gads::~tecno_gads()
{
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
void tecno_gads::littleInit()
{
	x_maximum = screen_width - ((64 + 16) * resolution);
	x_minimum = 3 * resolution;
	y_maximum = screen_height - 10  * resolution;
	y_minimum = 0 * resolution;
	frame_period = 6;	// vitesse d'animation
	frame_delay = 1;	// compteur tempo animation
	set_draw_method(sprite_object::CYCLE_PTAB);
}

//-----------------------------------------------------------------------------
// bricks levels: initialize new gadget (bonus or malus)
//-----------------------------------------------------------------------------
void tecno_gads::new_gadget(Sint32 nuGad)
{
	if(nuGad == GAD_MEGA00 || nuGad == GAD_RANDOM)
		set_draw_method(sprite_object::CYCLE_PTAB);
	else
		set_draw_method(sprite_object::DRAW_WITH_TABLES);
	gadget_num = nuGad;
	nuGad = nuGad >> 1;
	hasard_val += nuGad;
	const Sint16 *p = gagdetBrik + nuGad;
	if (*p < 0)
	{	fprintf(stderr,
			"tecno_gads::nouveauGad() : nuGad = %i, no used ! \n", nuGad);
		return;
	}
	is_enabled = 1;
	nuGad = *p;
	frame_index_min = nuGad;
	frame_index = nuGad;
	nuGad += XXX_IMAGES - 1;
	frame_index_max = nuGad;
	set_image(frame_index);
}

//-----------------------------------------------------------------------------
// bricks levels: drop new gadget from brick
//-----------------------------------------------------------------------------
void tecno_gads::nouveauGad(brickClear *briPT, Sint32 nuGad)
{
	if(nuGad == GAD_MEGA00 || nuGad == GAD_RANDOM)
		set_draw_method(sprite_object::CYCLE_PTAB);
	else
		set_draw_method(sprite_object::DRAW_WITH_TABLES);
	gadget_num = nuGad;
	x_coord = briPT->balle_posX;
	y_coord = briPT->balle_posY;
	sprite_paddle *raket = briPT->raquettePT;
	raquettePT = raket;
	directionX = raket->bumper_num();
	nuGad = nuGad >> 1;
	hasard_val += nuGad;
	const Sint16 *p = gagdetBrik + nuGad;
	if (*p < 0)
	{	fprintf(stderr,
			"tecno_gads::nouveauGad() : nuGad = %i, no used ! \n", nuGad);
		return;
	}
	is_enabled = 1;
	nuGad = *p;
	frame_index_min = nuGad;
	frame_index = nuGad;
	nuGad += XXX_IMAGES - 1;
	frame_index_max = nuGad;
	set_image(frame_index);

}
//-----------------------------------------------------------------------------
// bricks levels: drop new gadget from a BouiBoui
//-----------------------------------------------------------------------------
void tecno_gads::nouveauGad(sprite_ball *balle, Sint32 nuGad)
{
	if(nuGad == GAD_MEGA00 || nuGad == GAD_RANDOM)
		set_draw_method(sprite_object::CYCLE_PTAB);
	else
		set_draw_method(sprite_object::DRAW_WITH_TABLES);
	gadget_num = nuGad;
	x_coord = balle->get_x_coord();
	y_coord = balle->get_y_coord();
	sprite_paddle *raket = balle->donne_bump();
	raquettePT = raket;
	directionX = raket->bumper_num();
	nuGad = nuGad >> 1;
	hasard_val += nuGad;
	const Sint16 *p = gagdetBrik + nuGad;
	if (*p < 0)
	{	fprintf(stderr,
			"tecno_gads::nouveauGad() : nuGad = %i, no used ! \n", nuGad);
		return;
	}
	is_enabled = 1;
	nuGad = *p;
	frame_index_min = nuGad;
	frame_index = nuGad;
	nuGad += XXX_IMAGES - 1;
	frame_index_max = nuGad;
	set_image(frame_index);
}
//-----------------------------------------------------------------------------
// bricks levels: drop new gadget from a BouiBoui
//-----------------------------------------------------------------------------
void tecno_gads::nouveauGad(sprite_projectile *pfire, Sint32 nuGad)
{
	if(nuGad == GAD_MEGA00 || nuGad == GAD_RANDOM)
		set_draw_method(sprite_object::CYCLE_PTAB);
	else
		set_draw_method(sprite_object::DRAW_WITH_TABLES);
	gadget_num = nuGad;
	x_coord = pfire->get_x_coord();
	y_coord = pfire->get_y_coord();
	sprite_paddle *raket = pfire->raquettePT;
	raquettePT = raket;
	directionX = raket->bumper_num();
	nuGad = nuGad >> 1;
	hasard_val += nuGad;
	const Sint16 *p = gagdetBrik + nuGad;
	if (*p < 0)
	{	fprintf(stderr,
			"tecno_gads::nouveauGad() : nuGad = %i, no used ! \n", nuGad);
		return;
	}
	is_enabled = 1;
	nuGad = *p;
	frame_index_min = nuGad;
	frame_index = nuGad;
	nuGad += XXX_IMAGES - 1;
	frame_index_max = nuGad;
	set_image(frame_index);
}


//-----------------------------------------------------------------------------
// guards levels: drop new gadget gadget from a guard
//-----------------------------------------------------------------------------
void tecno_gads::new_gadget(sprite_ball *balle, Sint32 nuGad)
{
	if(nuGad == GAD_MEGA00 || nuGad == GAD_RANDOM)
		set_draw_method(sprite_object::CYCLE_PTAB);
	else
		set_draw_method(sprite_object::DRAW_WITH_TABLES);
	gadget_num = nuGad;
	x_coord = balle->get_x_coord();
	y_coord = balle->get_y_coord();
	sprite_paddle *raket = balle->donne_bump();
	raquettePT = raket;
	directionX = raket->bumper_num();
	nuGad = nuGad >> 1;
	hasard_val += nuGad;
	const Sint16 *p = gagdetGuar + nuGad;
	if (*p < 0)
	{	fprintf(stderr,
			"tecno_gads::nouveauGad() : nuGad = %i, no used ! \n", nuGad);
		return;
	}
	is_enabled = 1;
	nuGad = *p;
	frame_index_min = nuGad;
	frame_index = nuGad;
	nuGad += XXX_IMAGES - 1;
	frame_index_max = nuGad;
	set_image(frame_index);
}

//-----------------------------------------------------------------------------
// shop: new gadget
//-----------------------------------------------------------------------------
void tecno_gads::nouveauGad(Sint32 nuGad)
{
	if(gadget_num != nuGad)
	{	gadget_num = nuGad;
		nuGad = nuGad >> 1;
		hasard_val += nuGad;
		Sint32 i = gagdetBrik[nuGad];
		if(i == XXX_VIDE00)	// empty code, no gaget code ?
			is_enabled = 0;	// disable the objet
		else
		{	is_enabled = 1;
			frame_index_min = i;
			frame_index = i;
			i += XXX_IMAGES - 1;
			frame_index_max = i;
			set_image(frame_index);
		}
	}
}

//-----------------------------------------------------------------------------
// copie l'image d'un gadget dans un autre gadget 
// gadgt : pt to the source objet
//-----------------------------------------------------------------------------
void tecno_gads::copiegadet(tecno_gads *gadgt)
{
	gadget_num = gadgt->gadget_num;
	frame_index_min = gadgt->frame_index_min;
	frame_index = gadgt->frame_index;
	frame_index_max = gadgt->frame_index_max;
	set_image(frame_index);
}

//-----------------------------------------------------------------------------
// Donne le numero du gadget
//-----------------------------------------------------------------------------
Sint32 tecno_gads::get_gadget()
{
	return gadget_num;
}

//-----------------------------------------------------------------------------
// displacement and collision of the gadget
//-----------------------------------------------------------------------------
sprite_paddle *tecno_gads::deplaceMoi()
{
	if(is_enabled)
	{	sprite_paddle *raket = raquettePT;
		switch (directionX)
		{
			// bottom bumper
			case 1:
				y_coord += resolution;
				if(y_coord < y_maximum)
				{	if(collision1(raket))
					{	is_enabled = 0;
						joueurGere->add_scores(20);
						return raket;
					}
				}
				else
					is_enabled = 0;
				break;

			// right bumper
			case 2:
				x_coord += resolution;
				if(x_coord < x_maximum)
				{	if(collision1(raket))
					{	is_enabled = 0;
						joueurGere->add_scores(20);
						return raket;
					}
				}
				else
					is_enabled = 0;
				break;

			// top bumper
			case 3:
				y_coord -= resolution;
				if(y_coord > y_minimum)
				{	if(collision1(raket))
					{	is_enabled = 0;
						joueurGere->add_scores(20);
						return raket;
					}
				}
				else
					is_enabled = 0;
				break;

			// left bumper
			case 4:
				x_coord -= resolution;
				if(x_coord > x_minimum)
				{	if(collision1(raket))
					{	is_enabled = 0;
						joueurGere->add_scores(20);
						return raket;
					}
				}
				else
					is_enabled = 0;
				break;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------
// bricks levels : pointers on the images of the animation of the gadget 
//-------------------------------------------------------------------------------
const Sint16 tecno_gads::gagdetBrik[] = 
{	XXX_VIDE00,		// *unused*
	XXX_GLUE00,		// glue (bricks level only)
	XXX_VIDE00,		// *unused*
	XXX_FIRE01,		// fire power 1 (bricks level only)
	XXX_FIRE02,		// fire power 2 (bricks level only)
	XXX_SIZE_M,		// shrink bumper (bricks level only)
	XXX_SIZE_P,		// expand bumper (bricks level only)
	XXX_LIFE_M,		// lose a life (bricks level only)
	XXX_LIFE_P,		// extra life
	XXX_BALLE2,		// extra balls
	XXX_BALLE3,		// multi balls
	XXX_POWER1,		// power ball 1
	XXX_POWER2,		// extra life
	XXX_INVERS,		// inverse control commands (bricks level only)
	XXX_VIDE00,		// REBUILD WALL (ONLY SHOP)
	XXX_VIDE00,		// maxi ball speed (no gadget)
	XXX_VIDE00,		// bottom bumper[1] enable (no gadget)
	XXX_VIDE00,		// right bumper[2] enable (no gadget)
	XXX_VIDE00,		// top bumper[3] enable (no gadget)
	XXX_VIDE00,		// right bumper[4] enable (no gadget)
	XXX_SIZE01,		// ball size 2 
	XXX_SIZE02,		// ball size 3
	XXX_RANDOM,		// random (bricks level only)
	XXX_MEGA00,		// all options (bricks level only)
	XXX_VIDE00,		// Bonus price (shop's price at 1 in the shop) (no gadget)
	XXX_WALL01,		// bottom wall enable
	XXX_ROBOT1,		// robot bumper enable
	XXX_CONTRO,		// balls control 
	XXX_VIDE00,		// Oeil actif
	XXX_VIDE00,		// LESS-BRICK (SHOP ONLY)
	XXX_VIDE00,		// INFOS (SHOP ONLY)
	XXX_VIDE00		// EXIT (SHOP ONLY)
};

//-------------------------------------------------------------------------------
// guards levels : pointers on the images of the animation of the gadget 
//-------------------------------------------------------------------------------
const Sint16 tecno_gads::gagdetGuar[] = 
{	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_PROTEC,		// bumper protect (guards levels only)
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_LIFE_P,		// extra life
	XXX_VIDE00,		// *unused*
	XXX_BALLE3,		// multi balls
	XXX_SIZE01,		// ball size 2 
	XXX_SIZE02,		// ball size 3
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00,		// *unused*
	XXX_VIDE00		// *unused*
};
