//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "score_over.cc"
// created		: 2003-04-30
// updates		: 2003-09-01
// fonction	: display score table (game over and menu)
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
#include "../include/score_over.h"
#include "../include/print_menu.h"
#include "../include/handler_resources.h"
#include "../include/joueurData.h"
#include "../include/scoretable.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------	
score_over::score_over()
{
	clear_sprite_members();
	graphTexte = (bitmap_data*)NULL;
	width_font = 8 * resolution;
	heightfont = 8 * resolution;
	if(resolution == 2)
		space2next = 17;
	else
		space2next = 8;
	run_offset = 0;
	max_offset = TEXT_LARGE * TEXT_HAUTE;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
score_over::~score_over()
{
	if(graphTexte)
		delete graphTexte;
	if(GFX_fontes)
		delete GFX_fontes;
	GFX_fontes = (bitmap_data *)NULL;
	graphTexte = (bitmap_data *)NULL;
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 score_over::first_init(Sint32 offzt)
{
  
	error_init(init_print(handler_resources::RESFONTSCR));
	if(erreur_num) return (erreur_num);

	//###################################################################
	// allocate 512 * 323 pixels buffer for text menu 
	//###################################################################
	graphTexte = new bitmap_data();
	//graphTexte->create(TEXT_LARGE * width_font, TEXT_HAUTE * space2next , 1);
	graphTexte->create_surface(TEXT_LARGE * width_font, TEXT_HAUTE * space2next);

	//###################################################################
	// initialize sprite object
	//###################################################################
	make_sprite(graphTexte);
	enable();
	set_coordinates(32 * resolution + offzt, 80 * resolution);

	copyToText();
	
	return erreur_num;
}

//------------------------------------------------------------------------------
// display the text menu into buffer memory
//------------------------------------------------------------------------------
Sint32 score_over::displayTxt()
{ 
	//###################################################################
	// display menu text	
	//###################################################################
	char *desP1 = pixel_data;
	Sint32 offSc = off_source;
	Sint32 offDs = srceNextLn;
	Sint32 offD2 = srceNextLn * (space2next - 1);
	//Sint32 offD2 = srceNextLn * 8 * resolution;
	Sint32 *basPT = (Sint32 *)caract_adr;
	char *p = scorestext;
	char *c = ascii2code;
	Sint32 a, b, j;
	Sint32 offst = 0;
	Sint32 oldva = run_offset;

	if(resolution == 1)
	{
	for(Sint32 k = 0; k < TEXT_HAUTE; k++, desP1 += offD2)
	{	//###########################################################
		// display normal line of 32 characters
		//###########################################################
		for(j = 0; j < TEXT_LARGE; j++)
		{	a = *(p++) - 32;
			if (run_offset < max_offset &&  offst >= run_offset)
				run_offset++;
			if (offst < max_offset) offst++;
			if(a)
			{	b = c[a];
				Sint32 *s = (Sint32 *)basPT;
				Sint32 *d = (Sint32 *)desP1;
				b = b << 3;
				s = (Sint32 *)((char *)s + b);
				for(b = 0; b < 8; b++)
				{	d[0] = s[0];
					d[1] = s[1];
					s = (Sint32 *)((char *)s + offSc);
					d = (Sint32 *)((char *)d + offDs);
				}
				if(run_offset > oldva)
					return 0;
			}
			desP1 =  desP1 + 8;
		}
	}
	}
	else
	{
		
	for(Sint32 k = 0; k < TEXT_HAUTE; k++, desP1 += offD2)
	{	//###########################################################
		// display normal line of 32 characters
		//###########################################################
		for(j = 0; j < TEXT_LARGE; j++)
		{	a = *(p++) - 32;
			if (run_offset < max_offset &&  offst >= run_offset)
				run_offset++;
			if (offst < max_offset) offst++;
			if(a)
			{	b = c[a];
				Sint32 *s = (Sint32 *)basPT;
				Sint32 *d = (Sint32 *)desP1;
				b = b << 4;
				s = (Sint32 *)((char *)s + b);
				for(b = 0; b < 16; b++)
				{	d[0] = s[0];
					d[1] = s[1];
					d[2] = s[2];
					d[3] = s[3];
					s = (Sint32 *)((char *)s + offSc);
					d = (Sint32 *)((char *)d + offDs);
				}
				if(run_offset > oldva)
					return 0;
			}
			desP1 =  desP1 + 16;
		}
	}	
	}
	return 1;
}

//------------------------------------------------------------------------------
// copy scores into text field
//------------------------------------------------------------------------------
void score_over::copyToText()
{
	score_list* score = ptScoreTab->getScrList();
	char* ptext = scorestext + TEXT_LARGE * 2;
	for(Uint32 i = 0; i < scoretable::NUMBSCORES; i++)
	{	char *pName = score[i].playerName;
		for(Uint32 j = 0; j < 6; j++)
			ptext[j] = pName[j];
		intToASCII(score[i].scoreValue, &ptext[7], 5);
		intToASCII(score[i].score_area, &ptext[16], 0);
		intToASCII(score[i].scoreLevel, &ptext[21], 1);
		ptext += TEXT_LARGE;
	}
}

//------------------------------------------------------------------------------
// scores table text
//------------------------------------------------------------------------------
char score_over::scorestext[] =
	"    TECNOBALL TOP TEN  "	//0
	"NAME  SCORE  AREA LEVEL"	//1
	"...... 000000  :1    01"	//2
	"...... 000000  :1    01"	//3
	"...... 000000  :1    01"	//4
	"...... 000000  :1    01"	//5
	"...... 000000  :1    01"	//6
	"...... 000000  :1    01"	//7
	"...... 000000  :1    01"	//8
	"...... 000000  :1    01"	//9
	"...... 000000  :1    01"	//10
	"...... 000000  :1    01";	//11
