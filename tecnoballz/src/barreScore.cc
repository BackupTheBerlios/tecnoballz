//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "barreScore.cc"
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
#include "../include/barreScore.h"
#include "../include/handler_resources.h"
#include "../include/scoretable.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
barreScore::barreScore()
{ 
	GFX_Sbarre = (bitmap_data*)NULL;
	initial_me();
	temoinhaut = TEMOINHAUT * resolution;
	blitzcount = temoinhaut;
	blitztempo = TEMPOBLITZ;
	flip_white = 0;
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
barreScore::~barreScore()
{ 
	if(GFX_Sbarre)
	{	delete GFX_Sbarre;
		GFX_Sbarre = (bitmap_data*)NULL;
	}
	destroy_me();
}

//-------------------------------------------------------------------------------
// first initialization
//-------------------------------------------------------------------------------
Sint32 barreScore::first_init(joueurData * gamer, zeGigaBlit *gblit, controller_balls *balls)
{
	error_init(initialise());
	objetGamer = gamer;
	ptGigaBlit = gblit;
	ptNewBalls = balls;
	if(erreur_num)
		return (erreur_num);
	scoreAdres = display->buffer_pos
		(POSX_SCORE * resolution, POSY_SCORE * resolution);
	lifesAdres = display->buffer_pos
		(POSX_LIFES * resolution, POSY_LIFES * resolution);
	brickAdres = display->buffer_pos
		(POSX_BRICK * resolution, POSY_BRICK * resolution);
	temoinAdrs = display->buffer_pos
		(TEMOINPOSX * resolution, TEMOINPOSY * resolution);
	return (erreur_num);
}
//-------------------------------------------------------------------------------
// initialization before a level
//-------------------------------------------------------------------------------
Sint32 barreScore::affiche_me()
{
	GFX_Sbarre = new bitmap_data();
	GFX_Sbarre->load(handler_resources::RESBASCORE);
	GFX_Sbarre->copyTampon(0, 0, 256 * resolution, 0, 64 * resolution, 240 * resolution);
	joueurData *gamer = objetGamer;
	tamponAff1(POSX_AREAN * resolution, POSY_AREAN * resolution,
		gamer->areaNumber, 10);
	tamponAff1(POSX_LEVEL * resolution, POSY_LEVEL * resolution,
		gamer->levelTecno, 10);
	tamponAff2(POSX_NOMJO * resolution, POSY_NOMJO * resolution,
		gamer->nameString, 6);
	/*
	tamponAff1(POSX_BESTS * resolution, POSY_BESTS * resolution,
		gamer->best_score, 100000);
	tamponAff2(POSX_BESTN * resolution, POSY_BESTN * resolution,
		gamer->bestPlayer, 6);
	*/
	tamponAff1(POSX_BESTS * resolution, POSY_BESTS * resolution,
		ptScoreTab->best_score(), 100000);
	tamponAff2(POSX_BESTN * resolution, POSY_BESTN * resolution,
		ptScoreTab->bestPlayer(), 6);
	
	barreTemoin();
	delete GFX_Sbarre;
	GFX_Sbarre = (bitmap_data*)NULL;
	return erreur_num;
}

//-------------------------------------------------------------------------------
// display sore, number of lifes, number of bricks
//-------------------------------------------------------------------------------
void barreScore::scoreEcran()
{
	joueurData *gamer = objetGamer;
	affNombre1(scoreAdres, gamer->superScore, 100000);
	affNombre1(lifesAdres, superBrick, 100);
	affNombre1(brickAdres, gamer->superLifes, 10);
	//Sint32 nbfps = display->get_framepee();
	//affNombre1(lifesAdres, nbfps, 100);
}

//-------------------------------------------------------------------------------
// increase the score
//-------------------------------------------------------------------------------
void barreScore::scoreAjout(Sint32 ajout)
{	objetGamer->add_scores(ajout);
}

//-------------------------------------------------------------------------------
// increase the number of lifes
//-------------------------------------------------------------------------------
void barreScore::lifes_plus(Sint32 ajout)
{
	objetGamer->superLifes += ajout;
}

//-------------------------------------------------------------------------------
// decrease the number of lifes
//-------------------------------------------------------------------------------
Sint32 barreScore::lifesMoins(Sint32 retra)
{
	joueurData *gamer = objetGamer;
	gamer->superLifes -= retra;
	if(gamer->superLifes > 0)
		return 1;
	else
	{	gamer->superLifes = 0;
		return 0;
	}
}

//-------------------------------------------------------------------------------
// decrease the number of bricks
//-------------------------------------------------------------------------------
void barreScore::brickMoins(Sint32 retra)
{
	superBrick -= retra;
	if(superBrick < 1)
		superBrick = 0;
}

//-------------------------------------------------------------------------------
// return the number of bricks
//-------------------------------------------------------------------------------
Sint32 barreScore::resteBrick()
{ 
	return (superBrick);
}

//-------------------------------------------------------------------------------
// intialize the number of bricks
//-------------------------------------------------------------------------------
void barreScore::scoreBrick(Sint32 value)
{
	superBrick = value;
}

//-------------------------------------------------------------------------------
// display indicator bar of gigablitz
//-------------------------------------------------------------------------------
void barreScore::barreTemoin()
{
	if(keyboard->is_right_left_buttons() && !ptGigaBlit->isactivate() && 
		!ptNewBalls->least_glue())
	{	if(--blitztempo <= 0)
		{	if(blitzcount > 0)
			{	blitztempo = TEMPOBLITZ;
				blitzcount = blitzcount - resolution;
				if(blitzcount == 0) 			// indicator is highest?
					ptGigaBlit->initDepart();	// yes, launch the gigablitz!
			}
		}
	}
	else
	{	if(blitzcount < (temoinhaut))
		{	blitztempo = TEMPOBLITZ;
			blitzcount = blitzcount + resolution;
		}
	}
    
	char *d = temoinAdrs;
	Sint32 m = display->bufferLarg();
	char p = 0;
	
	
	if(resolution == 1)
	{
		for(Sint32 i = 0; i < blitzcount; i++)
		{	d[0] = p;
			d[1] = p;
			d[2] = p;
			d[3] = p;
			d[4] = p;
			d += m;
		}
	}
	else
	{	for(Sint32 i = 0; i < blitzcount; i++)
		{	d[0] = p;
			d[1] = p;
			d[2] = p;
			d[3] = p;
			d[4] = p;
			d[5] = p;
			d[6] = p;
			d[7] = p;
			d[8] = p;
			d[9] = p;
			d += m;
		}
	}


	
	Sint32 h = (temoinhaut) - blitzcount;

	flip_white =~ flip_white;
	if (flip_white)
	{	char p = 130;
	
		if(resolution == 1)
		{	for(Sint32 i = 0; i < h; i++)
			{	d[0] = p;
				d[1] = p;
				d[2] = p;
				d[3] = p;
				d[4] = p;
				d += m;
			}
		}
		else 
		{	for(Sint32 i = 0; i < h; i++)
			{	d[0] = p;
				d[1] = p;
				d[2] = p;
				d[3] = p;
				d[4] = p;
				d[5] = p;
				d[6] = p;
				d[7] = p;
				d[8] = p;
				d[9] = p;
				d += m;
			}
		}
		
		
	}
	else
	{
		if(resolution == 1)
		{	unsigned char *e = temoinCol1;
			for(Sint32 i = 0; i < h; i++)
			{	char p = (char)e[i];
				d[0] = p;
				d[1] = p;
				d[2] = p;
				d[3] = p;
				d[4] = p;
				d += m;
			}
		}
		else 
		{	unsigned char *e = temoinCol2;
			for(Sint32 i = 0; i < h; i++)
			{	char p = (char)e[i];
				d[0] = p;
				d[1] = p;
				d[2] = p;
				d[3] = p;
				d[4] = p;
				d[5] = p;
				d[6] = p;
				d[7] = p;
				d[8] = p;
				d[9] = p;
				d += m;
			}
		}
	}
}

//-------------------------------------------------------------------------------
// RAZ indicator bar of gigablitz
//-------------------------------------------------------------------------------
void barreScore::resetemoin()
{
	blitzcount = temoinhaut;
}
//-------------------------------------------------------------------------------
// list of 54 couleurs from gigablitz
//-------------------------------------------------------------------------------
unsigned char barreScore::temoinCol1[TEMOINHAUT] =
{	255,
 	255,
 	254,
 	254,
 	253,
	253,
	252,
	252,
	251,
	250,
	250,
	249,
	248,
	248,
	247,
	246,
	246,
	245,
	244,
	244,
	243,
	242,
	242,
	241,
	240,
	240,
	239
};

unsigned char barreScore::temoinCol2[TEMOINHAUT * 2] =
{	255,
 	255,
 	255,
 	255,
 	254,
 	254,
 	254,
 	254,
 	253,
 	253,
	253,
	253,
	252,
	252,
	252,
	251,
	251,
	251,
	250,
	250,
	250,
	249,
	249,
	249,
	248,
	248,
	248,
	247,
	247,
	247,
	246,
	246,
	246,
	245,
	245,
	245,
	244,
	244,
	244,
	243,
	243,
	243,
	242,
	242,
	242,
	241,
	241,
	241,
	240,
	240,
	240,
	239,
	239,
	239
};
