//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeDefilTxt.cc"
// created		: ?
// updates		: 2004-10-23
// fonctions	: management of menu scrolling text
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
#include "../include/zeDefilTxt.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zeDefilTxt::zeDefilTxt()
{
	littleInit();
	max_of_sprites = DEFINUMBER + 2;
	sprites_have_shades = true;
	sprite_type_id = BOB_DEFILE;
	offset_xx1 = 0;
	offset_yy1 = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeDefilTxt::~zeDefilTxt()
{
	release_sprites_list();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 zeDefilTxt::init_chars()
{
	error_init(init_liste());
	if(erreur_num) return erreur_num;
	Sint32 i;
	objectChar[0] = 0;
	Sint32 j = 0;
	for(i = 0; i < DEFINUMBER; i++, j = (j + 18) & SINUS_MASK)
	{	tecnoDefil *zeBOB = sprites_list[i];
		objectChar[i + 1] = zeBOB;
		zeBOB->zeCosValue = j;
		zeBOB->set_x_coord(100);
		zeBOB->set_y_coord(300);
		zeBOB->set_image(26);
	}
	objectChar[i + 1] = 0;
	objectLeft = sprites_list[i++];
	objectRigh = sprites_list[i];
	objectLeft->set_image(42);
	objectRigh->set_image(42);
	enable_sprites();
	object_ptr = &objectChar[DEFINUMBER];
	scroll_ptr = scrollText;
	return erreur_num;
}

//-----------------------------------------------------------------------------
// moving of the characters 
//-----------------------------------------------------------------------------
void zeDefilTxt::move_chars()
{
	Sint32 a, b, large;
	Sint16 *table, *sinPT, *cosPT;
	tecnoDefil *zeBOB;
	tecnoDefil *zzBOB;

	sinPT = table_sinL;
	cosPT = table_cosL;

	//###################################################################
	// vertical moving of the characters 
	//###################################################################

	//determine the width of scrolling text
	a = (offset_xx1 + 3) & SINUS_MASK;
	offset_xx1 = a;
	table = sinPT + a;
	large = ((*table * 10 * resolution) >> SINUS_DECA) + 132 * resolution; 
	Sint32 depla = 1;
	if(birth_flag) depla *= 2;
	tecnoDefil **BOBpt = object_ptr;
	for(Sint32 i = 0; i < DEFINUMBER; i++)
	{	if(!*BOBpt)
			BOBpt = &objectChar[DEFINUMBER];
		zeBOB = *(BOBpt--);
		a = zeBOB->zeCosValue + depla;
		
		//read a new character
		if(a >= 256)
		{	char *sText = scroll_ptr;
			Uint32 c = (Uint32)*sText;
			if(!c)
			{	sText = scrollText;
				scroll_ptr = sText;
				c = *sText;
			}
			scroll_ptr++;
			c -= 32;
			b = 127;
			c = c & b;
			b = asciiToBob[c];
			zeBOB->set_image(b);
			if(!*(object_ptr - 1))
				object_ptr = &objectChar[DEFINUMBER + 1];
			object_ptr--;
		}
		a = a & 255;	//radius only varies on 180 degree
		zeBOB->zeCosValue = a;
		table = cosPT + a;
		a = ((*table * large) >> SINUS_DECA) + 152 * resolution;
		zeBOB->set_x_coord(a);
	}

	//###################################################################
	// vertical moving of the characters 
	//###################################################################
	a = (offset_yy1 + 2) & SINUS_MASK;	// and 0x1ff => value 0 to 511
	offset_yy1 = a;
	depla = 4;
	if(birth_flag) depla *= 2;
	BOBpt = object_ptr;
	Sint32 zerad = 25 * resolution;
	Sint32 zeoff = 200 * resolution; 
	for(Sint32 i = 0; i < DEFINUMBER; i++)
	{	if(!*BOBpt)
			BOBpt = &objectChar[DEFINUMBER];
		zeBOB = *(BOBpt--);
		a = (a + depla) & SINUS_MASK;
		table = sinPT + a;
		b = ((*table * zerad) >> SINUS_DECA) + zeoff;
		zeBOB->set_y_coord(b);
	}

	// move the left mask
	zeBOB = objectRigh;
	table = cosPT;
	a = ((*table * large) >> SINUS_DECA) + 152 * resolution;
	zeBOB->set_x_coord(a);
	zzBOB = *(++BOBpt);
	a = zzBOB->get_y_coord();
	zeBOB->set_y_coord(a);

	// move the right mask
	zeBOB = objectLeft;
	table = cosPT + 255;
	a = ((*table * large) >> SINUS_DECA) + 152 * resolution;
	zeBOB->set_x_coord(a);
	zzBOB = *object_ptr;
	a = zzBOB->get_y_coord();
	zeBOB->set_y_coord(a);
}


char zeDefilTxt::scrollText[] =
	"WELCOME TO TECNOBALL Z. THIS GAME WAS ORIGINALLY A COMMODORE "\
	"AMIGA PRODUCTION. IN FACT IT'S THE FIRST PRODUCTION FROM TLK GAMES"\
	" RELEASED AT THE BEGINNING IF 1990. I COMPLETELY REWROTE THE GAME "\
	"WITH C LANGAGE AND SDL FOR LINUX. ENJOY IT !\0";
char zeDefilTxt::asciiToBob[128] =
{	26,	// 32 ' ' space
	37,	// 33 '!'
	39,	// 34 '''
	26,	// 35 '#' (space)
	26,	// 36 '$' (space)
	26,	// 37 '%' (space)
	26,	// 38 '&' (space)
	39,	// 39 "'"
	26,	// 40 '(' (space)
	26,	// 41 ')' (space)
	26,	// 42 '*' (space)
	26,	// 43 '+' (space)
	40,	// 44 ',' 
	41,	// 45 '-'
	38,	// 46 '.'
	26,	// 47 '/' (space)
	27,	// 48 '0'
	28,	// 49 '1'
	29,	// 50 '2'
	30,	// 51 '3'
	31,	// 52 '4'
	32,	// 53 '5'
	33,	// 54 '6'
	34,	// 55 '7'
	35,	// 56 '8'
	36,	// 57 '9'
	26,	// 58 ':' (space)
	26,	// 59 ';' (space)
	26,	// 60 '<' (space)
	26,	// 61 '=' (space)
	26,	// 62 '>' (space)
	26,	// 63 '?' (space)
	26,	// 64 '@' (space)
	00,	// 65 'A'
	01,	// 66 'B'
	02,	// 67 'C'
	03,	// 68 'D'
	04,	// 69 'E'
	05,	// 70 'F'
	06,	// 71 'G'
	7,	// 72 'H'
	8,	// 73 'I'
	9,	// 74 'J'
	10,	// 75 'K'
	11,	// 76 'L'
	12,	// 77 'M'
	13,	// 78 'N'
	14,	// 79 'O'
	15,	// 80 'P'
	16,	// 81 'Q'
	17,	// 82 'R'
	18,	// 83 'S'
	19,	// 84 'T'
	20,	// 85 'U'
	21,	// 86 'V'
	22,	// 87 'W'
	23,	// 88 'X'
	24,	// 89 'Y'
	25,	// 90 'Z'
	26,	// 91 '[' (space)
	26,	// 92 '\' (space)
	26,	// 93 ']' (space)
	26,	// 94 '^' (space)
	26,	// 95 '_' (space)
	26,	// 96 '`' (space)
	26,	// 97 'a' (space)
	26,	// 98 'b' (space)
	26,	// 99 'c' (space)
	26,	// 100 'd' (space)
	26,	// 101 'e' (space)
	26,	// 102 'f' (space)
	26,	// 103 'g' (space)
	26,	// 104 'h' (space)
	26,	// 105 'i' (space)
	26,	// 106 'j' (space)
	26,	// 107 'k' (space)
	26,	// 108 'l' (space)
	26,	// 109 'm' (space)
	26,	// 110 'n' (space)
	26,	// 111 'o' (space)
	26,	// 112 'p' (space)
	26,	// 113 'q' (space)
	26,	// 114 'r' (space)
	26,	// 115 's' (space)
	26,	// 116 't' (space)
	26,	// 117 'u' (space)
	26,	// 118 'v' (space)
	26,	// 119 'w' (space)
	26,	// 120 'x' (space)
	26,	// 121 'y' (space)
	26,	// 122 'z' (space)
	26,	// 123 '{' (space)
	26,	// 124 '|' (space)
	26,	// 125 '}' (space)
	26,	// 126 '~' (space)
	26	// 127 ' ' (space)
};
