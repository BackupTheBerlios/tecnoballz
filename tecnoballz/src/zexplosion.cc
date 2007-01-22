//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zexplosion.cc"
// created	: 2003-03-02
// updates	: 2006-10-03
// fonction	: handle explosions
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
#include "../include/zexplosion.h"
#include "../include/handler_audio.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zexplosion::zexplosion()
{
	littleInit();
	max_of_sprites = 28;
	objects_have_shades = false;
	vFrequency = 0;
	BOBtypeNum = BOB_EXPLO1;
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
zexplosion::~zexplosion()
{
	release_sprites_list();
}

//-------------------------------------------------------------------------------
// initialize the list of explosions
//-------------------------------------------------------------------------------
Sint32 zexplosion::init_liste()
{

	// allocate table memory 
	sprites_list = (sprite_object **)
		(memory->alloc(sizeof(sprite_object *) * max_of_sprites, 0x4F424A47));
	error_init(memory->retour_err());
	if(erreur_num)
		return (erreur_num);

	// allocate and initialize the objects "sprite_object"
	Sint32 bobn1 = BOB_EXPLO1;
	Sint32 bobn2 = BOB_EXPLO2;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *ptbob = new sprite_object();
		ptbob->set_object_pos(i);
		error_init(ptbob->initialise(bobn1, image_BOBs, 0));
		if(erreur_num)
			return erreur_num;
		Sint32 bobnu = bobn2;
		bobn2 = bobn1;
		bobn1 = bobnu;
		sprites_list[i] = ptbob;
		sprites->add(ptbob);
	}
	return erreur_num; 
}

//-------------------------------------------------------------------------------
// add a new explosion
//-------------------------------------------------------------------------------
void zexplosion::add_explos(Sint32 pos_x, Sint32 pos_y)
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *ptbob = sprites_list[i];
		if(!ptbob->flag_actif)
		{	if(++vFrequency > 4)
			{	vFrequency = 0;
#ifndef SOUNDISOFF
				audio->play_sound(S_BIGEXPLO);
#endif
			}
			ptbob->flag_actif = 1;
			ptbob->position_x = pos_x - (ptbob->BOBlargeur/2) ;
			ptbob->position_y = pos_y - (ptbob->BOBhauteur/2) ;
			ptbob->animOffset = ptbob->miniOffset;
			ptbob->init_tempo = 4 + (hasard_val & 7);
			ptbob->anim_tempo = ptbob->init_tempo;
			ptbob->change_GFX();
			return;
		}
	}
}

//-------------------------------------------------------------------------------
// animation of all explisions 
//-------------------------------------------------------------------------------
void zexplosion::execution1()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *ptbob = sprites_list[i];
		if(ptbob->flag_actif)
			ptbob->animUnique();
	}
}
