//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "scoretable.cc"
// created	: 2004-04-30
// updates	: 2005-01-11
// fonction	: display score table (game over and menu)
// id		: $Id: scoretable.cc,v 1.13 2007/02/28 21:08:09 gurumeditation Exp $
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
#include "../include/scoretable.h"
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------	
scoretable::scoretable()
{
	the_scores = (score_list**)NULL;
	buffersize = (sizeof(Uint32) * 3 + 8) * NUMBSCORES * NDIFFICULT +
			sizeof(Uint32);
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
scoretable::~scoretable()
{
	if(!the_scores) return;
	for(Uint32 i = 0; i < NDIFFICULT; i++)
	{	if (the_scores[i])
		{	memory->release((char *)the_scores[i]);
			the_scores[i] = NULL;
		}
	}
  printf("RELEASE scoretable\n");
	memory->release((char *)the_scores);
	the_scores = NULL;
}

//-----------------------------------------------------------------------------
//	perform some initializations
//-----------------------------------------------------------------------------
Sint32 scoretable::first_init()
{
	// allocate memory for the 4 pointers to the 4 scores tables
	the_scores = (score_list**) memory->alloc(sizeof(score_list *) * NDIFFICULT);
	error_init(memory->retour_err());
	if(erreur_num) return erreur_num;
	
	//fill score table
	for(Uint32 i = 0; i < NDIFFICULT; i++)
	{	the_scores[i] =
			(score_list*) memory->alloc(sizeof(score_list) * NUMBSCORES);
		error_init(memory->retour_err());
		if(erreur_num) return erreur_num;
		
		score_list* score = the_scores[i];
		for(Uint32 j = 0; j < NUMBSCORES; j++)
		{	score[j].score_area = 1;
			score[j].scoreLevel = 1;
			score[j].scoreValue = 1000 - j * 100;
			score[j].playerName[0] = 'T';
			score[j].playerName[1] = ' ';
			score[j].playerName[2] = 'L';
			score[j].playerName[3] = ' ';
			score[j].playerName[4] = 'K';
			score[j].playerName[5] = ' ';
			score[j].playerName[6] = 0;
			score[j].playerName[7] = 0;	//SPARC unaligned memory access
		}
	}

	//save scores tables
	if (!loadScores()) saveScores();
	return erreur_num;
}

//------------------------------------------------------------------------------
// load score
//------------------------------------------------------------------------------
Sint32 scoretable::loadScores()
{
	char *pData = resources->load_high_score_file();
	if(pData)
	{	Uint32 fsize = resources->get_filesize_loaded();
		if(fsize != buffersize)
		{	fprintf(stderr, "scoretable::loadScores(): bad file size, %i byte(s) instead %i bytes\n",
				fsize, 	buffersize);
      delete[]pData;
			return 0;
		}
		Uint32 ckVal;
		bigendianr((Uint32 *)pData, &ckVal);	
		Uint32 *pSelf = (Uint32 *)pData;
		Uint32 value = controlVal(pSelf + 1,
			(buffersize - sizeof(Uint32)) / sizeof(Uint32));
		//if(value != *pSelf)
		if(value != ckVal)
		{	fprintf(stderr, "scoretable::loadScores(): bad checksum, %x instead %x\n",
				value, *pSelf);
      delete[]pData;
			return 0;
		}

		// copy data into the structures
		char *pTemp = pData + sizeof(Uint32); 
		for(Uint32 i = 0; i < NDIFFICULT; i++)
		{	score_list* score = the_scores[i];
			for(Uint32 j = 0; j < NUMBSCORES; j++)
			{	Uint32 *ptM32 = (Uint32 *)pTemp;
				bigendianr(ptM32++, &score[j].score_area);
				bigendianr(ptM32++, &score[j].scoreLevel);
				bigendianr(ptM32++, &score[j].scoreValue);
				/*	
				score[j].score_area = *(ptM32++);
				score[j].scoreLevel = *(ptM32++);
				score[j].scoreValue = *(ptM32++);
				*/
				pTemp = (char *)ptM32;
				for(Uint32 k = 0; k < 8; k++)
					score[j].playerName[k] = *(pTemp++);
			}			
		}
    delete[]pData;
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
// save score
//------------------------------------------------------------------------------
Sint32 scoretable::saveScores()
{
	char *pData = memory->alloc(buffersize, 0x53434F52);
	error_init(memory->retour_err());
	if(erreur_num) return erreur_num;
	Uint32 *pSelf = (Uint32 *)pData;

	char *pTemp = pData + sizeof(Uint32); 
	for(Uint32 i = 0; i < NDIFFICULT; i++)
	{	score_list* score = the_scores[i];
		for(Uint32 j = 0; j < NUMBSCORES; j++)
		{	Uint32 *ptM32 = (Uint32 *)pTemp;
			bigendianw(&score[j].score_area, ptM32++);
			bigendianw(&score[j].scoreLevel, ptM32++);
			bigendianw(&score[j].scoreValue, ptM32++);
			/*
			*(ptM32++) = score[j].score_area;
			*(ptM32++) = score[j].scoreLevel;
			*(ptM32++) = score[j].scoreValue;
			*/
			pTemp = (char *)ptM32;
			for(Uint32 k = 0; k < 8; k++)
				*(pTemp++) = score[j].playerName[k];
		}			
	}

	//calculate checksum value
	Uint32 ckVal = controlVal(pSelf + 1,
		(buffersize - sizeof(Uint32)) / sizeof(Uint32));
	bigendianw(&ckVal, pSelf);
	/*	
	*pSelf = controlVal(pSelf + 1,
		(buffersize - sizeof(Uint32)) / sizeof(Uint32));
	*/
	resources ->save_high_score_file(pData, buffersize);
  printf("D RELEASE scoretable\n");
	memory->release(pData);
	return 0;
}

//------------------------------------------------------------------------------
// calculate checksum value
//------------------------------------------------------------------------------
Uint32 scoretable::controlVal(Uint32 *pBuff, Uint32 bsize)
{
	Uint32 value = 0;
	for(Uint32 i = 0; i < bsize; i++)
	{	Uint32 lword; 
		bigendianr(pBuff++, &lword);
		value |= lword;
		//value |= *(pBuff++);
	}
	return value;
}

//------------------------------------------------------------------------------
// check if the current player can be inserted in the scores table
//------------------------------------------------------------------------------
Sint32 scoretable::test_score()
{
	Sint32 fhigh = test_score(
		&current_player->player_name[0],
		current_player->score_value,
		current_player->level_number,
		current_player->area_number);
	if(fhigh)
	{	sort_score(difficulty_level);
		saveScores();
	}
	return fhigh;
}

//------------------------------------------------------------------------------
// insert a player in the score table if this player get a good score
//	input	=> pName: name of player
//			=> vScre: score
//			=> nLevl: level number
//			=> nArea: area number
//	output	<=	1: the player is in the scores table
//------------------------------------------------------------------------------
Sint32 scoretable::test_score(char *pName, Uint32 vScre, Uint32 nLevl, Uint32 nArea)
{
	//###################################################################
	// exit if cheat!
	//###################################################################
	if(cheat_flag || birth_flag) return 0;
	
	//###################################################################
	// verify if the name exists
	//###################################################################
	score_list* score = the_scores[difficulty_level - 1];
	for(Uint32 i = 0; i < NUMBSCORES; i++)
	{	if(chaine_cmp(pName, score[i].playerName, 6))
		{	if(vScre > score[i].scoreValue)
			{	score[i].scoreValue = vScre;
				score[i].scoreLevel = nLevl;
				score[i].score_area = nArea;
				return 1;
			}
			else
				return 0;
		}
	}
	
	for(Uint32 i = 0; i < NUMBSCORES; i++)
	{	if(vScre > score[i].scoreValue)
		{	i = NUMBSCORES - 1;
			score[i].scoreValue = vScre;
			score[i].scoreLevel = nLevl;
			score[i].score_area = nArea;
			char* p = score[i].playerName;
			for(Sint32 j = 0; j < 6; j++)
				p[j] = pName[j];
			if(is_verbose)
				printf ("scoretable::test_score() i=%i name: %s\n",
				i, score[i].playerName);
			return 1;
		}
	}
	return 0;	
}

//------------------------------------------------------------------------------
// sort scores table
//------------------------------------------------------------------------------
void scoretable::sort_score(Uint32 nHard)
{
	if (is_verbose)
		printf("scoretable::sort_score %i\n", nHard);
	Uint32 fExit;
	score_list* score = the_scores[nHard - 1];
	do
	{	fExit = 0;
		for (Uint32 i = 0; i < (NUMBSCORES - 1); i++)
		{	if(score[i].scoreValue < score[i + 1].scoreValue)
			{	Uint32 tempo = score[i].scoreValue;
				score[i].scoreValue = score[i + 1].scoreValue;
				score[i + 1].scoreValue = tempo;
				
				tempo = score[i].scoreLevel;
				score[i].scoreLevel = score[i + 1].scoreLevel;
				score[i + 1].scoreLevel = tempo;
				
				tempo = score[i].score_area;
				score[i].score_area = score[i + 1].score_area;
				score[i + 1].score_area = tempo;

				for(Uint32 j = 0; j < 6; j++)
				{	tempo = score[i].playerName[j];
					score[i].playerName[j] = score[i + 1].playerName[j];
					score[i + 1].playerName[j] = tempo;
				}
				fExit = 1;
			}
		}
	} while (fExit);
}

//------------------------------------------------------------------------------
// return pointer to the scores list structure
//------------------------------------------------------------------------------
score_list* scoretable::getScrList()
{
	return the_scores[difficulty_level - 1];
}

//------------------------------------------------------------------------------
// return the player name
//------------------------------------------------------------------------------
char* scoretable::bestPlayer()
{
	score_list* score = the_scores[difficulty_level - 1];
	return &score[0].playerName[0];
}

//------------------------------------------------------------------------------
// return the best player score
//------------------------------------------------------------------------------
Uint32 scoretable::best_score()
{
	score_list* score = the_scores[difficulty_level - 1];
	return score[0].scoreValue; 	
}
