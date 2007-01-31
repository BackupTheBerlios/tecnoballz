//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeMiniMess.cc"
// created		: 2002-12-30
// updates		: 2004-05-13
// fonctions	: display small messages into bricks levels
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
#include "../include/zeMiniMess.h"
#include "../include/handler_resources.h"

//------------------------------------------------------------------------------
// create the object
//------------------------------------------------------------------------------
zeMiniMess::zeMiniMess()
{
	minifontes = (bitmap_data *)NULL;
	messa_fond = (bitmap_data *)NULL;
	mess_pause = 0;
	mentatInit();
}

//------------------------------------------------------------------------------
// release the object
//------------------------------------------------------------------------------
zeMiniMess::~zeMiniMess()
{
	if(minifontes)
	{	delete minifontes;
		minifontes = (bitmap_data *)NULL;
	}
	if(messa_fond)
	{	delete messa_fond;
		messa_fond = (bitmap_data *)NULL;
	}
	mentatKill();
}

//-----------------------------------------------------------------------------
// perform some initializations
//-----------------------------------------------------------------------------
Sint32 zeMiniMess::intialise1()
{
	if(!minifontes)
	{	minifontes = new bitmap_data();
		minifontes->load(handler_resources::RESFONTMES);

		off_desti1 = game_screen->get_row_size();	// modulo destination
		off_source = minifontes->get_row_size();	// modulo source
		ptr_buffer = game_screen->get_pixel_data(MESSAGEPOSX * resolution,
												MESSAGEPOSY * resolution);
		ptr_tampon = background_screen->get_pixel_data(MESSAGEPOSX * resolution,
												MESSAGEPOSY * resolution);
		ptr_fontes = minifontes->get_pixel_data(); 
		ft_hauteur = 6 * resolution;	// 6 or 12
		ft_largeur = 4 * resolution;	// 4 or 8
		fonteslarg = ft_largeur * MESSAGE_MAX;	// 16*4=64 or 16*8=128 
	}

	if(!messa_fond)
	{	messa_fond = new bitmap_data();
		messa_fond->create(fonteslarg, ft_hauteur, 1);
		pt_mesfond = messa_fond->get_pixel_data();
	}

	//###################################################################
	// save the background
	//###################################################################
	char *pfond = pt_mesfond;
	char *ptamp = ptr_tampon;
	Sint32 zemod = display->tamponNext();
	for(Sint32 y = 0; y < ft_hauteur; y++)
	{	for(Sint32 x = 0; x < fonteslarg; x++)
			pfond[x] = ptamp[x];
		pfond = pfond + fonteslarg;
		ptamp = ptamp + zemod;
	}
 
	//###################################################################
	// efface la table de demande des messages
	//###################################################################
	erase_mess();
	return 0;
}

//------------------------------------------------------------------------------
// erase all messages
//------------------------------------------------------------------------------
void zeMiniMess::erase_mess()
{
	for(Sint32 i = 0; i<MESSAGENUMB; i++)
		zemessages[i] = 0;
}

//------------------------------------------------------------------------------
// request for display a message
// mesnb : number of the message 0 to 34
//------------------------------------------------------------------------------
void zeMiniMess::mesrequest(Sint32 mesnb)
{
	zemessages[mesnb] = 1;
}

//------------------------------------------------------------------------------
// runtime
//------------------------------------------------------------------------------
void  zeMiniMess::execution1()
{
	if(mess_pause > 0)
		execution2();
	else
	{	Sint32 i = MESSAGENUMB;
		while (--i >=0 )
		{	if(zemessages[i] > 0)
			{	zemessages[i] = 0;
				mess_pnter = 0;
				mess_pause = 66;
				mess_reque = zemessage0[i];
				execution2();
				return;
			}     
		}
	}
}

//------------------------------------------------------------------------------
// display or clear the message
//------------------------------------------------------------------------------
void  zeMiniMess::execution2()
{
	if(mess_pnter < 16)
		displaymes();	//display the message
	else
	{	mess_pause--;
		if(mess_pause < ft_hauteur)
			clear_mess();	//clear the message
	}
}

