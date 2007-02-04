//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "scoretable.cx"
// created	: 2003-04-30
// updates	: 2005-01-05
// fonction	: display score table (game over)
// id		: $Id: scoretable.h,v 1.3 2007/02/04 17:10:16 gurumeditation Exp $
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
#ifndef __SCORETABLE__
#define __SCORETABLE__
//...............................................................................
class scoretable;
//...............................................................................
#include "../include/tecnoballz.h"
//...............................................................................
typedef struct
{
	char	playerName[8];	//name of player
	Uint32	scoreValue; 
	Uint32	score_area; 
	Uint32	scoreLevel; 
} score_list;
//...............................................................................
class scoretable:public virtual tecnoballz
{
	public:
		static const Uint32	NUMBSCORES = 10;
	
	private:
		score_list**		the_scores;
		
	private:
		Uint32			buffersize;
		Sint32			test_score(char *pName, Uint32 vScre,
						Uint32 nLevl, Uint32 nArea);
    	void				sort_score(Uint32 nHard);
		Sint32			loadScores();
		Sint32			saveScores();
		Uint32			controlVal(Uint32 *pBuff, Uint32 bsize);
	public:
					scoretable();
					~scoretable();
		Sint32			first_init();
		Sint32			test_score();
		score_list*		getScrList();
		char*			bestPlayer();
		Uint32			best_score();
};
#endif
