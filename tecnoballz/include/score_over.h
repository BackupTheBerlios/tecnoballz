//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "score_over.h"
// created		: 2003-04-30
// updates		: 2003-05-23
// fonction	: display score table (game over)
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
#ifndef __SCORE_OVER__
#define __SCORE_OVER__
//...............................................................................
class score_over;
//...............................................................................
#include "../include/print_text.h"
#include "../include/BOB_killer.h"
#include "../include/GIF_bitMap.h"
//...............................................................................

class score_over:public virtual print_text, public virtual BOB_killer
{

	private:
		static const Sint32	TEXT_LARGE = 23;	//number of characters by lines
		static const Sint32	TEXT_HAUTE = 12;	//number of lines of characters
		static char			scorestext[];		//list of names and score
	
		Sint32				width_font;			//fonte's width font 8 or 16
		Sint32				heightfont;			//fonte's height
		Sint32				space2next;			//number of lines to next fonte 9 or 18
		GIF_bitMap*			graphTexte;
		Sint32				run_offset;
		Sint32				max_offset;
	
	public:
						score_over();
						~score_over();
		Sint32				first_init(Sint32 offz = 0);
		void 				copyToText();
		Sint32				displayTxt();
};
#endif