//------------------------------------------------------------------------------
// display the message character by character 
//------------------------------------------------------------------------------
void zeMiniMess::displaymes()
{
	const char *ptmes = mess_reque;
	ptmes = ptmes + mess_pnter;
	Sint32 chara = *ptmes - 'A';
	char  *ptfnt = ptr_fontes + chara * 16 * resolution;
	Sint32 offSc = off_source;
	Sint32 offDs = off_desti1;
	char *ptbuf = ptr_buffer + (mess_pnter * ft_largeur);
	char *ptamp = ptr_tampon + (mess_pnter * ft_largeur);
	for(Sint32 y = 0; y < ft_hauteur; y++)
	{	for(Sint32 x = 0; x < ft_largeur; x++)
		{	char pixel =  ptfnt[x];
			if(pixel > 0)
			{	ptbuf[x] = pixel;
				ptamp[x] = pixel;
			}
		}
		ptfnt = ptfnt + offSc;
		ptbuf = ptbuf + offDs;
		ptamp = ptamp + offDs;
	}
	mess_pnter++;
}

//------------------------------------------------------------------------------
// clear the message line by line
//------------------------------------------------------------------------------
void zeMiniMess::clear_mess()
{
	Sint32 pos_x = MESSAGEPOSX * resolution;
	Sint32 pos_y = (MESSAGEPOSY * resolution) + mess_pause;
	char *pbuff = game_screen->get_pixel_data(pos_x, pos_y);
	char *ptamp = background_screen->get_pixel_data(pos_x, pos_y);
	char *pfond = pt_mesfond + (mess_pause * fonteslarg);
	for(Sint32 x = 0; x < fonteslarg; x++)
	{	char pixel = pfond[x];
		pbuff[x] = pixel;
		ptamp[x] = pixel;
	}
}

//------------------------------------------------------------------------------
// Messages  displayed on the screen
// A to Z = A to Z
// [ = space
// \ = exclamation mark 
// ] = point d'interrogation
// ^ = dot
//------------------------------------------------------------------------------
const char* zeMiniMess::zemessage0[MESSAGENUMB] =
{	"[[[[[[[[[[[[[[[[",	//0
	"[ARE[YOU[READY[]",	//1
	"[YEAH[[YOU[WERE\\",	//2
	"[[[[[[[[[[[[[[[[",	//3
	"[[[SIMPLE[FIRE[[",	//4
	"[[[COOL[FIRE[[\\[",	//5
	"[[COOL[GUY[[\\[[",	//6
	"[[[[[[[[[[[[[[[[",	//7
	"[[[[[[[[[[[[[[[[",	//8
	"[[[[[[[[[[[[[[[[",	//9
	"[YOU[LOST[A[LIFE",	//10
	"[YOU[WON[A[LIFE\\",	//11
	"[[[[[[[[[[[[[[[[",	//12
	"[[[[[[[[[[[[[[[[",	//13
	"[[[[TOO[LATE^^^[",	//14
	"[BONUS[PRICE[[^[",	//15
	"[[GLUE[[OPTION[[",	//16
	"[[NEXT[LEVEL^^^[",	//17
	"[[[MALUS[SIZE[[[",	//18
	"[[[BONUS[SIZE[[[",	//19
	"[[BONUS[BALLS[[[",	//20
	"[[[MULTIBALLS[[[",	//21
	"[[[POWERBALLS[[[",	//22
	"[MEGAPOWERBALLS[",	//23
	"[INVERSOR[MALUS[",	//24
	"[SPEEDBALLS[MAXI",	//25
	"[[BUMPER[RIGHT[[",	//26
	"[[[BUMPER[[UP[[[",	//27
	"[[BUMPER[[LEFT[[",	//28
	"[BIGSIZE[[BALLS[",	//29
	"[MAXISIZE[BALLS[",	//30
	"[[MEGA[OPTIONS[[",	//31
	"[[WALL[[ENABLE[[",	//32
	"[[ROBOT[ENABLE[[",	//33
	"[CONTROL[[BALLS[",	//34
};
char zeMiniMess::zemessages[MESSAGENUMB];
