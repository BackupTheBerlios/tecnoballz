//******************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeNewBalls.cc"
// created	: ?
// updates	: 2005-08-26
// fonction	: manage the balls (move and collisions)
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
// Place - Suite 330, Boston, MA 021117-1307, USA.
//******************************************************************************
#include "../include/zeNewBalls.h"
#include "../include/joueurData.h"
#include "../include/handler_memory.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"
#include "../include/audiomixer.h"
#include "../include/list_sprites.h"

//-----------------------------------------------------------------------------
// bricks levels: create the object
//-----------------------------------------------------------------------------
zeNewBalls::zeNewBalls(ejectBalls *eject, lesBriques *brick, briqueCote *brico, 
	head_anima *gugus, zeBouiBoui *atoms, barreScore *score, sprite_object *pwall,
	zeMiniMess* pMess, ze_magneye* pEyes)
{
	littleInit();
	num_erreur = 0;
	
	ejectObjet = eject;
	brickObjet = brick;
	bricoObjet = brico;
	gugusObjet = gugus;
	ptBouiBoui = atoms;
	ptBarreScr = score;
	ptBob_wall = pwall;
	ptMiniMess = pMess;
	pt_magneye = pEyes;
	ptCapsules = NULL;
	ptguardian = NULL;
	pt_gadgets = NULL;
	
	balle_glue = 4 * 60;
	startCount = 60;
	balle_glue = 60;
	tempoVites = 60;
	balle_tilt = 60;
	balleVites = technoBall::donneSpeed(1);
	objetTotal = 20;
	objects_have_shades = 1;
	balle_ctrl = 0;
	BOBtypeNum = BOB_BALLES;
}

//-----------------------------------------------------------------------------
// guards levels: create the object
//-----------------------------------------------------------------------------
zeNewBalls::zeNewBalls(zeguardian *pGard, zeCapsules *pCaps, ze_gadgets *pGads)
{
	littleInit();
	num_erreur = 0;
	
	ejectObjet = NULL;
	brickObjet = NULL;
	bricoObjet = NULL;
	gugusObjet = NULL;
	ptBouiBoui = NULL;
	ptBarreScr = NULL;
	ptBob_wall = NULL;
	ptMiniMess = NULL;
	ptCapsules = pCaps;
	ptguardian = pGard;
	pt_gadgets = pGads;
	
	balle_glue = 4 * 60;
	startCount = 60;
	balle_glue = 60;
	tempoVites = 60;	
	balle_tilt = 60;
	balleVites = technoBall::donneSpeed(1);
	objetTotal = 20;
	objects_have_shades = true;
	balle_ctrl = 0;
	BOBtypeNum = BOB_BALLES;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeNewBalls::~zeNewBalls()
{
	littleDead();
}

//-------------------------------------------------------------------------------
// initialize the balls before one level
// input 	=>	raket	:
//				start	: time before the ball leaves the bumper (first time)
//				glueC	: time before the ball leaves (glue option)
//				speed	: time before the ball accelerates
//				tiltC	: time before "tilt" is available
//				table	: speed ball (1 to 4)
//-------------------------------------------------------------------------------
void zeNewBalls::init_balle(zeRaquette *raket, Sint32 start, Sint32 glueC, 
	Sint32 speed, Sint32 tiltC, Sint32 table)
{
	//start = start / 3;	//test only
	//glueC = glueC / 3;	//test only
	//speed = speed /10;	//test only
	//printf("zeNewBalls::init_balle() speed: %i\n", speed);
	
	startCount = start;
	balle_glue = glueC;
	tempoVites = speed;
	balle_tilt = tiltC;
	balleVites = technoBall::donneSpeed(table);
	tecBumper1 = raket->demandeRak(1);	// bottom bumper
	tecBumper2 = raket->demandeRak(2);	// right bumper
	tecBumper3 = raket->demandeRak(3);	// top bumper
	tecBumper4 = raket->demandeRak(4);	// left bumper
	tec_robot0 = raket->demandeRak(5);	// robot bumper

	// initialize the balls
	Sint32 bwgth;
	if(brickObjet)
		bwgth =  brickObjet->getBkWidth();
	else
		bwgth = 16 * resolution;
	//printf("bwght : %i\n", bwgth);	
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		balle->littleInit(start, speed, tecBumper1, balleVites, bwgth);
	}

	// first ball special initialization
	technoBall *balle = objetListe[0];
	tecBumper1->balleColle = balle;
	balle->startBalle(tecBumper1->colLargeur);
	objetNombr = 1; // one ball to screen
	if(ejectObjet)
		ejectObjet->ballPosIni(&technoBall::furaxTable[0]);
}

//-------------------------------------------------------------------------------
// bricks levels: balls moves and collisions
//-------------------------------------------------------------------------------
void zeNewBalls::vitusBalle()
{
	vitus_sort();	//test if balls go out of the screen
	vitus_tilt();
	vitusBrick();	//collisions balls and bricks
	vitus_move();	//move the balls
	vitus_bump();	//collisions balls and bumpers
	vitusrobot();
	vitus_cote();	//collisions balls and walls
	vitusEject();	//collisions balls and ejectors
	vitusAtoms();
	vitus_eyes();
	vitus_ctrl();	//control ball with the left mouse button
	time_2tilt();
	accelerate();
}

//-------------------------------------------------------------------------------
// guards levels: balls moves and collisions
//-------------------------------------------------------------------------------
void zeNewBalls::vitusBall2()
{
	vitussort2();	//test if balls go out of the screen
	vitus_tilt();
	vitusmove2();	//move the balls
	vituscote2();	//collisions balls and walls
	vitusbound();	//prevent that the ball remains blocked horizontally
	vitusbump2();	//collisions balls and the bumper
	vitusGuard();
	time2tilt2();
	accelerate();
}

