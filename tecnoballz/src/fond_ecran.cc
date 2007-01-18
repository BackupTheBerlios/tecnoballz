//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "fond_ecran.cc"
// created	: ?
// updates	: 2005-07-17
// fonctions	: display background (bricks levels)
// Id		: $Id: fond_ecran.cc,v 1.7 2007/01/18 08:42:04 gurumeditation Exp $
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
#include "../include/GIF_bitMap.h"
#include "../include/handler_display.h"
#include "../include/fond_ecran.h"
#include "../include/handler_keyboard.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
fond_ecran::fond_ecran()
{
	mentatInit();
	iFond_type = 2;		//fond de 64*64 : new map
	colorActif = 0;
	if(resolution == 1 || bg4_colors) iFond_type = 0;
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
fond_ecran::~fond_ecran()
{
	mentatKill();
}

// -----------------------------------------------------------------------------
// Charge le fond & affiche le fond / load and display background
// -----------------------------------------------------------------------------
Sint32 fond_ecran::instalFond(Sint32 nbkdg)
{
	if(is_verbose)
		printf("fond_ecran::instalFond() nbkdg = %i\n", nbkdg);
	GIF_bitMap *fonds = new GIF_bitMap();
	Sint32 large = 0;
	Sint32 haute = 0;
	Sint32 *t_pos;
	switch (iFond_type)
	{	
		//###############################################################
		// background map 's size : 32*32 pixels (320x200)
		//###############################################################
		case 0:
			// load all 60 backgrounds differents 
			if(is_verbose)
				printf("fond_ecran::instalFond TecnoballZ/min60map.bmp\n");
			error_init(fonds->decompacte(ressources::RES60BACKG)); 
			if(erreur_num) return erreur_num;
			large = FONDLARGE1;
			haute = FONDHAUTE1;
			t_pos = table_pos1;
			break;

		//###############################################################
		// new background map's size : 64*64 pixels 8 colors
		//###############################################################
		default:
		{	if (nbkdg < 1)
				nbkdg= (hasard_val & 127) + 1;
			 if(nbkdg > 77)
				nbkdg = nbkdg - 77;
			//nbkdg = 79; // test only
			char *pathname = pRessource->getTexFile(nbkdg);
			if (!pathname) {
				erreur_num = E_FILERROR;
				return erreur_num;
			}
			if(is_verbose)
				 printf("fond_ecran::instalFond() : try to initialize %s\n", 
					pathname);
			error_init(fonds->decompacte(pathname));
			if(erreur_num) return erreur_num;
			large = FONDLARGE2;
			haute = FONDHAUTE2;
			t_pos = table_pos1;
			if(fonds->GFXlargeur()/FONDLARGE2 > 5)
			t_pos = table_pos2;
		}
		break;
	}
	if(erreur_num) return erreur_num;

	Sint32 dHorz = (display->bufferLarg() - 64 * resolution) / large - 1;
	Sint32 oSour = fonds->GFX_nextLn();
	Sint32 dVert = (240 * resolution) / haute - 1;
	Sint32 mVert = (240 * resolution) % haute - 1;
	if(mVert > 0) dVert++;
	Sint32 baseX;
	Sint32 baseY;
	if(iFond_type < 2)
	{	//hasard_val=42;					//test only
		//select one of the 60 backgrounds
		baseY = hasard_val & 0x3F;			//value 0 to 63
		if(baseY >= 60)
			baseY -= 60;
		if(baseY & 0x1)
			baseX = fonds->GFXlargeur() / 2;	//second part
		else
			baseX = 0;
		baseY >>= 1;
		baseY = baseY * haute;
	}
	else
	{	baseX = 0;
		baseY = 0;
		dVert = 480 / haute;
	}
 
	//###############################################################
	// display background map 
	//###############################################################
	Sint32 src_X = 0;
#if __WORDSIZE == 64
	Sint32 h = (long)display;
	Sint32 k = (long)fonds;
#else
	Sint32 h = (Sint32)display;	//use pointer address as random value
	Sint32 k = (Sint32)fonds;	//use pointer address as random value
#endif
	Sint32 nline;
	if(mVert > 0)
		nline = mVert;
	else
		nline = haute;
	for(Sint32 det_Y = dVert; det_Y >= 0; det_Y--)
	{	for(Sint32 det_X = dHorz; det_X > -1; det_X--)
		{	hasard_val = hasard_val + h + k + 1 + keyGestion->sourisGetX();
			h = h + countframe + det_Y;
			k = k + display->get_framepee();
			src_X = hasard_val;
			src_X &= 0x0f;		//table index (0 to 15)
			src_X = t_pos[src_X];	//source position (0 to 4)
			src_X *= large;
			char *srcPT = fonds->GFXadresse(baseX + src_X, baseY);
			char *detPT = display->tampon_pos(det_X * large, det_Y * haute);
			switch (iFond_type)
			{	case 0:
					display->buf_affx32(srcPT, detPT, oSour, nline);
					break;
				case 1:
					display->buf_affx64(srcPT, detPT, oSour, nline);
					break;
				case 2:
					display->buf_affx64(srcPT, detPT, oSour, nline);
					break;
			}
		}
		nline = haute;
	}
 
	//###############################################################
	// display top shadow
	//###############################################################
	dHorz = display->get_width() - (64 * resolution);
	dVert = display->get_height();
	for(Sint32 det_Y = 0; det_Y < (handler_display::SHADOWOFFY * resolution); det_Y++)
	{	for(Sint32 det_X = 0; det_X < dHorz; det_X++)
		{	char *detPT = display->tampon_pos(det_X, det_Y);
			*detPT |= handler_display::SHADOW_PIX;
		}
	}

	//###############################################################
	// display right shadow
	//###############################################################
	for(Sint32 det_Y = 0; det_Y < dVert; det_Y++)
	{	for(Sint32 det_X = (252 * resolution); det_X < (256 * resolution); det_X++)
		{	char *detPT = display->tampon_pos(det_X, det_Y);
			*detPT |= handler_display::SHADOW_PIX;
		}
	}

	//###############################################################
	// initialize color palette
	//###############################################################
	if (iFond_type == 2)
	{	unsigned char* colPT = fonds->paletteADR();
		SDL_Color *palPT = display->paletteAdr();
		SDL_Color *palP1 = palPT;
		SDL_Color *palP2 = palP1 + 128;
		unsigned char pixel;
		for(Sint32 i = 0; i < 16; i++)
		{	pixel = *(colPT++);	//red
			palP1->r = pixel;
			pixel >>= 1;
			palP2->r = pixel;
			pixel = *(colPT++);	//green
			palP1->g = pixel;
			pixel >>= 1;
			palP2->g = pixel;
			pixel = *(colPT++);	//blue
			palP1->b = pixel;
			pixel >>= 1;
			palP2->b = pixel;
			palP1++;
			palP2++;
		}
		display->palette_go(palPT); 
	}
	else
	{	coulDuFond();
	}
	delete fonds;

	return (erreur_num);
}

//------------------------------------------------------------------------------
// sources x position map background (original and new)
//------------------------------------------------------------------------------
Sint32 fond_ecran::table_pos1[16] =
{	3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 0, 0, 2, 4 };
Sint32 fond_ecran::table_pos2[16] =
{	3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 5, 0, 5, 4 };


//------------------------------------------------------------------------------
// select one background palette color (original 4 colors map)
//------------------------------------------------------------------------------
void fond_ecran::coulDuFond()
{
	Sint32 j = hasard_val & 0x1ff;
	if(j >= 448)
		j -= 448;	//112 preset colors 
	j = j & 0xfff0;
	coulDuFond(j);
}
//------------------------------------------------------------------------------
// select next background palette color (original 4 colors map)
//------------------------------------------------------------------------------
void fond_ecran::next_color()
{
	colorActif += 16;
	if(colorActif >= 432)
		colorActif = 0;
	coulDuFond(colorActif);
}

//------------------------------------------------------------------------------
// select previous background palette color (original 4 colors map)
//------------------------------------------------------------------------------
void fond_ecran::prev_color()
{
	colorActif -= 16;
	if(colorActif < 0)
		colorActif = 432;
	coulDuFond(colorActif);
}
//------------------------------------------------------------------------------
// initialize a background palette color (original 4 colors map)
//------------------------------------------------------------------------------
void fond_ecran::coulDuFond(Sint32 zecol)
{
	colorActif = zecol;
	char *color =&couleurs[0];
	unsigned char *colPT = (unsigned char *)(color) + zecol;
	SDL_Color *palPT = display->paletteAdr();
	SDL_Color *palP1 = palPT + 1;
	SDL_Color *palP2 = palP1 + 128;
	Sint32 j = 4;
	for(Sint32 i = 0; i < j; i++)
	{	unsigned char pixel = *(colPT++);
		pixel = *(colPT++);	//red
		palP1->r = pixel;
		pixel >>= 1;
		palP2->r = pixel;
		pixel = *(colPT++);	//green
		palP1->g = pixel;
		pixel >>= 1;
		palP2->g = pixel;
		pixel = *(colPT++);	//red
		palP1->b = pixel;
		pixel >>= 1;
		palP2->b = pixel;
		palP1++;
		palP2++;
	}
	display->palette_go(palPT);
}

//------------------------------------------------------------------------------
// preset palettes: 4 colors original background (mode 320x200)
//------------------------------------------------------------------------------
// 448 / 16	: 28 preset palettes
// 4 * 4	: 16 composantes by palette
// 112 * 4	: 448 composantes
char fond_ecran::couleurs[448] = {
0x00, 0x40, 0x20, 0x40, 0x00, 0x60, 0x40, 0x60, 0x00, 0x80, 0x60, 0x80, 0x00, 0xA0, 0x80, 0xA0, 0x00, 0x00, 0x20, 0x40, 0x00, 0x20,
0x40, 0x60, 0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x00, 0x20, 0x20, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
0x00, 0x60, 0x80, 0x80, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x60, 0x80, 0x00, 0x40, 0x80, 0xA0, 0x00, 0x30,
0x40, 0x30, 0x00, 0x50, 0x60, 0x50, 0x00, 0x70, 0x80, 0x70, 0x00, 0x90, 0xA0, 0x90, 0x00, 0x20, 0x20, 0x40, 0x00, 0x40, 0x40, 0x60,
0x00, 0x60, 0x60, 0x80, 0x00, 0x80, 0x80, 0xA0, 0x00, 0x00, 0x40, 0x40, 0x00, 0x20, 0x60, 0x60, 0x00, 0x40, 0x80, 0x80, 0x00, 0x60,
0xA0, 0xA0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x40, 0x20, 0x40, 0x00, 0x60, 0x40, 0x60, 0x00, 0x80, 0x60, 0x80, 0x00, 0x00, 0x40, 0x20,
0x00, 0x00, 0x60, 0x40, 0x00, 0x20, 0x80, 0x60, 0x00, 0x40, 0xA0, 0x80, 0x00, 0x40, 0x20, 0x00, 0x00, 0x60, 0x40, 0x20, 0x00, 0x80,
0x60, 0x40, 0x00, 0xA0, 0x80, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x20, 0x20, 0x00, 0x80, 0x40, 0x40, 0x00, 0xA0, 0x60, 0x60,
0x00, 0x40, 0x00, 0x40, 0x00, 0x60, 0x20, 0x60, 0x00, 0x80, 0x40, 0x80, 0x00, 0xA0, 0x60, 0xA0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20,
0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60, 0x00, 0x20, 0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60,
0x00, 0x80, 0xA0, 0x80, 0x00, 0x40, 0x40, 0x00, 0x00, 0x60, 0x60, 0x20, 0x00, 0x80, 0x80, 0x40, 0x00, 0xA0, 0xA0, 0x60, 0x00, 0x00,
0x40, 0x00, 0x00, 0x20, 0x60, 0x20, 0x00, 0x40, 0x80, 0x40, 0x00, 0x60, 0xA0, 0x60, 0x00, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40,
0x00, 0x60, 0x60, 0x60, 0x00, 0x80, 0x80, 0x80, 0x00, 0x40, 0x20, 0x60, 0x00, 0x60, 0x40, 0x80, 0x00, 0x80, 0x60, 0xA0, 0x00, 0xA0,
0x80, 0xC0, 0x00, 0x20, 0x20, 0x00, 0x00, 0x40, 0x40, 0x20, 0x00, 0x60, 0x60, 0x40, 0x00, 0x80, 0x80, 0x60, 0x00, 0x20, 0x40, 0x60,
0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x80, 0xA0, 0xC0, 0x00, 0x60, 0x40, 0x20, 0x00, 0x80, 0x60, 0x40, 0x00, 0xA0,
0x80, 0x60, 0x00, 0xC0, 0xA0, 0x80, 0x00, 0x40, 0x00, 0x60, 0x00, 0x60, 0x20, 0x80, 0x00, 0x80, 0x40, 0xA0, 0x00, 0xA0, 0x60, 0xC0,
0x00, 0x40, 0x00, 0x20, 0x00, 0x60, 0x20, 0x40, 0x00, 0x80, 0x40, 0x60, 0x00, 0xA0, 0x60, 0x80, 0x00, 0x20, 0x20, 0x60, 0x00, 0x40,
0x40, 0x80, 0x00, 0x60, 0x60, 0xA0, 0x00, 0x80, 0x80, 0xC0, 0x00, 0x60, 0x40, 0x00, 0x00, 0x80, 0x60, 0x20, 0x00, 0xA0, 0x80, 0x40,
0x00, 0xC0, 0x80, 0x60, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60, 0x20, 0x00, 0x60, 0x80, 0x40, 0x00, 0x80, 0xA0, 0x60, 0x00, 0x40,
0x20, 0x20, 0x00, 0x60, 0x40, 0x40, 0x00, 0x80, 0x60, 0x60, 0x00, 0xA0, 0x80, 0x80, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
0x00, 0x60, 0x80, 0x80, 0x00, 0x80, 0xA0, 0xA0
};
