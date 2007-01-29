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
	sprites_have_shades = false;
	vFrequency = 0;
	sprite_type_id = BOB_EXPLO1;
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
void zexplosion::create_explosions_list()
{

        alloc_sprites_list ();
	// allocate and initialize the objects "sprite_object"
	Sint32 bobn1 = BOB_EXPLO1;
	Sint32 bobn2 = BOB_EXPLO2;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *ptbob = new sprite_object();
		ptbob->set_object_pos(i);
		ptbob->create_sprite(bobn1, sprites_bitmap, 0);
		Sint32 bobnu = bobn2;
		bobn2 = bobn1;
		bobn1 = bobnu;
		sprites_list[i] = ptbob;
		sprites->add(ptbob);
	}
}

//-------------------------------------------------------------------------------
// add a new explosion
//-------------------------------------------------------------------------------
void zexplosion::add_explos(Sint32 pos_x, Sint32 pos_y)
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *ptbob = sprites_list[i];
		if(!ptbob->is_enabled)
		{	if(++vFrequency > 4)
			{	vFrequency = 0;
#ifndef SOUNDISOFF
				audio->play_sound(S_BIGEXPLO);
#endif
			}
			ptbob->is_enabled = 1;
			ptbob->x_coord = pos_x - (ptbob->sprite_width/2) ;
			ptbob->y_coord = pos_y - (ptbob->sprite_height/2) ;
			ptbob->frame_index = ptbob->frame_index_min;
			ptbob->frame_period = 4 + (hasard_val & 7);
			ptbob->frame_delay = ptbob->frame_period;
			ptbob->set_image();
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
		if(ptbob->is_enabled)
			ptbob->play_animation_once();
	}
}
