//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeGameOver.cc"
// created		: 2002-12-14
// updates		: 2004-10-23
// fonctions	: handle the "Game Over" (move the 8 letters) 
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
//
//*****************************************************************************
#include "../include/zeGameOver.h"
#include "../include/handler_audio.h"
#include "../include/ressources.h"
#include "../include/scoretable.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------	
zeGameOver::zeGameOver()
{
	littleInit();
	max_of_sprites = 8;				//there are 8 letters 
	objects_have_shades = true;				//shadow enable
	max_of_sprites = 8;
	BOBtypeNum = BOB_GAMEOV;
	go_deplace = 0;
	ptGfxFonte = (GIF_bitMap*)NULL;
	ptScorOver = (score_over*)NULL;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeGameOver::~zeGameOver()
{
	if(ptScorOver)
		delete ptScorOver;
	if(ptGfxFonte)
		delete ptGfxFonte;
	release_sprites_list();
}

//-----------------------------------------------------------------------------
// rerturn pointer to the "score_over" object
//-----------------------------------------------------------------------------
score_over* zeGameOver::gtScorOver()
{
	return 	ptScorOver;
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 zeGameOver::first_init(Sint32 offzt)
{
	chrOffsetX = offzt;
	
	//###################################################################
	// load the bitmap fontes
	//###################################################################
	ptGfxFonte = new GIF_bitMap();
	error_init(ptGfxFonte->decompacte(ressources::RESFONTSCR));
	if(erreur_num) return erreur_num;
	
	//###################################################################
	//initialize score table 
	//###################################################################
	ptScorOver = new score_over();
	error_init(ptScorOver->first_init(chrOffsetX));
	if(erreur_num) return erreur_num;
	return erreur_num;
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
void zeGameOver::initialize(Sint32 iswin)
{
	Sint32 x = 100 * resolution;
	Sint32 y = 200 * resolution;
	const Sint32 *p = zeus_over1;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		zebob->enable();
		zebob->set_coordinates(x, y);
		zebob->change_GFX(i);
		Sint32 x2 = *(p++);  
		Sint32 y2 = *(p++);  
		zebob->maximum_X1 =x2;
		zebob->maximum_Y1 =y2;
	}
	go_deplace = 1;
	go_zetempo = 50 * 10;
#ifndef SOUNDISOFF
	Sint32 iscla = ptScoreTab->test_score();
	if(iswin)
		audio->play_music(MUSICCONGR);
	else
	{	if(iscla)
			audio->play_music(MUSICSCORE);
		else
			audio->play_music(MUSICGOVER);
	}
#else
	ptScoreTab->test_score();
#endif
	ptScorOver->copyToText();
}

//-----------------------------------------------------------------------------
// runtime
//-----------------------------------------------------------------------------
void zeGameOver::execution1(Sint32 iswin)
{
	switch (go_deplace)
	{ 
		case 0:
			initialize(iswin);
			break;
    
		case 1:
			deplace_01();  
			break;

		case 2:
			deplace_02();  
			break;

		case 3:
			deplace_03();  
			break;

		case 4:
			deplace_04();  
			break;
	}
	ptScorOver->displayTxt();
	ptScorOver->MSKaffiche();
}

//-----------------------------------------------------------------------------
// movement 1
//-----------------------------------------------------------------------------
void zeGameOver::deplace_01()
{
	Sint32 maxi = SIZETSINUS;
	Sint32 decal = 32 * resolution;
	const Sint32 *sinus = sinus_over;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(++zebob->maximum_X1 >= maxi)
			zebob->maximum_X1 = 0;
		if(++zebob->maximum_Y1 >= maxi)
			zebob->maximum_Y1 = 0;
		Sint32 x = chrOffsetX + decal + sinus[zebob->maximum_X1] * resolution;
		Sint32 y = decal + sinus[zebob->maximum_Y1] * resolution;
		zebob->set_coordinates(x, y);
		hasard_val += y;
	}
	if(--go_zetempo <= 0)
		go_deplace = 2;
}

//-----------------------------------------------------------------------------
// movement 2
//-----------------------------------------------------------------------------
void zeGameOver::deplace_02()
{
	Sint32 maxi = SIZETSINUS;
	Sint32 decal = 32 * resolution;
	const Sint32 *sinus = sinus_over;
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(++zebob->maximum_Y1 >= maxi)
			zebob->maximum_Y1 = 0;
		Sint32 y = decal + sinus[zebob->maximum_Y1] * resolution;
		zebob->set_y_coord(y);
		hasard_val += y;
	}
  
	// move the letters "G", "A", "M", "E"
	Sint32 f = 0;
	Sint32 v = 32 * resolution + chrOffsetX;
	for(Sint32 i = 0; i < 4; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(zebob->x_coord > v)
		{	zebob->x_coord -= resolution;
			f = 1;
		}
	}

	// move the letters "O", "V", "E", "R"
	v = 192 * resolution + chrOffsetX;
	for(Sint32 i = 4; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(zebob->x_coord < v)
		{	zebob->x_coord += resolution;
			f = 1;
		}
	}

	if(f <= 0)
		go_deplace = 3;
}

