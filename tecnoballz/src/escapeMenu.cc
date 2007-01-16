//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "escapeMenu.cc"
// created		: 2004-08-08
// updates		: 2004-08-28
// fonction	: handle escape menu
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
#include "../include/ressources.h"
#include "../include/escapeMenu.h"
#include "../include/sprite_object.h"
#include "../include/clavierMac.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
escapeMenu::escapeMenu()
{
	yPressLeft = YCOORDNULL;
	yPressRigh = YCOORDNULL;
	menu_color = 0;
	menuNumber = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
escapeMenu::~escapeMenu()
{
	if(graphTexte)
		delete graphTexte;
	if(GFX_fontes)
		delete GFX_fontes;
	GFX_fontes = (GIF_bitMap *)NULL;
	graphTexte = (GIF_bitMap *)NULL;
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 escapeMenu::first_init(GIF_bitMap *image, Sint32 nmenu, Sint32 large, Sint32 resto,
	Sint32 palet)
{
	
	//###################################################################
	// determine if restore background (bricks levels and shop only)
	//###################################################################
	restoreBkg = resto;	//1: restore background when leave menu

	//###################################################################
	// initialize palette color chars, if necessary (shop only)
	//###################################################################
	if(palet)
	{	SDL_Color *palPT = ecran_gere->paletteAdr();
		SDL_Color *palP1 = palPT + 239;
		Sint32 i = hasard_val & 0x0F;
		if(i >= 10)
			i = i - 10;
		const Uint32 *ptpal = (ressources::tabledegas + i * 18);
		for(i = 0; i < 17; i++)
		{	Uint32 vacol = ptpal[i];
			Uint32 vablu = vacol & 0x000000ff;
			Uint32 vagre = vacol & 0x0000ff00;
			vagre = vagre >> 8;
			Uint32 vared = vacol & 0x00ff0000;
			vared = vared >> 16;
			palP1->r = vared;
			palP1->g = vagre;
			palP1->b = vablu;
			palP1++;
		}
		ecran_gere->palette_go(palPT); 
	}
	
	//###################################################################
	// determine line height into menu box
	//###################################################################
	if(resolution == 2)
		space2next = 17;
	else
		space2next = 8;

	//###################################################################
	// load fontes bitmap files and perform some initializations
	//###################################################################
	error_init(init_print(ressources::RESFONTMEN));
	if(erreur_num) return (erreur_num);	
	
	//###################################################################
	// determine height of the menu box
	//###################################################################
	menuNumber = nmenu;
	if(nmenu == 1)
		nb_ofLines = 3;
	else
		nb_ofLines = 4;
	nb_columns = 25;
	Sint32 nbcol = nb_columns + 2; 
	Sint32 nblin = nb_ofLines + 2;

	//###################################################################
	// allocate graphic buffer of menu box  
	//###################################################################
	graphTexte = new GIF_bitMap();
	error_init(graphTexte->GFXnouveau(nbcol * charHeight,
		nblin * space2next , 1));
	if(erreur_num) return (erreur_num);
	graphTexte->buffer_clr();
	
	
	//###################################################################
	// save coordinates of the sprites
	//###################################################################
	char* ptAdd[8];
	const bb_describ *pDesc = sprite_object::zelistBOB[BOB_ESCMEN];
	Sint32 heigh = pDesc->BB_HAUTEUR;		//high in pixels
	heigh *= resolution;
	Sint32 nanim = pDesc->BB_ANIMATE;		//number of animations
	Sint32 width = pDesc->BB_LARGEUR;		//width in pixels
	width *= resolution;
	bbPosition *coord = pDesc->BBPOSITION;
	for(Sint32 i = 0; i <  nanim; i++)	
	{	Sint32 pos_x = (Sint32)coord[i].BB_COORDX;
		pos_x *= resolution;
		pos_x *= 16;
		Sint32 pos_y = (Sint32)coord[i].BB_COORDY;
		pos_y *= resolution;
		ptAdd[i] = image->GFXadresse(pos_x, pos_y);
	}
	Sint32 NxLine = image->GFX_nextLn();
		
	//###################################################################
	// initialize sprite object
	//###################################################################
	error_init(initialBOB(graphTexte, 0));
	if(erreur_num) return (erreur_num);
	Sint32 y = (240 * resolution -  nblin * space2next) / 2;
	y = (y / space2next) * space2next;
	coordonnee((large - BOBlargeur) / 2, y);	

	//###################################################################
	// build the frame of menu box (with sprites)
	//###################################################################
	Sint32 m = graphTexte->GFXlargeur() / width - 2;
	Sint32 x = width;
	y = graphTexte->GFXhauteur() - heigh;
	displayBOB(ptAdd[0], 0, 0, NxLine, width, heigh);
	displayBOB(ptAdd[3], 0, y, NxLine, width, heigh);
	for(Sint32 i = 0; i <  m; i++, x+= width)
	{	displayBOB(ptAdd[4], x, 0, NxLine, width, heigh);
		displayBOB(ptAdd[6], x, y, NxLine, width, heigh);
	}
	displayBOB(ptAdd[1], x, 0, NxLine, width, heigh);
	displayBOB(ptAdd[2], x, y, NxLine, width, heigh);

	m = graphTexte->GFXhauteur() / heigh - 2;
	if (graphTexte->GFXhauteur() % heigh)
		m++;
	
	x = graphTexte->GFXlargeur() - width;
	y = heigh;
	for(Sint32 i = 0; i <  m; i++, y+= heigh)
	{	displayBOB(ptAdd[5], 0, y, NxLine, width, heigh);
		displayBOB(ptAdd[7], x, y, NxLine, width, heigh);
	}
	return erreur_num;
}

//------------------------------------------------------------------------------
// display a sprite into the "buffer" (copy byte to byte)
//------------------------------------------------------------------------------
void escapeMenu::displayBOB(char *ptSrc, Sint32 pos_x, Sint32 pos_y, 
	Sint32 NxLine, Sint32 width, Sint32 heigh)
{
	char *s = ptSrc;
	char *d = graphTexte->GFXadresse(pos_x, pos_y);
	Sint32 m = NxLine;
	Sint32 n = graphTexte->GFX_nextLn();
	Sint32 h = width;
	Sint32 l = heigh;
	for(Sint32 i = 0; i < h; i++)
	{	for(Sint32 j = 0; j < l; j++)
		{	char p = s[j];		//read the pixel
			if(p)				//color 0 ?
				d[j] = p;		//no, put the pixel
		}
		s += m;
		d += n;
	}
}

//------------------------------------------------------------------------------
// display and handle menu
//------------------------------------------------------------------------------
Sint32 escapeMenu::execution1()
{
	Sint32 event = -1;

	//##############################################################
	// [ESC] key: enable / disable menu box
	//##############################################################
	if(keyGestion->specialKey(clavierMac::ESCAPEMENU)) {
		flag_actif = 1;
	} else {
		if(flag_actif && restoreBkg)
			MSK_bitclr();
		flag_actif = 0;
		return event;	
	}
	
	//##############################################################
	// check if right or left button are pressed
	//##############################################################
	Sint32 mposx, pos_y, freeL, freeR;
	Sint32 presL = keyGestion->leftButton();
	Sint32 presR = keyGestion->righButton();

	//##############################################################
	// read y where is pressed 
	//##############################################################
	if(presL && yPressLeft == YCOORDNULL)
		yPressLeft = keyGestion->sourisGetY();
	else
	{	if(presR && yPressRigh == YCOORDNULL)
			yPressRigh = keyGestion->sourisGetY();
	}	

	freeR = 0;	
	freeL = keyGestion->sourisRela(&mposx, &pos_y);
	if(!freeL)
		freeR = keyGestion->isReleaseR(&mposx, &pos_y);	

	if((freeL && pos_y == yPressLeft) || (freeR && pos_y == yPressRigh))
	{	Sint32 incre = 0;
		if(freeL)
		{	incre = 1;
			yPressLeft = YCOORDNULL;
		}
		if(freeR)
		{	incre = -1;
			yPressRigh = YCOORDNULL;
		}
		event = (pos_y - position_y) / space2next;
		/*printf("escapeMenu::execution1() pos_y:%i / position_y:%i / space2next:%i / event:%i\n",
			pos_y, position_y, space2next, event);*/
	}
	if(!presL)
		yPressLeft = YCOORDNULL;
	if(!presR)
		yPressRigh = YCOORDNULL;

	
	//###################################################################
	// read color table offset (color line hover by mouse )
	//###################################################################
	if(menu_color++ > 32)
		menu_color = 0;
	if(resolution == 1)
		display320();
	else	
		display640();
	
	//###################################################################
	// copy menu box into screen
	//###################################################################
	MSKbitcopy();

	if(event >= WECONTINUE && event <= WEQUITGAME)
	{	if(flag_actif && restoreBkg)
			MSK_bitclr();
		flag_actif = 0;
		keyGestion->resetpause();
	}
	
	if(menuNumber == 1 && event >= GOGAMEOVER)
		event++;
	if(event > 0)
		SDL_ShowCursor(SDL_DISABLE);
	return event;
}

//------------------------------------------------------------------------------
// display text in 640x480 mode
//------------------------------------------------------------------------------
void escapeMenu::display640() 
{
	Sint32 color = menu_color;
	char *desP1 = adresseGFX + charHeight + srceNextLn * space2next;
	Sint32 offSc = off_source;
	Sint32 offDs = srceNextLn;
	Sint32 offD2 = srceNextLn * (space2next - 1) + (charHeight * 2);
	Sint32 *basPT = (Sint32 *)caract_adr;
	const char *p = menu_texts[menuNumber];
	char *c = ascii2code;


	Sint32 a, b, j;
	Sint32 y = (keyGestion->sourisGetY() - position_y) / space2next;
	y--;
	for(Sint32 k = 0; k < nb_ofLines; k++, desP1 += offD2)
	{	
		if(y != k)
		{	
			//###########################################################
			// display normal line of n characters
			//###########################################################
			for(j = 0; j < nb_columns; j++)
			{	a = *(p++) - 32;
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
				}
				desP1 = desP1 + 16;
			}
		}
		else
		{	
			//###########################################################
			// display selected line of 32 characters
			//###########################################################
			for(j = 0; j < nb_columns; j++)
			{	unsigned char pixel = cyclingtab[color];
				char a = *(p++) - 32;
				if(a)
				{	b = c[a];
					unsigned char *s = (unsigned char *)basPT;
					unsigned char *d = (unsigned char *)desP1;
					b = b << 4;
					s = s + b;
					for(b = 0; b < 16; b++)
					{	for(Sint32 z = 0; z < 16; z++)
						{	a = s[z];
							if(a)
							{	a = pixel;
								d[z] = pixel;
							}
						}
						s = s + offSc;
						d = d + offDs;
					}
				}
				desP1 = desP1 + 16;
				if(color++ > 32) color = 0;
			}
		}
	}
		
	
}

//------------------------------------------------------------------------------
// display text in 320x240 mode
//------------------------------------------------------------------------------
void escapeMenu::display320() 
{
	Sint32 color = menu_color;
	char *desP1 = adresseGFX + charHeight + srceNextLn * space2next;
	Sint32 offSc = off_source;
	Sint32 offDs = srceNextLn;
	Sint32 offD2 = srceNextLn * (space2next - 1) + (charHeight * 2);
	Sint32 *basPT = (Sint32 *)caract_adr;
	const char *p = menu_texts[menuNumber];
	char *c = ascii2code;


	Sint32 a, b, j;
	Sint32 y = (keyGestion->sourisGetY() - position_y) / space2next;
	y--;
	for(Sint32 k = 0; k < nb_ofLines; k++, desP1 += offD2)
	{	
		if(y != k)
		{	
			//###########################################################
			// display normal line of n characters
			//###########################################################
			for(j = 0; j < nb_columns; j++)
			{	a = *(p++) - 32;
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
				}
				desP1 = desP1 + 8;
			}
		}
		else
		{	
			//###########################################################
			// display selected line of 32 characters
			//###########################################################
			for(j = 0; j < nb_columns; j++)
			{	unsigned char pixel = cyclingtab[color];
				char a = *(p++) - 32;
				if(a)
				{	b = c[a];
					unsigned char *s = (unsigned char *)basPT;
					unsigned char *d = (unsigned char *)desP1;
					b = b << 3;
					s = s + b;
					for(b = 0; b < 8; b++)
					{	for(Sint32 z = 0; z < 8; z++)
						{	a = s[z];
							if(a)
							{	a = pixel;
								d[z] = pixel;
							}
						}
						s = s + offSc;
						d = d + offDs;
					}
				}
				desP1 = desP1 + 8;
				if(color++ > 32) color = 0;
			}
		}
	}
}

const unsigned char escapeMenu::cyclingtab[] =
{	239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
	253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
	243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
	249, 250, 251, 252, 253, 254, 255
} ;
 

const char escapeMenu::menu_text1[] =
{	"CONTINUE THE PLAY CURRENT" \
	"     CAUSE GAME OVER     " \
	"      QUIT TO INTRO      " \
	"     QUIT TECNOBALLZ     " 
};
const char escapeMenu::menu_text2[] =
{	"CONTINUE THE PLAY CURRENT" \
	"      QUIT TO INTRO      " \
	"     QUIT TECNOBALLZ     " 
};

const char *escapeMenu::menu_texts[2] = {
		&escapeMenu::menu_text1[0],
		&escapeMenu::menu_text2[0],
};
