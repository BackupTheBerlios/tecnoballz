//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "lastScroll.cc"
// created	: ?
// updates	: 2005-01-07
// fonction	: handle of the scrolling background (menu and gards levels)
// id		: $Id: lastScroll.cc,v 1.10 2007/01/24 17:10:41 gurumeditation Exp $
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
#include "../include/lastScroll.h"
#include "../include/ressources.h"
#include "../include/handler_memory.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
lastScroll::lastScroll()
{
	mentatInit();
	carteFirst = (Uint16 *) NULL;
	gfx_bitMap = (bitmap_data *) NULL;
	mapAddress = (char **) NULL;
	motiflarge = 16 * resolution;
	motifhaute = 16 * resolution;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
lastScroll::~lastScroll()
{
	if(gfx_bitMap)
		delete gfx_bitMap;
	if(carteFirst)
		memory->release((char *)carteFirst);
	if(mapAddress)
		memory->release((char *)mapAddress);
	gfx_bitMap = (bitmap_data*) NULL;
	carteFirst = (Uint16 *) NULL;
	mapAddress = (char **) NULL;
	mentatKill();
}

//-----------------------------------------------------------------------------
// return bitmap object
//-----------------------------------------------------------------------------
bitmap_data*	lastScroll::getBitMap()
{	return gfx_bitMap;
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 lastScroll::initialise(Uint32 PalNu, Uint32 edmap)
{
	if(is_verbose)
		fprintf (stdout,
			"lastScroll::initialise() : [START]\n");

	//###################################################################
	// load the page of graphics maps im memory
	//###################################################################
	gfx_bitMap = new bitmap_data();
	gfx_bitMap->load(ressources::RESMAPEDIT);

	//###################################################################
	// load the map file in memory
	//###################################################################
	error_init(ld_mapfile(edmap));
	if(erreur_num) return erreur_num;

	//###################################################################
	// calculation the table of the addresses all maps
	//###################################################################
	error_init(initMapAdr());
	if(erreur_num) return erreur_num;

	//###################################################################
	// calculation certain values for the display loop
	//###################################################################
	hauteMotif = display->bufferHaut() / motifhaute;
	largeMotif = display->bufferLarg() / motiflarge;
	afficheAdr = (Sint32 *)display->buffer_adr();
	destinMod1 = display->buffer_mod(0);
	largeEcran = display->bufferNext();
	afficheAdr = afficheAdr - largeEcran * motifhaute;
	destinMod2 = (motifhaute * largeEcran) - motiflarge;
	destinMod3 = (motifhaute * largeEcran) - (motiflarge * largeMotif);
	source_mod = gfx_bitMap->get_row_size();
	y_coord = 0;

	//###################################################################
	// display all maps
	//###################################################################
	scrolling1(0);

	//###################################################################
	// initialize color palette
	//###################################################################
	enable_palette (PalNu);

	if(is_verbose)
		fprintf (stdout,
			"lastScroll::initialise() : [END]\n");	
	return erreur_num;
}

//-----------------------------------------------------------------------------
// get width of tile
//-----------------------------------------------------------------------------
Sint32 lastScroll::tile_width()
{
	return motiflarge;
}

//-----------------------------------------------------------------------------
// get y position
//-----------------------------------------------------------------------------
Sint32 lastScroll::returnPosy()
{
	return y_coord;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sint32 lastScroll::swapScroll(Uint32 PalNu, Uint32 edmap)
{
	if(carteFirst)
		memory->release((char *)carteFirst);
	error_init(ld_mapfile(edmap));
	if(erreur_num) return erreur_num;
	enable_palette (PalNu);
	y_coord = 0;
	return erreur_num;
}

//-----------------------------------------------------------------------------
// initialize color palette
//-----------------------------------------------------------------------------
void lastScroll::enable_palette(Uint32 PalNu)
{
	SDL_Color *palPT = display->paletteAdr();
	SDL_Color *palP1 = palPT;
	SDL_Color *palP2 = palP1 + 128;
	const unsigned char *colPT = colors_map;
	for(Sint32 i = 0; i < 16; i++)
	{	unsigned char r, b, g;

		switch(PalNu)
		{
			default:
				r = *(colPT++);
				g = *(colPT++);
				b = *(colPT++);
				break;
			case 1:
				r = *(colPT++);
				b = *(colPT++);
				g = *(colPT++);
			break;
			case 2:
				b = *(colPT++);
				g = *(colPT++);
				r = *(colPT++);
			break;
		}

		palP1->r = r;
		palP1->g = g;
		palP1->b = b;
		palP1->g = g;
		palP1->b = b;
		r >>= 1;
		g >>= 1;
		b >>= 1;
		palP2->r = r;
		palP2->g = g;
		palP2->b = b;
		palP1++;
		palP2++;
	}
	display->enable_palette(palPT);
}

//------------------------------------------------------------------------------
// color palette (16 colors)
//------------------------------------------------------------------------------
const unsigned char lastScroll::colors_map[48] =
{	2, 2, 2,
	17, 33, 50,
	66, 98, 130,
	19, 50, 81, 
	27, 58, 90,
	85, 101, 133,
	36, 66, 98,
	39, 70, 102,
	146, 146, 178,
	82, 82, 114,
	129, 129, 161,
	59, 90, 122,
	98, 98, 130,
	42, 74, 106,
	50, 82, 114,
	111, 114, 146
};


//------------------------------------------------------------------------------
// scrolling
//------------------------------------------------------------------------------
void lastScroll::scrolling1(Sint32 index)
{
	Sint32 i = y_coord + index;
	Sint32 j = CARTEHAUTE * motifhaute;	// 273 * 32 = 8736
	if(i < 0)
		i += j;
	if(i > j)
		i -= j;
	y_coord = i;
	if(resolution == 1)
		j = i & 15;
	else
		j = i & 31;
	offset_aff = j;
	i /= motifhaute;
	i *= CARTELARGE;
	carteTable = carteFirst + i;
	afficheAll();
}

//------------------------------------------------------------------------------
// display all maps
//------------------------------------------------------------------------------
void lastScroll::afficheAll()
{
	if(resolution == 1)
		display320();
	else
		display640();
}
	
//------------------------------------------------------------------------------
// display all maps (320 pixels mode)
//------------------------------------------------------------------------------
void lastScroll::display320()
{
	Sint32 m1 = destinMod1;
	Sint32 n1 = source_mod;
	Sint32 *dt = (Sint32 *)afficheAdr;
	char **mapPT = mapAddress;	// pointer of each map of the page maps
	Uint16 *carte = carteTable;
	Sint32 lg = largeMotif;
	Sint32 m2, m3;
	Sint32 j = offset_aff;
	j = motifhaute - j;
	m2 = (j * largeEcran) - motiflarge;
	m3 = (j * largeEcran) - (motiflarge * largeMotif);

	//###################################################################
	// display the first line (possible to cut)
	//###################################################################
	for(Sint32 x = 0; x < lg; x++)
	{	Sint32 i = *(carte++);
		Sint32 *s = (Sint32 *) mapPT[i];
		for(i = 0; i < j; i++)
		{	dt[0] = s[0];
			dt[1] = s[1];
			dt[2] = s[2];
			dt[3] = s[3];
			s = (Sint32 *)((char *)s + n1);
			dt =(Sint32 *)((char *)dt + m1);
		}
		dt =(Sint32 *)((char *)dt - m2);
	}
 
	//###################################################################
	// display the middle lines (always enterly)
	//###################################################################
	//(char *)dt += m3;
	dt = (Sint32 *)((char *)dt + m3);
	Sint32 ht = hauteMotif - 1;
	m2 = destinMod2;
	m3 = destinMod3;
	for(Sint32 y = 0; y < ht; y++)
	{	for(Sint32 x = 0; x < lg; x++)
		{	Sint32 i = *(carte++);
			Sint32 *s = (Sint32 *) mapPT[i];
			for(i = 0; i < motifhaute; i++)
			{	dt[0] = s[0];
				dt[1] = s[1];
				dt[2] = s[2];
				dt[3] = s[3];
				s = (Sint32 *)((char *)s + n1);
				dt =(Sint32 *)((char *)dt + m1);
			}
			dt =(Sint32 *)((char *)dt - m2);
		}
		dt =(Sint32 *)((char *)dt + m3);
	}

	//###################################################################
	// display the last line (possible to cut)
	//###################################################################
	j = offset_aff;
	if(j)
	{	m2 = (j * largeEcran) - motiflarge;
		m3 = (j * largeEcran) - (motiflarge * largeMotif);
		for(Sint32 x = 0; x < lg; x++)
		{	Sint32 i = *(carte++);
			Sint32 *s = (Sint32 *) mapPT[i];
			for(i = 0; i < j; i++)
			{	dt[0] = s[0];
				dt[1] = s[1];
				dt[2] = s[2];
				dt[3] = s[3];
				s = (Sint32 *)((char *)s + n1);
				dt =(Sint32 *)((char *)dt + m1);
			}
			dt =(Sint32 *)((char *)dt - m2);
		}
		dt =(Sint32 *)((char *)dt + m3);
	}
}

//------------------------------------------------------------------------------
// display all maps (640 pixels mode)
//------------------------------------------------------------------------------
void lastScroll::display640()
{
	Sint32 m1 = destinMod1;
	Sint32 n1 = source_mod;
	Sint32 *dt = (Sint32 *)afficheAdr;
	char **mapPT = mapAddress;	// pointer of each map of the page maps
	Uint16 *carte = carteTable;
	Sint32 lg = largeMotif;
	Sint32 m2, m3;
	Sint32 j = offset_aff;
	j = motifhaute - j;
	m2 = (j * largeEcran) - motiflarge;
	m3 = (j * largeEcran) - (motiflarge * largeMotif);

	//###################################################################
	// display the first line (possible to cut)
	//###################################################################
	for(Sint32 x = 0; x < lg; x++)
	{	Sint32 i = *(carte++);
		Sint32 *s = (Sint32 *) mapPT[i];
		for(i = 0; i < j; i++)
		{	dt[0] = s[0];
			dt[1] = s[1];
			dt[2] = s[2];
			dt[3] = s[3];
			dt[4] = s[4];
			dt[5] = s[5];
			dt[6] = s[6];
			dt[7] = s[7];
			s = (Sint32 *)((char *)s + n1);
			dt =(Sint32 *)((char *)dt + m1);
		}
		dt =(Sint32 *)((char *)dt - m2);
	}
 
	//###################################################################
	// display the middle lines (always enterly)
	//###################################################################
	//(char *)dt += m3;
	dt = (Sint32 *)((char *)dt + m3);
	Sint32 ht = hauteMotif - 1;
	m2 = destinMod2;
	m3 = destinMod3;
	for(Sint32 y = 0; y < ht; y++)
	{	for(Sint32 x = 0; x < lg; x++)
		{	Sint32 i = *(carte++);
			Sint32 *s = (Sint32 *) mapPT[i];
			for(i = 0; i < motifhaute; i++)
			{	dt[0] = s[0];
				dt[1] = s[1];
				dt[2] = s[2];
				dt[3] = s[3];
				dt[4] = s[4];
				dt[5] = s[5];
				dt[6] = s[6];
				dt[7] = s[7];
				s = (Sint32 *)((char *)s + n1);
				dt =(Sint32 *)((char *)dt + m1);
			}
			dt =(Sint32 *)((char *)dt - m2);
		}
		dt =(Sint32 *)((char *)dt + m3);
	}

	//###################################################################
	// display the last line (possible to cut)
	//###################################################################
	j = offset_aff;
	if(j)
	{	m2 = (j * largeEcran) - motiflarge;
		m3 = (j * largeEcran) - (motiflarge * largeMotif);
		for(Sint32 x = 0; x < lg; x++)
		{	Sint32 i = *(carte++);
			Sint32 *s = (Sint32 *) mapPT[i];
			for(i = 0; i < j; i++)
			{	dt[0] = s[0];
				dt[1] = s[1];
				dt[2] = s[2];
				dt[3] = s[3];
				dt[4] = s[4];
				dt[5] = s[5];
				dt[6] = s[6];
				dt[7] = s[7];
				s = (Sint32 *)((char *)s + n1);
				dt =(Sint32 *)((char *)dt + m1);
			}
			dt =(Sint32 *)((char *)dt - m2);
		}
		dt =(Sint32 *)((char *)dt + m3);
	}
}

//------------------------------------------------------------------------------
// calculation the table of the addresses all maps
//------------------------------------------------------------------------------
Sint32 lastScroll::initMapAdr()
{
	Sint32 error = 0;
	bitmap_data *gfxPT = gfx_bitMap;
	Sint32 l = gfxPT->get_width();	//320 or 640 pixels width
	Sint32 h = gfxPT->get_height();	//624 or 1248 lines height
	dalleTotal = (l / motiflarge) * (h / motifhaute);	//780 maps
	mapAddress = (char **)memory->alloc(dalleTotal * sizeof(char *),
													0x6D670141);
	if(!mapAddress) return (memory->retour_err());

	char **m = mapAddress;
	Sint32 nbMap = 0;
	for(Sint32 y = 0; y < h; y += motifhaute)
	{	for(Sint32 x = 0; x < l; x += motiflarge)
		{	char *p = gfxPT->get_pixel_data(x, y);
			*(m++) = p;
			nbMap++;
		}
	}
	return error;
}

//------------------------------------------------------------------------------
// load and convert the map file
//------------------------------------------------------------------------------
Sint32 lastScroll::ld_mapfile(Uint32 edmap)
{
	switch (edmap)
	{	case MAPED_GARD:
		default:
			edmap = ressources::RESEDMAP01;
			break;
		case MAPED_MENU:
			edmap = ressources::RESEDMAP03;
			break;
		case MAPED_CONG:
			edmap = ressources::RESEDMAP02;
			break;
	}
	
	//###################################################################
	// load the map table in memory (created on Amiga)
	//	- size of file			: 10920 bytes (5460 maps code)
	//	- number of raw		: 20 
	//	- number of lines		: 273
	//	- size of map code	: 2 bytes	
	//###################################################################
	Uint16 *zfile = (Uint16 *)pRessource->getResData(edmap);
	if(!zfile)
	{	erreur_num = E_FILERROR;
		return erreur_num;
	}

	//###################################################################
	// allocate memory for the map editor
	//###################################################################
	Sint32 tsupp = (display->get_height() / motifhaute) * 2;
	Sint32 msize = (tsupp + CARTEHAUTE) * CARTELARGE * sizeof(Uint16);
	carteFirst = (Uint16 *) memory->alloc(msize, 0x54425249);
	error_init(memory->retour_err());
	if(erreur_num)
	{	memory->release((char *)zfile);
		return erreur_num;
	}

	//###################################################################
	// convert "big-endian" to "little-endian" or "big-endian"
	//###################################################################
	Sint32 i = 0;
	unsigned char* ptmap = (unsigned char *)zfile;
	for(Sint32 j = 0; j < (CARTEHAUTE * CARTELARGE); j++)
	{	Uint16 codem = 0;
		codem = (Uint16) ptmap[0];
		codem = codem << 8;
		codem = codem | ptmap[1];
		codem = codem >> 2;	//divide by 4, because increment pointer = 4 bytes 
		carteFirst[i++] = codem;
		ptmap = ptmap + 2;
	}
	memory->release((char *)zfile);

	//###################################################################
	// copy a height of the screen (for scrolling rotation)
	//###################################################################
	zfile = carteFirst;
	for(Sint32 j = 0; j < (tsupp * CARTELARGE); j++)
		carteFirst[i++] = zfile[j];
	carteTable = carteFirst;
	return erreur_num;
}