//-----------------------------------------------------------------------------
// movement 3
//-----------------------------------------------------------------------------
void zeGameOver::deplace_03()
{
  // move the letters "G", "A", "M", "E"
	Sint32 f = 0;
	Sint32 maxi = SIZETSINUS;
	Sint32 decal = 32 * resolution;
	const Sint32 *sinus = sinus_over;
	for(Sint32 i = 0; i < 4; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(++zebob->maximum_Y1 >= maxi)
			zebob->maximum_Y1 = 0;
		if(zebob->y_coord <= decal)
		{	zebob->y_coord = decal;
			f++;
		}
		else
		{	Sint32 y = decal + sinus[zebob->maximum_Y1] * resolution;
			zebob->set_y_coord(y);
		}
		hasard_val += zebob->maximum_Y1;
	}

	// move the letters "O", "V", "E", "R"
	Sint32 v = 191 * resolution;
	for(Sint32 i = 4; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		if(++zebob->maximum_Y1 >= maxi)
			zebob->maximum_Y1 = 0;
		if(zebob->y_coord >= v)
		{	zebob->y_coord = v;
			f++;
		}
		else
		{	Sint32 y = decal + sinus[zebob->maximum_Y1] * resolution;
			zebob->set_y_coord(y);
		}
		hasard_val += zebob->maximum_Y1;
	}
	if(f == 8)
		go_deplace = 4;
}

//-----------------------------------------------------------------------------
// movement 4 : the letters are moving towards their final abscissas
//-----------------------------------------------------------------------------
void zeGameOver::deplace_04()
{
	// move the letters "G", "A", "M", "E"
	Sint32 offst = 2 * resolution;
	Sint32 final = 35 * resolution + chrOffsetX;
	for(Sint32 i = 0; i < 4; i++)
	{	sprite_object *zebob = sprites_list[i];
		Sint32 o = final - zebob->x_coord;
		if(o > offst)
			o = offst;
		else
		{	if(o < (-offst))
				o = -offst;
		}
		zebob->x_coord += o;
		final = final + 54 * resolution;
		hasard_val += zebob->x_coord;
	}

	// move the letters "O", "V", "E", "R"
	final = 32 * resolution + chrOffsetX;
	for(Sint32 i = 4; i < max_of_sprites; i++)
	{	sprite_object *zebob = sprites_list[i];
		Sint32 o = final - zebob->x_coord;
		if(o > offst)
			o = offst;
		else
		{	if(o < (-offst))
				o = -offst;
		}
		zebob->x_coord += o;
		final = final + 54 * resolution;
		hasard_val += zebob->x_coord;
	}
}

// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
const Sint32 zeGameOver::sinus_over[SIZETSINUS] = 
{	159,159,159,159,158,158,158,157,156,156,
	155,154,153,152,151,150,149,148,147,145,
	144,142,141,139,137,135,134,132,130,128,
	126,124,122,120,117,115,113,111,108,106,
	104,101, 99, 96, 94, 91, 89, 86, 84, 81,
	79, 76, 74, 71, 69, 66, 64, 61, 59, 57,
	54, 52, 50, 47, 45, 43, 40, 38, 36, 34,
	32, 30, 28, 26, 24, 23, 21, 19, 18, 16,
	15, 13, 12, 11,  9,  8,  7,  6,  5,  4,
	4,  3,  2,  2,  1,  1,  0,  0,  0,  0,
	0,  0,  0,  0,  1,  1,  2,  2,  3,  4,
	4,  5,  6,  7,  8,  9, 11, 12, 13, 15,
	16, 18, 19, 21, 23, 24, 26, 28, 30, 32,
	34, 36, 38, 40, 43, 45, 47, 50, 52, 54,
	57, 59, 61, 64, 66, 69, 71, 74, 76, 79,
	81, 84, 86, 89, 91, 94, 96, 99,101,104,
	106,108,111,113,115,117,120,122,124,126,
	128,130,132,134,135,137,139,141,142,144,
	145,147,148,149,150,151,152,153,154,155,
	156,156,157,158,158,158,159,159,159,159,
};        
        
// <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
const Sint32 zeGameOver::zeus_over1[] =
{	0,175,
	25,150,
	50,125,
	75,100,
	100, 75,
	125, 50,
	150, 25,
	175,  0
};
