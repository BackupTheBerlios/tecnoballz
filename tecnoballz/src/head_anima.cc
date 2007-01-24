//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "head_anima.cc"
// created		: ?
// updates		: 2004-08-31
// fonction	: anim head
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
//*****************************************************************************
#include "../include/head_anima.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
head_anima::head_anima()
{
	mentatInit();
	headbitmap = (bitmap_data*)NULL;
	head_count = 0;
	head_reque = 0;	//animation requested 
	head_runni = 0;	//current animation
	head_step1 = 0;	//offset (1 or -1)
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
 head_anima::~head_anima()
{
	mentatKill();
	if(headbitmap)
	{	delete headbitmap;
		headbitmap = (bitmap_data*)NULL;
	}
}

//-------------------------------------------------------------------------------
// perform some initializations
//-------------------------------------------------------------------------------
Sint32 head_anima::initialise()
{
	bitmap_data *gfxPT = new bitmap_data();
	headbitmap = gfxPT;
	gfxPT->load(ressources::RESHEADANI);
	off_source = gfxPT->get_row_size();
	off_destin = display->bufferLarg();
	adr_source = gfxPT->get_pixel_data();
	adr_destin = display->buffer_pos(272 * resolution, 106 * resolution);
	la_hauteur = gfxPT->get_height();
	return (erreur_num);
}

//-------------------------------------------------------------------------------
// display memory in meory buffer
//-------------------------------------------------------------------------------
void head_anima::affiche_me()
{
	Sint32 *desti = (Sint32 *)adr_destin;
	Sint32 *surce = (Sint32 *)(adr_source + (head_runni * 32 * resolution)) ;
	Sint32 off_s = off_source  ;
	Sint32 off_d = off_destin;
	Sint32 j = la_hauteur;
	if(resolution == 1)
	{	for(Sint32 i = 0; i < j; i++)
		{	desti[0]  = surce[0];
			desti[1]  = surce[1];
			desti[2]  = surce[2];
			desti[3]  = surce[3];
			desti[4]  = surce[4];
			desti[5]  = surce[5];
			desti[6]  = surce[6];
			desti[7]  = surce[7];
			surce = (Sint32 *)((char *)surce + off_s);
			desti = (Sint32 *)((char *)desti + off_d);
		}
	}
	else
	{	for(Sint32 i = 0; i < j; i++)
		{	desti[0]  = surce[0];
			desti[1]  = surce[1];
			desti[2]  = surce[2];
			desti[3]  = surce[3];
			desti[4]  = surce[4];
			desti[5]  = surce[5];
			desti[6]  = surce[6];
			desti[7]  = surce[7];
			desti[8]  = surce[8];
			desti[9]  = surce[9];
			desti[10] = surce[10];
			desti[11] = surce[11];
			desti[12] = surce[12];
			desti[13] = surce[13];
			desti[14] = surce[14];
			desti[15] = surce[15];
			surce = (Sint32 *)((char *)surce + off_s);
			desti = (Sint32 *)((char *)desti + off_d);
		}
	}
}

//------------------------------------------------------------------------------
// handler of head animations
//------------------------------------------------------------------------------
void head_anima::execution1()
{
	affiche_me();
	if(head_count)	// past temporization? 
	{	head_count--;
		return;
	}

	//###################################################################
	// initialize new animation
	//###################################################################
	Sint32 r = head_reque;	// animation requested 
	head_reque = 0;
	if(!head_runni)	//animation in progress?
	{	head_runni = r;
		head_step1 = 1;
		head_count = TETESPEED1;
		return;
	}
	
	//###################################################################
	// forward animation
	//###################################################################
	if(head_step1 >= 0)
	{	switch(head_runni)
		{	case TETE_PARA2:	//disturbance
				head_step1 = -1;
				head_runni = head_runni + head_step1;
				break;
			case TETE_BAIL2:	//yawning
				head_step1 = -1;
				head_count = TETESPEED2;
				break;
			case TETE_RIRE2:	//laugh
				head_step1 = -1;
				head_count = TETESPEED2;
				break;
			default:
				head_count = TETESPEED1;
				head_runni = head_runni + head_step1;
		}
		return;
	}

	//###################################################################
	// backward animation
	//###################################################################
	switch(head_runni)
	{	case TETE_PARA1:	//disturbance
			head_runni = 0;
			break;
		case TETE_BAIL1:	//yawning
			head_runni = 0;
			break;
		case TETE_RIRE1:	//laugh
            head_runni = 0;
            break;
		default:
			head_count = TETESPEED1;
			head_runni = head_runni + head_step1;
	}
}

//-------------------------------------------------------------------------------
// enable laugh
//-------------------------------------------------------------------------------
void head_anima::teterigole()
{
	head_reque = TETE_RIRE1;
}

//-------------------------------------------------------------------------------
// enable yawning 
//-------------------------------------------------------------------------------
void head_anima::tetebaille()
{
	head_reque = TETE_BAIL1;
}

//-------------------------------------------------------------------------------
// enable disturbance
//-------------------------------------------------------------------------------
void head_anima::teteparasi()
{	
	head_reque = TETE_PARA1;
}
