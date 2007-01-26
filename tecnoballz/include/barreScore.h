//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "barreScore.h"
// created		: ?
// updates		: 2004-10-10
// fonction	: manage right score panel (bricks levels only)
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
//******************************************************************************
#ifndef __BARRESCORE__
#define __BARRESCORE__
class barreScore;
//-------------------------------------------------------------------------------
#include "../include/bitmap_data.h"
#include "../include/print_text.h"
#include "../include/joueurData.h"
#include "../include/handler_keyboard.h"
#include "../include/zeGigaBlit.h"
#include "../include/controller_balls.h"
//-------------------------------------------------------------------------------
class barreScore:public print_text
{
	private:
		static const Sint32		TEMPOBLITZ = 10;
		static const Sint32		TEMOINHAUT = 27;
		static const Sint32 	TEMOINPOSX = 526/2;
		static const Sint32 	TEMOINPOSY = 420/2;
		static const Sint32 	TEMOINLARG = 10/2;
		static const Sint32 	POSX_AREAN = 528/2;
		static const Sint32 	POSY_AREAN = 306/2;
		static const Sint32 	POSX_LEVEL = 592/2;
		static const Sint32 	POSY_LEVEL = 306/2;
		static const Sint32 	POSX_NOMJO = 528/2;
		static const Sint32 	POSY_NOMJO =  78/2;
		
		static const Sint32 	POSX_BESTN = 528/2;
		static const Sint32 	POSY_BESTN = 344/2;
		
		static const Sint32 	POSX_BESTS = 528/2;
		static const Sint32 	POSY_BESTS = 382/2;
		
		static const Sint32 	POSX_SCORE = 528/2;
		static const Sint32 	POSY_SCORE = 116/2;
		static const Sint32 	POSX_LIFES = 528/2;
		static const Sint32 	POSY_LIFES = 152/2;
		static const Sint32 	POSX_BRICK = 592/2;
		static const Sint32 	POSY_BRICK = 152/2;
		Sint32					superBrick;		// number of bricks which remain
		Sint32					flip_white;
		bitmap_data*				GFX_Sbarre;		// score panel image bitmap 
		joueurData*				objetGamer;
		zeGigaBlit*				ptGigaBlit;
		controller_balls*				ptNewBalls;
		char*					scoreAdres;		// buffer address score
		char*					lifesAdres;		// buffer address lifes
		char*					brickAdres;		// buffer address number bricks
		char*					temoinAdrs;		// buffer address indicator blitz
		Sint32					blitzcount;		// high of indicator blitz
		Sint32					blitztempo;
		Sint32					temoinhaut;
		static unsigned char	temoinCol1[TEMOINHAUT];
		static unsigned char	temoinCol2[TEMOINHAUT * 2];
		
    
	public:
								barreScore();
								~barreScore();
		Sint32					first_init(joueurData *gamer,  
										zeGigaBlit *gblit, controller_balls *balls);
		Sint32					affiche_me();
		void					scoreEcran();
		void					scoreAjout(Sint32 ajout);
		void					lifes_plus(Sint32 ajout);
		Sint32					lifesMoins(Sint32 retra);
		void					brickMoins(Sint32 retra);
		Sint32					resteBrick();
		void					scoreBrick(Sint32 value);
		void					barreTemoin();
		void					resetemoin();
};
#endif
