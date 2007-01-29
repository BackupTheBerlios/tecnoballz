//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "scrolledit.cc"
// created      : 2004-09-13
// updates      : 2004-09-14
// fonction     : edit scrolling map (menu and guards)
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
//******************************************************************************
#include "../include/scrolledit.h"
#include "../include/handler_resources.h"
#include "../include/list_sprites.h"
#include "../include/handler_keyboard.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>


//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
scrolledit::scrolledit()
{
	initialise();
	defilement = new lastScroll();	// background scrolling
	objetMouse = new mousePoint();
	displayMod = 0;
	flagSpaceK = 0;
	titlesPosy = 0;
	
	flag_press = 0;
	
	box_colour = 0;
	
	pBrushTile = (Uint16*) NULL;
	pBrush_bob = (bitmap_data*) NULL;
	
	flag_press = 0;
	flagPress2 = 0;
	brush_posx = 0;
	brush_posy = 0;
	brushWidth = 0;
	brushHeigh = 0;
	
	keyS_press = 0;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
scrolledit::~scrolledit()
{
	delete objetMouse;
	delete defilement;
	if(pt_select1)
		memory->release((char *)pt_select1);

	if(pBrushTile)
		memory->release((char *)pBrushTile);
	if(pBrush_bob)
		delete pBrush_bob;
	
	liberation();
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 scrolledit::first_init()
{	
	
	pt_select1 = (selectinfo *) memory->alloc 
		(sizeof(selectinfo) * 2, 0x4D454741);
	error_init(memory->retour_err());
	pt_select2 = &pt_select1[1];
	pt_select0 = pt_select1;
	
	screen_height = display->get_height();
	screen_width = display->get_width();

	sprites->reset();
	resources->load_sprites_bitmap();
	objetMouse->create_BOB(sprites_bitmap);
	resources->release_sprites_bitmap();

	
	//Sint32	edmap = lastScroll::MAPED_MENU;
	//Sint32	edmap = lastScroll::MAPED_GARD;
	Sint32	edmap = lastScroll::MAPED_CONG;
	
	error_init(defilement->initialise(lastScroll::TECZ_COLOR_MENU, edmap));
	if(erreur_num) return (erreur_num);
	ptrGBitMap = defilement->getBitMap();
	
	tile_width = defilement->tile_width();
	tile_mask1 = 0xffffffff ^ (tile_width - 1);
	tile_mask2 = ~tile_mask1;

	display->gradation1();
	return erreur_num;
}


//------------------------------------------------------------------------------
// main loop
//------------------------------------------------------------------------------
Sint32 scrolledit::zeMainLoop()
{
	display->wait_frame();
	display->verouiller();

	pt_select1->boxOffsetY = defilement->returnPosy();
	pt_select2->boxOffsetY = titlesPosy;
	
	switch (displayMod)
	{	case 1:
			view_tiles();
			break;
		default:
			vscrolling();
			break;
	}
		
	objetMouse->bouge_test();
	sprites->draw();
	
	
	//###################################################################
	// copy whole buffer surface into screen surface
	//###################################################################
	display->deverouill();
	display->bufferCTab();

	//###################################################################
	// escape key to quit the game !
	//###################################################################
	if(keyboard->command_is_pressed(handler_keyboard::TOEXITFLAG))
		end_return = -1;

	check_keys();
	if(keyboard->key_is_pressed(SDLK_F10))
	{	end_return = 4;
	}
	
	if(keyboard->key_is_pressed(SDLK_s) && !keyS_press)
		keyS_press = 1;

	if(keyboard->key_is_released(SDLK_s) && keyS_press)
	{	keyS_press = 0;
		saveTheMap();
	}	
	
	
	return end_return;
}

//------------------------------------------------------------------------------
// map view mode
//------------------------------------------------------------------------------
void scrolledit::vscrolling()
{
	pt_select0 = pt_select1;
	Sint32 speed = getZeSpeed();
	defilement->scrolling1(speed);		// scrolling of the background of the screen
	select_box();
	drawingBox();
	brush_draw();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void scrolledit::maps2brush()
{
	printf("scrolledit::maps2brush() : [%i, %i, %i, %i]\n", 
		pt_select0->box_pos_x1, pt_select0->box_pos_y1, pt_select0->box_pos_x2, pt_select0->box_pos_y2);

	//###################################################################
	// allocate map memory
	//###################################################################
	if(pBrushTile)
		memory->release((char *)pBrushTile);
	pBrushTile = (Uint16 *) memory->alloc 
		(sizeof(Uint16) * pt_select0->box_height * pt_select0->box_widthT,
		0x4D454741);
	error_init(memory->retour_err());
	if(erreur_num) return;

	//Sint32 scrlY = defilement->returnPosy();
	Sint32 i = pt_select0->box_pos_y1;
	i = (i / defilement->motifhaute) + 4;
	i *= lastScroll::CARTELARGE;
	i += (pt_select0->box_pos_x1 / defilement->motiflarge);

	Uint16* carte = defilement->carteFirst + i;
	//Uint16* carte = defilement->carteFirst;
	Uint16* ptBrh = pBrushTile;
	for (Sint32 y = 0; y < pt_select0->box_height ; y++)
	{	for (Sint32 x = 0; x < pt_select0->box_widthT; x++)
			*(ptBrh++) = carte[x];
		carte += lastScroll::CARTELARGE;
	}
	brushAlloc();	
}
	
//------------------------------------------------------------------------------
// tiles wiew mode
//------------------------------------------------------------------------------
void scrolledit::view_tiles()
{
	pt_select0 = pt_select2;
	Sint32 speed = getZeSpeed();
	Sint32 y_max = ptrGBitMap->get_height() - screen_height;
	
	titlesPosy = titlesPosy + speed;
	if(titlesPosy < 0)
			titlesPosy = 0;
	if(titlesPosy > y_max)
			titlesPosy = y_max;
	
	//printf("y_max :%i / titlesPosy: %i\n", y_max, titlesPosy);
	ptrGBitMap->copyBuffer(0, titlesPosy, 0, 0, screen_width, screen_height);
	select_box();
	drawingBox();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void scrolledit::tile2brush()
{
	printf("scrolledit::tile2brush() : [%i, %i, %i, %i]\n", 
		pt_select0->box_pos_x1, pt_select0->box_pos_y1, pt_select0->box_pos_x2, pt_select0->box_pos_y2);

	
	Sint32 o =  (pt_select0->box_pos_y1 / tile_width) * lastScroll::CARTELARGE +
		(pt_select0->box_pos_x1 / tile_width);

	//###################################################################
	// allocate map memory
	//###################################################################
	if(pBrushTile)
		memory->release((char *)pBrushTile);
	pBrushTile = (Uint16 *) memory->alloc 
		(sizeof(Uint16) * pt_select0->box_height * pt_select0->box_widthT,
		0x4D454741);
	error_init(memory->retour_err());
	if(erreur_num) return;
	
	
	Uint16 *ptBrh = pBrushTile;
	for (Sint32 y = 0; y < pt_select0->box_height ; y++)
	{
		Sint32 p = o;
		for (Sint32 x = 0; x < pt_select0->box_widthT; x++)
		{	//printf("%i ", p);
			*(ptBrh++) = p;
			p++;
		}
		o += lastScroll::CARTELARGE;
		//printf("\n");
	}
	brushAlloc();
}




//------------------------------------------------------------------------------
// check keyboard keys
//------------------------------------------------------------------------------
void scrolledit::check_keys()
{
	if(keyboard->key_is_pressed(SDLK_SPACE))
		flagSpaceK = 1;
	if(keyboard->key_is_released(SDLK_SPACE) && flagSpaceK)
	{	switch(displayMod)
		{	case 0:
				displayMod = 1;
				break;
			default:
				displayMod = 0;
				break;
		}
		flagSpaceK = 0;
	}
}

//------------------------------------------------------------------------------
// determine scrolling speed
//------------------------------------------------------------------------------
Sint32 scrolledit::getZeSpeed()
{	
	Sint32 speed = 0;
	Sint32 mousY = keyboard->get_mouse_y();
	if(mousY > 0 && mousY < 8 * resolution)
			speed = -16 * resolution;
	if(mousY >= 8 * resolution && mousY < 16 * resolution)
			speed = -8 * resolution;
	if(mousY >= 16 * resolution && mousY < 24 * resolution)
			speed = -4 * resolution;
	if(mousY <= screen_height - 16 * resolution && mousY > screen_height - 24 * resolution)
			speed = 4 * resolution;
	if(mousY <= screen_height - 8 * resolution && mousY > screen_height - 16 * resolution)
			speed = 8 * resolution;
	if(mousY < screen_height && mousY > screen_height - 8 * resolution)
			speed = 16 * resolution;
	return speed;
}


//------------------------------------------------------------------------------
// handle selection (right mouse button)
//------------------------------------------------------------------------------
void scrolledit::select_box()
{
	
	
	//Sint32 presL = keyboard->is_left_button();
	Sint32 presR = keyboard->is_right_button();

	//##############################################################
	// read y where is pressed 
	//##############################################################
	if(presR && !flag_press)
	{	flag_press = 1;
		pt_select0->box_pos_x1 = keyboard->get_mouse_x();
		pt_select0->box_pos_y1 = pt_select0->boxOffsetY + keyboard->get_mouse_y();
		pt_select0->box_pos_x1 &= tile_mask1;
		pt_select0->box_pos_y1 &= tile_mask1;
		if(pBrush_bob)
		{	delete pBrush_bob;
			pBrush_bob = (bitmap_data*) NULL;
		}
	}


	if(flag_press)
	{	
		pt_select0->box_pos_x2 = keyboard->get_mouse_x();
		pt_select0->box_pos_y2 = keyboard->get_mouse_y() + pt_select0->boxOffsetY;
		
		if (pt_select0->box_pos_x2 & tile_mask2)
			pt_select0->box_pos_x2 += tile_width;
		if (pt_select0->box_pos_y2 & tile_mask2)
			pt_select0->box_pos_y2 += tile_width;
		
		pt_select0->box_pos_x2 &= tile_mask1;
		pt_select0->box_pos_y2 &= tile_mask1;
		if(pt_select0->box_pos_x2 >= pt_select0->box_pos_x1 &&
			pt_select0->box_pos_x2 - pt_select0->box_pos_x1 < tile_width)
			pt_select0->box_pos_x2 = pt_select0->box_pos_x1 + tile_width;
		
		if(pt_select0->box_pos_x2 < pt_select0->box_pos_x1 &&
			pt_select0->box_pos_x1 - pt_select0->box_pos_x2 < tile_width)
			pt_select0->box_pos_x2 = pt_select0->box_pos_x1 - tile_width;

		if(pt_select0->box_pos_y2 >= pt_select0->box_pos_y1 &&
			pt_select0->box_pos_y2 - pt_select0->box_pos_y1 < tile_width)
			pt_select0->box_pos_y2 = pt_select0->box_pos_y1 + tile_width;

		if(pt_select0->box_pos_y2 < pt_select0->box_pos_y1 &&
			pt_select0->box_pos_y1 - pt_select0->box_pos_y2 < tile_width)
			pt_select0->box_pos_x2 = pt_select0->box_pos_x1 - tile_width;
		


			if(pt_select0->box_pos_y1 < pt_select0->box_pos_y2
				&& pt_select0->box_pos_y2 - pt_select0->box_pos_y1 > (screen_height / 2))
				pt_select0->box_pos_y2 = pt_select0->box_pos_y1 + (screen_height / 2);

			if(pt_select0->box_pos_y1 > pt_select0->box_pos_y2
				&& pt_select0->box_pos_y1 - pt_select0->box_pos_y2 > (screen_height / 2))
				pt_select0->box_pos_y2 = pt_select0->box_pos_y1 - (screen_height / 2);
		
		
	}


	if(flag_press && presR)
	{	pt_select0->box_typeID = 2;
		//printf("scrolledit::select_box() / pressed\n");
	}
	else
	{	
		
		//###############################################################
		// left mouse button relased
		//###############################################################
		if(!presR && flag_press)
		{	flag_press = 0;
			pt_select0->box_typeID = 1;
			printf("scrolledit::select_box() / relased\n");

			if(pt_select0->box_pos_x1 > pt_select0->box_pos_x2 )
			{	Sint32 x = pt_select0->box_pos_x1;
				pt_select0->box_pos_x1 = pt_select0->box_pos_x2;
				pt_select0->box_pos_x2  = x;
			}			
			if(pt_select0->box_pos_y1 > pt_select0->box_pos_y2)
			{	Sint32 y = pt_select0->box_pos_y1;
				pt_select0->box_pos_y1 = pt_select0->box_pos_y2;
				pt_select0->box_pos_y2 = y;
			}

			
			pt_select0->box_widthT =
				(pt_select0->box_pos_x2 - pt_select0->box_pos_x1) / tile_width;
			pt_select0->box_height =
				(pt_select0->box_pos_y2 - pt_select0->box_pos_y1) / tile_width;
			
			
			switch (displayMod)
			{	case 1:
					tile2brush();
					break;
				default:
					maps2brush();
					break;
			}
			
		}
	}

}


//------------------------------------------------------------------------------
// draw selection
//------------------------------------------------------------------------------
void scrolledit::drawingBox()
{
		char *pBuff;
		Sint32 tmpco = 0;
	
		if(pt_select0->box_pos_x2 == pt_select0->box_pos_x1 || 
				pt_select0->box_pos_y2 == pt_select0->box_pos_y1)
		{	pt_select0->box_typeID = 0;
		}
		if(!pt_select0->box_typeID) return;

		Sint32 x1 = pt_select0->box_pos_x1;
		Sint32 x2 = pt_select0->box_pos_x2;
		Sint32 y1 = pt_select0->box_pos_y1 - pt_select0->boxOffsetY;
		Sint32 y2 = pt_select0->box_pos_y2 - pt_select0->boxOffsetY;
		
		if(x1 > x2 )
		{	Sint32 x = x1;
			x1 = x2 ;
			x2  = x;
		}			
		if(y1 > y2)
		{	Sint32 y = y1;
			y1 = y2;
			y2 = y;
		}

		if(box_colour++ > 32)
			box_colour = 0;
		Sint32 color = box_colour;

		
		//char *ptBuf = display->buffer_pos(box_pos_x1, box_pos_y1);
		
		Sint32 width = x2 - x1;
		Sint32 heigh = y2 - y1;
		
		/*printf("scrolledit::drawingBox() : [%i, %i, %i, %i]\n", 
			x1, y1, x2, y2);*/

	
		// top
		if(y1 >= 0 && y1 < screen_height)
		{		
		pBuff = display->buffer_pos(x1, y1);
		tmpco = 0; 
		for(Sint32 i = 0; i < width; i++)
		{	unsigned char pixel = cyclingtab[color];
			pBuff[i] = pixel;
			if(++tmpco == 5)
			{	tmpco = 0;
				if(color++ > 32) color = 0;
			}
		}
		}
		
		// right
		Sint32 nextl = display->bufferNext();
		pBuff = display->buffer_pos(x2 - 1,  y1 + 1);
		for(Sint32 i = 1; i < heigh; i++)
		{	unsigned char pixel = cyclingtab[color];
			if(y1 + i >= 0 && y1 + i < screen_height)
				*pBuff = pixel;
			if(++tmpco == 5)
			{	tmpco = 0;
				if(color++ > 32) color = 0;
			}
			pBuff += nextl;
		}
		
		// bottom
		if(y2 >= 0 && y2 < screen_height)
		{
		pBuff = display->buffer_pos(x1, y2);
		for(Sint32 i = width - 1; i >= 0; i--)
		{	unsigned char pixel = cyclingtab[color];
			pBuff[i] = pixel;
			if(++tmpco == 5)
			{	tmpco = 0;
				if(color++ > 32) color = 0;
			}
		}
		}
		
		// left
		pBuff = display->buffer_pos(x1, y2 - 1);
		for(Sint32 i = 1; i < heigh; i++)
		{	unsigned char pixel = cyclingtab[color];
			if(y2 - i >= 0 && y2 - i < screen_height)
				*pBuff = pixel;
			if(++tmpco == 5)
			{	tmpco = 0;
				if(color++ > 32) color = 0;
			}
			pBuff -= nextl;
		}
}

//------------------------------------------------------------------------------
// allocate brush 
//------------------------------------------------------------------------------
void scrolledit::brushAlloc()
{
	if(pBrush_bob)
		delete pBrush_bob;
	pBrush_bob = new bitmap_data();
	pBrush_bob->create(pt_select0->box_widthT * tile_width,
		pt_select0->box_height * tile_width, 1);

	brushWidth = pt_select0->box_widthT;
	brushHeigh = pt_select0->box_height;
	
	Sint32 m1 = pBrush_bob->get_line_modulo(0);
	Sint32 m2 = (defilement->motifhaute * pBrush_bob->get_row_size()) - defilement->motiflarge;
	Sint32 m3 = ((defilement->motifhaute - 1) * pBrush_bob->get_row_size());

	Sint32 n1 = defilement->source_mod;
	char **mapPT = defilement->mapAddress;	// pointer of each map of the page maps
	Uint16 *carte = pBrushTile;	
	Sint32 *dt = (Sint32 *)pBrush_bob->get_pixel_data();

	if(resolution == 1)
	{	for(Sint32 y = 0; y < pt_select0->box_height; y++)
		{	for(Sint32 x = 0; x < pt_select0->box_widthT; x++)
			{	Sint32 i = *(carte++);
				Sint32 *s = (Sint32 *) mapPT[i];
				for(i = 0; i < defilement->motifhaute; i++)
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
	else
	{	for(Sint32 y = 0; y < pt_select0->box_height; y++)
		{	for(Sint32 x = 0; x < pt_select0->box_widthT; x++)
			{	Sint32 i = *(carte++);
				Sint32 *s = (Sint32 *) mapPT[i];
				for(i = 0; i < defilement->motifhaute; i++)
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
}

//------------------------------------------------------------------------------
// display brush 
//------------------------------------------------------------------------------
void scrolledit::brush_draw()
{
	if(!pBrush_bob) return;
	Sint32 pos_x = keyboard->get_mouse_x();
	Sint32 pos_y = keyboard->get_mouse_y();
	pos_x &= tile_mask1;
	pos_y &= tile_mask1;
	if(pos_x > screen_width -  pBrush_bob->get_width())
		pos_x = screen_width -  pBrush_bob->get_width();
	if(pos_y > screen_height -  pBrush_bob->get_height())
		pos_y = screen_height -  pBrush_bob->get_height();
	

	Sint32 scrlY = defilement->returnPosy();
	Sint32 presL = keyboard->is_left_button();
	if(presL  && !flagPress2)
	{	flagPress2 = 1;
		brush_posx = pos_x;
		brush_posy = pos_y;
	}

	if(presL && flagPress2)
	{	pos_x = brush_posx;
		pos_y = brush_posy;
	}
	else
	{	if(!presL  && flagPress2)
		{	flagPress2 = 0;
			printf("scrolledit::brush_draw() : brush_posx:%i / brush_posy:%i\n",
				brush_posx, brush_posy);
			
			
			Sint32 i = scrlY + brush_posy;


			printf("scrolledit::brush_draw() : scrlY:%i /  i :%i motifhaute:%i\n",
				scrlY, i, defilement->motifhaute);

			i = (i / defilement->motifhaute) + 4;
			i *= lastScroll::CARTELARGE;
			Uint16* brush = pBrushTile; 
			Uint16* table = defilement->carteFirst + i;
			Uint16* t_end = defilement->carteFirst + (lastScroll::CARTEHAUTE * lastScroll::CARTELARGE);
			table += (brush_posx / defilement->motiflarge);

			printf("scrolledit::brush_draw() : (table - carteFirst):%i /  i:%i\n",
				(table - defilement->carteFirst), i);
			
			
			for(i = 0; i < brushHeigh; i++)
			{	if(table > t_end)
					table -= (lastScroll::CARTEHAUTE * lastScroll::CARTELARGE);
				
				for(Sint32 j = 0; j < brushWidth; j++)
					table[j] = *(brush++);
				table += lastScroll::CARTELARGE;
			}
			//###################################################################
			// copy a height of the screen (for scrolling rotation)
			//###################################################################
			table = defilement->carteFirst;
			i = (lastScroll::CARTEHAUTE * lastScroll::CARTELARGE);
			Sint32 tsupp = ( display->get_height() / defilement->motifhaute) * 2;
			for(Sint32 j = 0; j < (tsupp * lastScroll::CARTELARGE); j++)
				defilement->carteFirst[i++] = table[j];
		}
	}

	pBrush_bob->copyBuffer(0, 0, pos_x , pos_y - (scrlY & tile_mask2), -1, -1);
	
	
	
}

Sint32 scrolledit::saveTheMap()
{
	Sint32 zsize = lastScroll::CARTEHAUTE * lastScroll::CARTELARGE;
	
	Sint32 msize = zsize * sizeof(Uint16);
	Uint16 *carte = (Uint16 *) memory->alloc(msize, 0x54425249);
	error_init(memory->retour_err());
	if(erreur_num) return (erreur_num);
	
	//unsigned char* ptSrc = (unsigned char *)defilement->carteFirst;
	Uint16* ptSrc = (Uint16*)defilement->carteFirst;
	unsigned char* ptDes = (unsigned char *)carte;
	for(Sint32 i = 0; i < zsize; i++)
	{	
		Uint16 codem = *ptSrc;
		codem = codem << 2;
		ptDes[1]  = codem;
		codem = codem >> 8;
		ptDes[0]  = codem;
		ptSrc++;
		ptDes += 2;
	}


	char* fnamescore = "edmap.data";

	umask(0002);
	Sint32 fhand = open( fnamescore, O_WRONLY | O_CREAT, 00666);
	if(fhand == -1)
	{	fprintf(stderr, "scrolledit::saveTheMap(): file:%s / error:%s\n", 
					fnamescore, strerror(errno));
		memory->release((char *)ptDes);
		return 0;
	}
	write(fhand, carte, msize);
	if(close(fhand) == -1)
	{	fprintf(stderr, "scrolledit::saveTheMap(): file:%s / error:%s\n", 
		fnamescore, strerror(errno));
		memory->release((char *)ptDes);
		return 0;
	}
	memory->release((char *)carte);
	printf("scrolledit::saveTheMap() : %s file was saved\n", fnamescore);
	return 	erreur_num;
}

const unsigned char scrolledit::cyclingtab[] =
{	239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
	253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
	243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
	249, 250, 251, 252, 253, 254, 255
} ;