//------------------------------------------------------------------------------
// test si les balles sortent de l'ecran de jeu 
// test if balls go out of the screen of game
//------------------------------------------------------------------------------
void zeNewBalls::vitus_sort()
{
	// pointer to the object "bumper of bottom"
	tecno_bump *raket = tecBumper1;
	Sint32 min_x = technoBall::MINIMUM_PX * resolution;
	Sint32 max_x = technoBall::MAXIMUM_PX * resolution;
	Sint32 min_y = technoBall::MINIMUM_PY * resolution;
	Sint32 max_y = technoBall::MAXIMUM_PY * resolution;
	
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		tecno_bump *rakPT;
		if(balle->flag_actif)
		{	rakPT = (tecno_bump*)NULL;
			Sint32 j = balle->position_x;
			if(j < min_x)
				rakPT = tecBumper4;
			else
			{	if(j > max_x)
					rakPT = tecBumper2;
				else
				{	j = balle->position_y;
					if(j < min_y)
						rakPT = tecBumper3;
					else
						if(j > max_y)
							rakPT = tecBumper1;
				}
			}
 
			if(rakPT)						//one ball is out ?
			{	if(!rakPT->flag_actif)		//bumper is actif ?
				rakPT = raket;				//no, bumper of bottom by default
				if((--objetNombr) > 0)
					balle->goSleeping(raket);

				//######################################################
				// the player lost a life 
				//######################################################
				else
				{	
					// rest one ball
					objetNombr = 1;
					//raket->attachBall(balle);
					//balle->reStarting(raket);
					balle->raket_ball->attachBall(balle);
					balle->reStarting(balle->raket_ball);
					// start parasite animation head
					gugusObjet->teteparasi();	
					ptBarreScr->lifesMoins(1);
					// force the explosion of all the "atoms"
					ptBouiBoui->atomexplos();
#ifndef SOUNDISOFF
					ptAudiomix->lost_music ();
					ptAudiomix->sound_play(S_ENLEVVIE);
#endif
					ptMiniMess->mesrequest(10);
					ptMiniMess->mesrequest(1);
					ptBarreScr->resetemoin();
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// guards levels: test if balls go out of the screen of game
//------------------------------------------------------------------------------
void zeNewBalls::vitussort2()
{
	Sint32 max_y = technoBall::MAXIMUM_PY * resolution;
	tecno_bump *raket = tecBumper1;	 //pointer to the object "bumper of bottom"
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	Sint32 j = balle->position_y;
			if(j > max_y)
			{	if((--objetNombr) > 0)
					balle->goSleeping(raket);
				else
				{	//###################################################
					// the player lost a life
					//###################################################
					objetNombr = 1;		//rest one ball
					raket->attachBall(balle);
					balle->reStarting(raket);
					balle->ballPower2();
					joueurGere->lifesMoins(1);
#ifndef SOUNDISOFF
					ptAudiomix->sound_play(S_ENLEVVIE);
#endif
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// 2 buttons at the same time = tilt
//------------------------------------------------------------------------------
void zeNewBalls::vitus_tilt()
{
	Uint32 ftilt = 0;
	if(keyGestion->buttonBlit())
	{	Sint32 t = balle_tilt;
		Sint32 h = hasard_val;
		h = h & 15;
		for(Sint32 i = 0; i < objetTotal; i++)
		{	technoBall *balle = objetListe[i];
			if(balle->flag_actif)
			{	if(balle->tiltCompte >= t)
				{	if(balle->directBall < 64)
					{	Sint32 d = (balle->directBall >> 2) & 0xf;
						balle->directBall = technoBall::tilt_table[d][h];
					}
					balle->tiltCompte = 0;
					if(!ftilt)
					{	ftilt = 1;
#ifndef SOUNDISOFF
						ptAudiomix->sound_play(S_TECNOBAL);
#endif
					}
				}
			}
		}
		if(ftilt)
			display->tiltscreen();
	}
}

//-------------------------------------------------------------------------------
// ball accelerates
//-------------------------------------------------------------------------------
void zeNewBalls::accelerate()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
			balle->accelerate();
	}
}

//-------------------------------------------------------------------------------
// bricks levels: displacement of the balls glued on the bumper 
//-------------------------------------------------------------------------------
void zeNewBalls::vitus_move()
{

	Sint32 j;
	Sint32 *monPT;
	tecno_bump *raket;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	
			
			//###########################################################
			// the ball is glued 
			//###########################################################
			if(balle->colleBallF)
			{	raket = balle->raket_ball;
				if(!(--balle->startCount))
				{	balle->colleBallF = 0;
					if(raket->bumperGlue == 2)
						raket->bumperGlue = 1;
					raket->balleColle = (technoBall *)NULL;
				}
				else
				{	switch (balle->colleBallF)
					{	case 1:
							j = (raket->colLargeur >> 1) -
								((balle->colLargeur >> 1) + 1);
							j += raket->position_x;
							balle->position_x = j;
							j = (raket->position_y) - (balle->colHauteur + 1);
							balle->position_y = j;
						break;

						case 2:
							j = (raket->position_x) - (balle->colLargeur - 1);
							balle->position_x = j;
							j = (raket->colHauteur >> 1) - ((balle->colHauteur >> 1) + 1);
							j += raket->position_y;
							balle->position_y = j;
							break;

						case 3:
							j = (raket->colLargeur >> 1) - ((balle->colLargeur >> 1) + 1);
							j += raket->position_x;
							balle->position_x = j;
							j = (raket->position_y) + raket->colHauteur + 1;
							balle->position_y = j;
							break;

						case 4:
							j = (raket->position_x) + (raket->colLargeur) + 1;
							balle->position_x = j;
							j = (raket->colHauteur >> 1) - ((balle->colHauteur >> 1) + 1);
							j += raket->position_y;
							balle->position_y = j;
							break;

					}

					
					if(--balle->tempo_rota < 0)
					{	balle->tempo_rota = 8;
						if(++balle->balle_rota > 13)
							balle->balle_rota = 0;
						raket = balle->raket_ball;
						monPT = raket->direct_tab + balle->balle_rota;
						balle->directBall = *monPT;
					}

				}
			}

			//###########################################################
			// the ball is not glued
			//###########################################################
			else
			{	j = balle->directBall;	//ball is moving
				if(j > 64)
				{	fprintf(stderr,
						"zeNewBalls::vitus_move() balle->directBall = %i\n",
						j);
					j = 60;
				}
				Sint16 *table = balle->speedBallT;
				table = (Sint16*)((char *)table + j);
				Sint32 k;
				k = *(table++);
				balle->position_x += (k* resolution);
				k = *table;
				balle->position_y += (k * resolution);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// guards levels: displacement of the balls glued on the bumper 
//-------------------------------------------------------------------------------
void zeNewBalls::vitusmove2()
{
	Sint32 j;
	Sint32 *monPT;
	tecno_bump *raket;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	
			
			//###########################################################
			// the ball is glued 
			//###########################################################
			if(balle->colleBallF)
			{	raket = balle->raket_ball;
				if(!(--balle->startCount))
				{	balle->colleBallF = 0;
					if(raket->bumperGlue == 2)
						raket->bumperGlue = 1;
					raket->balleColle = (technoBall *)NULL;
				}
				else
				{	switch (balle->colleBallF)
					{	case 1:
							j = (raket->colLargeur >> 1) -
								((balle->colLargeur >> 1) + 1);
							j += raket->position_x;
							balle->position_x = j;
							j = (raket->position_y) -
								(balle->colHauteur + 1);
							balle->position_y = j;
							break;
					}

					if(--balle->tempo_rota < 0)
					{	balle->tempo_rota = 8;
						if(++balle->balle_rota > 13)
						balle->balle_rota = 0;
						raket = balle->raket_ball;
						monPT = raket->direct_tab + balle->balle_rota;
						balle->directBall = *monPT;
					}

				}
			}

			//###########################################################
			// the ball is not glued
			//###########################################################
			else
			{	j = balle->directBall;	//ball is moving
				if(j > 64)
				{	fprintf(stderr,
						"zeNewBalls::vitus_move() balle->directBall = %i\n",
						j);
					j = 60;
				}
				Sint16 *table = balle->speedBallT;
				table = (Sint16*)((char *)table + j);
				Sint32 k;
				k = *(table++);
				balle->position_x += (k* resolution);
				k = *table;
				balle->position_y += (k* resolution);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// bricks level: collisions balls and bumpers 
//-------------------------------------------------------------------------------
void zeNewBalls::vitus_bump()
{
	Sint32 j, x, y;
	const Sint32 *monPT;
	tecno_bump *raket, *bumpX;
	tecBumper1->balleTouch = 0;
	tecBumper2->balleTouch = 0;
	tecBumper3->balleTouch = 0;
	tecBumper4->balleTouch = 0;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif && !balle->colleBallF && balle->directBall < 64)
		{
			//###########################################################
			// bottom bumper
			//###########################################################
			raket = tecBumper1;
			bumpX = 0;
			if(raket->flag_actif)
			{	x = raket->position_x;
				y = raket->position_y;
				if(balle->position_x + balle->colLargeur > x &&
					balle->position_y + balle->colHauteur > y &&
					balle->position_x < x + raket->colLargeur &&
					balle->position_y < y + raket->colHauteur)
				{	balle->position_y = y - balle->colHauteur;
					bumpX = raket;
					ptBarreScr->resetemoin();
				}
			}
			
			//###########################################################
			// right bumper
			//###########################################################
			if(!bumpX)
			{	raket = tecBumper2;
				if(raket->flag_actif)
				{	x = raket->position_x;
					y = raket->position_y;
					if(balle->position_x + balle->colLargeur > x &&
						balle->position_y + balle->colHauteur > y &&
						balle->position_x < x + raket->colLargeur &&
						balle->position_y < y + raket->colHauteur)
					{	
						balle->position_x = x - balle->colLargeur;
						bumpX = raket;
					}
				}
			}

			//###########################################################
			// top bumper
			//###########################################################
			if(!bumpX)
			{	raket = tecBumper3;
				if(raket->flag_actif)
				{	x = raket->position_x;
					y = raket->position_y;
					if(balle->position_x + balle->colLargeur > x &&
						balle->position_y + balle->colHauteur > y &&
						balle->position_x < x + raket->colLargeur &&
						balle->position_y < y + raket->colHauteur)
					{
						balle->position_y = y + raket->colHauteur;
						bumpX = raket;
					}
				}
			}

			//###########################################################
			// left bumper
			//###########################################################
			if(!bumpX)
			{
				raket = tecBumper4;
				if(raket->flag_actif)
				{	x = raket->position_x;
					y = raket->position_y;
					if(balle->position_x + balle->colLargeur > x &&
						balle->position_y + balle->colHauteur > y &&
						balle->position_x < x + raket->colLargeur &&
						balle->position_y < y + raket->colHauteur)
					{
						balle->position_x = x + raket->colLargeur;
						bumpX = raket;
					}
				}
			}
			
			//###########################################################
			// does the ball touch a bumper? 
			//###########################################################
			if(bumpX)
			{	bumpX->balleTouch = 1;
#ifndef SOUNDISOFF
				ptAudiomix->sound_play(S_TOUCHRAK);
#endif
				balle->raket_ball = bumpX;
				balle->tiltCompte = 0;
				j = balle->directBall;
				if(j > 64)
				{	fprintf(stderr,
						"zeNewBalls::vitus_bump() (1) balle->directBall = %i\n",
						j);
					j = 64;
				}
				monPT = bumpX->rebonds_GD;
				//(char *)monPT += j;
				monPT = (Sint32 *)((char *)monPT + j); 
				j = *monPT;

				if(j > 64)
				{	fprintf(stderr,
						"zeNewBalls::vitus_bump() (2) balle->directBall = %i (%i)\n",
						j, balle->directBall);
					for(Sint32 v = 0; v < 16; v++)
						printf("%i ; ",  bumpX->rebonds_GD[v]);
					monPT = bumpX->rebonds_GD;
					j = balle->directBall;
					//(char *)monPT += j;
					monPT = (Sint32 *)((char *)monPT + j); 
					/*printf("monPT = %x / bumpX->rebonds_GD = %x / *monPT = %i\n",
						(long)monPT, (long)bumpX->rebonds_GD, *monPT);
					printf("%i %i\n", bumpX->rebonds_GD[16], bumpX->rebonds_GD[64]);*/
					j = 60;
				}
				

				//balle->directBall = *monPT;
				balle->directBall = j;
				if(bumpX->bumperGlue == 1 && !bumpX->balleColle)
				{	bumpX->bumperGlue = 2;	//ball glued to the bumper 
					bumpX->balleColle = (technoBall *) balle;
					balle->raket_glue = bumpX;
					balle->startCount = balle_glue;	//time of the glue 
					balle->colleBallF = raket->bumpNumero;
				}
			
			}

		}
	}
}

//-------------------------------------------------------------------------------
// guards level: collisions balls and bumper 
//-------------------------------------------------------------------------------
void zeNewBalls::vitusbump2()
{
	Sint32 j, x, y;
	const Sint32 *monPT;
	tecno_bump *raket, *bumpX;
	tecBumper1->balleTouch = 0;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	raket = tecBumper1;
			bumpX = 0;
			if(raket->flag_actif)
			{	x = raket->position_x;
				y = raket->position_y;
				if(balle->position_x + balle->colLargeur > x &&
					balle->position_y + balle->colHauteur > y &&
					balle->position_x < x + raket->colLargeur &&
					balle->position_y < y + raket->colHauteur)
				{
					balle->position_y = y - balle->colHauteur;
					bumpX = raket;
				}
			}

			//###########################################################
			// does the ball touch a bumper?
			//###########################################################
			if(bumpX)
			{	bumpX->balleTouch = 1;
#ifndef SOUNDISOFF
				ptAudiomix->sound_play(S_TOUCHRAK);
#endif
				balle->raket_ball = bumpX;
				balle->tiltCompte = 0;
				j = balle->directBall;
				monPT = bumpX->rebonds_GD;
				//(char *)monPT += j;
				monPT = (Sint32 *)((char *)monPT + j); 
				balle->directBall = *monPT;
				if(bumpX->bumperGlue == 1)
				{	bumpX->bumperGlue = 2;	//ball glued to the bumper 
					bumpX->balleColle = (technoBall *) balle;
					balle->raket_glue = bumpX;
					balle->startCount = balle_glue;	//time of the glue 
					balle->colleBallF = raket->bumpNumero;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------
// bricks level: collisions balls and robot bumper
//-------------------------------------------------------------------------------
void zeNewBalls::vitusrobot()
{
	if(tec_robot0->bump_actif)
	{	tecno_bump *raket = tec_robot0;
		raket->balleTouch = 0;
		Sint32 x1 = raket->position_x;
		Sint32 y1 = raket->position_y;
		Sint32 x2 = raket->position_x + raket->colLargeur;
		Sint32 y2 = raket->position_y + raket->colHauteur + 8;
		const Sint32 *monPT;
		Sint32 j;
		for(Sint32 i = 0; i < objetTotal; i++)
		{	technoBall *balle = objetListe[i];
			if(balle->flag_actif)
			{	if(balle->position_x + balle->colLargeur > x1 &&
					balle->position_y + balle->colHauteur > y1 &&
					balle->position_x < x2 &&
					balle->position_y < y2)
				{
					balle->position_y = y1 - balle->colHauteur;
					raket->balleTouch = 1;
					j = balle->directBall;
					monPT = raket->rebonds_GD;
					monPT = (Sint32*)((char *)monPT + j);
					balle->directBall = *monPT;
#ifndef SOUNDISOFF
					ptAudiomix->sound_play(S_TOUCHRAK);
#endif
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// bricks levels: handle ejectos/balls collisions
//-----------------------------------------------------------------------------
void zeNewBalls::vitusEject()
{
	sprite_object *coin1 = ejectObjet->demandeBOB(1);
	sprite_object *coin2 = ejectObjet->demandeBOB(2);
	sprite_object *coin3 = ejectObjet->demandeBOB(3);
	sprite_object *coin4 = ejectObjet->demandeBOB(4);

	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
 		if(balle->flag_actif)
		{

			//###############################################################
			// test if the ball is already into an ejector
			//###############################################################
			Sint32 *monPT = balle->eject_ball;
			Sint32 *table = 0;
			if(*(monPT++))
				table = ballEject1;
			else
			{	if(*(monPT++))
					table = ballEject2;
				else
				{	if(*(monPT++))
					table = ballEject3;
						else
					{	if(*(monPT++))
							table = ballEject4;
					}
				}
			}

			//###########################################################
			// the ball is into an ejector
			//###########################################################
			if(table)
			{	Sint32 j = ++(*(--monPT));
				if(j == 160)	// time before ejection
				{	Sint32 j = hasard_val & 0xF;
					table += j;
					balle->directBall = *table;
#ifndef SOUNDISOFF
					ptAudiomix->sound_play (S_COINEJEC);
#endif
				}
				else
				{	if(j == 200)
						*monPT = 0;
				}
			}

			//###########################################################
			// If not test if a ball is aspired by an ejector 
			//###########################################################
			else
			{	if(balle->collision1(coin1))	//top-left
				{	balle->aspire_BOB(coin1, 10 * resolution, 10 * resolution);
					balle->eject_ball[0] = 1;
					balle->directBall = 64;
					ptBarreScr->scoreAjout(10);
#ifndef SOUNDISOFF
					ptAudiomix->sound_play (S_COINASPI);
#endif
				}
				else
				{	if(balle->collision1(coin2))	//top-right
					{	balle->aspire_BOB(coin2, 5 * resolution,
											10 * resolution);
						balle->eject_ball[3] = 1;
						balle->directBall = 64;
						ptBarreScr->scoreAjout(10);
#ifndef SOUNDISOFF
						ptAudiomix->sound_play (S_COINASPI);
#endif
					}
					else
					{	if(balle->collision1(coin3))	//bottom-left
						{	balle->aspire_BOB(coin3, 10 * resolution,
												5 * resolution);
							balle->eject_ball[2] = 1;
							balle->directBall = 64;
							ptBarreScr->scoreAjout(10);
#ifndef SOUNDISOFF
							ptAudiomix->sound_play (S_COINASPI);
#endif
						}
						else
						{	if(balle->collision1(coin4))	//bottom-right
							{	balle->aspire_BOB(coin4, 5 * resolution,
									5 * resolution);
								balle->eject_ball[1] = 1;
								balle->directBall = 64;
								ptBarreScr->scoreAjout(10);
#ifndef SOUNDISOFF
								ptAudiomix->sound_play (S_COINASPI);
#endif
							}
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// bricks levels: handle the collision with the 3 walls
//------------------------------------------------------------------------------
void zeNewBalls::vitus_cote()
{
	Sint32 murGa = bricoObjet->getCollisG();
	Sint32 murDr = bricoObjet->getCollisD();
	Sint32 murHt = bricoObjet->getCollisH();
	Sint32 murBa = ptBob_wall->retournePY();
	Sint32 fwall = ptBob_wall->is_enable();
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	Sint32 x = balle->position_x;
			Sint32 y = balle->position_y;
			Sint32 *monPT = NULL;

			//###########################################################
			// collision with the bottom wall (if it's enable)
			//###########################################################
			if (fwall && y > (murBa - balle->colHauteur))
			{	monPT = rb7;
			}				
			else
			{	//########################################################
				// collision with the left wall
				//########################################################
				if(x < murGa)
				{	if(bricoObjet->collGauche(y) != 0)
						monPT = rb5;
				}
				else
				{	//####################################################
					// collision with the right wall
					//####################################################
					x += balle->colLargeur;
					if(x > murDr)
					{	if(bricoObjet->collDroite(y) != 0)
						monPT = rb1;
					}
					else
					{	//#################################################
						// collision with the up wall 
						//#################################################
						if(y < murHt)
						{	if(bricoObjet->coll_haute(x) != 0)
							monPT = rb3;
						}
					}
				}
			}

			//###########################################################
			// collision dectected
			//###########################################################
			if(monPT)
			{	//(char *)monPT += balle->directBall;
				monPT = (Sint32 *)((char *)monPT + balle->directBall); 
				balle->directBall = *monPT;
#ifndef SOUNDISOFF
				ptAudiomix->sound_play(S_BRICOTES);
#endif
			}
		}
	}
}

//------------------------------------------------------------------------------
// guards levels: handle the collision with the 3 walls
//------------------------------------------------------------------------------
void zeNewBalls::vituscote2()
{
	Sint32 murGa = 16 * resolution;
	Sint32 murDr = 300 * resolution;
	Sint32 murHt = 8 * resolution;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(balle->flag_actif)
		{	Sint32 x = balle->position_x;
			Sint32 y = balle->position_y;
			Sint32 *monPT = 0; 
			if(x < murGa)
			{	monPT = rb5;
				balle->position_x = murGa;
#ifndef SOUNDISOFF
				ptAudiomix->sound_play(S_BRICOTES);
#endif
				balle->colli_wall = 4;
			}
			else
			{	if(x > murDr)
				{	monPT = rb1;
					balle->position_x = murDr;
#ifndef SOUNDISOFF
					ptAudiomix->sound_play(S_BRICOTES);
#endif
					balle->colli_wall = 2;
				}
				else
				{	if(y < murHt)
					{	monPT = rb3;
						balle->position_y = murHt;
#ifndef SOUNDISOFF
						ptAudiomix->sound_play(S_BRICOTES);
#endif
						balle->colli_wall = 3;
					}
					else
						balle->colli_wall = 0;
				}
			}
			if(monPT)
			{	//(char *)monPT += balle->directBall;
				monPT = (Sint32 *)((char *)monPT + balle->directBall); 
				balle->directBall = *monPT;
			}
		}
	}
}

//------------------------------------------------------------------------------
// guards levels: prevent that the ball remains blocked horizontally 
//------------------------------------------------------------------------------
void zeNewBalls::vitusbound()
{
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = objetListe[i];
		if(!balle->flag_actif) continue;
		Sint32 dball = balle->directBall;
		if(dball >= 32) dball -= 32;		
		if(dball == balle->save_Dball) 
		{	if(balle->countDball++ > 360 && dball == 0 && balle->colli_wall)
				if(balle->directBall == 32)
					balle->directBall = 28;
				else
					balle->directBall = 4;
		}
		else
		{	balle->save_Dball = dball;
			balle->countDball = 0;
		}
	}			
}

//-------------------------------------------------------------------------------
// bricks levels: collision of balls with bricks
//-------------------------------------------------------------------------------
void zeNewBalls::vitusBrick()
{
	Sint32 bwght = brickObjet->getBkWidth();	//brick's width in pixels
	Sint32 byoff = brickObjet->getYOffset();	//y-offset between 2 bricks
	Sint32 indus = brickObjet->getBkIndus();	//first indestructible brick
	//printf("bwght:%i, byoff:%i, indus:%i\n",bwght, byoff, indus);
	
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	brickInfos *tMega = brickObjet->mega_table;
	Sint32 save = brickObjet->briqueSave;		// save => offset on "brique_pnt"
	brickClear *briPT = brickObjet->brique_pnt;	// pointer to structure "brickClear" (display and clear the bricks)
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
		{	Sint32 indes = 0;					// flag brique indestructible touchee
			Sint32 *colTB = balle->collisionT;	// table des 4 points de collision 
			Sint32 incre = 1;					// incremente flag rebond
			Sint32 rebon = 0;					// flag rebond
			for(Sint32 j = 0; j < 4; j++)		// 4 points par balle
			{	Sint32 x = balle->position_x;
				x += *(colTB++);
				Sint32 y = balle->position_y;
				y += *(colTB++);
				brickClear *briP2 = briPT + save;
				briP2->balle_posX = x;
				briP2->balle_posY = y;
				x /= bwght;
				y /= byoff;
				y *= lesBriques::NB_BRICKSH;
				x += y;
				brickInfos *megaT = (tMega + x);
				x = megaT->brique_rel;
				if(x)	//collision ball and brick?
				{	
					briP2->raquettePT = balle->raket_ball;
					
					x = x - indus;
					if(x >= 0)
					{	//#################################################
						// touched indestructible brick
						//#################################################
						indes = 1;				//collision with indestructible 
						if((x -= bwght) > 0)	//indestructible-destructible bricks?
						{	if(balle->ballPowerX == technoBall::BALLPOWER2)
							{	briP2->adresseAff = megaT->adresseAff;
								briP2->adresseTab = megaT;
								briP2->balle_posX = -1;
								megaT->brique_rel = 0;	//reset code brick
								briP2->brique_num = megaT->brique_num;
								briP2->briqueFlag = 1;	//1 = restore background
								save += 1;				// augmente pointeur table brique effacement
								save &= (lesBriques::MAXBRIKCLR - 1);
							}
							else
							{	x = 2;
#ifndef SOUNDISOFF
								ptAudiomix->sound_play(S_TOINDES2);
#endif
							}
						}
						else
						{	x = 1;	//brick's really indestructible
#ifndef SOUNDISOFF
							ptAudiomix->sound_play(S_TOINDES1);
#endif
						}
					}

					//####################################################
					// touched normal brick
					//####################################################
					else
					{	briP2->adresseAff = megaT->adresseAff;
						briP2->adresseTab = megaT;
						megaT->briquePosX = megaT->briquePosX - (balle->powerBall1*2);
						if(megaT->briquePosX <= 0)
						{	megaT->briquePosX = 0;
							megaT->brique_rel = 0;	//reset code brick
							briP2->brique_num = megaT->brique_num;
							briP2->briqueFlag = 1;	//1 = restore background 
						}
						else
						{	megaT->brique_rel = megaT->brique_rel - balle->powerBall2;
							briP2->brique_num = megaT->brique_rel;
							briP2->briqueFlag = 0;	//flag redraw new brick
						}
						save += 1;	// augmente pointeur table brique effacement
						save &= (lesBriques::MAXBRIKCLR - 1);
					}
					rebon += incre;	// incremente le flag rebond
				}
				incre = incre + incre;
			} // boucle 4 points de collision

			// 
			if((--rebon) >= 0)
			{	if( indes>0 || balle->ballPowerX == technoBall::BALLNORMAL )
				{	Sint32 *rebPT = *(brick_jump + rebon);
					//(char *)rebPT += balle->directBall;
					rebPT = (Sint32 *)((char *)rebPT + balle->directBall); 
					balle->directBall = *rebPT;
				}
			}
			brickObjet->briqueSave = save;
		} // ball is active ?
	} // ball loop
}

//----------------------------------------------------------------------
// bricks levels: collision of balls with eyes
//----------------------------------------------------------------------
void zeNewBalls::vitus_eyes()
{
	Sint32 vhypo = pt_magneye->hypotenuse;
	technoBall **liste = objetListe;
	Sint32 nbEye = pt_magneye->totalObjet();
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = *(liste++);
		if(!balle->flag_actif) continue;
		techno_eye **pEyes = pt_magneye->listeObjet();
		for(Sint32 j = 0; j < nbEye; j++)
		{	techno_eye *ptEye = *(pEyes++);
			if(!ptEye->flag_actif) continue;
			Sint32 centX = ptEye->position_x + pt_magneye->eyeCenterX;
			Sint32 centY = ptEye->position_y + pt_magneye->eyeCenterY;
			
			Sint32 deltX = balle->position_x + (balle->colLargeur / 2) - centX;
			deltX = deltX * deltX;
			Sint32 deltY = balle->position_y + (balle->colLargeur / 2) - centY;
			deltY = deltY * deltY;
			if(deltX + deltY >= vhypo) continue;
				
			deltX = balle->position_x + (balle->colLargeur / 2) - centX;
			deltY = balle->position_y + (balle->colLargeur / 2) - centY;
			
			if(deltY == 0)
			{	if(deltX < 0) balle->directBall  = 32;
				else balle->directBall = 0;
			}

			// DELTA Y>0
			if(deltY > 0)
			{	if(deltX == 0)  balle->directBall = 48;
				if(deltX < 0) 
				{	deltX = -deltX;
					if(deltX == deltY) balle->directBall = 40;
					if(deltX < deltY) balle->directBall = 44;
					else balle->directBall = 36;
				}
				else
				{	if(deltX == deltY) balle->directBall = 56;
					if(deltX < deltY) balle->directBall = 52;
					else balle->directBall = 56;
				}
			}

			if(deltY < 0)
			{	deltY = -deltY;
				if(deltX == 0)  balle->directBall = 16;
				if(deltX < 0) 
				{	deltX = -deltX;
					if(deltX == deltY) balle->directBall = 24;
					if(deltX < deltY) balle->directBall = 20;
					else balle->directBall = 28;
				}
				else
				{	if(deltX == deltY) balle->directBall = 8;
					if(deltX < deltY) balle->directBall = 12;
					else balle->directBall = 4;
				}
			}
			
		}
	}
}


//----------------------------------------------------------------------
// bricks levels: collision of balls with Bouibouis
//----------------------------------------------------------------------
void zeNewBalls::vitusAtoms()
{
	technoBall **liste = objetListe;
	Sint32 t = ptBouiBoui->totalObjet();
	tecno_boui **aList = ptBouiBoui->listeObjet();
	Sint32 nouve = (hasard_val & 0xF) << 2;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif && balle->ClBouiBoui == 0)
		{	Sint32 h = balle->colLargeur;
			h = h - 2;
			Sint32 x1 = balle->position_x;
			Sint32 x2 = x1 + h;
			x1 -= 20;
			Sint32 y1 = balle->position_y;
			Sint32 y2 = y1 + h;
			y1 -= 26;
			tecno_boui **monPT = aList;
			for(Sint32 j = 0; j < t; j++)
			{	tecno_boui *atome = *(monPT++);
				if(!atome->atom_actif)
				{	Sint32 k = atome->position_y;
					if(k < y2 && k > y1)
					{	k = atome->position_x;
						if(k < x2 && k > x1)
						{	ptBarreScr->scoreAjout(100);
#ifndef SOUNDISOFF
							ptAudiomix->sound_play(S_TO_ATOMS);
#endif
							k = (balle->ballPowerX + 1) * 4;
							atome->atom_power -= k;
							if(atome->atom_power < 1)
								atome->explosion1(balle);
							balle->directBall = nouve;
						}
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------
// guards levels: collision of balls with the guardiens
//----------------------------------------------------------------------
void zeNewBalls::vitusGuard()
{
	Sint32 u = objetTotal;					// number of balls (1 to n)
	technoBall **liste = objetListe;
	Sint32 t = ptguardian->totalObjet();	// number of guards (1 or 2)
	tecno_gard **aList = ptguardian->listeObjet();
	for(Sint32 j = 0; j < t; j++)
	{	tecno_gard *pGard = aList[j];
		technoBall *balok = NULL; 
		if (pGard->flag_actif && pGard->gard_power > 0) 
		{	Sint32 grdx1 = pGard->position_x;
			Sint32 grdx2 = grdx1 + pGard->gard_colx2;
			grdx1 += pGard->gard_colx1;
			Sint32 grdy1 = pGard->position_y;
			Sint32 grdy2 = grdy1 + pGard->gard_coly2;
			grdy1 += pGard->gard_coly1;
			for(Sint32 i = 0; i < u; i++)
			{	technoBall *balle = liste[i];
				if(balle->flag_actif)
				{	Sint32 x = balle->position_x;
					if(x <= grdx2)
					{	Sint32 y = balle->position_y;
						if(y <= grdy2)
						{	x += balle->colLargeur;
							if(x > grdx1)
							{	y += balle->colLargeur;
								if(y > grdy1)
								{	x = ((hasard_val + i) & 0xF) << 2;
#ifndef SOUNDISOFF
									ptAudiomix->sound_play(S_GARDIENT);
#endif
									balle->directBall = x;
									pGard->gard_touch = 5;
									pGard->gard_power -= balle->powerBall1;
									if(pGard->gard_power <= 0)
									{	pGard->gard_power = 0;
										pGard->explo_time = 500;
									}
									balok = balle;
								}
							}
						}
					}
				}
			}
			if(balok)
			{	ptCapsules->envoieFric(balok);
				pt_gadgets->envoieGads(balok);
			}
		}
	}
}

//----------------------------------------------------------------------
// return the first enable ball
//----------------------------------------------------------------------
technoBall *zeNewBalls::first_ball()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			return (balle);
	}
	return (objetListe[0]);
}

//----------------------------------------------------------------------
// bricks levels: enable control ball with the left mouse button
//----------------------------------------------------------------------
void zeNewBalls::runcontrol()
{
	balle_ctrl = 1;
}

//-------------------------------------------------------------------------------
// bricks levels: extra balls; add 2 balls into ejectors
//-------------------------------------------------------------------------------
void zeNewBalls::run_2balls()
{
	run_nballs(2);
}

//-------------------------------------------------------------------------------
// bricks levels: add n ball(s) into ejectors
//-------------------------------------------------------------------------------
void zeNewBalls::run_nballs(Sint32 nball)
{
	if(nball < 1) nball = objetTotal;
	technoBall **liste = objetListe;
	Sint32 k = 0;
	Sint32 e = hasard_val & 3;
	Sint32 t = objetTotal;
	Sint32 otime = 1;
	for(Sint32 i = 0; i < t && k < nball; i++)
	{	technoBall *balle = *(liste++);
		if(!balle->flag_actif)
		{	balle->ball2eject(e++, otime);
			k++;
			objetNombr++;
			otime += 2;
		}
	}
}

//----------------------------------------------------------------------
// bricks levels: add 3 balls starting from the first enable ball 
//----------------------------------------------------------------------
void zeNewBalls::run_3balls()
{
	technoBall *model = first_ball();
	Sint32 j = model->directBall;	//direction of the current ball 
	Sint32 i = 0;
	Sint32 k = 0;
	technoBall **liste = objetListe;
	while (i < objetTotal && k < 3)
	{	technoBall *balle = *(liste++);
		if(!balle->flag_actif)
		{	j += 8;
			j &= 60;
			balle->duplicate3(model, j);
			objetNombr++;
			k++;
		}
		i++;
	}
}

//----------------------------------------------------------------------
// transform all the enable balls into balls power 1
//----------------------------------------------------------------------
void zeNewBalls::run_power1()
{
	technoBall **liste = objetListe;
	for(Sint32 i = 0; i < objetTotal; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			balle->ballPower1();
	}
}

//----------------------------------------------------------------------
// transform all the enable balls into balls power 2
//----------------------------------------------------------------------
void zeNewBalls::run_power2()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			balle->ballPower2();
	}
}

//----------------------------------------------------------------------
// transform all the enable balls into balls size 2 (7*7 or 14*14)
//----------------------------------------------------------------------
void zeNewBalls::run_size01()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			balle->ball_size2();
	}
}

//----------------------------------------------------------------------
// transform all the enable balls into balls size 3 (10*10 or 20*20)
//----------------------------------------------------------------------
void zeNewBalls::run_size02()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			balle->ball_size3();
	}
}

//----------------------------------------------------------------------
// increase the speed of the balls to the maximum
//----------------------------------------------------------------------
void zeNewBalls::maxi_speed()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
			balle->very_speed();
	}
}

//------------------------------------------------------------------------------
// bricks levels: test if the player can use the tilt 
//------------------------------------------------------------------------------
void zeNewBalls::time_2tilt()
{
	Uint32 tilt = 0;
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	Sint32 v = balle_tilt;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
		{	if(!balle->colleBallF)
			{	if(balle->tiltCompte == v)
				{	if(!tilt)
					{	gugusObjet->tetebaille();
#ifndef SOUNDISOFF
						ptAudiomix->sound_play(S_TILTALAR);
#endif
						tilt = 1;
					}
					balle->tiltCompte++;
				}
				else
					balle->tiltCompte++;
			}
		}
		else
			balle->tiltCompte++;
	}
}

//------------------------------------------------------------------------------
// guards levels: test if the player can use the tilt 
//------------------------------------------------------------------------------
void zeNewBalls::time2tilt2()
{
	Uint32 tilt = 0;
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	Sint32 v = balle_tilt;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->flag_actif)
		{	if(!balle->colleBallF)
			{	if(balle->tiltCompte == v)
				{	if(!tilt)
					{	tilt = 1;
#ifndef SOUNDISOFF
						ptAudiomix->sound_play(S_TILTALAR);
#endif
					}
					balle->tiltCompte++;
				}
				else
					balle->tiltCompte++;
			}
		}
		else
			balle->tiltCompte++;
	}
}

//------------------------------------------------------------------------------
// bricks levels: handle the control of the ball with the left mouse button
//------------------------------------------------------------------------------
void zeNewBalls::vitus_ctrl()
{
	if(balle_ctrl > 0)
	{	if(keyGestion->righButton())
		{	technoBall **liste = objetListe;
			Sint32 t = objetTotal;
			for(Sint32 i = 0; i < t; i++)
			{	technoBall *balle = *(liste++);
				if(balle->flag_actif)
				{	Sint32 dball = balle->directBall;
					if(dball < 64)
					{	dball = dball +4;
						dball = dball & 60;
						balle->directBall = dball;
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// bricks levels: check if there remains at least a ball glue
//------------------------------------------------------------------------------
Sint32 zeNewBalls::least_glue()
{
	technoBall **liste = objetListe;
	Sint32 t = objetTotal;
	for(Sint32 i = 0; i < t; i++)
	{	technoBall *balle = *(liste++);
		if(balle->colleBallF)
			return 1;
	}
	return 0;
}


//------------------------------------------------------------------------------
// directions of the ball when it is leave an ejector. 
//------------------------------------------------------------------------------
// top-left
Sint32 zeNewBalls::ballEject1[] =
{	52, 56, 60, 60, 52, 56, 60, 60, 52, 52, 56, 52, 52, 60, 56, 52, 56, 56
};
// bottom-left
Sint32 zeNewBalls::ballEject2[] =
{	8, 4, 12, 12, 8, 4, 4, 12, 8, 4, 12, 4, 8, 12, 4, 8, 12, 4, 4
};
// bottom-right
Sint32 zeNewBalls::ballEject3[] =
{	20, 28, 24, 20, 20, 28, 28, 24, 20, 28, 24, 24, 28, 28, 20, 20, 24, 24, 28
};
// top-right 
Sint32 zeNewBalls::ballEject4[] =
{	36, 44, 40, 36, 36, 44, 44, 40, 40, 36, 44, 40, 40, 36, 36, 44, 44, 40, 36
};

//------------------------------------------------------------------------------
// directions of the ball after a rebound on a brick. 
//------------------------------------------------------------------------------
Sint32 zeNewBalls::rb0[16] =
{	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 };
// right
Sint32 zeNewBalls::rb1[16] =
{	32, 28, 24, 20, 20, 24, 24, 28, 32, 36, 40, 40, 44, 44, 40, 36 };
Sint32 zeNewBalls::rb2[16] =
{	48, 36, 40, 44, 32, 44, 24, 28, 32, 36, 40, 44, 48, 48, 44, 40 };
//top
Sint32 zeNewBalls::rb3[16] =
{	60, 60, 56, 52, 48, 44, 40, 36, 36, 40, 40, 44, 48, 52, 56, 56 };
Sint32 zeNewBalls::rb4[16] =
{	0, 4, 8, 0, 0, 52, 56, 60, 48, 52, 56, 44, 48, 52, 56, 60 };
// left
Sint32 zeNewBalls::rb5[16] =
{	0, 4, 8, 8, 12, 12, 8, 4, 0, 60, 56, 52, 52, 36, 56, 60 };
Sint32 zeNewBalls::rb6[16] =
{	0, 4, 8, 12, 16, 20, 24, 12, 16, 12, 8, 4, 0, 4, 8, 60 };
// bottom
Sint32 zeNewBalls::rb7[16] =
{	4, 8, 12, 12, 16, 20, 20, 24, 28, 28, 24, 20, 16, 12, 8, 4 };
Sint32 zeNewBalls::rb8[16] =
{	16, 20, 24, 12, 16, 20, 24, 28, 32, 36, 40, 28, 32, 20, 24, 28 };
Sint32 *zeNewBalls::brick_jump[15] =
{	rb1, rb3, rb2, rb5, rb1, rb4, rb3, rb7, rb8, rb2, rb1, rb6, rb7, rb5, rb1 };
