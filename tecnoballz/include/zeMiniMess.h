//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeMiniMess.h"
// created		: 2002-12-30
// updates		: 2004-05-03
// fonctions	: display small messages into bricks levels
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
//.............................................................................
#ifndef __ZEMINIMESS__
#define __ZEMINIMESS__
class zeMiniMess;
#include "../include/handler_display.h"
#include "../include/mentatCode.h"
#include "../include/GIF_bitMap.h"
//.............................................................................
class zeMiniMess:public virtual mentatCode
{
	private:
		GIF_bitMap*			minifontes;
		GIF_bitMap*			messa_fond;
		static const Sint32	MESSAGENUMB = 35;
		static const Sint32	MESSAGEPOSX = 96;
		static const Sint32	MESSAGEPOSY = 37;
		static const Sint32	MESSAGE_MAX = 16;	//number of chars of a message

		Sint32				mess_pause;	//tempo counter if<5 then clear message
		Sint32				mess_pnter;	//number of the chars to display 0 to 15
		const char*			mess_reque;
		Sint32				off_desti1;	// modulo destination (buffer)
		Sint32				off_source;	// modulo source (graphic page)
		char*				ptr_fontes;
		char*				ptr_buffer;
		char*				ptr_tampon;
		char*				pt_mesfond;
		Sint32				ft_hauteur;	//height of a character 
		Sint32				ft_largeur;	//width of a character 
		Sint32				fonteslarg;	//width of message's buffer  
		static const char*	zemessage0[MESSAGENUMB];
		static char			zemessages[MESSAGENUMB];

	private:
		void				execution2();
		void				displaymes();
		void				clear_mess();

	public:
							zeMiniMess();
							~zeMiniMess();
		void				erase_mess();
		Sint32				intialise1();
		void				mesrequest(Sint32 mesnb);
		void				execution1();
};
#endif
